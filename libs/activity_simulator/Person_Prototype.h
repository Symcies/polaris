#pragma once

#include "Activity_Simulator_Includes.h"
//#include "Activity_Prototype.h"
//#include "Network_Event_Prototype.h"
//#include "Activity_Location_Prototype.h"
//#include "Network_Prototype.h"
//#include "Zone_Prototype.h"
//#include "Movement_Plan_Prototype.h"
//#include "Network_Skimming_Prototype.h"
//#include "Person_Properties_Prototype.h"
//#include "Person_Planner_Prototype.h"
//#include "Person_Scheduler_Prototype.h"
//#include "Person_Perception_Prototype.h"
//#include "Person_Mover_Prototype.h"
//#include "Destination_Chooser_Prototype.h"

namespace Person_Components
{

namespace Concepts
{
	concept struct Is_Person
	{
		check_template_method_name(Has_Initialize_Defined,Initialize);
		check_template_method_name(Has_Properties_Defined,Properties);
		check_template_method_name(Has_Planner_Defined,Planning_Faculty);
		define_default_check(Has_Initialize_Defined && Has_Properties_Defined && Has_Planner_Defined);
	};

	concept struct Is_Transims_Person
	{
		check_concept(Is_Person_Check, Is_Person, T, V);
		check_typedef_type(Is_Transims_Check,Is_Transims,true_type);
		define_default_check(Is_Person_Check && Is_Transims_Check);
	};
	concept struct Is_CTRAMP_Person
	{
		check_concept(Is_Person_Check, Is_Person, T, V);
		check_typedef_type(Is_CTRAMP_Check,Is_Transims,true_type);
		define_default_check(Is_Person_Check && Is_CTRAMP_Check);
	};

