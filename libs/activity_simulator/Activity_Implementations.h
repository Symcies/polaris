#pragma once
#include "Activity_Prototype.h"


namespace Activity_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		//=======================================================================
		// Activity Classes
		//-----------------------------------------------------------------------
		implementation struct Basic_Activity_Plan_Implementation : public Polaris_Component<MasterType,INHERIT(Basic_Activity_Plan_Implementation),Execution_Object,true>
		{
			// Tag As Implementation
			typedef typename Polaris_Component<MasterType,INHERIT(Basic_Activity_Plan_Implementation),Execution_Object>::Component_Type ComponentType;


			//=================================================================
			// Pointer back to planner
			m_prototype(Person_Components::Prototypes::Person_Planner< typename MasterType::person_planner_type>, Parent_Planner, NONE, NONE);

			// pointer to movement plan associated with activity
			m_prototype(Movement_Plan_Components::Prototypes::Movement_Plan< typename MasterType::movement_plan_type>, movement_plan, NONE, NONE);
			//m_prototype(Movement_Plan_Components::Prototypes::Movement_Plan< typename MasterType::movement_plan_record_type>, movement_record, NONE, NONE);

			static int Route_Planning_Counter[num_sim_threads()];
			static m_data(int, Route_Planning_Count, NONE, NONE);
			static m_data(_lock, update_lock, NONE, NONE);

			//================================================================================================================================================================================================
			//================================================================================================================================================================================================
			// Interfaces
			typedef Prototypes::Activity_Planner<ComponentType> this_itf;
			typedef  Person_Components::Prototypes::Person< typename type_of(Parent_Planner)::type_of(Parent_Person)> _person_itf;
			typedef  Household_Components::Prototypes::Household< typename _person_itf::get_type_of(Household)> _household_itf;
			typedef  Person_Components::Prototypes::Person_Scheduler< typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(Scheduling_Faculty)> _scheduler_itf;
			typedef  Person_Components::Prototypes::Person_Properties< typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(Properties)> _properties_itf;
			typedef Person_Components::Prototypes::Person_Planner<type_of(Parent_Planner)> _planning_itf;
			typedef Person_Components::Prototypes::Destination_Chooser<typename _planning_itf::get_type_of(Destination_Choice_Faculty)> _dest_choice_itf;
			typedef  Scenario_Components::Prototypes::Scenario< typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference)> _scenario_itf;
			typedef  Network_Components::Prototypes::Network< typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(network_reference)> _network_itf;
			typedef Network_Skimming_Components::Prototypes::Network_Skimming< typename _network_itf::get_type_of(skimming_faculty)> _skim_itf;
			typedef  Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer< typename _network_itf::get_type_of(activity_locations_container)::value_type>::type>  _activity_location_itf;
			typedef  Random_Access_Sequence< typename _network_itf::get_type_of(activity_locations_container), _activity_location_itf*> _activity_locations_container_itf;

			typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename _activity_location_itf::get_type_of(origin_links)::value_type>::type>  _link_itf;
			typedef  Random_Access_Sequence< typename _activity_location_itf::get_type_of(origin_links), _link_itf*> _links_container_itf;

			typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _link_itf::get_type_of(outbound_turn_movements)::value_type>::type>  _turn_itf;
			typedef  Random_Access_Sequence< typename _link_itf::get_type_of(outbound_turn_movements), _turn_itf*> _turns_container_itf;

			typedef  Zone_Components::Prototypes::Zone<typename remove_pointer< typename _network_itf::get_type_of(zones_container)::value_type>::type>  _zone_itf;
			typedef  Pair_Associative_Container< typename _network_itf::get_type_of(zones_container), _zone_itf*> _zones_container_itf;

			typedef Activity_Components::Prototypes::Activity_Planner<typename remove_pointer< typename _scheduler_itf::get_type_of(Activity_Container)::value_type>::type> _activity_plan_itf;
			typedef Back_Insertion_Sequence< typename _scheduler_itf::get_type_of(Activity_Container),_activity_plan_itf*> _activity_plans_container_itf;

			typedef Movement_Plan_Components::Prototypes::Movement_Plan<typename remove_pointer< typename _scheduler_itf::get_type_of(Movement_Plans_Container)::value_type>::type> _movement_plan_itf;
			typedef Back_Insertion_Sequence< typename _scheduler_itf::get_type_of(Movement_Plans_Container),_movement_plan_itf*> _movement_plans_container_itf;

			typedef Prototypes::Activity_Planner<typename MasterType::at_home_activity_plan_type> _home_activity_itf;
			//================================================================================================================================================================================================
			//================================================================================================================================================================================================

		
			// Fundamental activity properties
			m_data(int, Activity_Plan_ID, NONE, NONE);
			m_data(Types::ACTIVITY_TYPES, Activity_Type, NONE, NONE);
			template<typename TargetType> TargetType Parent_ID()
			{
				return this->_Parent_Planner->template Parent_Person<_person_itf*>()->template uuid<TargetType>();
			}	tag_getter_as_available(Parent_ID);
			
			m_data(Types::FLEXIBILITY_VALUES, Location_Flexibility, NONE, NONE);
			m_data(Types::FLEXIBILITY_VALUES, Mode_Flexibility, NONE, NONE);
			m_data(Types::FLEXIBILITY_VALUES, Start_Time_Flexibility, NONE, NONE);
			m_data(Types::FLEXIBILITY_VALUES, Duration_Flexibility, NONE, NONE);
			m_data(Types::FLEXIBILITY_VALUES, Involved_Persons_Flexibility, NONE, NONE);

			// Activity attributes
			m_prototype(Activity_Location_Components::Prototypes::Activity_Location_Prototype< typename MasterType::activity_location_type>, Location, check(strip_modifiers(TargetType),Activity_Location_Components::Concepts::Is_Activity_Location), check(strip_modifiers(TargetType),Activity_Location_Components::Concepts::Is_Activity_Location));
			m_data(Vehicle_Components::Types::Vehicle_Type_Keys, Mode, NONE, NONE);
			member_component_and_feature_accessor(Start_Time, Value, Basic_Units::Prototypes::Time,Basic_Units::Implementations::Time_Implementation<NT>)
			member_component_and_feature_accessor(Duration, Value, Basic_Units::Prototypes::Time,Basic_Units::Implementations::Time_Implementation<NT>)
			member_component_and_feature_accessor(Expected_Travel_Time, Value, Basic_Units::Prototypes::Time, Basic_Units::Implementations::Time_Implementation<NT>);
			member_component_and_feature_accessor(Actual_Travel_Time, Value, Basic_Units::Prototypes::Time, Basic_Units::Implementations::Time_Implementation<NT>);
			m_container(boost::container::vector<typename MasterType::person_type*>, Involved_Persons_Container, NONE, NONE);
			template<typename TargetType> TargetType End_Time()
			{
				this_itf* pthis = (this_itf*)this;
				return pthis->template Start_Time<TargetType>() + pthis->template Duration<TargetType>();
			}	tag_getter_as_available(End_Time);
			template<typename TargetType> void End_Time(TargetType value, bool maintain_duration)
			{
				this_itf* pthis = (this_itf*)this;

				if (maintain_duration) pthis->template Start_Time<TargetType>(value - pthis->template Duration<TargetType>());
				else
				{
					if (value < pthis->template Start_Time<TargetType>()) 
					{
						THROW_WARNING("WARNING: new end time less than start time is not possible if start time is fixed (maintain_duration=false)");
					}
					else pthis->template Duration<TargetType>(value - pthis->template Start_Time<TargetType>());
				}
			}	tag_setter_as_available(End_Time);

			// Planning event time members
			m_data(Revision,Location_Planning_Time,check_2(strip_modifiers(TargetType),Revision,is_same), check_2(strip_modifiers(TargetType),Revision,is_same));
			m_data(Revision,Mode_Planning_Time,check_2(strip_modifiers(TargetType),Revision,is_same), check_2(strip_modifiers(TargetType),Revision,is_same));
			m_data(Revision,Start_Time_Planning_Time,check_2(strip_modifiers(TargetType),Revision,is_same), check_2(strip_modifiers(TargetType),Revision,is_same));
			m_data(Revision,Duration_Planning_Time,check_2(strip_modifiers(TargetType),Revision,is_same), check_2(strip_modifiers(TargetType),Revision,is_same));
			m_data(Revision,Involved_Persons_Planning_Time,check_2(strip_modifiers(TargetType),Revision,is_same), check_2(strip_modifiers(TargetType),Revision,is_same));
			m_data(Revision,Route_Planning_Time,check_2(strip_modifiers(TargetType),Revision,is_same), check_2(strip_modifiers(TargetType),Revision,is_same));

			m_data(Revision,Stored_Location_Planning_Time,check_2(strip_modifiers(TargetType),Revision,is_same), check_2(strip_modifiers(TargetType),Revision,is_same));
			m_data(Revision,Stored_Mode_Planning_Time,check_2(strip_modifiers(TargetType),Revision,is_same), check_2(strip_modifiers(TargetType),Revision,is_same));
			m_data(Revision,Stored_Start_Time_Planning_Time,check_2(strip_modifiers(TargetType),Revision,is_same), check_2(strip_modifiers(TargetType),Revision,is_same));
			m_data(Revision,Stored_Duration_Planning_Time,check_2(strip_modifiers(TargetType),Revision,is_same), check_2(strip_modifiers(TargetType),Revision,is_same));
			m_data(Revision,Stored_Involved_Persons_Planning_Time,check_2(strip_modifiers(TargetType),Revision,is_same), check_2(strip_modifiers(TargetType),Revision,is_same));
			m_data(Revision,Stored_Route_Planning_Time,check_2(strip_modifiers(TargetType),Revision,is_same), check_2(strip_modifiers(TargetType),Revision,is_same));

			template<typename TargetType> bool Location_Is_Planned(){return (_Location_Planning_Time._iteration >= (int)END && _Location_Planning_Time._sub_iteration >= (int)END);} tag_feature_as_available(Location_Is_Planned);
			template<typename TargetType> bool Mode_Is_Planned(){return (_Mode_Planning_Time._iteration >= (int)END && _Mode_Planning_Time._sub_iteration >= (int)END);}	tag_feature_as_available(Mode_Is_Planned);
			template<typename TargetType> bool Duration_Is_Planned(){return (_Duration_Planning_Time._iteration >= (int)END && _Duration_Planning_Time._sub_iteration >= (int)END);}	tag_feature_as_available(Duration_Is_Planned);
			template<typename TargetType> bool Involved_Persons_Is_Planned(){return (_Involved_Persons_Planning_Time._iteration >= (int)END && _Involved_Persons_Planning_Time._sub_iteration >= (int)END);}	tag_feature_as_available(Involved_Persons_Is_Planned);
			template<typename TargetType> bool Start_Is_Planned(){return (_Start_Time_Planning_Time._iteration >= (int)END && _Start_Time_Planning_Time._sub_iteration >= (int)END);} tag_feature_as_available(Start_Is_Planned);
			template<typename TargetType> bool Route_Is_Planned(){return (_Route_Planning_Time._iteration >=(int)END && _Route_Planning_Time._sub_iteration >= (int)END);} tag_feature_as_available(Route_Is_Planned);

			// Basic Activity Events - Plan route and add to schedule
			template<typename TargetType> void Initialize(TargetType act_type)
			{
				UNLOCK(this->_update_lock);
				this_itf* pthis = (this_itf*)this;
				pthis->Activity_Type<TargetType>(act_type);
				pthis->Duration<Time_Seconds>(END);
				pthis->Start_Time<Time_Seconds>(END);
				pthis->Location<_activity_location_itf*>(nullptr);
				pthis->Mode<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
			}
			tag_feature_as_available(Initialize);

			template<typename TargetType> void Set_Attribute_Planning_Times(TargetType planning_time, requires(TargetType,check_2(TargetType, Simulation_Timestep_Increment, is_same)))
			{
				Revision &start = this->Start_Time_Planning_Time<  Revision&>();
				Revision &dur = this->Duration_Planning_Time<  Revision&>();
				Revision &loc = this->Location_Planning_Time<  Revision&>();
				Revision & mode = this->Mode_Planning_Time<  Revision&>();
				Revision & persons = this->Involved_Persons_Planning_Time<  Revision&>();
				Revision &route = this->Route_Planning_Time<  Revision&>();
				
				start._iteration = END;				
				dur._iteration = END;				
				loc._iteration = END;				
				mode._iteration = END;				
				persons._iteration = END;			
				route._iteration = min(_iteration+1, (int)planning_time);

				start._sub_iteration = 0;	
				dur._sub_iteration = 0;
				loc._sub_iteration = 0;
				mode._sub_iteration = 0;
				persons._sub_iteration = 0;
				route._sub_iteration = 5;	
			}
			template<typename TargetType> void Set_Attribute_Planning_Times(TargetType planning_time, requires(TargetType,!check_2(TargetType, Simulation_Timestep_Increment, is_same)))
			{
				assert_check_2(TargetType, Simulation_Timestep_Increment, is_same, "Error: planning_time must be passed as a Simulation_Timestep_Increment type when using this function.");
			}
			tag_feature_as_available(Set_Attribute_Planning_Times);

			template<typename TargetType> void Route_Planning_Event_Handler()
			{			
				PUSH_TO_STACK("Route_Planning_Event_Handler");

				// Create movement plan and give it an ID
				_movement_plan_itf* move = (_movement_plan_itf*)Allocate<typename _scheduler_itf::get_type_of(Movement_Plans_Container)::value_type>();
				move->template initialize_trajectory<NULLTYPE>();
				move->template destination_activity_reference<ComponentType*>((ComponentType*)this);

				// General interfaces, to parent and global classes
				_planning_itf* planner = this->Parent_Planner<_planning_itf*>();
				_person_itf* person = planner->template Parent_Person<_person_itf*>();
				_network_itf* network = person->template network_reference<_network_itf*>();
				_scheduler_itf* scheduler = person->template Scheduling_Faculty<_scheduler_itf*>();
				_scenario_itf* scenario = (_scenario_itf*)_global_scenario;

				
				// Get the origin and destination locations
				_activity_location_itf* orig;
				_activity_location_itf* dest = this->Location<_activity_location_itf*>();
				Time_Seconds start = this->Start_Time<Time_Seconds>();
				this_itf* prev_act = scheduler->template previous_activity_plan<Time_Seconds,this_itf*>(this->Start_Time<Time_Seconds>());		
				if (prev_act == nullptr)  orig = person->template Home_Location<_activity_location_itf*>();
				else
				{
					orig = prev_act->template Location<_activity_location_itf*>();
					// check if a stop at home will fit prior to activity
					Time_Seconds time_before = start - (prev_act->template Start_Time<Time_Seconds>() + prev_act->template Duration<Time_Seconds>());
					Time_Seconds ttime_prev_to_home = network->template Get_TTime<_activity_location_itf*,Vehicle_Type_Keys, Time_Seconds,Time_Seconds>(orig, person->template Home_Location<_activity_location_itf*>(),SOV,start);
					Time_Seconds ttime_home_to_this = network->template Get_TTime<_activity_location_itf*,Vehicle_Type_Keys, Time_Seconds,Time_Seconds>(person->template Home_Location<_activity_location_itf*>(),dest, SOV,start);
					Time_Seconds ttime_prev_to_this = network->template Get_TTime<_activity_location_itf*,Vehicle_Type_Keys, Time_Seconds,Time_Seconds>(orig, dest, SOV, start);
					// enough time between previous activity and this activity to go home, stay there for a minimimum amount of time (equal to the shortest leg of the return home trip) and get to this activity
					float min_home_time = min((float)ttime_prev_to_home,(float)ttime_home_to_this);	
					// don't reset origin, however, if this trip is a return-to-home trip
					if (ttime_prev_to_home + ttime_home_to_this < time_before - min_home_time && dest != person->template Home_Location<_activity_location_itf*>())
						orig = person->template Home_Location<_activity_location_itf*>();
				}

				// set the reference to the movement plan
				this->movement_plan<_movement_plan_itf*>(move);
				
				if (orig == nullptr || dest == nullptr)
				{
					THROW_WARNING("Null origin or destination values specified");
					POP_FROM_STACK;
					return;
				}

				// update the movement plan with the specified origin/destination
				this->Update_Movement_Plan<_activity_location_itf*>(orig,dest,iteration());

				// exit if no trip is needed
				if (orig->template internal_id<int>() == dest->template internal_id<int>())
				{
					POP_FROM_STACK;
					return;
				}
				
				// route if planning router specified
				if(scenario->template do_planner_routing<bool>())
				{
					planner->template Schedule_New_Routing<_movement_plan_itf*>(iteration()+1, move);
				}
				POP_FROM_STACK;
			}
			template<typename TargetType> void Add_Activity_To_Schedule_Event_Handler()
			{
				// General interfaces
				_person_itf* person = this->_Parent_Planner->template Parent_Person<_person_itf*>();
				_household_itf* household = person->template Household<_household_itf*>();
				_scheduler_itf* scheduler = person->template Scheduling_Faculty<_scheduler_itf*>();
				_movement_plan_itf* move = this->movement_plan<_movement_plan_itf*>();
				_scenario_itf* scenario = (_scenario_itf*)_global_scenario;



				// exit if movement plan origin/destination not set
				if (move->template origin<_activity_location_itf*>() == nullptr || move->template destination<_activity_location_itf*>() == nullptr) return;

				if (scenario->template do_planner_routing<bool>())
				{
					// Assign the movement plan to the persons activity schedule, if null movement, leave valid_trajectory to false
					if (move->template valid_trajectory<bool>() || move->template origin<_activity_location_itf*>() == move->template destination<_activity_location_itf*>())
					{			
						Simulation_Timestep_Increment ttime = move->template routed_travel_time<Simulation_Timestep_Increment>();
						Simulation_Timestep_Increment depart = this->Start_Time<Simulation_Timestep_Increment>() - ttime;
						if (depart < _Parent_Planner->template Next_Planning_Time<Simulation_Timestep_Increment>())
						{
							depart = _Parent_Planner->template Next_Planning_Time<Simulation_Timestep_Increment>();
							this->Start_Time<Simulation_Timestep_Increment>(depart + ttime);
						}

						this->Expected_Travel_Time<Simulation_Timestep_Increment>(move->template routed_travel_time<Simulation_Timestep_Increment>());
						move->template departed_time<Simulation_Timestep_Increment>(depart);
						scheduler->template Add_Movement_Plan<_movement_plan_itf*>(move);

					}
					else THROW_WARNING("WARNING: ACTIVITY NOT SCHEDULED, no valid route found from origin to destination. (PERID,ACTID,O,D) "<< person->template uuid<int>() <<","<< concat(this->Activity_Plan_ID<int>()) << "," <<concat(move->template origin<_activity_location_itf*>()->template uuid<int>()) << ", " <<concat(move->template destination<_activity_location_itf*>()->template uuid<int>())<< ",at iteration " << _iteration << "." << sub_iteration()<<". Scheduled for route planning @ " << move->template planning_time<Simulation_Timestep_Increment>() << ", and departure @ " << move->template departed_time<Simulation_Timestep_Increment>());		
				}
				else
				{
					Simulation_Timestep_Increment ttime = this->Expected_Travel_Time<Simulation_Timestep_Increment>();
					Simulation_Timestep_Increment depart = move->template departed_time<Simulation_Timestep_Increment>();
					if (depart < _Parent_Planner->template Next_Planning_Time<Simulation_Timestep_Increment>() && ttime > 0)
					{
						depart = _Parent_Planner->template Next_Planning_Time<Simulation_Timestep_Increment>();
						move->template departed_time<Simulation_Timestep_Increment>(depart);
						this->Start_Time<Simulation_Timestep_Increment>(depart + ttime);
					}
					scheduler->template Add_Movement_Plan<_movement_plan_itf*>(move);
				}
			}		
			template<typename TargetType> void Update_Movement_Plan(TargetType origin, TargetType destination, Simulation_Timestep_Increment min_departure)
			{
				PUSH_TO_STACK("Update_Movement_Plan");

				_movement_plan_itf* move = this->movement_plan<_movement_plan_itf*>();
				_activity_location_itf* orig = (_activity_location_itf*)origin;
				_activity_location_itf* dest = (_activity_location_itf*)destination;
				_activity_plan_itf* activity = move->template destination_activity_reference<_activity_plan_itf*>();

				// General interfaces, to parent and global classes
				_person_itf* person = this->_Parent_Planner->template Parent_Person<_person_itf*>();
				_network_itf* network = person->template network_reference<_network_itf*>();

				// continue if a valid movement is specified
				if (orig != nullptr && dest != nullptr) 
				{
					// If the trip is valid, assign to a movement plan and add to the schedule
					if (orig->template origin_links<_links_container_itf&>().size() != 0 && dest->template origin_links<_links_container_itf&>().size() != 0)
					{		
						// add attributes to plan
						move->template origin<_activity_location_itf*>(orig);
						move->template destination<_activity_location_itf*>(dest);
						move->template origin<_link_itf*>(orig->template origin_links<_links_container_itf&>().at(0));
						move->template destination<_link_itf*>(dest->template origin_links<_links_container_itf&>().at(0));

						if (move->template origin<_link_itf*>()->template outbound_turn_movements<_turns_container_itf*>()->size() == 0 || move->template destination<_link_itf*>()->template outbound_turn_movements<_turns_container_itf*>()->size() == 0)
						{
							_link_itf* o_link =move->template origin<_link_itf*>();
							_link_itf* d_link =move->template destination<_link_itf*>();
							//THROW_WARNING("WARNING: cannot route trip as orig or dest links do not have valid turn movements: [Perid.actid,acttype,orig_link,dest_link,orig_zone,dest_zone]: "<<concat(this->Parent_ID<int>()) << "." << concat(this->Activity_Plan_ID< int>()) <<", " << concat(this->Activity_Type< ACTIVITY_TYPES>()) << ", " <<o_link->uuid<int>() << ", " << d_link->uuid<int>() << ", "  << orig->zone<_zone_itf*>()->uuid<int>() << ", " << dest->zone<_zone_itf*>()->uuid<int>());
							POP_FROM_STACK;
							return;
						}
						
						
						// shift departure time by estimated travel time, and make sure that it does not occur before next iteration
						Simulation_Timestep_Increment start = this->Start_Time<Simulation_Timestep_Increment>();
						Simulation_Timestep_Increment ttime;
						if (orig == dest) ttime = 0;
						else ttime = network->template Get_TTime<_activity_location_itf*,Vehicle_Components::Types::Vehicle_Type_Keys,Simulation_Timestep_Increment,Simulation_Timestep_Increment>(orig, dest, Vehicle_Components::Types::Vehicle_Type_Keys::SOV, start);
						this->Expected_Travel_Time<Simulation_Timestep_Increment>(ttime);

						// recalculate the departure time based on new estimated travel time
						Simulation_Timestep_Increment depart =  start - ttime;
						// unless the activity is a return home activity, in which case, use the fixed departure time
						if (activity->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::AT_HOME_ACTIVITY)
						{
							_home_activity_itf* home_activity = (_home_activity_itf*)activity;
							depart = home_activity->template Fixed_Departure<Simulation_Timestep_Increment>();
						}
						
						// make sure not departure before minimum allowed
						if (depart < min_departure)
						{
							depart = min_departure+1;
							this->Start_Time<Simulation_Timestep_Increment>(depart + ttime);
						}

						// schedule the routing and do routin if requested through scenario, otherwise move to the activity scheduling phase
						move->template departed_time<Simulation_Timestep_Increment>(depart);
					}
					else
					{
						move->template origin<_activity_location_itf*>(orig);
						move->template destination<_activity_location_itf*>(dest);
						THROW_WARNING("WARNING: movement from " << orig->template uuid<int>() << " to " << dest->template uuid<int>() << ", can not happen as no origin / destination links are available for the locations.");
					}
				}
				else
				{
					THROW_WARNING("Null origin or destination values specified");
				}

				POP_FROM_STACK;
			}
			template<typename TargetType> void Arrive_At_Activity()
			{
				// Person has arrived at activity destination
				_Parent_Planner->template Parent_Person<_person_itf*>()->template Arrive_At_Destination<NT>();
			}
			template<typename TargetType> void Depart_From_Activity()
			{

			}

			template<typename TargetType> string Get_Type_String()
			{
				if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::AT_HOME_ACTIVITY)
					return string("HOME");
				else if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::EAT_OUT_ACTIVITY)
					return string("EAT OUT");
				else if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::ERRANDS_ACTIVITY)
					return string("ERRANDS");
				else if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::HEALTHCARE_ACTIVITY)
					return string("HEALTHCARE");
				else if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::LEISURE_ACTIVITY)
					return string("LEISURE");
				else if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::MAJOR_SHOPPING_ACTIVITY)
					return string("SHOP-MAJOR");
				else if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::OTHER_SHOPPING_ACTIVITY)
					return string("SHOP-OTHER");
				else if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::PERSONAL_BUSINESS_ACTIVITY)
					return string("PERSONAL");
				else if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::PICK_UP_OR_DROP_OFF_ACTIVITY)
					return string("PICKUP-DROPOFF");
				else if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::PRIMARY_WORK_ACTIVITY)
					return string("WORK");
				else if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::RECREATION_ACTIVITY)
					return string("RECREATION");
				else if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::RELIGIOUS_OR_CIVIC_ACTIVITY)
					return string("RELIGIOUS-CIVIC");
				else if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::SCHOOL_ACTIVITY)
					return string("SCHOOL");
				else if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::SERVICE_VEHICLE_ACTIVITY)
					return string("SERVICE");
				else if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::SOCIAL_ACTIVITY)
					return string("SOCIAL");
				else if (_Activity_Type == Activity_Components::Types::ACTIVITY_TYPES::WORK_AT_HOME_ACTIVITY)
					return string("WORK AT HOME");
				else
					return string ("OTHER");
			}		
		};



		// Specialization for ADAPTS-style activities with plan horizon info and more detailed set_plan_times function
		implementation struct ADAPTS_Activity_Plan_Implementation : public Basic_Activity_Plan_Implementation<MasterType,INHERIT(ADAPTS_Activity_Plan_Implementation)>
		{
			typedef Basic_Activity_Plan_Implementation<MasterType,INHERIT(ADAPTS_Activity_Plan_Implementation)> base_type;
			typedef typename Basic_Activity_Plan_Implementation<MasterType,INHERIT(ADAPTS_Activity_Plan_Implementation)>::Component_Type ComponentType;

			//================================================================================================================================================================================================
			//================================================================================================================================================================================================
			// Interfaces
			typedef Prototypes::Activity_Planner<ComponentType> this_itf;
			typedef  Person_Components::Prototypes::Person_Properties< typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(Properties)> _properties_itf;
			typedef  Person_Components::Prototypes::Person_Scheduler< typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(Scheduling_Faculty)> _scheduler_itf;
			typedef  Person_Components::Prototypes::Person_Properties< typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(Static_Properties)> _static_properties_itf;
			typedef Person_Components::Prototypes::Person_Planner<typename type_of(base_type::Parent_Planner)> _planning_itf;
			typedef Person_Components::Prototypes::Person<typename _planning_itf::get_type_of(Parent_Person)> _person_itf;
			typedef Person_Components::Prototypes::Destination_Chooser<typename _planning_itf::get_type_of(Destination_Choice_Faculty)> _dest_choice_itf;
			typedef Person_Components::Prototypes::Mode_Chooser<typename _planning_itf::get_type_of(Mode_Choice_Faculty)> _mode_choice_itf;
			typedef Person_Components::Prototypes::Activity_Timing_Chooser<typename _planning_itf::get_type_of(Timing_Chooser)> _timing_choice_itf;
			typedef  Scenario_Components::Prototypes::Scenario< typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference)> _scenario_itf;
			typedef  Network_Components::Prototypes::Network< typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(network_reference)> _network_itf;
			typedef Network_Skimming_Components::Prototypes::Network_Skimming< typename _network_itf::get_type_of(skimming_faculty)> _skim_itf;
			typedef Network_Skimming_Components::Prototypes::LOS<typename MasterType::los_value_type> los_itf;
			typedef  Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer< typename _network_itf::get_type_of(activity_locations_container)::value_type>::type>  _activity_location_itf;
			typedef  Random_Access_Sequence< typename _network_itf::get_type_of(activity_locations_container), _activity_location_itf*> _activity_locations_container_itf;

			typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename _activity_location_itf::get_type_of(origin_links)::value_type>::type>  _link_itf;
			typedef  Random_Access_Sequence< typename _activity_location_itf::get_type_of(origin_links), _link_itf*> _links_container_itf;

			typedef  Zone_Components::Prototypes::Zone<typename remove_pointer< typename _network_itf::get_type_of(zones_container)::value_type>::type>  _zone_itf;
			typedef  Pair_Associative_Container< typename _network_itf::get_type_of(zones_container), _zone_itf*> _zones_container_itf;

			//typedef Activity_Components::Prototypes::Activity_Planner<typename remove_pointer< typename type_of(base_type::Parent_Planner)::type_of(Activity_Container)::value_type>::type> _activity_plan_itf;
			//typedef Back_Insertion_Sequence< typename type_of(base_type::Parent_Planner)::type_of(Activity_Container),_activity_plan_itf*> _activity_plans_container_itf;

			//typedef Movement_Plan_Components::Prototypes::Movement_Plan<typename remove_pointer< typename type_of(base_type::Parent_Planner)::type_of(Movement_Plans_Container)::value_type>::type> _movement_plan_itf;
			//typedef Back_Insertion_Sequence< typename type_of(base_type::Parent_Planner)::type_of(Movement_Plans_Container),_movement_plan_itf*> _movement_plans_container_itf;

			//================================================================================================================================================================================================
			//================================================================================================================================================================================================
		
			m_data(Basic_Units::Implementations::Time_Implementation<NT>,_Activity_Planning_Time, NONE, NONE);
			member_component_feature(Activity_Planning_Time, _Activity_Planning_Time, Value, Basic_Units::Prototypes::Time)

			// Activity attribute planning properties		
			m_data(Types::PLAN_HORIZON_VALUES, Activity_Plan_Horizon, NONE, NONE);
			m_data(Types::PLAN_HORIZON_VALUES, Location_Plan_Horizon, NONE, NONE);
			//m_data(Types::FLEXIBILITY_VALUES, Location_Flexibility, NONE, NONE);
			m_data(Types::PLAN_HORIZON_VALUES, Mode_Plan_Horizon, NONE, NONE);
			//m_data(Types::FLEXIBILITY_VALUES, Mode_Flexibility, NONE, NONE);
			m_data(Types::PLAN_HORIZON_VALUES, Start_Time_Plan_Horizon, NONE, NONE);
			//m_data(Types::FLEXIBILITY_VALUES, Start_Time_Flexibility, NONE, NONE);
			m_data(Types::PLAN_HORIZON_VALUES, Duration_Plan_Horizon, NONE, NONE);
			//m_data(Types::FLEXIBILITY_VALUES, Duration_Flexibility, NONE, NONE);
			m_data(Types::PLAN_HORIZON_VALUES, Involved_Persons_Plan_Horizon, NONE, NONE);
			//m_data(Types::FLEXIBILITY_VALUES, Involved_Persons_Flexibility, NONE, NONE);

			// Activity methods
			template<typename TargetType> void Initialize(TargetType act_type)
			{
				//UNLOCK(this->_update_lock);
				this_itf* pthis = (this_itf*)this;
				pthis->template Activity_Type<TargetType>(act_type);
				pthis->template Duration<Time_Seconds>(END);
				pthis->template Start_Time<Time_Seconds>(END);
				pthis->template Location<_activity_location_itf*>(nullptr);
				pthis->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
			}

			template<typename TargetType> void Set_Meta_Attributes()
			{
				//===========================================================================================================================
				// Initialize local variables for use
				//---------------------------------------------------------------------------------------------------------------------------

				// get references to use in model
				base_type* base_this = (base_type*)this;
				this_itf* pthis = (this_itf*)this;
				_planning_itf* planner = pthis->template Parent_Planner<_planning_itf*>();
				_person_itf* person = planner->template Parent_Person<_person_itf*>();
				_properties_itf* properties = person->template Properties<_properties_itf*>();
				_static_properties_itf* static_props = person->template Static_Properties<_static_properties_itf*>();

				//Create local variables
				int ACT1, ACT2, ACT3, ACT4, ACT5, Employed, Student, Male, Senior, TELEWORK, ICT_USE;
				ACT1 = 0; ACT2 = 0; ACT3 = 0; ACT4 = 0; ACT5 = 0; Employed = 0; Student = 0; Male = 0; Senior = 0; TELEWORK = 0; ICT_USE = 0;
				float AvgDur, AvgFreq;

				if (base_this->_Activity_Type == WORK_AT_HOME_ACTIVITY || base_this->_Activity_Type == PRIMARY_WORK_ACTIVITY || base_this->_Activity_Type == SCHOOL_ACTIVITY || base_this->_Activity_Type == OTHER_WORK_ACTIVITY) ACT1 = 1;
				else if (base_this->_Activity_Type == HEALTHCARE_ACTIVITY || base_this->_Activity_Type == RELIGIOUS_OR_CIVIC_ACTIVITY || base_this->_Activity_Type == PERSONAL_BUSINESS_ACTIVITY) ACT2 = 1;
				else if (base_this->_Activity_Type == Types::ERRANDS_ACTIVITY || base_this->_Activity_Type == PICK_UP_OR_DROP_OFF_ACTIVITY || base_this->_Activity_Type == SERVICE_VEHICLE_ACTIVITY) ACT3 = 1;
				else if (base_this->_Activity_Type == Types::LEISURE_ACTIVITY|| base_this->_Activity_Type == Types::EAT_OUT_ACTIVITY || base_this->_Activity_Type == Types::RECREATION_ACTIVITY || base_this->_Activity_Type == Types::SOCIAL_ACTIVITY) ACT4 = 1;
				else if (base_this->_Activity_Type == Types::OTHER_SHOPPING_ACTIVITY ||base_this-> _Activity_Type == Types::MAJOR_SHOPPING_ACTIVITY) ACT5 = 1;
				if (static_props->template Is_Employed<bool>()) Employed = 1;
				if (static_props->template Is_Student<bool>()) Student = 1;
				if (static_props->template Gender<GENDER>() == GENDER::MALE) Male = 1;
				if (static_props->template Age<int>() >= 65) Senior = 1;
				//if (PER.PersonData.ICT_Use != IctType.NULL || PER.PersonData.ICT_Use != IctType.UseLow) ICT_USE = 1;
				if (static_props->template Journey_To_Work_Mode<Person_Components::Types::JOURNEY_TO_WORK_MODE>() == JOURNEY_TO_WORK_MODE::WORKMODE_WORK_AT_HOME) TELEWORK = 1;
				AvgFreq = properties->template Average_Activity_Frequency<ACTIVITY_TYPES,float>(base_this->_Activity_Type);
				AvgDur = properties->template Average_Activity_Duration<ACTIVITY_TYPES,Time_Minutes>(base_this->_Activity_Type);


				//===========================================================================================================================
				// FLEXIBILITY CALCULATIONS
				//---------------------------------------------------------------------------------------------------------------------------
				matrix<double> alpha = matrix<double>(matrix<double>::index_type(3,5)); // limits matrix
				alpha(0,0)=-99999999;	alpha(0,1)=-99999999;	alpha(0,2)=-99999999;	alpha(0,3)=-99999999;	alpha(0,4)=-99999999;
				alpha(1,0)=0;			alpha(1,1)=0;			alpha(1,2)=0;			alpha(1,3)=0;			alpha(1,4)=0;
				alpha(2,0)=99999999;	alpha(2,1)=99999999;	alpha(2,2)=99999999;	alpha(2,3)=99999999;	alpha(2,4)=99999999;

				matrix<double> Sigma = matrix<double>(matrix<double>::index_type(5,5)); // covariance matrix
				Sigma(0,0)=1;			Sigma(0,1)=0;			Sigma(0,2)=0;			Sigma(0,3)=0;			Sigma(0,4)=0;
				Sigma(1,0)=0.099646;	Sigma(1,1)=1;			Sigma(1,2)=0;			Sigma(1,3)=0;			Sigma(1,4)=0;
				Sigma(2,0)=-0.053507;	Sigma(2,1)=-0.03372;	Sigma(2,2)=1;			Sigma(2,3)=0;			Sigma(2,4)=0;
				Sigma(3,0)=0.095299;	Sigma(3,1)=0.305332;	Sigma(3,2)=-0.113887;	Sigma(3,3)=1;			Sigma(3,4)=0;
				Sigma(4,0)=0.124233;	Sigma(4,1)=0.168151;	Sigma(4,2)=0.025553;	Sigma(4,3)=-0.099794;	Sigma(4,4)=1;

				double P_per[2]; // Holds probability for each level (index by i) of each response (index by j)
				double P_mod[2];
				double P_loc[2];
				double P_tim[2];
				double P_dur[2];

				//Calculate xB values for each response
				double xB_mod = -0.383692 + 0.362839 * Employed + 0.4215 * Student - 0.217904 * Male - 0.267344 * Senior + 0.691999 * Male * Senior + 0.231177 * AvgFreq + 0.568976 * ACT1 * ICT_USE - 1.039075 * ACT1 * AvgFreq;
				double xB_per = -0.338294 + 0.347752 * Student - 0.536749 * Male + 0.259776 * Senior + 0.478152 * Male * Senior + 0.532325 * ICT_USE + 1.508163 * TELEWORK - 1.655547 * ICT_USE * TELEWORK - 1.390818 * ACT1 * +1.352461 * ACT1 * TELEWORK - 0.598393 * ACT2 + 11.034344 * ACT2 * AvgDur - 0.617948 * ACT3 + 1.727868 * AvgFreq * ACT3 + 0.437761 * AvgFreq * ACT4 + 6.708181 * ACT4 * AvgDur - 0.606538 * ACT5 + 0.325567 * AvgFreq * ACT5;
				double xB_loc = -0.368786 - 1.174171 * Male - 0.1914 * Senior + 1.346607 * Male * Senior - 0.3674 * ICT_USE + 0.99392 * TELEWORK - 0.815509 * ICT_USE * TELEWORK + 1.023933 * ACT1 * TELEWORK + 2.325112 * ACT1 * AvgFreq - 8.966545 * ACT2 * AvgDur - 1.02207 * AvgFreq * ACT3 + 0.904036 * ACT4 + 0.637477 * AvgFreq * ACT4 - 8.32642 * ACT4 * AvgDur + 2.382859 * ACT5;
				double xB_tim = 1.326259 - 0.38711 * Male - 0.226637 * Senior + 0.484623 * Male * Senior - 0.302255 * ICT_USE + 0.196488 * TELEWORK + 3.3758 * ACT2 * AvgDur - 0.400564 * ACT3 + 1.301273 * AvgFreq * ACT3 - 0.601686 * ACT5 + 0.192867 * AvgFreq * ACT5;
				double xB_dur = -1.239259 + 1.294471 * Employed + 1.461113 * Student - 0.246144 * Senior - 0.374409 * Male * Senior - 0.23527 * ICT_USE * TELEWORK + 1.30007 * AvgDur + 0.621008 * ACT1 * +0.994951 * ACT1 * ICT_USE - 1.124459 * ACT1 * TELEWORK - 0.56222 * ACT1 * AvgFreq - 2.070631 * ACT1 * AvgDur - 0.167278 * ICT_USE * ACT4 + 0.769745 * AvgFreq * ACT4 + 0.487925 * AvgFreq * ACT5;			

				// Get probabilities of each response level
				for (int i = 0; i < 2; i++)
				{
					P_mod[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha(i + 1, 0) - xB_mod);
					P_per[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha(i + 1, 1) - xB_per);
					P_loc[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha(i + 1, 2) - xB_loc);
					P_tim[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha(i + 1, 3) - xB_tim);
					P_dur[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha(i + 1, 4) - xB_dur);
				}

				// Set correlated random variables
				boost::container::vector<double> rand;
				GLOBALS::Normal_RNG.template Correlated_Rands<double>(rand, Sigma);

				// Set flexibility values
				for (int i = 0; i < 2; i++)
				{
					if (rand[0] < P_mod[i]) {base_this->_Mode_Flexibility = (Types::FLEXIBILITY_VALUES)i;				rand[0] = 999;}
					if (rand[1] < P_per[i]) {base_this->_Involved_Persons_Flexibility = (Types::FLEXIBILITY_VALUES)i;	rand[1] = 999; }
					if (rand[2] < P_loc[i]) {base_this->_Location_Flexibility = (Types::FLEXIBILITY_VALUES)i;			rand[2] = 999; }
					if (rand[3] < P_tim[i]) {base_this->_Start_Time_Flexibility = (Types::FLEXIBILITY_VALUES)i;			rand[3] = 999; }
					if (rand[4] < P_dur[i]) {base_this->_Duration_Flexibility = (Types::FLEXIBILITY_VALUES)i;			rand[4] = 999; }
				}
            
				// Set results of flexiblity model to use in next models
				double IFLOC, IFTIM, IFDUR, IFMOD;
				IFMOD = P_mod[0];
				IFLOC = P_loc[0];
				IFTIM = P_tim[0];
				IFDUR = P_dur[0];




				//===========================================================================================================================
				// Activity Plan Horizon Calculations
				//---------------------------------------------------------------------------------------------------------------------------
				double xB = 0.130 + 0.640303 * IFLOC - 0.63621 * IFTIM - 1.498554 * IFDUR + 0.767701 * Employed + 0.485362 * ICT_USE
					- 0.560652 * TELEWORK + 1.049907 * ACT1 - 0.446779 * ACT1 * AvgFreq - 0.789474 * ACT1 * IFLOC + 2.241387 * ACT1 * IFDUR
					- 0.892963 * ACT1 * Employed + 1.717603 * ACT1 * Student + 15.248565 * ACT2 * AvgDur + 1.497258 * IFDUR * ACT2
					- 0.613958 * ICT_USE * ACT2 + 1.573672 * AvgFreq * ACT3 + 0.633234 * IFDUR * ACT3 - 0.745918 * Employed * ACT3
					- 1.115487 * Student * ACT3 + 1.049812 * ACT3 * Senior - 0.593425 * ACT3 * Male + 0.519961 * AvgFreq * ACT4 + 2.200494 * AvgDur * ACT4
					+ 1.403807 * IFDUR * ACT4 + 0.837724 * Student * ACT4 + 0.681377 * Senior * ACT4 - 0.793118 * Male * ACT4 - 0.432584 * ICT_USE * ACT4
					+ 0.289143 * AvgFreq * ACT5 + 1.128966 * IFDUR * ACT5 - 0.610606 * Employed * ACT5 + 0.415577 * Senior * ACT5
					- 0.752066 * ICT_USE * ACT5;
				double Pimp, Pday, Pweek;
				double a1 = 1.646398;
				double a2 = 3.505000;
				double r = Uniform_RNG.template Next_Rand<double>();

				// calculate probabilities
				Pimp = 1.0 / (1.0 + exp(xB));
				Pday = 1.0 / (1.0 + exp(-a1 + xB));
				Pweek = 1.0 / (1.0 + exp(-a2 + xB));

				// Set activity plan horizon
				if (r < Pimp) this->_Activity_Plan_Horizon = Types::PLAN_HORIZON_VALUES::IMPULSIVE;
				else if (r < Pday) this->_Activity_Plan_Horizon = Types::PLAN_HORIZON_VALUES::PREPLANNED_SAME_DAY;
				else if (r < Pweek) this->_Activity_Plan_Horizon = Types::PLAN_HORIZON_VALUES::PREPLANNED_2_TO_7_DAYS;
				else this->_Activity_Plan_Horizon = Types::PLAN_HORIZON_VALUES::PREPLANNED_OVER_7_DAYS;

				


				//===========================================================================================================================
				// ATTRIBUTE Plan Horizon Calculations
				//---------------------------------------------------------------------------------------------------------------------------
				double PLANIMP = Pimp, PLANSAME = Pday-Pimp, PLANWEEK = Pweek-Pday, PLANONTOUR;
				PLANONTOUR = PLANIMP * 1;

				matrix<double> alpha2 = matrix<double>(matrix<double>::index_type(6,5));
				alpha2(0,0)=-99999999;	alpha2(0,1)=-99999999;	alpha2(0,2)=-99999999;	alpha2(0,3)=-99999999;	alpha2(0,4)=-99999999;
				alpha2(1,0)=0;			alpha2(1,1)=0;			alpha2(1,2)=0;			alpha2(1,3)=0;			alpha2(1,4)=0;
				alpha2(2,0)=0.835415;	alpha2(2,1)=2.029384;	alpha2(2,2)=1.305499;	alpha2(2,3)=1.151026;	alpha2(2,4)=0.434582;
				alpha2(3,0)=1.486357;	alpha2(3,1)=3.614181;	alpha2(3,2)=2.316088;	alpha2(3,3)=2.026512;	alpha2(3,4)=0.879016;
				alpha2(4,0)=1.794494;	alpha2(4,1)=4.401609;	alpha2(4,2)=2.796206;	alpha2(4,3)=2.643064;	alpha2(4,4)=1.095505;
				alpha2(5,0)=99999999;	alpha2(5,1)=99999999;	alpha2(5,2)=99999999;	alpha2(5,3)=99999999;	alpha2(5,4)=99999999;

				matrix<double> Sigma2 = matrix<double>(matrix<double>::index_type(5,5));
				Sigma2(0,0)=1;			Sigma2(0,1)=0;			Sigma2(0,2)=0;			Sigma2(0,3)=0;			Sigma2(0,4)=0;
				Sigma2(1,0)=0.14308;	Sigma2(1,1)=1;			Sigma2(1,2)=0;			Sigma2(1,3)=0;			Sigma2(1,4)=0;
				Sigma2(2,0)=0.148218;	Sigma2(2,1)=0.104424;	Sigma2(2,2)=1;			Sigma2(2,3)=0;			Sigma2(2,4)=0;
				Sigma2(3,0)=0.159259;	Sigma2(3,1)=0.180953;	Sigma2(3,2)=0.352088;	Sigma2(3,3)=1;			Sigma2(3,4)=0;
				Sigma2(4,0)=0.186676;	Sigma2(4,1)=0.177994;	Sigma2(4,2)=0.218223;	Sigma2(4,3)=0.538872;	Sigma2(4,4)=1;

				double P_pmod[5];
				double P_pper[5]; // Holds probability for each level (index by i) of each response (index by j)
				double P_ploc[5];
				double P_ptim[5];
				double P_pdur[5];

				//Calculate xB values for each response
				double xB_pmod = 0.618 + 0.262288 * ACT2 + 0.376779 * ACT4 + 0.195715 * ACT5 + 0.575386 * PLANONTOUR - 1.527302 * PLANIMP 
								 - 0.492435 * PLANSAME - 0.271618 * PLANWEEK + 0.633625 * Employed + 1.043977 * Student - 0.115081 * Senior
								 + 0.106084 * IFDUR + 0.785746 * ACT1 * AvgDur + 0.353502 * PLANWEEK * ACT3 - 3.92232 * ACT4 * AvgDur
								 + 0.456054 * ACT1 * TELEWORK + 0.130093 * ICT_USE;
				double xB_pper = 4.096 - 5.532536 * PLANIMP - 2.912268 * PLANSAME - 1.231383 * PLANWEEK + 0.825496 * ACT2 * Student
								 - 5.690886 * ACT4 * AvgDur + 0.358763 * ACT4 * ICT_USE;
				double xB_ploc = 2.079 - 3.2086 * PLANIMP - 1.40061 * PLANSAME - 0.581665 * PLANWEEK + 0.202875 * Employed + 0.350806 * Student
								 + 1.846201 * AvgDur + 0.951409 * ACT1 * IFLOC + 0.278635 * ACT4 * IFLOC + 0.839625 * ICT_USE
								 - 0.736206 * ACT2 * ICT_USE - 0.692824 * ACT3 * ICT_USE - 1.033742 * ACT4 * ICT_USE - 1.166372 * ACT5 * ICT_USE;
				double xB_ptim = 1.612 + 0.206278 * ACT5 - 3.178382 * PLANIMP - 1.570305 * PLANSAME - 0.932677 * PLANWEEK + 0.359391 * Employed
								 + 0.163188 * IFDUR + 0.616433 * ACT3 * IFTIM + 0.756115 * ACT1 * TELEWORK - 0.145507 * ICT_USE
								 + 0.185875 * ACT4 * ICT_USE - 0.497667 * ACT5 * ICT_USE;
				double xB_pdur = -1.264 + 0.323092 * ACT2 - 1.466871 * PLANIMP - 0.518569 * PLANSAME - 0.35598 * PLANWEEK + 0.742175 * Employed
								 + 0.157659 * Senior + 0.130234 * Male + 0.297442 * IFTIM + 0.835193 * IFDUR + 0.266187 * AvgFreq
								 - 0.395776 * ACT2 * IFDUR - 0.282278 * ACT5 * IFDUR - 0.338128 * ACT5 * IFTIM + 0.879233 * ACT1 * TELEWORK;

				// Get probabilities of each response level
				for (int i = 0; i < 5; i++)
				{
					P_pmod[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha2[i + 1, 0] - xB_pmod, 0, 1);
					P_pper[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha2[i + 1, 1] - xB_pper, 0, 1);
					P_ploc[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha2[i + 1, 2] - xB_ploc, 0, 1);
					P_ptim[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha2[i + 1, 3] - xB_ptim, 0, 1);
					P_pdur[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha2[i + 1, 4] - xB_pdur, 0, 1);
				}

				// Set correlated random variables
				boost::container::vector<double> rand2;
				GLOBALS::Normal_RNG.template Correlated_Rands<double>(rand2, Sigma2);

				// Set attribute plan horizon values
				for (int i = 0; i < 5; i++)
				{
					// convert from i to planhorizon values (skip 'previous day')
					int x = i + 1; if (i > 1) x = x + 1;
					PLAN_HORIZON_VALUES value;
					switch (i)
					{
					case 0 : value = PLAN_HORIZON_VALUES::IMPULSIVE; break;
					case 1 : value = PLAN_HORIZON_VALUES::PREPLANNED_SAME_DAY; break;
					case 2 : value = PLAN_HORIZON_VALUES::PREPLANNED_2_TO_7_DAYS; break;
					case 3 : value = PLAN_HORIZON_VALUES::PREPLANNED_OVER_7_DAYS; break;
					default : value = ROUTINE;
					}

					if (rand2[0] < P_pmod[i]) { this->_Mode_Plan_Horizon = value;				rand2[0] = 999; }
					if (rand2[1] < P_pper[i]) { this->_Involved_Persons_Plan_Horizon = value;	rand2[1] = 999; }
					if (rand2[2] < P_ploc[i]) { this->_Location_Plan_Horizon = value;			rand2[2] = 999; }
					if (rand2[3] < P_ptim[i]) { this->_Start_Time_Plan_Horizon = value;			rand2[3] = 999; }
					if (rand2[4] < P_pdur[i]) { this->_Duration_Plan_Horizon = value;			rand2[4] = 999; }
				}
			}
			tag_feature_as_available(Set_Meta_Attributes);

			template<typename TargetType> void Set_Attribute_Planning_Times(TargetType planning_time, requires(TargetType,check_2(TargetType, Simulation_Timestep_Increment, is_same)))
			{
				// get references to use in model
				base_type* base_this = (base_type*)this;

				Revision &start	= base_this->template Start_Time_Planning_Time<  Revision&>();
				Revision &dur		= base_this->template Duration_Planning_Time<  Revision&>();
				Revision &loc		= base_this->template Location_Planning_Time<  Revision&>();
				Revision &mode	= base_this->template Mode_Planning_Time<  Revision&>();
				Revision &persons	= base_this->template Involved_Persons_Planning_Time<  Revision&>();
				Revision &route	= base_this->template Route_Planning_Time<  Revision&>();


				// Set revisions based on the plan-horizons for each attribute
				int dur_sub = (int)(this->_Duration_Plan_Horizon);
				int loc_sub = (int)(this->_Location_Plan_Horizon);
				int mode_sub = (int)(this->_Mode_Plan_Horizon);
				int inv_sub = (int)(this->_Involved_Persons_Plan_Horizon);
				int start_sub = (int)(this->_Start_Time_Plan_Horizon);
			
				// give intrinsic order when simultaneous plan horizons are specified
				if (start_sub == dur_sub) dur_sub++;
				if (start_sub == loc_sub) loc_sub++;
				if (start_sub == mode_sub) mode_sub++;
				if (start_sub == inv_sub) inv_sub++;
				if (dur_sub == loc_sub) loc_sub++;
				if (dur_sub == mode_sub) mode_sub++;
				if (dur_sub == inv_sub) inv_sub++;
				if (loc_sub == mode_sub) mode_sub++;
				if (loc_sub == inv_sub) inv_sub++;
				if (mode_sub == inv_sub) inv_sub++;

				int route_sub = max(start_sub,max(dur_sub,max(loc_sub,max(mode_sub,inv_sub)))) + 1;

				start._iteration = max(_iteration+1, (int)planning_time);
				dur._iteration = END; //dur._iteration = max(_iteration+1, (int)planning_time);
				loc._iteration = max(_iteration+1,(int)planning_time);
				mode._iteration = max(_iteration+1, (int)planning_time);
				persons._iteration = max(_iteration+1, (int)planning_time);
				route._iteration = max(_iteration+1, (int)planning_time);
				
				start._sub_iteration = start_sub;
				dur._sub_iteration = END; //dur._sub_iteration = dur_sub;
				loc._sub_iteration = loc_sub;
				mode._sub_iteration = mode_sub;
				persons._sub_iteration = inv_sub;
				route._sub_iteration = route_sub;
			}
			template<typename TargetType> void Set_Attribute_Planning_Times(TargetType planning_time, requires(TargetType,!check_2(TargetType, Simulation_Timestep_Increment, is_same)))
			{
				assert_check_2(TargetType, Simulation_Timestep_Increment, is_same, "Error: planning_time must be passed as a Simulation_Timestep_Increment type when using this function.");
			}
			tag_feature_as_available(Set_Attribute_Planning_Times);

			// Attribute planning events
			template<typename TargetType> void Location_Planning_Event_Handler()
			{
				base_type* bthis = (base_type*)this;
				this_itf* pthis = (this_itf*)this;

				// references to external agents
				_planning_itf* planner = bthis->template Parent_Planner<_planning_itf*>();
				_person_itf* person = planner->template Parent_Person<_person_itf*>();
				_scheduler_itf* scheduler = person->template Scheduling_Faculty<_scheduler_itf*>();
				_dest_choice_itf* dest_chooser = planner->template Destination_Choice_Faculty<_dest_choice_itf*>();
				_network_itf* network = person->template network_reference<_network_itf*>();

				_activity_location_itf *home_loc = person->template Home_Location<_activity_location_itf*>();

				//// timing variables
				//Time_Minutes start, end, prev_end, next_start, time_before, time_after, ttime_prev_to_home, ttime_home_to_next;
				//start = pthis->template Start_Time<Time_Minutes>();
				//end = pthis->template Start_Time<Time_Minutes>() + pthis->template Duration<Time_Minutes>();

				//// Get half-tour origin from previous planned activity and destination from next planned activity (or from home if no preceeding activities)
				//_activity_location_itf *prev_loc, *next_loc, *home_loc;
				

				//if (bthis->template Start_Is_Planned<bool>())
				//{
				//	// set previous activity location, =home if no previous activity or if person can go home before current activity
				//	this_itf* previous = person->template previous_activity_plan<Time_Seconds,this_itf*>(bthis->template Start_Time<Time_Seconds>());
				//	if (previous == nullptr) prev_loc = home_loc;
				//	else prev_loc = scheduler->template previous_location<_activity_location_itf**>(this);

				//	// set next activity location, =home if no next activity
				//	this_itf* next = person->template previous_activity_plan<Time_Seconds,this_itf*>(bthis->template Start_Time<Time_Seconds>());
				//	if (next == nullptr) next_loc = home_loc;
				//	else next_loc = scheduler->template next_location<_activity_location_itf**>(this);
				//}
				//// if the start time is not known, treat the tour as originating/destined for home
				//else 
				//{
				//	prev_loc = home_loc;
				//	next_loc = home_loc;
				//}

				// call destination choice - set to home of no location could be chosen
				_activity_location_itf* dest = dest_chooser->template Choose_Destination<this_itf*,_activity_location_itf*>(pthis);
				

				// check that origin and destination are valid
				if (dest != nullptr) 
				{
					bthis->template Location<_activity_location_itf*>(dest);
				}
				else
				{
					// Delete the activity from schedule if no location chosen
					pthis->template Unschedule_Activity_Events<NT>();
					scheduler->template Remove_Activity_Plan<this_itf*>(pthis);

					/*dest = home_loc;
					bthis->template Location<_activity_location_itf*>(dest);*/
					//----------------------------------------------------------------
					// Print to log file
					//stringstream s;
					//s <<"ACTIVITY NOT GENERATED, null origin or destination: "<< person->template uuid<int>();
					//s << "," << dest<<endl;
					//cout << s.str();
					//planner->template Write_To_Log<stringstream&>(s);
					//----------------------------------------------------------------
				}
			}
			template<typename TargetType> void Mode_Planning_Event_Handler()
			{
				//====================================================================
				// Simplified mode choice model, based only on the expected travel time by auto
				// And 3 activity types.  Distributions of auto share by travel time were
				// estimated from CMAP travel tracker survey and are applied here.
				// If the time to travel for the activity is not known, 12pm is assumed (for skim time lookup)
				// If the locaion has not been planned we cannot estimate an approximate time and the average mode share is used
				//--------------------------------------------------------------------
				base_type* bthis = (base_type*)this;
				this_itf* pthis = (this_itf*)this;

				// references to external agents
				_planning_itf* planner = bthis->template Parent_Planner<_planning_itf*>();
				_person_itf* person = planner->template Parent_Person<_person_itf*>();
				_scheduler_itf* scheduler = person->template Scheduling_Faculty<_scheduler_itf*>();
				_mode_choice_itf* mode_chooser = planner->template Mode_Choice_Faculty<_mode_choice_itf*>();
				_network_itf* network = person->template network_reference<_network_itf*>();


				Vehicle_Components::Types::Vehicle_Type_Keys mode = mode_chooser->template Choose_Mode<this_itf*,Vehicle_Components::Types::Vehicle_Type_Keys>(pthis);	
				pthis->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>(mode);

			}
			template<typename TargetType> void Duration_Planning_Event_Handler()
			{
				
			}
			template<typename TargetType> void Start_Time_Planning_Event_Handler()
			{
				PUSH_TO_STACK("Start_Time_Planning_Event_Handler");

				this_itf* pthis = (this_itf*)this;
				base_type* bthis = (base_type*)this;

				// interfaces
				_planning_itf* planner = pthis->template Parent_Planner<_planning_itf*>();
				_person_itf* person = planner->template Parent_Person<_person_itf*>();
				_scheduler_itf* scheduler = person->template Scheduling_Faculty<_scheduler_itf*>();
				_scenario_itf* scenario = (_scenario_itf*)_global_scenario;

				// get the combined start time and duration
				_timing_choice_itf* timing_planner = planner->template Timing_Chooser<_timing_choice_itf*>();
				pair<Time_Seconds,Time_Seconds> start_and_duration = timing_planner->template Get_Start_Time_and_Duration<ComponentType*,Time_Seconds>(this);

				// make sure start time is not prior to current iteration
				Time_Seconds time_min = Simulation_Time.template Future_Time<Time_Seconds,Time_Seconds>(planner->template Planning_Time_Increment<Time_Seconds>());
				pthis->template Start_Time<Time_Seconds>(max<int>(start_and_duration.first,time_min.Value));


				// set the duration, making sure it fits into current schedule slot
				pthis->template Duration<Time_Seconds>(0.0f);
				float duration = max<float>(start_and_duration.second, (float)pthis->Minimum_Duration<Time_Seconds>());
				pthis->template Duration<Time_Seconds>(duration);

				//========================================================================
				// Resolve timing conflicts when timing is known
				bool is_scheduled =	scheduler->template Resolve_Timing_Conflict<this_itf*>(pthis,false);
				// if conflict not resolved remove activity from schedule and modify routing response time so no further planning is done
				if (!is_scheduled) 
				{
					pthis->template Unschedule_Activity_Events<NT>();
					scheduler->template Remove_Activity_Plan<this_itf*>(pthis);
				}


				//========================================================================
				// END HERE if no planner routing is requested through scenario
				if (!scenario->template do_planner_routing<bool>())	
				{
					POP_FROM_STACK;
					return;
				}
				//------------------------------------------------------------------------




				//========================================================================
				// start routing on the planning timestep at 2 times the estimated travel time from skims prior to departure - rounded to nearest minute
				Time_Minutes exp_ttime=0.0f;
				if (bthis->template Location_Is_Planned<NT>())
				{
					_network_itf* network = person->template network_reference<_network_itf*>();
					_activity_location_itf* orig = person->template Home_Location<_activity_location_itf*>();
					_activity_location_itf* dest = pthis->template Location<_activity_location_itf*>();
					exp_ttime = network->template Get_TTime<_activity_location_itf*,Vehicle_Components::Types::Vehicle_Type_Keys, Time_Minutes, Time_Minutes>(orig,dest,Vehicle_Components::Types::Vehicle_Type_Keys::SOV, pthis->template Start_Time<Time_Minutes>());
				}			
				exp_ttime = max<float>(exp_ttime,30.0f);


				//==============================================================================================
				// Reset the route planning time based on the expected departure time, and aggregate as needed
				//---------------------------
				// Aggregate plan routing
				if (scenario->template aggregate_routing<bool>())
				{
					Time_Minutes start_minutes = (int)(pthis->template Start_Time<Time_Minutes>() - (exp_ttime * 2.0));
					int start_increment = max<int>(Simulation_Time.template Convert_Time_To_Simulation_Timestep<Time_Minutes>(start_minutes), iteration());
					pthis->template Route_Planning_Time<Revision&>()._iteration = start_increment;
				}
				//---------------------------
				// Disaggregate plan routing
				else
				{
					Simulation_Timestep_Increment start_seconds = pthis->template Start_Time<Simulation_Timestep_Increment>() - Simulation_Time.template Convert_Time_To_Simulation_Timestep<Time_Minutes>(exp_ttime * 2.0);
					int start_increment = max<int>(start_seconds, iteration());
					pthis->template Route_Planning_Time<Revision&>()._iteration = start_increment;
				}	
				POP_FROM_STACK;
			}
			template<typename TargetType> void Involved_Persons_Planning_Event_Handler()
			{
				
			}
			
		};
		


		// Specialization for ADAPTS-style routine activities
		implementation struct ADAPTS_Routine_Activity_Plan_Implementation : public Basic_Activity_Plan_Implementation<MasterType,INHERIT(ADAPTS_Routine_Activity_Plan_Implementation)>
		{
			typedef Basic_Activity_Plan_Implementation<MasterType,INHERIT(ADAPTS_Routine_Activity_Plan_Implementation)> base_type;
			typedef typename Basic_Activity_Plan_Implementation<MasterType,INHERIT(ADAPTS_Routine_Activity_Plan_Implementation)>::Component_Type ComponentType;
			typedef Prototypes::Activity_Planner<ComponentType> this_itf;
		
			//================================================================================================================================================================================================
			//================================================================================================================================================================================================
			// Interfaces
			typedef  Person_Components::Prototypes::Person_Properties< typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(Properties)> _properties_itf;
			typedef  Person_Components::Prototypes::Person_Properties< typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(Static_Properties)> _static_properties_itf;
			typedef Person_Components::Prototypes::Person_Planner<typename type_of(base_type::Parent_Planner)> _planning_itf;
			typedef  Person_Components::Prototypes::Person_Scheduler< typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(Scheduling_Faculty)> _scheduler_itf;
			typedef Person_Components::Prototypes::Person<typename _planning_itf::get_type_of(Parent_Person)> _person_itf;
			typedef Person_Components::Prototypes::Destination_Chooser<typename _planning_itf::get_type_of(Destination_Choice_Faculty)> _dest_choice_itf;
			typedef Person_Components::Prototypes::Mode_Chooser<typename _planning_itf::get_type_of(Mode_Choice_Faculty)> _mode_choice_itf;
			typedef  Scenario_Components::Prototypes::Scenario< typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference)> _scenario_itf;
			typedef  Network_Components::Prototypes::Network< typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(network_reference)> _network_itf;
			typedef Network_Skimming_Components::Prototypes::Network_Skimming< typename _network_itf::get_type_of(skimming_faculty)> _skim_itf;
			typedef Person_Components::Prototypes::Activity_Timing_Chooser<typename _planning_itf::get_type_of(Timing_Chooser)> _timing_choice_itf;
			typedef  Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer< typename _network_itf::get_type_of(activity_locations_container)::value_type>::type>  _activity_location_itf;
			typedef  Random_Access_Sequence< typename _network_itf::get_type_of(activity_locations_container), _activity_location_itf*> _activity_locations_container_itf;

			typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename _activity_location_itf::get_type_of(origin_links)::value_type>::type>  _link_itf;
			typedef  Random_Access_Sequence< typename _activity_location_itf::get_type_of(origin_links), _link_itf*> _links_container_itf;

			typedef  Zone_Components::Prototypes::Zone<typename remove_pointer< typename _network_itf::get_type_of(zones_container)::value_type>::type>  _zone_itf;
			typedef  Pair_Associative_Container< typename _network_itf::get_type_of(zones_container), _zone_itf*> _zones_container_itf;

		/*	typedef Activity_Components::Prototypes::Activity_Planner<typename remove_pointer< typename type_of(base_type::Parent_Planner)::type_of(Activity_Container)::value_type>::type> _activity_plan_itf;
		/*	typedef Back_Insertion_Sequence< typename type_of(base_type::Parent_Planner)::type_of(Activity_Container),_activity_plan_itf*> _activity_plans_container_itf;

			typedef Movement_Plan_Components::Prototypes::Movement_Plan<typename remove_pointer< typename type_of(base_type::Parent_Planner)::type_of(Movement_Plans_Container)::value_type>::type> _movement_plan_itf;
			typedef Back_Insertion_Sequence< typename type_of(base_type::Parent_Planner)::type_of(Movement_Plans_Container),_movement_plan_itf*> _movement_plans_container_itf;

			//================================================================================================================================================================================================
			//================================================================================================================================================================================================

			// Fundamental activity properties
			template<typename TargetType> Types::PLAN_HORIZON_VALUES Activity_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Activity_Plan_Horizon);

			// Activity attribute planning properties
			template<typename TargetType> Types::PLAN_HORIZON_VALUES Location_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Location_Plan_Horizon);
			template<typename TargetType> Types::PLAN_HORIZON_VALUES Mode_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Mode_Plan_Horizon);
			template<typename TargetType> Types::PLAN_HORIZON_VALUES Start_Time_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Start_Time_Plan_Horizon);
			template<typename TargetType> Types::PLAN_HORIZON_VALUES Duration_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Duration_Plan_Horizon);
			template<typename TargetType> Types::PLAN_HORIZON_VALUES Involved_Persons_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Involved_Persons_Plan_Horizon);


			// Activity methods
			template<typename TargetType> void Initialize(TargetType act_type)
			{
				//UNLOCK(this->_update_lock);
				this_itf* pthis = (this_itf*)this;
				pthis->template Activity_Type<TargetType>(act_type);
				pthis->template Duration<Time_Seconds>(END);
				pthis->template Start_Time<Time_Seconds>(END);
				pthis->template Location<_activity_location_itf*>(nullptr);
				pthis->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
			}

			template<typename TargetType> void Set_Attribute_Planning_Times(TargetType planning_time, requires(TargetType,check_2(TargetType, Simulation_Timestep_Increment, is_same)))
			{
				base_type* base_this = (base_type*)this;

				base_this->template Start_Time_Planning_Time<  Revision&>()._iteration = max(_iteration+1, (int)planning_time);
				base_this->template Start_Time_Planning_Time<  Revision&>()._sub_iteration = 0;
				base_this->template Duration_Planning_Time<  Revision&>()._iteration = max(_iteration+1, (int)planning_time);
				base_this->template Duration_Planning_Time<  Revision&>()._sub_iteration = 1;
				base_this->template Location_Planning_Time<  Revision&>()._iteration = max(_iteration+1, (int)planning_time);
				base_this->template Location_Planning_Time<  Revision&>()._sub_iteration = 2;
				base_this->template Mode_Planning_Time<  Revision&>()._iteration = max(_iteration+1, (int)planning_time);
				base_this->template Mode_Planning_Time<  Revision&>()._sub_iteration = 3;
				base_this->template Involved_Persons_Planning_Time<  Revision&>()._iteration = max(_iteration+1, (int)planning_time);
				base_this->template Involved_Persons_Planning_Time<  Revision&>()._sub_iteration = 4;
				base_this->template Route_Planning_Time<  Revision&>()._iteration = max(_iteration+1, (int)planning_time);
				base_this->template Route_Planning_Time<  Revision&>()._sub_iteration = 5;
			}
			template<typename TargetType> void Set_Attribute_Planning_Times(TargetType planning_time, requires(TargetType,!check_2(TargetType, Simulation_Timestep_Increment, is_same)))
			{
				assert_check_2(TargetType, Simulation_Timestep_Increment, is_same, "Error: planning_time must be passed as a Simulation_Timestep_Increment type when using this function.");
			}
			tag_feature_as_available(Set_Attribute_Planning_Times);

			template<typename TargetType> void Location_Planning_Event_Handler()
			{
				this_itf* pthis = (this_itf*)this;
				base_type* base_this = (base_type*)this;
				_person_itf* person = base_this->_Parent_Planner->template Parent_Person<_person_itf*>();
				_activity_location_itf* orig = person->template Home_Location<_activity_location_itf*>();


				// Select the location based on the activity type
				_activity_location_itf* dest = nullptr;		
				if (pthis->template Activity_Type<Types::ACTIVITY_TYPES>() == Types::AT_HOME_ACTIVITY)
				{
					dest = person->template Home_Location<_activity_location_itf*>();
				}
				else if (pthis->template Activity_Type<Types::ACTIVITY_TYPES>() == Types::ACTIVITY_TYPES::PRIMARY_WORK_ACTIVITY)
				{
					dest = person->template Work_Location<_activity_location_itf*>();
				}
				else if (pthis->template Activity_Type<Types::ACTIVITY_TYPES>() == Types::ACTIVITY_TYPES::SCHOOL_ACTIVITY)
				{
					dest = person->template School_Location<_activity_location_itf*>();
				}
				else if (pthis->template Activity_Type<Types::ACTIVITY_TYPES>() == Types::ACTIVITY_TYPES::WORK_AT_HOME_ACTIVITY)
				{
					dest = person->template Home_Location<_activity_location_itf*>();
				}
				else
				{
					//----------------------------------------------------------------
					// Print to log file
					stringstream s;
					s <<"ACTIVITY NOT GENERATED, only work, home, school and work at home activities can be routine at this point: "<< person->template uuid<int>();
					s << "," <<orig << ", " <<dest<<endl;
					//base_this->_Parent_Planner->template Write_To_Log<stringstream&>(s);
					//----------------------------------------------------------------
					return;
				}

				// check that origin and destination are valid
				if (orig != nullptr && dest != nullptr) 
				{
					pthis->template Location<_activity_location_itf*>(dest);
				}
				else
				{
					//----------------------------------------------------------------
					// Print to log file
					stringstream s;
					s <<"ACTIVITY NOT GENERATED, null origin or destination: "<< person->template uuid<int>();
					s << "," <<orig << ", " <<dest<<endl;
					//base_this->_Parent_Planner->template Write_To_Log<stringstream&>(s);
					//----------------------------------------------------------------
				}
			}

			template<typename TargetType> void Mode_Planning_Event_Handler()
			{
				//====================================================================
				// Simplified mode choice model, based only on the expected travel time by auto
				// And 3 activity types.  Distributions of auto share by travel time were
				// estimated from CMAP travel tracker survey and are applied here.
				// If the time to travel for the activity is not known, 12pm is assumed (for skim time lookup)
				// If the locaion has not been planned we cannot estimate an approximate time and the average mode share is used
				//--------------------------------------------------------------------
				base_type* bthis = (base_type*)this;
				this_itf* pthis = (this_itf*)this;

				// references to external agents
				_planning_itf* planner = bthis->template Parent_Planner<_planning_itf*>();
				_person_itf* person = planner->template Parent_Person<_person_itf*>();
				_scheduler_itf* scheduler = person->template Scheduling_Faculty<_scheduler_itf*>();
				_mode_choice_itf* mode_chooser = planner->template Mode_Choice_Faculty<_mode_choice_itf*>();
				_network_itf* network = person->template network_reference<_network_itf*>();

				Vehicle_Components::Types::Vehicle_Type_Keys mode = mode_chooser->template Choose_Mode<this_itf*,Vehicle_Components::Types::Vehicle_Type_Keys>(pthis);	
				pthis->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>(mode);
			}

			template<typename TargetType> void Duration_Planning_Event_Handler()
			{
				this_itf* pthis = (this_itf*)this;
				base_type* bthis = (base_type*)this;

				_person_itf* person = bthis->_Parent_Planner->template Parent_Person<_person_itf*>();
				_static_properties_itf* static_properties = person->template Static_Properties<_static_properties_itf*>();

				Time_Minutes duration; 	
	
				if (pthis->template Activity_Type<ACTIVITY_TYPES>() == PRIMARY_WORK_ACTIVITY || pthis->template Activity_Type<ACTIVITY_TYPES>() == WORK_AT_HOME_ACTIVITY)
				{
					duration = static_properties->template Work_Hours<Time_Minutes>()/5.0;
				}
				else if (pthis->template Activity_Type<Types::ACTIVITY_TYPES>() == Types::ACTIVITY_TYPES::SCHOOL_ACTIVITY)
				{
					duration = 60.0 * 7.0;
				}
				else
				{
					THROW_EXCEPTION("ERROR: only work and school activities are currently allowed to be routine.");
				}
				pthis->template Duration<Time_Minutes>(duration);

			}

			template<typename TargetType> void Start_Time_Planning_Event_Handler()
			{
				PUSH_TO_STACK("Start_Time_Planning_Event_Handler");

				this_itf* pthis = (this_itf*)this;
				base_type* bthis = (base_type*)this;

				_person_itf* person = bthis->_Parent_Planner->template Parent_Person<_person_itf*>();
				_static_properties_itf* static_properties = person->template Static_Properties<_static_properties_itf*>();
				_scheduler_itf* scheduler = person->template Scheduling_Faculty<_scheduler_itf*>();
				_scenario_itf* scenario = (_scenario_itf*)_global_scenario;

				ACTIVITY_TYPES act_type = pthis->template Activity_Type<ACTIVITY_TYPES>();

				// School Activity start time (randomly between 7 and 9AM)
				if (act_type == ACTIVITY_TYPES::SCHOOL_ACTIVITY || act_type == ACTIVITY_TYPES::PRIMARY_WORK_ACTIVITY)
				{
					// interfaces
					_planning_itf* planner = pthis->template Parent_Planner<_planning_itf*>();
					_person_itf* person = planner->template Parent_Person<_person_itf*>();

					// get the combined start time and duration
					_timing_choice_itf* timing_planner = planner->template Timing_Chooser<_timing_choice_itf*>();
					pair<Time_Seconds,Time_Seconds> start_and_duration = timing_planner->template Get_Start_Time_and_Duration<ComponentType*,Time_Seconds>(this);

					// make sure start time is not prior to current iteration
					Time_Seconds time_min = Simulation_Time.template Future_Time<Time_Seconds,Time_Seconds>(planner->template Planning_Time_Increment<Time_Seconds>());
					pthis->template Start_Time<Time_Seconds>(max<int>(start_and_duration.first,time_min.Value));
					pthis->template Duration<Time_Seconds>(start_and_duration.second);
				}
				else THROW_EXCEPTION("ERROR: only work and school activities are currently allowed to be routine.");


				//========================================================================
				// Resolve timing conflicts when timing is known
				//------------------------------------------------------------------------
				bool is_scheduled =	scheduler->template Resolve_Timing_Conflict<this_itf*>(pthis,false);
				// if conflict not resolved remove activity from schedule and modify routing response time so no further planning is done
				if (!is_scheduled) 
				{
					pthis->template Unschedule_Activity_Events<NT>();
					scheduler->template Remove_Activity_Plan<this_itf*>(pthis);
				}


				//========================================================================
				// END HERE if no planner routing is requested through scenario
				if (!scenario->template do_planner_routing<bool>())
				{
					POP_FROM_STACK;
					return;
				}
				//------------------------------------------------------------------------



				//==============================================================================================
				// OTHERWISE, reset the event time for the planning router based on expected departure time
				//------------------------------------------------------------------------
				// start routing on the planning timestep at 1.5 times the estimated travel time from skims prior to departure - rounded to nearest minute
				Time_Minutes exp_ttime=0.0f;
				if (bthis->template Location_Is_Planned<NT>())
				{
					_network_itf* network = person->template network_reference<_network_itf*>();
					_activity_location_itf* orig = person->template Home_Location<_activity_location_itf*>();
					_activity_location_itf* dest = bthis->template Location<_activity_location_itf*>();

					exp_ttime = network->template Get_TTime<_activity_location_itf*,Vehicle_Components::Types::Vehicle_Type_Keys, Time_Hours,Time_Minutes >(orig,dest,Vehicle_Components::Types::Vehicle_Type_Keys::SOV, pthis->template Start_Time<Time_Hours>());
				}
				exp_ttime = max<float>(exp_ttime,30.0f);
				
				//---------------------------
				// Aggregate plan routing
				if (scenario->template aggregate_routing<bool>())
				{
					Time_Minutes start_minutes = (int)(pthis->template Start_Time<Time_Minutes>() - (exp_ttime * 2.0));
					int start_increment = max<int>(Simulation_Time.template Convert_Time_To_Simulation_Timestep<Time_Minutes>(start_minutes), iteration());
					pthis->template Route_Planning_Time<Revision&>()._iteration = start_increment;
				}
				//---------------------------
				// Disaggregate plan routing
				else
				{
					Simulation_Timestep_Increment start_seconds = pthis->template Start_Time<Simulation_Timestep_Increment>() - Simulation_Time.template Convert_Time_To_Simulation_Timestep<Time_Minutes>(exp_ttime * 2.0);
					int start_increment = max<int>(start_seconds, iteration());
					pthis->template Route_Planning_Time<Revision&>()._iteration = start_increment;
				}

				POP_FROM_STACK;
			}

			template<typename TargetType> void Involved_Persons_Planning_Event_Handler()
			{
				
			}

			template<typename TargetType> void Set_Meta_Attributes()
			{
				//===========================================================================================================================
				// Initialize local variables for use
				//---------------------------------------------------------------------------------------------------------------------------

				// get references to use in model
				base_type* base_this = (base_type*)this;
				this_itf* pthis = (this_itf*)this;
				_planning_itf* planner = pthis->template Parent_Planner<_planning_itf*>();
				_person_itf* person = planner->template Parent_Person<_person_itf*>();
				_properties_itf* properties = person->template Properties<_properties_itf*>();
				_static_properties_itf* static_props = person->template Static_Properties<_static_properties_itf*>();

				//Create local variables
				int ACT1, ACT2, ACT3, ACT4, ACT5, Employed, Student, Male, Senior, TELEWORK, ICT_USE;
				ACT1 = 0; ACT2 = 0; ACT3 = 0; ACT4 = 0; ACT5 = 0; Employed = 0; Student = 0; Male = 0; Senior = 0; TELEWORK = 0; ICT_USE = 0;
				float AvgDur, AvgFreq;

				if (base_this->_Activity_Type == WORK_AT_HOME_ACTIVITY || base_this->_Activity_Type == PRIMARY_WORK_ACTIVITY || base_this->_Activity_Type == SCHOOL_ACTIVITY || base_this->_Activity_Type == OTHER_WORK_ACTIVITY) ACT1 = 1;
				else if (base_this->_Activity_Type == HEALTHCARE_ACTIVITY || base_this->_Activity_Type == RELIGIOUS_OR_CIVIC_ACTIVITY || base_this->_Activity_Type == PERSONAL_BUSINESS_ACTIVITY) ACT2 = 1;
				else if (base_this->_Activity_Type == Types::ERRANDS_ACTIVITY || base_this->_Activity_Type == PICK_UP_OR_DROP_OFF_ACTIVITY || base_this->_Activity_Type == SERVICE_VEHICLE_ACTIVITY) ACT3 = 1;
				else if (base_this->_Activity_Type == Types::LEISURE_ACTIVITY|| base_this->_Activity_Type == Types::EAT_OUT_ACTIVITY || base_this->_Activity_Type == Types::RECREATION_ACTIVITY || base_this->_Activity_Type == Types::SOCIAL_ACTIVITY) ACT4 = 1;
				else if (base_this->_Activity_Type == Types::OTHER_SHOPPING_ACTIVITY ||base_this-> _Activity_Type == Types::MAJOR_SHOPPING_ACTIVITY) ACT5 = 1;
				if (static_props->template Is_Employed<bool>()) Employed = 1;
				if (static_props->template Is_Student<bool>()) Student = 1;
				if (static_props->template Gender<GENDER>() == GENDER::MALE) Male = 1;
				if (static_props->template Age<int>() >= 65) Senior = 1;
				//if (PER.PersonData.ICT_Use != IctType.NULL || PER.PersonData.ICT_Use != IctType.UseLow) ICT_USE = 1;
				if (static_props->template Journey_To_Work_Mode<Person_Components::Types::JOURNEY_TO_WORK_MODE>() == JOURNEY_TO_WORK_MODE::WORKMODE_WORK_AT_HOME) TELEWORK = 1;
				AvgFreq = properties->template Average_Activity_Frequency<ACTIVITY_TYPES,float>(base_this->_Activity_Type);
				AvgDur = properties->template Average_Activity_Duration<ACTIVITY_TYPES,Time_Minutes>(base_this->_Activity_Type);


				//===========================================================================================================================
				// FLEXIBILITY CALCULATIONS
				//---------------------------------------------------------------------------------------------------------------------------
				matrix<double> alpha = matrix<double>(matrix<double>::index_type(3,5));
				alpha(0,0)=-99999999;	alpha(0,1)=-99999999;	alpha(0,2)=-99999999;	alpha(0,3)=-99999999;	alpha(0,4)=-99999999;
				alpha(1,0)=0;			alpha(1,1)=0;			alpha(1,2)=0;			alpha(1,3)=0;			alpha(1,4)=0;
				alpha(2,0)=99999999;	alpha(2,1)=99999999;	alpha(2,2)=99999999;	alpha(2,3)=99999999;	alpha(2,4)=99999999;

				matrix<double> Sigma = matrix<double>(matrix<double>::index_type(5,5)); // covariance matrix
				Sigma(0,0)=1;			Sigma(0,1)=0;			Sigma(0,2)=0;			Sigma(0,3)=0;			Sigma(0,4)=0;
				Sigma(1,0)=0.099646;	Sigma(1,1)=1;			Sigma(1,2)=0;			Sigma(1,3)=0;			Sigma(1,4)=0;
				Sigma(2,0)=-0.053507;	Sigma(2,1)=-0.03372;	Sigma(2,2)=1;			Sigma(2,3)=0;			Sigma(2,4)=0;
				Sigma(3,0)=0.095299;	Sigma(3,1)=0.305332;	Sigma(3,2)=-0.113887;	Sigma(3,3)=1;			Sigma(3,4)=0;
				Sigma(4,0)=0.124233;	Sigma(4,1)=0.168151;	Sigma(4,2)=0.025553;	Sigma(4,3)=-0.099794;	Sigma(4,4)=1;

				double P_per[2]; // Holds probability for each level (index by i) of each response (index by j)
				double P_mod[2];
				double P_loc[2];
				double P_tim[2];
				double P_dur[2];

				//Calculate xB values for each response
				double xB_mod = -0.383692 + 0.362839 * Employed + 0.4215 * Student - 0.217904 * Male - 0.267344 * Senior + 0.691999 * Male * Senior + 0.231177 * AvgFreq + 0.568976 * ACT1 * ICT_USE - 1.039075 * ACT1 * AvgFreq;
				double xB_per = -0.338294 + 0.347752 * Student - 0.536749 * Male + 0.259776 * Senior + 0.478152 * Male * Senior + 0.532325 * ICT_USE + 1.508163 * TELEWORK - 1.655547 * ICT_USE * TELEWORK - 1.390818 * ACT1 * +1.352461 * ACT1 * TELEWORK - 0.598393 * ACT2 + 11.034344 * ACT2 * AvgDur - 0.617948 * ACT3 + 1.727868 * AvgFreq * ACT3 + 0.437761 * AvgFreq * ACT4 + 6.708181 * ACT4 * AvgDur - 0.606538 * ACT5 + 0.325567 * AvgFreq * ACT5;
				double xB_loc = -0.368786 - 1.174171 * Male - 0.1914 * Senior + 1.346607 * Male * Senior - 0.3674 * ICT_USE + 0.99392 * TELEWORK - 0.815509 * ICT_USE * TELEWORK + 1.023933 * ACT1 * TELEWORK + 2.325112 * ACT1 * AvgFreq - 8.966545 * ACT2 * AvgDur - 1.02207 * AvgFreq * ACT3 + 0.904036 * ACT4 + 0.637477 * AvgFreq * ACT4 - 8.32642 * ACT4 * AvgDur + 2.382859 * ACT5;
				double xB_tim = 1.326259 - 0.38711 * Male - 0.226637 * Senior + 0.484623 * Male * Senior - 0.302255 * ICT_USE + 0.196488 * TELEWORK + 3.3758 * ACT2 * AvgDur - 0.400564 * ACT3 + 1.301273 * AvgFreq * ACT3 - 0.601686 * ACT5 + 0.192867 * AvgFreq * ACT5;
				double xB_dur = -1.239259 + 1.294471 * Employed + 1.461113 * Student - 0.246144 * Senior - 0.374409 * Male * Senior - 0.23527 * ICT_USE * TELEWORK + 1.30007 * AvgDur + 0.621008 * ACT1 * +0.994951 * ACT1 * ICT_USE - 1.124459 * ACT1 * TELEWORK - 0.56222 * ACT1 * AvgFreq - 2.070631 * ACT1 * AvgDur - 0.167278 * ICT_USE * ACT4 + 0.769745 * AvgFreq * ACT4 + 0.487925 * AvgFreq * ACT5;			

				// Get probabilities of each response level
				for (int i = 0; i < 2; i++)
				{
					P_mod[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha(i + 1, 0) - xB_mod);
					P_per[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha(i + 1, 1) - xB_per);
					P_loc[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha(i + 1, 2) - xB_loc);
					P_tim[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha(i + 1, 3) - xB_tim);
					P_dur[i] = Normal_Distribution->template Cumulative_Distribution_Value<float>(alpha(i + 1, 4) - xB_dur);
				}

				// Set correlated random variables
				boost::container::vector<double> rand;
				GLOBALS::Normal_RNG.template Correlated_Rands<double>(rand, Sigma);

				// Set flexibility values
				for (int i = 0; i < 2; i++)
				{
					if (rand[0] < P_mod[i]) {base_this->_Mode_Flexibility = (Types::FLEXIBILITY_VALUES)i;				rand[0] = 999;}
					if (rand[1] < P_per[i]) {base_this->_Involved_Persons_Flexibility = (Types::FLEXIBILITY_VALUES)i;	rand[1] = 999; }
					if (rand[2] < P_loc[i]) {base_this->_Location_Flexibility = (Types::FLEXIBILITY_VALUES)i;			rand[2] = 999; }
					if (rand[3] < P_tim[i]) {base_this->_Start_Time_Flexibility = (Types::FLEXIBILITY_VALUES)i;			rand[3] = 999; }
					if (rand[4] < P_dur[i]) {base_this->_Duration_Flexibility = (Types::FLEXIBILITY_VALUES)i;			rand[4] = 999; }
				}
            
				// Set results of flexiblity model to use in next models
				double IFLOC, IFTIM, IFDUR, IFMOD;
				IFMOD = P_mod[0];
				IFLOC = P_loc[0];
				IFTIM = P_tim[0];
				IFDUR = P_dur[0];

				pthis->template Location_Flexibility<Types::FLEXIBILITY_VALUES>(Types::FLEXIBILITY_VALUES::LOW_FLEXIBILITY);
			}
			tag_feature_as_available(Set_Meta_Attributes);
		};



		// Specialization for ADAPTS-style at-home activities
		implementation struct ADAPTS_At_Home_Activity_Plan_Implementation : public Basic_Activity_Plan_Implementation<MasterType,INHERIT(ADAPTS_At_Home_Activity_Plan_Implementation)>
		{
			typedef Basic_Activity_Plan_Implementation<MasterType,INHERIT(ADAPTS_At_Home_Activity_Plan_Implementation)> base_type;
			typedef typename Basic_Activity_Plan_Implementation<MasterType,INHERIT(ADAPTS_At_Home_Activity_Plan_Implementation)>::Component_Type ComponentType;
			typedef Prototypes::Activity_Planner<ComponentType> this_itf;
		
			member_component_and_feature_accessor(Fixed_Departure, Value, Basic_Units::Prototypes::Time,Basic_Units::Implementations::Time_Implementation<NT>)
			
			//================================================================================================================================================================================================
			//================================================================================================================================================================================================
			// Interfaces
			typedef  Person_Components::Prototypes::Person_Properties< typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(Properties)> _properties_itf;
			typedef  Person_Components::Prototypes::Person_Properties< typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(Static_Properties)> _static_properties_itf;
			typedef Person_Components::Prototypes::Person_Planner<typename type_of(base_type::Parent_Planner)> _planning_itf;
			typedef Person_Components::Prototypes::Person<typename _planning_itf::get_type_of(Parent_Person)> _person_itf;
			typedef Person_Components::Prototypes::Destination_Chooser<typename _planning_itf::get_type_of(Destination_Choice_Faculty)> _dest_choice_itf;
			typedef  Scenario_Components::Prototypes::Scenario< typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference)> _scenario_itf;
			typedef  Network_Components::Prototypes::Network< typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(network_reference)> _network_itf;
			typedef Network_Skimming_Components::Prototypes::Network_Skimming< typename _network_itf::get_type_of(skimming_faculty)> _skim_itf;
			typedef Person_Components::Prototypes::Activity_Timing_Chooser<typename _planning_itf::get_type_of(Timing_Chooser)> _timing_choice_itf;
			typedef  Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer< typename _network_itf::get_type_of(activity_locations_container)::value_type>::type>  _activity_location_itf;
			typedef  Random_Access_Sequence< typename _network_itf::get_type_of(activity_locations_container), _activity_location_itf*> _activity_locations_container_itf;

			typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename _activity_location_itf::get_type_of(origin_links)::value_type>::type>  _link_itf;
			typedef  Random_Access_Sequence< typename _activity_location_itf::get_type_of(origin_links), _link_itf*> _links_container_itf;

			typedef  Zone_Components::Prototypes::Zone<typename remove_pointer< typename _network_itf::get_type_of(zones_container)::value_type>::type>  _zone_itf;
			typedef  Pair_Associative_Container< typename _network_itf::get_type_of(zones_container), _zone_itf*> _zones_container_itf;

		/*	typedef Activity_Components::Prototypes::Activity_Planner<typename remove_pointer< typename type_of(base_type::Parent_Planner)::type_of(Activity_Container)::value_type>::type> _activity_plan_itf;
		/*	typedef Back_Insertion_Sequence< typename type_of(base_type::Parent_Planner)::type_of(Activity_Container),_activity_plan_itf*> _activity_plans_container_itf;

			typedef Movement_Plan_Components::Prototypes::Movement_Plan<typename remove_pointer< typename type_of(base_type::Parent_Planner)::type_of(Movement_Plans_Container)::value_type>::type> _movement_plan_itf;
			typedef Back_Insertion_Sequence< typename type_of(base_type::Parent_Planner)::type_of(Movement_Plans_Container),_movement_plan_itf*> _movement_plans_container_itf;

			//================================================================================================================================================================================================
			//================================================================================================================================================================================================

			// Fundamental activity properties
			template<typename TargetType> Types::PLAN_HORIZON_VALUES Activity_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Activity_Plan_Horizon);

			// Activity attribute planning properties
			template<typename TargetType> Types::PLAN_HORIZON_VALUES Location_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Location_Plan_Horizon);
			template<typename TargetType> Types::PLAN_HORIZON_VALUES Mode_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Mode_Plan_Horizon);
			template<typename TargetType> Types::PLAN_HORIZON_VALUES Start_Time_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Start_Time_Plan_Horizon);
			template<typename TargetType> Types::PLAN_HORIZON_VALUES Duration_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Duration_Plan_Horizon);
			template<typename TargetType> Types::PLAN_HORIZON_VALUES Involved_Persons_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Involved_Persons_Plan_Horizon);
			/*m_data(Types::FLEXIBILITY_VALUES, Location_Flexibility, NONE, NONE);
			m_data(Types::FLEXIBILITY_VALUES, Mode_Flexibility, NONE, NONE);
			m_data(Types::FLEXIBILITY_VALUES, Start_Time_Flexibility, NONE, NONE);
			m_data(Types::FLEXIBILITY_VALUES, Duration_Flexibility, NONE, NONE);
			m_data(Types::FLEXIBILITY_VALUES, Involved_Persons_Flexibility, NONE, NONE);*/


			// Activity methods
			template<typename TimeType, typename ModeType> void Initialize(TimeType departure_time, TimeType start_time, TimeType duration, ModeType mode)
			{		
				//UNLOCK(this->_update_lock);
				this_itf* pthis = (this_itf*)this;

				_planning_itf* planner = pthis->template Parent_Planner<_planning_itf*>();
				_person_itf* person = planner->template Parent_Person<_person_itf*>();
				
				pthis->template Activity_Type<Types::ACTIVITY_TYPES>(Types::ACTIVITY_TYPES::AT_HOME_ACTIVITY);
				pthis->template Duration<TimeType>(duration);
				pthis->template Start_Time<TimeType>(start_time);
				pthis->template Location<_activity_location_itf*>(person->template Home_Location<_activity_location_itf*>());
				pthis->template Mode<Vehicle_Components::Types::Vehicle_Type_Keys>(mode);
				pthis->template Fixed_Departure<TimeType>(departure_time);
				
			}

			template<typename TargetType> void Set_Attribute_Planning_Times(TargetType planning_time, requires(TargetType,check_2(TargetType, Simulation_Timestep_Increment, is_same)))
			{
				base_type* base_this = (base_type*)this;

				base_this->template Start_Time_Planning_Time<  Revision&>()._iteration = END+1;
				base_this->template Start_Time_Planning_Time<  Revision&>()._sub_iteration = END+1;
				base_this->template Duration_Planning_Time<  Revision&>()._iteration = END+1;
				base_this->template Duration_Planning_Time<  Revision&>()._sub_iteration = END+1;
				base_this->template Location_Planning_Time<  Revision&>()._iteration = END+1;
				base_this->template Location_Planning_Time<  Revision&>()._sub_iteration = END+1;
				base_this->template Mode_Planning_Time<  Revision&>()._iteration = END+1;
				base_this->template Mode_Planning_Time<  Revision&>()._sub_iteration = END+1;
				base_this->template Involved_Persons_Planning_Time<  Revision&>()._iteration = END+1;
				base_this->template Involved_Persons_Planning_Time<  Revision&>()._sub_iteration = END+1;
				base_this->template Route_Planning_Time<  Revision&>()._iteration = _iteration+1;
				base_this->template Route_Planning_Time<  Revision&>()._sub_iteration = Scenario_Components::Types::ACTIVITY_ATTRIBUTE_PLANNING_SUB_ITERATION;
			}
			template<typename TargetType> void Set_Attribute_Planning_Times(TargetType planning_time, requires(TargetType,!check_2(TargetType, Simulation_Timestep_Increment, is_same)))
			{
				assert_check_2(TargetType, Simulation_Timestep_Increment, is_same, "Error: planning_time must be passed as a Simulation_Timestep_Increment type when using this function.");
			}
			tag_feature_as_available(Set_Attribute_Planning_Times);

			template<typename TargetType> void Location_Planning_Event_Handler(){} tag_feature_as_available(Location_Planning_Event_Handler);
			template<typename TargetType> void Mode_Planning_Event_Handler(){} tag_feature_as_available(Mode_Planning_Event_Handler);
			template<typename TargetType> void Duration_Planning_Event_Handler(){} tag_feature_as_available(Duration_Planning_Event_Handler);
			template<typename TargetType> void Start_Time_Planning_Event_Handler(){} tag_feature_as_available(Start_Time_Planning_Event_Handler);
			template<typename TargetType> void Involved_Persons_Planning_Event_Handler(){} tag_feature_as_available(Involved_Persons_Planning_Event_Handler);

			template<typename TargetType> void Set_Meta_Attributes()
			{
				base_type* base_this = (base_type*)this;
				base_this->_Duration_Flexibility = Types::FLEXIBILITY_VALUES::HIGH_FLEXIBILITY;
				base_this->_Location_Flexibility = Types::FLEXIBILITY_VALUES::HIGH_FLEXIBILITY;
				base_this->_Mode_Flexibility = Types::FLEXIBILITY_VALUES::HIGH_FLEXIBILITY;
				base_this->_Start_Time_Flexibility = Types::FLEXIBILITY_VALUES::HIGH_FLEXIBILITY;
				base_this->_Involved_Persons_Flexibility = Types::FLEXIBILITY_VALUES::HIGH_FLEXIBILITY;
		
				this->_Duration_Planning_Time = Types::PLAN_HORIZON_VALUES::IMPULSIVE;
				this->_Location_Planning_Time = Types::PLAN_HORIZON_VALUES::IMPULSIVE;
				this->_Mode_Planning_Time = Types::PLAN_HORIZON_VALUES::IMPULSIVE;
				this->_Start_Time_Planning_Time = Types::PLAN_HORIZON_VALUES::IMPULSIVE;
				this->_Involved_Persons_Planning_Time = Types::PLAN_HORIZON_VALUES::IMPULSIVE;
			}
			tag_feature_as_available(Set_Meta_Attributes);
		};



		// Stripped down activity record with minimal memory usage (used for storing and printing completed activities)
		implementation struct Activity_Record : public Polaris_Component< MasterType,INHERIT(Activity_Record), Data_Object, ParentType>
		{
			// Tag as implementation
			typedef typename Polaris_Component<MasterType,INHERIT(Activity_Record),Data_Object>::Component_Type ComponentType;

			template<typename TargetType> void Initialize(TargetType object/*,requires(TargetType,check(strip_modifiers(TargetType), Concepts::Is_Activity_Plan_Prototype))*/)
			{
				typedef Activity_Location_Components::Prototypes::Activity_Location<typename MasterType::activity_location_type> location_itf;
				typedef Zone_Components::Prototypes::Zone<typename MasterType::zone_type> zone_itf;
				typedef Prototypes::Activity_Planner<typename MasterType::activity_type> object_itf;
				object_itf* obj = (object_itf*)object;

				this->_Activity_Plan_ID = obj->Activity_Plan_ID<char>();
				this->_Activity_Type = (char)obj->Activity_Type<Types::ACTIVITY_TYPES>();
				this->Location<location_itf*>(obj->Location<location_itf*>());
				this->_Start_Time = obj->Start_Time<Time_Minutes>();
				this->_Duration = obj->Duration<Time_Minutes>();
			}

			// Fundamental activity properties
			m_data(char, Activity_Plan_ID, NONE, NONE);
			m_data(char, Activity_Type, NONE, NONE);
			m_prototype(Activity_Location_Components::Prototypes::Activity_Location_Prototype< typename MasterType::activity_location_type>, Location, check(strip_modifiers(TargetType),Activity_Location_Components::Concepts::Is_Activity_Location), check(strip_modifiers(TargetType),Activity_Location_Components::Concepts::Is_Activity_Location));
			m_data(Time_Minutes, Start_Time, check_2(TargetType,Time_Minutes, is_same),check_2(TargetType,Time_Minutes, is_same));
			m_data(Time_Minutes, Duration, check_2(TargetType,Time_Minutes, is_same),check_2(TargetType,Time_Minutes, is_same));
			m_data(Time_Minutes, Travel_Time, check_2(TargetType,Time_Minutes, is_same),check_2(TargetType,Time_Minutes, is_same));
		};



		//=======================================================================
		// Static activity plan member initialization
		//-----------------------------------------------------------------------
		//static_array_definition(Basic_Activity_Plan_Implementation, Location_Planning_Counter, int);
		static_array_definition(Basic_Activity_Plan_Implementation, Route_Planning_Counter, int);
		//static_array_definition(Basic_Activity_Plan_Implementation, Scheduling_Counter, int);
		//static_member_initialization(Basic_Activity_Plan_Implementation, Location_Planning_Count, 0);
		static_member_initialization(Basic_Activity_Plan_Implementation, Route_Planning_Count, 0);
		//static_member_initialization(Basic_Activity_Plan_Implementation, Scheduling_Count, 0);
		template<typename MasterType, typename InheritanceList> _lock Basic_Activity_Plan_Implementation<MasterType,  InheritanceList>::_update_lock = 0;
	}
}

