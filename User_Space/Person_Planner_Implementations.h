#pragma once

#include "Person_Prototype.h"
#include "Movement_Plan_Prototype.h"
#include "Network_Skimming_Prototype.h"
#include "Activity_Prototype.h"
#include "Population_Unit_Implementations.h"
#include "Person_Implementations.h"


namespace Person_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Planning classes
		//----------------------------------------------------------------------------------
		implementation struct General_Person_Planner_Implementation : public Polaris_Component<APPEND_CHILD(General_Person_Planner_Implementation),MasterType,Execution_Object,ParentType>
		{
			static ofstream logs[_num_threads];
			static member_data(bool, write_activity_files,none,none);
			feature_implementation void Write_To_Log(stringstream& s)
			{
				if (_write_activity_files) this->logs[_thread_id] << s.str();
			}

			// Pointer to the Parent class
			member_prototype(Person_Components::Prototypes::Person, Parent_Person, typename MasterType::person_type, none, none);

			// Pointer to the child classses
			member_prototype(Prototypes::Activity_Generator, Activity_Generator, typename MasterType::activity_generator_type,none,none);
			member_prototype(Prototypes::Destination_Choice, Destination_Chooser, typename MasterType::person_destination_choice_type,none,none);

			// Next Activity Generation Time member - used to schedule the next activity generation
			member_data_component(typename Basic_Units::Implementations::Time_Implementation<MasterType>,_Generation_Time,none,none);
			member_component_feature(Next_Activity_Generation_Time, _Generation_Time, Value, Basic_Units::Prototypes::Time_Prototype);
			// Planning Time Increment member - sets the next iteration after all planning is completed
			member_data_component(typename Basic_Units::Implementations::Time_Implementation<MasterType>,_Planning_Time_Increment,none,none);
			member_component_feature(Planning_Time_Increment, _Planning_Time_Increment, Value, Basic_Units::Prototypes::Time_Prototype);
			// Generation Time Increment member - sets the next Generation iteration 
			member_data_component(typename Basic_Units::Implementations::Time_Implementation<MasterType>,_Generation_Time_Increment,none,none);
			member_component_feature(Generation_Time_Increment, _Generation_Time_Increment, Value, Basic_Units::Prototypes::Time_Prototype);
			//Containers for activity planning events and movement planning events
			member_container(list<typename MasterType::activity_plan_type*>,Activity_Plans_Container,none,none);
			member_container(list<typename MasterType::movement_plan_type*>,Movement_Plans_Container,none,none);


			// Interface definitions
			define_component_interface(_Scenario_Interface, typename type_of(Parent_Person)::type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
			define_component_interface(_Network_Interface, typename type_of(Parent_Person)::type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);	
			define_component_interface(_Skim_Interface, typename _Network_Interface::get_type_of(skimming_faculty),Network_Skimming_Components::Prototypes::Network_Skimming_Prototype,ComponentType);
			define_container_and_value_interface(_Activity_Locations_Container_Interface, _Activity_Location_Interface, typename _Network_Interface::get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
			define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, typename _Activity_Location_Interface::get_type_of(origin_links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface(_Zones_Container_Interface, _Zone_Interface, typename _Network_Interface::get_type_of(zones_container), Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
			define_container_and_value_interface_unqualified_container(Activity_Plans,Activity_Plan, type_of(Activity_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Planner,ComponentType);
			define_container_and_value_interface_unqualified_container(Movement_Plans,Movement_Plan, type_of(Movement_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);
		

			// Activity generation functionality
			feature_implementation void Activity_Generation()
			{
			}
			tag_feature_as_available(Activity_Generation);
			feature_implementation TargetType current_movement_plan(requires(check_as_given(TargetType,is_pointer) && check(TargetType,Movement_Plan_Components::Concepts::Is_Movement_Plan_Prototype)))
			{
				// Define interfaces to the container members of the class			
				define_container_and_value_interface_unqualified_container(Movement_Plans_List,Movement_Plan,type_of(Movement_Plans_Container),Associative_Container_Prototype,Movement_Plan_Prototype,ComponentType);		
				Movement_Plans_List* movement_plans = this->template Movement_Plans_Container<ComponentType,CallerType, Movement_Plans_List*>();
				typename Movement_Plans_List::iterator itr;
				if ((itr = movement_plans->find(_iteration)) != movement_plans->end()) return (TargetType)*itr;
				else return NULL;
			}
			tag_feature_as_available(current_movement_plan);
			feature_implementation TargetType current_activity_plan(requires(check_as_given(TargetType,is_pointer) && check(TargetType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)))
			{
				// Define interfaces to the container members of the class
				define_container_and_value_interface_unqualified_container(Activity_Plans_List,Activity_Plan,type_of(Activity_Plans_Container),Associative_Container_Prototype,Activity_Plan_Prototype,ComponentType);
				Activity_Plans_List* activity_plans = this->template Activity_Plans_Container<Activity_Plans_List*>();
				typename Activity_Plans_List::iterator itr;
				if ((itr = activity_plans->find(_iteration)) != activity_plans->end()) return (TargetType)*itr;
				else return NULL;
			}
			tag_feature_as_available(current_activity_plan);

			// Adding activities and movements to the planning schedules
			feature_implementation void Add_Movement_Plan(TargetType movement_plan, requires(check_as_given(TargetType,is_pointer) && check(TargetType,Movement_Plan_Components::Concepts::Is_Movement_Plan_Prototype)))
			{
				// define interfaces
				typename Movement_Plans::iterator move_itr;

				// get interface to the movement plan to be added
				Movement_Plan* move = (Movement_Plan*)movement_plan;

				// print to log file if requested
				if (_write_activity_files) 
				{			
					stringstream s;
					int o_id = move->template origin<_Activity_Location_Interface*>()->template zone<_Zone_Interface*>()->template uuid<int>();
					int d_id = move->template destination<_Activity_Location_Interface*>()->template zone<_Zone_Interface*>()->template uuid<int>();
					_Skim_Interface* skim = _Parent_Person->template network_reference<_Network_Interface*>()->template skimming_faculty<_Skim_Interface*>();		
					s << _Parent_Person->template uuid<int>() << "," << move->template departed_time<Time_Hours>();
					s << "," << move->template origin<_Activity_Location_Interface*>()->uuid<int>() << "," << move->template destination<_Activity_Location_Interface*>()->uuid<int>();
					s << "," << skim->template Get_LOS<Target_Type<NULLTYPE,Time_Minutes,int,Vehicle_Components::Types::Vehicle_Type_Keys> >(o_id, d_id, Vehicle_Components::Types::SOV)<<endl;
					this->Write_To_Log<ComponentType,CallerType,TargetType>(s);
				}

				// catch skipped movement plans
				if (move->template departed_time<Simulation_Timestep_Increment>() < _iteration)
				{
					if (_write_activity_files) this->logs[_thread_id] <<",invalid-start prior to current iteration."<<endl;
					return;
				}

				// Insert new movement plan at the appropriate space in plan schedule
				Movement_Plans* movements = this->template Movement_Plans_Container<ComponentType,CallerType,Movement_Plans*>();
				move_itr = movements->begin();
				while(move_itr != movements->end())
				{
					Movement_Plan* current_move = *move_itr;
					if (current_move->template departed_time<Simulation_Timestep_Increment>() > move->template departed_time<Simulation_Timestep_Increment>()) break;
					++move_itr;
				}
				movements->insert(move_itr,move);
				
			}
			feature_implementation void Add_Movement_Plan(TargetType movement_plan, requires(!check_as_given(TargetType,is_pointer) || !check(TargetType,Movement_Plan_Components::Concepts::Is_Movement_Plan_Prototype)))
			{
				assert_check_as_given(TargetType,is_pointer, "Error, TargetType must be passed as a pointer");
				assert_check(TargetType, Movement_Plan_Components::Concepts::Is_Movement_Plan_Prototype, "Error, Function requires TargetType to be a Movement_Plan_Prototype.");
			}
			tag_feature_as_available(Add_Movement_Plan);
			feature_implementation void Add_Activity_Plan(TargetType activity_plan, requires(check_as_given(TargetType,is_pointer) && check(TargetType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)))
			{
				define_container_and_value_interface_unqualified_container(Activity_Plans,Activity_Plan,type_of(Activity_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Plan,ComponentType);	
				Activity_Plan* act = (Activity_Plan*)activity_plan;

				long t1 = act->template Activity_Plan_Horizon<Simulation_Timestep_Increment>();
				long t2 = this->template Planning_Time_Increment<ComponentType,CallerType,Simulation_Timestep_Increment>();
				long remain = (long)(t1 / t2);
				Simulation_Timestep_Increment departure_time = remain * this->template Planning_Time_Increment<ComponentType,CallerType,Simulation_Timestep_Increment>();

				// key the movement plan on the planning timestep just prior to departure
				Activity_Plans* activities = this->template Activity_Plans_Container<Activity_Plans*>();
				activities->push_back(act);
			}
		};
		// static member definition
		template<typename MasterType, typename ParentType, typename InheritanceList> ofstream General_Person_Planner_Implementation<MasterType, ParentType, InheritanceList>::logs[_num_threads];
		static_member_definition(General_Person_Planner_Implementation,write_activity_files);

		implementation struct ADAPTS_Person_Planner_Implementation : public General_Person_Planner_Implementation<MasterType, ParentType, APPEND_CHILD(ADAPTS_Person_Planner_Implementation)>
		{
			typedef General_Person_Planner_Implementation<MasterType, ParentType, APPEND_CHILD(ADAPTS_Person_Planner_Implementation)> base_type;

			feature_implementation void Initialize(requires(check(typename ComponentType::Parent_Type,Concepts::Is_Person)))
			{	
				this->template Generation_Time_Increment<ComponentType,CallerType,Time_Minutes>(15);
				this->template Planning_Time_Increment<ComponentType,CallerType,Time_Minutes>(15);

				// get reference to the parent pointer and set the first activity generation time to be the parent first iteration
				/*define_component_interface(parent_itf,typename base_type::type_of(Parent_Person),Prototypes::Person_Prototype,ComponentType);
				parent_itf* parent = this->template Parent_Person<ComponentType,CallerType,parent_itf*>();*/
				this->template Next_Activity_Generation_Time<ComponentType,CallerType,Time_Minutes>(_Parent_Person->template First_Iteration<Time_Minutes>());	
			}
			feature_implementation void Initialize(requires(check(typename ComponentType::Parent_Type,!Concepts::Is_Person)))
			{	
				assert_sub_check(typename ComponentType::Parent_Type,Concepts::Is_Person,Has_Initialize_Defined, "The specified ParentType is not a valid Person type.");
				assert_sub_check(typename ComponentType::Parent_Type,Concepts::Is_Person,Has_Properties_Defined, "The specified ParentType does not have the required Properties member defined.");
				assert_sub_check(typename ComponentType::Parent_Type,Concepts::Is_Person,Has_Planner_Defined, "The specified ParentType does not have the required Planner member defined.");
			}
			tag_feature_as_available(Initialize);

			feature_implementation void Activity_Generation()
			{
				// Create alias for this to use in conditional
				typedef Prototypes::Person_Planner<ComponentType, ComponentType> _Planning_Interface;
				_Planning_Interface* this_ptr=(_Planning_Interface*)this;

				// Generate average of 4 activities per day

				define_container_and_value_interface(Activity_Plans,Activity_Plan,typename base_type::type_of(Activity_Plans_Container),Associative_Container_Prototype,Activity_Components::Prototypes::Activity_Plan_Prototype,ComponentType);
				Activity_Plan* act = (Activity_Plan*)Allocate<typename base_type::type_of(Activity_Plans_Container)::unqualified_value_type>();
				act->template Activity_Plan_ID<long>(_iteration);
				Activity_Plans* activities = this_ptr->template Activity_Plans_Container<Activity_Plans*>();
				activities->insert(Simulation_Time.Future_Time<Time_Minutes,Simulation_Timestep_Increment>(15),act);

				define_container_and_value_interface(Movement_Plans,Movement_Plan,typename base_type::type_of(Movement_Plans_Container),Associative_Container_Prototype,Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);
				Movement_Plan* move = (Movement_Plan*)Allocate<typename base_type::type_of(Movement_Plans_Container)::unqualified_value_type>();
				move->template Movement_Plan_ID<long>(_iteration);
				Movement_Plans* movements = this_ptr->template Movement_Plans_Container<Movement_Plans*>();
				movements->insert(Simulation_Time.Future_Time<Time_Minutes,Simulation_Timestep_Increment>(30),move);
			}
			tag_feature_as_available(Activity_Generation);
		};

		implementation struct CTRAMP_Person_Planner_Implementation : public General_Person_Planner_Implementation<MasterType, ParentType, APPEND_CHILD(CTRAMP_Person_Planner_Implementation)>
		{
			// IMPLEMENTATION TYPEDEFS AND INTERFACES
			typedef General_Person_Planner_Implementation<MasterType, ParentType, APPEND_CHILD(CTRAMP_Person_Planner_Implementation)> base_type;

			feature_implementation void Initialize(requires(check(typename ComponentType::Parent_Type,Concepts::Is_Person)))
			{	
				base_type::template Generation_Time_Increment<ComponentType,CallerType,Time_Minutes>(END);
				base_type::template Planning_Time_Increment<ComponentType,CallerType,Time_Minutes>(5);
				// get reference to the parent pointer and set the first activity generation time to be the parent first iteration
				this->template Next_Activity_Generation_Time<ComponentType,CallerType,Time_Minutes>(_Parent_Person->template First_Iteration<Time_Minutes>());	
			}
			feature_implementation void Initialize(requires(check(typename ComponentType::Parent_Type,!Concepts::Is_Person)))
			{	
				assert_sub_check(typename ComponentType::Parent_Type,Concepts::Is_Person,Has_Initialize_Defined, "The specified ParentType is not a valid Person type.");
				assert_sub_check(typename ComponentType::Parent_Type,Concepts::Is_Person,Has_Properties_Defined, "The specified ParentType does not have the required Properties member defined.");
				assert_sub_check(typename ComponentType::Parent_Type,Concepts::Is_Person,Has_Planner_Defined, "The specified ParentType does not have the required Planner member defined.");
			}
			tag_feature_as_available(Initialize);

		};
		
		implementation struct TRANSIMS_Person_Planner_Implementation : public General_Person_Planner_Implementation<MasterType, ParentType, APPEND_CHILD(TRANSIMS_Person_Planner_Implementation)>
		{
			typedef General_Person_Planner_Implementation<MasterType, ParentType, APPEND_CHILD(TRANSIMS_Person_Planner_Implementation)> base_type;

			feature_implementation void Initialize(TargetType trip, requires(check(typename ComponentType::Parent_Type,Concepts::Is_Person)))
			{	
				// Set the event schedule parameters
				base_type::template Generation_Time_Increment<ComponentType,CallerType,Time_Minutes>(END);
				base_type::template Planning_Time_Increment<ComponentType,CallerType,Time_Minutes>(15);
				base_type::template Next_Activity_Generation_Time<ComponentType,CallerType,Time_Minutes>(END);

				// Create alias for this to use in conditional
				typedef Prototypes::Person_Planner<ComponentType, ComponentType> _Planning_Interface;
				_Planning_Interface* this_ptr=(_Planning_Interface*)this;

				// Add the trip input parameter to the movement schedule
				define_container_and_value_interface(Movement_Plans,Movement_Plan,typename base_type::type_of(Movement_Plans_Container),Associative_Container_Prototype,Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);
				Movement_Plans* movements = this_ptr->template Movement_Plans_Container<Movement_Plans*>();
				Movement_Plan* move = (Movement_Plan*)trip;
				this->template Add_Movement_Plan<ComponentType,CallerType,Movement_Plan*>(move);
			
			} 
			feature_implementation void Initialize(TargetType trip, requires(check(typename ComponentType::Parent_Type,!Concepts::Is_Person)))
			{	
				assert_check(typename ComponentType::Parent_Type,Concepts::Is_Person, "The specified ParentType is not a valid Person type.");
			}
			feature_implementation void Initialize()
			{	
				//static_assert(false, "\n\n\n[--------- Error, a TRANSIMS Planner Initialization must be called with an input trip parameter. ---------]\n\n");
			}
			tag_feature_as_available(Initialize);
		};

	}
}