	concept struct Has_Initialize
	{
		check_template_method_name(Has_Initialize_Defined,Initialize);
		define_default_check(Has_Initialize_Defined);
	};

}

namespace Types
{

}

namespace Prototypes
{
	prototype struct Person
	{
		tag_as_prototype;

		// Event handling
		static void Agent_Event_Controller(ComponentType* _this,Event_Response& response)
		{
			typedef Person_Planner<typename get_type_of(Planning_Faculty)> planner_itf;
			typedef Person<ComponentType> _Person_Interface;
			ComponentType* _pthis = (ComponentType*)_this;
			_Person_Interface* pthis =(_Person_Interface*)_pthis;
			planner_itf* planner = pthis->template Planning_Faculty<planner_itf*>();


			// First do the 'Set Locations Event', 
			if (iteration() == pthis->template First_Iteration<Simulation_Timestep_Increment>())
			{
				Simulation_Timestep_Increment first_plan_time = planner->template Next_Planning_Time<Simulation_Timestep_Increment>() + planner->template Planning_Time_Increment<Simulation_Timestep_Increment>();
				response.next._iteration = first_plan_time;
				response.next._sub_iteration = 0;
				pthis->Set_Locations_Event<NT>();
			}
			// then, prior to the first planning period, dump all preplanned activities to file
			else
			{	
				//_pthis->Swap_Event((Event)&Person::Print_Preplanned_Activities_Event<NULLTYPE>);
				response.next._iteration = END;
				response.next._sub_iteration = 0;
				pthis->Print_Preplanned_Activities_Event<NT>();
			}
		}
		template<typename TargetType> void Set_Locations_Event()
		{
			typedef Activity_Location_Components::Prototypes::Activity_Location<typename get_type_of(current_location)> location_itf;

			// set the home/workplace/school locations on event
			this_component()->template Set_Locations< TargetType>();

			// start the agent off at home
			this->current_location<location_itf*>(this->Home_Location<location_itf*>());
		}
		template<typename T> void Print_Preplanned_Activities_Event()
		{
			typedef Person<ComponentType> _Person_Interface;
			_Person_Interface* pthis =(_Person_Interface*)this;
			typedef Person_Scheduler<typename get_type_of(Scheduling_Faculty)> scheduler_itf;
			typedef Scenario_Components::Prototypes::Scenario<typename get_type_of(scenario_reference)> scenario_itf;

			typedef Back_Insertion_Sequence< typename scheduler_itf::get_type_of(Activity_Container)> Activities;
			typedef Activity_Components::Prototypes::Activity_Planner<typename get_component_type(Activities)>  Activity;
			
			typedef Back_Insertion_Sequence< typename get_type_of(Activity_Record_Container)> Activity_Records;
			typedef Activity_Components::Prototypes::Activity_Planner<typename get_component_type(Activity_Records)>  Activity_Record;
			
			
			typedef  Person_Components::Prototypes::Person_Data_Logger< typename ComponentType::person_data_logger_type> _Logger_Interface;
			

			scheduler_itf* scheduler = pthis->template Scheduling_Faculty<scheduler_itf*>();
			Activities* activities = scheduler->template Activity_Container<Activities*>();
			Activity_Records* activity_records = pthis->template Activity_Record_Container<Activity_Records*>();

			for (typename Activities::iterator itr = activities->begin(); itr != activities->end(); ++itr)
			{
				//cout << endl <<"Person ID: " << (*itr)->Parent_ID<int>() << "Activity Type: " << (*itr)->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>();
				//((_Logger_Interface*)_global_person_logger)->template Add_Record<Activity*>(*itr,false);

				// store activity records in the person activity record container.
				Activity_Record* new_record = (Activity_Record*)Allocate<typename get_component_type(Activity_Records)>();
				new_record->Initialize<Activity*>(*itr);
				activity_records->push_back(new_record);
			}
			
			pthis->Sort_Activity_Records<void>();

			//// exit if no activity output is specified
			//scenario_itf* scenario = (scenario_itf*)_global_scenario;
			//if (!scenario->template write_activity_output<bool>()) return;


			// push the start-of-day at home activity to the output database
			typename Activities::iterator itr = activities->begin();
			Activity* act = (Activity*)(*itr);
			if (act->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::AT_HOME_ACTIVITY)
			{
				((_Logger_Interface*)_global_person_logger)->template Add_Record<Activity*>(act,true);
			}
		}

		// Initializers
		template<typename TargetType> void Initialize(TargetType id, requires(TargetType,check(ComponentType,Concepts::Has_Initialize) && check_2(typename ComponentType::Object_Type,Execution_Object,is_same)))
		{
			this->First_Iteration<Simulation_Timestep_Increment>(iteration()+1);

			int starting_subiteration = (int)(GLOBALS::Uniform_RNG.Next_Rand<float>()*30.0);

			this_component()->template Initialize< TargetType>(id);	

			//load_event(ComponentType,Agent_Conditional,Set_Locations_Event,this->First_Iteration<Simulation_Timestep_Increment>(),starting_subiteration,NULLTYPE);
			((ComponentType*)this)->Load_Event<ComponentType>(&Agent_Event_Controller,this->First_Iteration<Simulation_Timestep_Increment>(),starting_subiteration);
		}
		template<typename TargetType> void Initialize(TargetType id, requires(TargetType,check(ComponentType,Concepts::Has_Initialize) && check_2(typename ComponentType::Object_Type,Data_Object,is_same)))
		{
			this_component()->template Initialize< TargetType>(id);	
		}
		template<typename TargetType> void Initialize(TargetType id, requires(TargetType,!check(ComponentType,Concepts::Has_Initialize) || (check_2(typename ComponentType::Object_Type,Execution_Object,is_same) && check_2(typename ComponentType::Object_Type,Data_Object,is_same))))
		{
			assert_check(ComponentType,Concepts::Has_Initialize,"This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");
			assert_check_2(typename ComponentType::Object_Type,Execution_Object,is_same, "ComponentType must be an execution object, or ");
			assert_check_2(typename ComponentType::Object_Type,Data_Object,is_same, "ComponentType must be an data object.");
		}
		template<typename IdType, typename SynthesisZoneType, typename NetworkRefType, typename ScenarioRefType> void Initialize(IdType id, SynthesisZoneType home_zone, NetworkRefType network_ref, ScenarioRefType scenario_ref,requires(IdType,check(ComponentType,Concepts::Has_Initialize) && check_2(typename ComponentType::Object_Type,Execution_Object,is_same)))
		{
			this->First_Iteration<Simulation_Timestep_Increment>(iteration()+1);
			int starting_subiteration = (int)(GLOBALS::Uniform_RNG.Next_Rand<float>()*30.0);

			this_component()->template Initialize< IdType, SynthesisZoneType, NetworkRefType, ScenarioRefType>(id, home_zone, network_ref, scenario_ref);		

			//load_event(ComponentType,Agent_Conditional,Set_Locations_Event,this->First_Iteration<Simulation_Timestep_Increment>(),starting_subiteration,NULLTYPE);
			((ComponentType*)this)->Load_Event<ComponentType>(&Agent_Event_Controller,this->First_Iteration<Simulation_Timestep_Increment>(),starting_subiteration);
		}
		template<typename IdType, typename SynthesisZoneType, typename NetworkRefType, typename ScenarioRefType> void Initialize(IdType id, SynthesisZoneType home_zone, NetworkRefType network_ref, ScenarioRefType scenario_ref,requires(IdType,check(ComponentType,Concepts::Has_Initialize) && check_2(typename ComponentType::Object_Type,Data_Object,is_same)))
		{
			this_component()->template Initialize< IdType, SynthesisZoneType, NetworkRefType, ScenarioRefType>(id, home_zone, network_ref, scenario_ref);		
		}
		template<typename IdType, typename SynthesisZoneType, typename NetworkRefType, typename ScenarioRefType> void Initialize(IdType id, SynthesisZoneType home_zone, NetworkRefType network_ref, ScenarioRefType scenario_ref,requires(IdType,!check(ComponentType,Concepts::Has_Initialize) || (!check_2(typename ComponentType::Object_Type,Execution_Object,is_same) && !check_2(typename ComponentType::Object_Type,Data_Object,is_same))))
		{
			assert_check(ComponentType,Concepts::Has_Initialize,"This ComponentType is not a valid Agent, does not have an initializer.   Did you forget to use tag_feature_as_available macro?");
			assert_check_2(typename ComponentType::Object_Type,Execution_Object,is_same, "ComponentType must be an execution object, or ");
			assert_check_2(typename ComponentType::Object_Type,Data_Object,is_same, "ComponentType must be an data object.");
		}

		// Sub-component accessors	
		accessor(Household, NONE, NONE);
		accessor(Planning_Faculty, NONE, NONE);
		accessor(Perception_Faculty, NONE, NONE);
		accessor(Scheduling_Faculty, NONE, NONE);
		accessor(router, NONE, NONE);
		accessor(Moving_Faculty, NONE, NONE);
		accessor(Properties, NONE, NONE);
		accessor(Static_Properties, NONE, NONE);
		accessor(vehicle, NONE, NONE);
		accessor(has_done_replanning, NONE, NONE);
		
		accessor(has_pretrip_information, NONE, NONE);

		// External knowledge accessor
		accessor(network_reference, NONE, NONE);
		accessor(scenario_reference, NONE, NONE);


		// Basic property members
		accessor(uuid, NONE, NONE);
		accessor(internal_id, NONE, NONE);
		accessor(Next_Rand, NONE, NONE);
		accessor(First_Iteration, NONE, NONE);
		accessor(Write_Activity_Files, NONE, NONE);
		accessor(Activity_Record_Container, NONE, NONE);
		accessor(current_location, NONE, NONE);

		// Database record for this person
		accessor(person_record, NONE, NONE);

		template<typename TargetType> void Arrive_At_Destination()
		{
			typedef Person_Mover< typename get_type_of(Moving_Faculty)> mover_itf;
			mover_itf* mover = this->Moving_Faculty<mover_itf*>();
			mover->template Arrive_At_Destination<TargetType>();
		}

		// Accessors for setting the home/work locations (stores only an index into the network_reference::activity_locations_container) - overloaded to return either th loc_index, the location interface or the zone interface
		template<typename TargetType> TargetType Home_Location()
		{
			typedef Household_Components::Prototypes::Household<typename get_type_of(Household)> household_itf;
			household_itf* household = this->Household<household_itf*>();
			return household->template Home_Location<TargetType>();					
		}
		/*template<typename TargetType> TargetType Home_Location(requires(TargetType,check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location) && check(strip_modifiers(TargetType),is_pointer)))
		{
			typedef Household_Components::Prototypes::Household<typename get_type_of(Household)> household_itf;
			typedef Person_Properties<typename get_type_of(Properties)> properties_itf;
			typedef Person_Perception<typename get_type_of(Perception_Faculty)> perception_itf;
			typedef Network_Components::Prototypes::Network< typename perception_itf::get_type_of(Network)> network_itf;

			typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer< typename network_itf::get_type_of(activity_locations_container)::value_type>::type>  activity_location_itf;
			typedef  Random_Access_Sequence< typename network_itf::get_type_of(activity_locations_container), activity_location_itf*> activity_locations_container_itf;

			properties_itf* properties = this->Properties<properties_itf*>();
			network_itf* network = this->Perception_Faculty<perception_itf*>()->template Network<network_itf*>();
			activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();

			household_itf* household = this->Household<household_itf*>();
			
			int loc_id = properties->template home_location_id<int>();
			return (TargetType)(*locations)[loc_id];						
		}
		template<typename TargetType> TargetType Home_Location(requires(TargetType,check(strip_modifiers(TargetType), Zone_Components::Concepts::Is_Zone) && check(strip_modifiers(TargetType),is_pointer)))
		{
			typedef Person_Properties<typename get_type_of(Properties)> properties_itf;
			typedef Network_Components::Prototypes::Network< typename get_type_of(network_reference)> network_itf;
			typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer< typename network_itf::get_type_of(activity_locations_container)::value_type>::type>  activity_location_itf;
			typedef  Random_Access_Sequence< typename network_itf::get_type_of(activity_locations_container), activity_location_itf*> activity_locations_container_itf;

			
			properties_itf* properties = this->Properties<properties_itf*>();
			network_itf* network = this->network_reference<network_itf*>();
			activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();

			int loc_id = properties->template home_location_id<int>();
			activity_location_itf* loc = (*locations)[loc_id];	
			return loc->template zone<TargetType>();
		}
		template<typename TargetType> TargetType Home_Location(requires(TargetType,check(strip_modifiers(TargetType), is_integral)))
		{
			typedef Person_Properties<typename get_type_of(Properties)> properties_itf;
			properties_itf* properties = this->Properties<properties_itf*>();
			return properties->template home_location_id<TargetType>();
		}
		template<typename TargetType> TargetType Home_Location(requires(TargetType,check(strip_modifiers(TargetType), !is_integral) && ( (check(strip_modifiers(TargetType),!Activity_Location_Components::Concepts::Is_Activity_Location) && check(strip_modifiers(TargetType),!Zone_Components::Concepts::Is_Zone) ) || check(strip_modifiers(TargetType),!is_pointer) ) ) )
		{
			assert_check(strip_modifiers(TargetType),is_integral,"Error, Home_Location can only be requested as an Integral type - which returns location index, or as an Activity_Location refernence type, which returns the actual location.");
		}
	*/	//template<typename TargetType> void Home_Location(TargetType location_index, requires(TargetType,check(strip_modifiers(TargetType), is_integral)))
		//{
		//	typedef Person_Properties<typename get_type_of(Properties)> properties_itf;
		//	typedef Network_Components::Prototypes::Network< typename get_type_of(network_reference)> network_itf;
		//	typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer< typename network_itf::get_type_of(activity_locations_container)::value_type>::type>  activity_location_itf;
		//	typedef  Random_Access_Sequence< typename network_itf::get_type_of(activity_locations_container), activity_location_itf*> activity_locations_container_itf;

		//	properties_itf* properties = this->Properties<properties_itf*>();
		//	network_itf* network = this->network_reference<network_itf*>();
		//	activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();

		//	if (location_index < 0 || location_index >= locations->size()) THROW_EXCEPTION("Error: location index "<<location_index<<" does not exist in network locations container.  Index out of range (0,"<<locations->size()<<").");
		//	properties->template home_location_id<TargetType>(location_index);
		//}
		//template<typename TargetType> void Home_Location(TargetType location_index, requires(TargetType,check(strip_modifiers(TargetType), !is_integral)))
		//{
		//	assert_check(strip_modifiers(TargetType), is_integral, "Error, Home_Location can only be set by passing an integral index from network::activity_locations_container");
		//}
		
		template<typename TargetType> TargetType Work_Location(requires(TargetType,check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location) && check(TargetType,is_pointer)))
		{
			typedef Person_Properties<typename get_type_of(Properties)> properties_itf;
			typedef Network_Components::Prototypes::Network< typename get_type_of(network_reference)> network_itf;

			typedef Random_Access_Sequence< typename network_itf::get_type_of(activity_locations_container)> activity_locations_container_itf;
			typedef Activity_Location_Components::Prototypes::Activity_Location<typename get_component_type(activity_locations_container_itf)>  activity_location_itf;
			
			properties_itf* properties = this->Properties<properties_itf*>();
			network_itf* network = this->network_reference<network_itf*>();
			activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();

			int loc_id = properties->template work_location_id<int>();
			if (loc_id < 0)
			{
				THROW_WARNING("Warning: Person '" << this->uuid<int>() << "' does not have a valid work location.  Should not be requesting this.");
				return nullptr;
			}
			return (TargetType)(*locations)[loc_id];						
		}
		template<typename TargetType> TargetType Work_Location(requires(TargetType,check(strip_modifiers(TargetType), Zone_Components::Concepts::Is_Zone) && check(TargetType,is_pointer)))
		{
			typedef Person_Properties<typename get_type_of(Properties)> properties_itf;
			typedef Network_Components::Prototypes::Network< typename get_type_of(network_reference)> network_itf;
			
			typedef Random_Access_Sequence< typename network_itf::get_type_of(activity_locations_container)> activity_locations_container_itf;
			typedef Activity_Location_Components::Prototypes::Activity_Location<typename get_component_type(activity_locations_container_itf)>  activity_location_itf;
			
			
			properties_itf* properties = this->Properties<properties_itf*>();
			network_itf* network = this->network_reference<network_itf*>();
			activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();

			int loc_id = properties->template work_location_id<int>();
			if (loc_id < 0 || loc_id >= locations->size())
			{
				THROW_WARNING("Warning: Person '" << this->uuid<int>() << "' does not have a valid work location.  Should not be requesting this.");
				return nullptr;
			}
			activity_location_itf* loc = (activity_location_itf*)((*locations)[loc_id]);	
			TargetType z = loc->template zone<TargetType>();
			if (z == nullptr)
			{
				THROW_WARNING("Warning: Person '" << this->uuid<int>() << "' work location is not associated with a Traffic Analysis Zone.  Location reset to home location");
				return nullptr;
			}
			return loc->template zone<TargetType>();
		}
		template<typename TargetType> TargetType Work_Location(requires(TargetType,check(strip_modifiers(TargetType), is_integral)))
		{
			typedef Person_Properties<typename get_type_of(Properties)> properties_itf;
			properties_itf* properties = this->Properties<properties_itf*>();
			return properties->template work_location_id<TargetType>();
		}
		template<typename TargetType> TargetType Work_Location(requires(TargetType,check(strip_modifiers(TargetType), !is_integral) && ( (check(strip_modifiers(TargetType),!Activity_Location_Components::Concepts::Is_Activity_Location) && check(strip_modifiers(TargetType),!Zone_Components::Concepts::Is_Zone) ) || check(TargetType,!is_pointer) ) ) )
		{
			assert_check(strip_modifiers(TargetType),is_integral,"Error, Home_Location can only be requested as an Integral type - which returns location index, or as an Activity_Location refernence type, which returns the actual location.");
		}
		template<typename TargetType> void Work_Location(TargetType location, requires(TargetType,check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location) && check(TargetType,is_pointer)))
		{
			typedef Person_Properties<typename get_type_of(Properties)> properties_itf;
			typedef Network_Components::Prototypes::Network< typename get_type_of(network_reference)> network_itf;

			typedef Random_Access_Sequence< typename network_itf::get_type_of(activity_locations_container)> activity_locations_container_itf;
			typedef Activity_Location_Components::Prototypes::Activity_Location<typename get_component_type(activity_locations_container_itf)>  activity_location_itf;
			
			properties_itf* properties = this->Properties<properties_itf*>();
			network_itf* network = this->network_reference<network_itf*>();
			activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();
			int location_index = location->internal_id<int>();
			properties->template work_location_id<int>(location_index);					
		}
		template<typename TargetType> void Work_Location(TargetType location_index, requires(TargetType,check(strip_modifiers(TargetType), is_integral)))
		{
			typedef Person_Properties<typename get_type_of(Properties)> properties_itf;
			typedef Network_Components::Prototypes::Network< typename get_type_of(network_reference)> network_itf;

			typedef Random_Access_Sequence< typename network_itf::get_type_of(activity_locations_container)> activity_locations_container_itf;
			typedef Activity_Location_Components::Prototypes::Activity_Location<typename get_component_type(activity_locations_container_itf)>  activity_location_itf;
			
			properties_itf* properties = this->Properties<properties_itf*>();
			network_itf* network = this->network_reference<network_itf*>();
			activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();

			if (location_index < -1 || location_index >= (signed int)locations->size()) THROW_EXCEPTION("Error: location index "<<location_index<<" does not exist in network locations container.");
			properties->template work_location_id<TargetType>(location_index);
		}
		template<typename TargetType> void Work_Location(TargetType location_index, requires(TargetType,!check(strip_modifiers(TargetType), is_integral) && (!check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location) || !check(TargetType,is_pointer))))
		{
			assert_check(strip_modifiers(TargetType), is_integral, "Error, work location can only be set by passing an integral index from network::activity_locations_container");
			assert_check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location, "Error, work location can only be set by passing an activity_location prototype");
			assert_check(TargetType, is_pointer, "Error, work location can only be set by passing a pointer to an activity location prototype, target type is not a pointer type.");
		}

		template<typename TargetType> TargetType School_Location(requires(TargetType,check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location) && check(TargetType,is_pointer)))
		{
			typedef Person_Properties<typename get_type_of(Properties)> properties_itf;
			typedef Network_Components::Prototypes::Network< typename get_type_of(network_reference)> network_itf;
			
			typedef Random_Access_Sequence< typename network_itf::get_type_of(activity_locations_container)> activity_locations_container_itf;
			typedef Activity_Location_Components::Prototypes::Activity_Location<typename get_component_type(activity_locations_container_itf)>  activity_location_itf;
			
			properties_itf* properties = this->Properties<properties_itf*>();
			network_itf* network = this->network_reference<network_itf*>();
			activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();

			int loc_id = properties->template school_location_id<int>();
			if (loc_id < 0)
			{
				THROW_WARNING("Warning: Person '" << this->uuid<int>() << "' does not have a valid school location.  Should not be requesting this.");
				return nullptr;
			}
			return (TargetType)(*locations)[loc_id];						
		}
		template<typename TargetType> TargetType School_Location(requires(TargetType,check(strip_modifiers(TargetType), Zone_Components::Concepts::Is_Zone) && check(TargetType,is_pointer)))
		{
			typedef Person_Properties<typename get_type_of(Properties)> properties_itf;
			typedef Network_Components::Prototypes::Network< typename get_type_of(network_reference)> network_itf;
			typedef Random_Access_Sequence< typename network_itf::get_type_of(activity_locations_container)> activity_locations_container_itf;
			typedef Activity_Location_Components::Prototypes::Activity_Location<typename get_component_type(activity_locations_container_itf)>  activity_location_itf;
			
			
			properties_itf* properties = this->Properties<properties_itf*>();
			network_itf* network = this->network_reference<network_itf*>();
			activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();

			int loc_id = properties->template school_location_id<int>();
			if (loc_id < 0)
			{
				THROW_WARNING("Warning: Person '" << this->uuid<int>() << "' does not have a valid school location.  Should not be requesting this.");
				return nullptr;
			}
			activity_location_itf* loc = (*locations)[loc_id];	
			return loc->template zone<TargetType>();
		}
		template<typename TargetType> TargetType School_Location(requires(TargetType,check(strip_modifiers(TargetType), is_integral)))
		{
			typedef Person_Properties<typename get_type_of(Properties)> properties_itf;
			properties_itf* properties = this->Properties<properties_itf*>();
			return properties->template school_location_id<TargetType>();
		}
		template<typename TargetType> TargetType School_Location(requires(TargetType,check(strip_modifiers(TargetType), !is_integral) && ((check(strip_modifiers(TargetType),!Activity_Location_Components::Concepts::Is_Activity_Location) && check(strip_modifiers(TargetType),!Zone_Components::Concepts::Is_Zone) ) || check(TargetType,!is_pointer) ) ) )
		{
			assert_check(strip_modifiers(TargetType),is_integral,"Error, School_Location can only be requested as an Integral type - which returns location index, or as an Activity_Location refernence type, which returns the actual location.");
		}
		template<typename TargetType> void School_Location(TargetType location_index, requires(TargetType,check(strip_modifiers(TargetType), is_integral)))
		{
			typedef Person_Properties<typename get_type_of(Properties)> properties_itf;
			typedef Network_Components::Prototypes::Network< typename get_type_of(network_reference)> network_itf;
			typedef Random_Access_Sequence< typename network_itf::get_type_of(activity_locations_container)> activity_locations_container_itf;
			typedef Activity_Location_Components::Prototypes::Activity_Location<typename get_component_type(activity_locations_container_itf)>  activity_location_itf;
			
			properties_itf* properties = this->Properties<properties_itf*>();
			network_itf* network = this->network_reference<network_itf*>();
			activity_locations_container_itf* locations = network->template activity_locations_container<activity_locations_container_itf*>();

			if (location_index < -1 || location_index >= (signed int)locations->size()) THROW_EXCEPTION("Error: location index "<<location_index<<" does not exist in network locations container.");
			properties->template school_location_id<TargetType>(location_index);
		}
		template<typename TargetType> void School_Location(TargetType location_index, requires(TargetType,check(strip_modifiers(TargetType), !is_integral)))
		{
			assert_check(strip_modifiers(TargetType), is_integral, "Error, work location can only be set by passing an integral index from network::activity_locations_container");
		}
		
		local_check_template_method_name(has_Choose_Work_Location,Choose_Work_Location);
		template<typename TargetType> void Choose_Work_Location(/*requires(TargetType,check(ComponentType,has_Choose_Work_Location))*/)
		{
			assert_check(ComponentType,has_Choose_Work_Location,"ComponentType does not have Choose_Work_Location feature.");

			this_component()->template Choose_Work_Location<TargetType>();
		}
		//template<typename TargetType> void Choose_Work_Location(requires(TargetType,!check(ComponentType,has_Choose_Work_Location)))
		//{
		//	assert_check(ComponentType,has_Choose_Work_Location,"ComponentType does not have Choose_Work_Location feature.");
		//}

		local_check_template_method_name(has_Choose_School_Location,Choose_School_Location);
		template<typename TargetType> void Choose_School_Location(requires(TargetType,check(ComponentType,has_Choose_School_Location)))
		{
			assert_check(ComponentType,has_Choose_School_Location,"ComponentType does not have Choose_School_Location feature.");

			this_component()->template Choose_School_Location<TargetType>();
		}
		//template<typename TargetType> void Choose_School_Location(requires(TargetType,!check(ComponentType,has_Choose_School_Location)))
		//{
		//	assert_check(ComponentType,has_Choose_School_Location,"ComponentType does not have Choose_School_Location feature.");
		//}

		template<typename TargetType> string To_String()
		{
			typedef Person_Properties<typename get_type_of(Properties)> properties_itf;
			typedef Person_Properties<typename get_type_of(Static_Properties)> static_properties_itf;
			typedef Network_Components::Prototypes::Network< typename get_type_of(network_reference)> network_itf;
			
			typedef Pair_Associative_Container< typename network_itf::get_type_of(zones_container)> zone_container_itf;
			typedef Zone_Components::Prototypes::Zone<typename get_mapped_component_type(zone_container_itf)>  zone_itf;
			
			typedef Random_Access_Sequence< typename network_itf::get_type_of(activity_locations_container)> locations_container_interface;
			typedef Activity_Location_Components::Prototypes::Activity_Location<typename get_component_type(activity_locations_container_itf)>  location_interface;
			

			properties_itf* props = this->Properties<properties_itf*>();
			static_properties_itf* static_props = this->Static_Properties<static_properties_itf*>();

			stringstream s;
			s << this->uuid<int>() << ", HOME_ZONE,";
			s << this->Home_Location<location_interface*>()->template zone<zone_itf*>()->template uuid<int>();
			s << ", HOME_LOC,";
			s << this->Home_Location<location_interface*>()->template uuid<int>();
			if (static_props->template Is_Employed<bool>())
			{
				//cout << endl << "Work location id: " << this->Work_Location<int>()<<endl;
				s << ", WORK_ZONE," << this->Work_Location<zone_itf*>()->template uuid<int>() << ", WORK_LOC,"<<this->Work_Location<location_interface*>()->template uuid<int>();
			}
			else
			{
				s << ", DOESNT_WORK,,,";
			}
			if (static_props->template Is_Student<bool>())
			{
				s << ", SCHOOL_ZONE," << this->School_Location<zone_itf*>()->template uuid<int>()  << ", SCHOOL_LOC,"<<this->School_Location<location_interface*>()->template uuid<int>();
			}
			else
			{
				s << ", NOT_IN_SCHOOL,,,";
			}
			return s.str();
		}
		template<typename TargetType> void Sort_Activity_Records()
		{
			this_component()->Sort_Activity_Records<TargetType>();
		}


		// PASS-THROUGH FEATURES OF SUB-COMPONENTS
		template<typename TargetType> TargetType current_movement_plan()
		{
			typedef Prototypes::Person_Scheduler<typename get_type_of(Scheduling_Faculty)> scheduler_itf;
			scheduler_itf* scheduler = this->Scheduling_Faculty<scheduler_itf*>();
			return scheduler->template current_movement_plan<TargetType>();
		}
		template<typename TargetType> TargetType current_activity_plan()
		{
			typedef Prototypes::Person_Scheduler<typename get_type_of(Scheduling_Faculty)> scheduler_itf;
			scheduler_itf* scheduler = this->Scheduling_Faculty<scheduler_itf*>();
			return scheduler->template current_activity_plan<TargetType>();
		}
		template<typename ParamType, typename ReturnType> ReturnType next_activity_plan(ParamType current_time_or_activity)
		{
			typedef Prototypes::Person_Scheduler<typename get_type_of(Scheduling_Faculty)> scheduler_itf;
			scheduler_itf* scheduler = this->Scheduling_Faculty<scheduler_itf*>();
			return scheduler->template next_activity_plan<ParamType, ReturnType>(current_time_or_activity);
		}
		template<typename ParamType, typename ReturnType> typename ReturnType previous_activity_plan(ParamType current_time)
		{
			typedef Prototypes::Person_Scheduler<typename get_type_of(Scheduling_Faculty)> scheduler_itf;
			scheduler_itf* scheduler = this->Scheduling_Faculty<scheduler_itf*>();
			return scheduler->template previous_activity_plan<ParamType, ReturnType>(current_time);
		}
		template<typename TargetType> void Add_Movement_Plan(TargetType movement_plan)
		{
			this_component()->template Add_Movement_Plan<TargetType>(movement_plan);
		}
		template<typename TargetType> void Add_Activity_Plan(TargetType activity_plan)
		{
			this_component()->template Add_Activity_Plan<TargetType>(activity_plan);
		}
	};

}

}