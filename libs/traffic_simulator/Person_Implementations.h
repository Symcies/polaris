#pragma once

#include "Scenario_Prototype.h"
#include "Person_Prototype.h"
#include "Person_Planner_Implementations.h"
#include "Movement_Plan_Prototype.h"
#include "Network_Skimming_Prototype.h"
#include "Activity_Prototype.h"


namespace Person_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Person Agent classes
		//----------------------------------------------------------------------------------
		implementation struct Person_Implementation : public Polaris_Component<MasterType,INHERIT(Person_Implementation),Execution_Object,true>
		{
			// Tag as Implementation
			typedef typename Polaris_Component<MasterType,INHERIT(Person_Implementation),Execution_Object>::Component_Type ComponentType;

			typedef typename MasterType::person_data_logger_type person_data_logger_type;

			// Parent agent
			m_prototype(Household_Components::Prototypes::Household< typename MasterType::household_type>, Household, NONE, NONE);

			//=======================================================================================================================================================================
			// DATA MEMBERS
			//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
			m_prototype(PopSyn::Prototypes::Synthesis_Zone< typename MasterType::zone>, home_synthesis_zone, NONE, NONE);
			m_prototype(Vehicle_Components::Prototypes::Vehicle<typename MasterType::vehicle_type>, vehicle, NONE, NONE);
			m_prototype(Routing_Components::Prototypes::Routing<typename MasterType::routing_type>, router, NONE, NONE);
			
			m_prototype(Prototypes::Person_Mover<typename MasterType::person_mover_type>, Moving_Faculty, NONE, NONE);
			m_prototype(Prototypes::Person_Planner< typename MasterType::person_planner_type>, Planning_Faculty, NONE, NONE);
			m_prototype(Prototypes::Person_Scheduler< typename MasterType::person_scheduler_type>, Scheduling_Faculty, NONE, NONE);
			m_prototype(Prototypes::Person_Perception< typename MasterType::person_perception_type>, Perception_Faculty, NONE, NONE);
			m_prototype(Prototypes::Person_Properties< typename MasterType::person_properties_type>, Properties, NONE, NONE);
			m_prototype(Prototypes::Person_Properties<typename MasterType::person_static_properties_type>, Static_Properties, NONE, NONE);
			
			m_prototype(Activity_Location_Components::Prototypes::Activity_Location< typename MasterType::activity_location_type>, current_location, NONE, NONE);

			//m_prototype(Network_Components::Prototypes::Network< typename MasterType::network_type>, network_reference, NONE, NONE);
			//m_prototype(Scenario_Components::Prototypes::Scenario< typename MasterType::scenario_type>, scenario_reference, NONE, NONE);
			// quick fix for moving network and scenario handling to the perception class
			typedef typename Perception_Faculty_type::type_of(Network) network_reference_type;
			typedef typename Perception_Faculty_type::type_of(Scenario) scenario_reference_type;
			template<typename TargetType> TargetType network_reference()
			{
				return this->_Perception_Faculty->template Network<TargetType>();
			}
			template<typename TargetType> void network_reference(TargetType set_value)
			{
				this->_Perception_Faculty->template Network<TargetType>(set_value);
			}	
			tag_getter_as_available(network_reference);		
			tag_setter_as_available(network_reference);	
			template<typename TargetType> TargetType scenario_reference()
			{
				return this->_Perception_Faculty->template Scenario<TargetType>();
			}
			template<typename TargetType> void scenario_reference(TargetType set_value)
			{
				this->_Perception_Faculty->template Scenario<TargetType>(set_value);
			}	
			tag_getter_as_available(scenario_reference);
			tag_setter_as_available(scenario_reference);
			typedef Network_Components::Prototypes::Network<typename Perception_Faculty_type::type_of(Network)> network_reference_interface;
			typedef Scenario_Components::Prototypes::Scenario<typename Perception_Faculty_type::type_of(Scenario)> scenario_reference_interface;


			// Agent ID
			m_data(long,uuid, NONE, NONE);
			m_data(long,internal_id, NONE, NONE);
			m_data(bool,has_pretrip_information,check_2(TargetType,bool,is_same), check_2(TargetType,bool,is_same));
			m_data(bool,has_done_replanning,check_2(TargetType,bool,is_same), check_2(TargetType,bool,is_same));

			m_data(shared_ptr<polaris::io::Person>, person_record, NONE, NONE);

			// First iteration  - sets the next iteration after all planning is completed
			member_component_and_feature_accessor(First_Iteration, Value, Basic_Units::Prototypes::Time, Basic_Units::Implementations::Time_Implementation<NT>);

		
			// Record of completed activities (stores a simplified subset of activity data)
			m_container(boost::container::list<typename MasterType::activity_record_type*>, Activity_Record_Container, NONE, NONE);

			//=======================================================================================================================================================================
			// INTERFACE DEFINITIONS
			//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
			typedef Prototypes::Person<ComponentType> this_itf;	
			typedef Prototypes::Activity_Generator<typename Planning_Faculty_interface::get_type_of(Activity_Generation_Faculty)> generator_itf;
			typedef Prototypes::Destination_Chooser<typename type_of(Planning_Faculty)::type_of(Destination_Choice_Faculty)> destination_choice_itf;
			typedef Prototypes::Mode_Chooser<typename type_of(Planning_Faculty)::type_of(Mode_Choice_Faculty)> mode_choice_itf;
			typedef Prototypes::Activity_Timing_Chooser<typename type_of(Planning_Faculty)::type_of(Timing_Chooser)> timing_choice_itf;
			typedef  Zone_Components::Prototypes::Zone<typename remove_pointer< typename network_reference_interface::get_type_of(zones_container)::value_type>::type>  zone_interface;
			typedef Pair_Associative_Container< typename network_reference_interface::get_type_of(zones_container), zone_interface*> zones_container_interface;

			typedef  Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer< typename network_reference_interface::get_type_of(activity_locations_container)::value_type>::type>  location_interface;
			typedef Random_Access_Sequence< typename network_reference_interface::get_type_of(activity_locations_container), location_interface*> locations_container_interface;

			typedef Activity_Components::Prototypes::Activity_Planner<typename remove_pointer< typename type_of(Activity_Record_Container)::value_type>::type> Activity_Record;
			typedef Back_Insertion_Sequence< typename type_of(Activity_Record_Container),Activity_Record*> Activity_Records;

			
			//=======================================================================================================================================================================
			// FEATURES
			//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
			template<typename TargetType> void Initialize(TargetType id)
			{	
				// Create and Initialize the Properties faculty
				_Properties = (Properties_interface*)Allocate<type_of(Properties)>();
				_Properties->template Initialize<void>();
				_Properties->template Parent_Person<ComponentType*>(this);

				// Create and Initialize the Planner faculty and its subcomponents
				_Planning_Faculty = (Planning_Faculty_interface*)Allocate<type_of(Planning_Faculty)>();	
				_Planning_Faculty->template Parent_Person<ComponentType*>(this);
				_Planning_Faculty->template Initialize<NULLTYPE>();
				generator_itf* generator = (generator_itf*)Allocate<typename type_of(Planning_Faculty)::type_of(Activity_Generation_Faculty)>();
				generator->template Parent_Planner<Planning_Faculty_interface*>(_Planning_Faculty);
				_Planning_Faculty->template Activity_Generation_Faculty<generator_itf*>(generator);

				destination_choice_itf* destination_chooser = (destination_choice_itf*)Allocate<typename type_of(Planning_Faculty)::type_of(Destination_Choice_Faculty)>();
				destination_chooser->template Parent_Planner<Planning_Faculty_interface*>(_Planning_Faculty);
				_Planning_Faculty->template Destination_Choice_Faculty<destination_choice_itf*>(destination_chooser);

				mode_choice_itf* mode_chooser = (mode_choice_itf*)Allocate<typename type_of(Planning_Faculty)::type_of(Mode_Choice_Faculty)>();
				mode_chooser->template Parent_Planner<Planning_Faculty_interface*>(_Planning_Faculty);
				_Planning_Faculty->template Mode_Choice_Faculty<mode_choice_itf*>(mode_chooser);

				timing_choice_itf* timing_chooser = (timing_choice_itf*)Allocate<typename type_of(Planning_Faculty)::type_of(Timing_Chooser)>();
				timing_chooser->template Parent_Planner<Planning_Faculty_interface*>(_Planning_Faculty);
				_Planning_Faculty->template Timing_Chooser<timing_choice_itf*>(timing_chooser);

				// Create and Initialize the Scheduling faculty and its subcomponents
				_Scheduling_Faculty = (Scheduling_Faculty_interface*)Allocate<type_of(Scheduling_Faculty)>();
				_Scheduling_Faculty->template Parent_Person<ComponentType*>(this);
				_Scheduling_Faculty->template Initialize<void>();

				// Create and Initialize the Perception faculty and its subcomponents
				_Perception_Faculty = (Perception_Faculty_interface*)Allocate<type_of(Perception_Faculty)>();	
				_Perception_Faculty->template Parent_Person<ComponentType*>(this);
				_Perception_Faculty->template Initialize<void>();

				// Create and Initialize the routing faculty
				_router=(router_interface*)Allocate<type_of(router)>();
				_router->template traveler<ComponentType*>(this);
					

				// Moving faculty
				_Moving_Faculty = (Moving_Faculty_interface*)Allocate<type_of(Moving_Faculty)>();
				_Moving_Faculty->template Parent_Person<ComponentType*>(this);

				// Create and Initialize the vehicle		
				_vehicle = (vehicle_interface*)Allocate<type_of(vehicle)>(); 	
				_vehicle->template uuid<int>(id);
				_vehicle->template internal_id<int>(id);
				_vehicle->template traveler<ComponentType*>(this);
				_vehicle->template router<router_interface*>(_router);
				_vehicle->template initialize<NT>();
				_vehicle->template is_integrated<bool>(true);

				this->_current_location = nullptr;

				// Add basic traveler properties							
				this->template uuid<int>(id);
				this->template internal_id<int>(id);

				
			}
			template<typename TargetType> void Initialize(typename TargetType::ParamType id, typename TargetType::Param2Type home_zone, typename TargetType::Param3Type network_ref, typename TargetType::Param4Type scenario_ref)
			{
				this->Initialize< typename TargetType::ParamType>(id);
				this->home_synthesis_zone< typename TargetType::Param2Type>(home_zone);
				this->_Perception_Faculty->template Network<typename TargetType::Param3Type>(network_ref);
				this->_Perception_Faculty->template Scenario<typename TargetType::Param4Type>(scenario_ref);
				this->_router->template network<typename TargetType::Param3Type>(network_ref);

				// Randomly determine if person uses pretrip-information sources (Radio, internet, news, etc.)
				scenario_reference_interface* scenario = this->scenario_reference<scenario_reference_interface*>();
				this->_has_pretrip_information = (GLOBALS::Uniform_RNG.template Next_Rand<float>() < scenario->template pretrip_informed_market_share<float>());
			}
			tag_feature_as_available(Initialize);

			template<typename TargetType> void Set_Locations()
			{
				// This call sets the work/school locations from the properties sub class and uses the functions below
//TODO
//				_Properties->template Initialize<Target_Type<NT,void,home_synthesis_zone_interface*> >(this->_home_synthesis_zone);
			}

			template<typename TargetType> void Choose_Work_Location()
			{
				PUSH_TO_STACK("Choose_Work_Location");

				// interface to this
				this_itf* pthis = (this_itf*)this;
				destination_choice_itf* dest_chooser = this->_Planning_Faculty->template Destination_Choice_Faculty<destination_choice_itf*>();
				
				// first, make sure person is worker, if not exit
				EMPLOYMENT_STATUS status = _Static_Properties->template Employment_Status<EMPLOYMENT_STATUS>();
				if (status != EMPLOYMENT_STATUS::EMPLOYMENT_STATUS_CIVILIAN_AT_WORK && status != EMPLOYMENT_STATUS::EMPLOYMENT_STATUS_ARMED_FORCES_AT_WORK)
				{
					pthis->template Work_Location<int>(-1);
					return;
				}
				
				location_interface* dest = dest_chooser->template Choose_Routine_Destination<location_interface*>(Activity_Components::Types::PRIMARY_WORK_ACTIVITY);

				if (dest == nullptr) pthis->Work_Location<location_interface*>(pthis->Home_Location<location_interface*>());
				else pthis->Work_Location<location_interface*>(dest);

				scenario_reference_interface* scenario = pthis->scenario_reference<scenario_reference_interface*>();
				zone_interface* zone = pthis->Work_Location<zone_interface*>();
				
				// update the simulated employment in the work zone
				zone->employment_simulated<int&>() += 1.0f / scenario->percent_to_synthesize<float>();

				POP_FROM_STACK;
			}
			tag_feature_as_available(Choose_Work_Location);
			template<typename TargetType> void Choose_Work_Location_Based_On_Census()
			{
				PUSH_TO_STACK("Choose_Work_Location");

				// interface to this
				this_itf* pthis = (this_itf*)this;


				// first, make sure person is worker, if not exit
				EMPLOYMENT_STATUS status = _Static_Properties->template Employment_Status<EMPLOYMENT_STATUS>();
				if (status != EMPLOYMENT_STATUS::EMPLOYMENT_STATUS_CIVILIAN_AT_WORK && status != EMPLOYMENT_STATUS::EMPLOYMENT_STATUS_ARMED_FORCES_AT_WORK)
				{
					pthis->template Work_Location<int>(-1);
					return;
				}


				// Get the expected travel time from the persons static properties
				Time_Minutes ttime = _Static_Properties->template Journey_To_Work_Travel_Time<Time_Minutes>();

				// if minimimal travel time, assign the home location as the work location
				if (ttime < 2)
				{
					pthis->template Work_Location<int>(pthis->template Home_Location<int>());
					return;
				}

				//=========================================================
				// Find available zones within the specified target range of the given work travel time
				//---------------------------------------------------------
				zones_container_interface* zones = this->network_reference<network_reference_interface*>()->template zones_container<zones_container_interface*>();
				typename zones_container_interface::iterator z_itr;
				boost::container::vector<zone_interface*> temp_zones;
				boost::container::vector<float> temp_zone_probabilities;
				zone_interface* orig = pthis->template Home_Location<zone_interface*>();

				// loop through all zones, store those within +- 2 min of estimated work travel time that have available work locations
				float time_range_to_search = 2.0;
				int employment = 0;
				while (temp_zones.size() == 0)
				{
					for (z_itr = zones->begin(); z_itr != zones->end(); ++z_itr)
					{
						zone_interface* zone = z_itr->second;
//TODO
//						Time_Minutes t = network_reference<network_reference_interface*>()->template Get_TTime<Target_Type<NT,Time_Minutes,zone_interface*,Vehicle_Components::Types::Vehicle_Type_Keys,Time_Hours> >(orig,zone, Vehicle_Components::Types::SOV,7);
						if (t > ttime - time_range_to_search && t < ttime + time_range_to_search && zone->template work_locations<locations_container_interface*>()->size() > 0 && zone->template employment_total<int>() > 0)
						{
							employment += zone->template employment_total<int>();
							temp_zones.push_back(zone);
						}
					}
					// expand the search time radius if no available zones found
					if (time_range_to_search >= 20) break;
					time_range_to_search += time_range_to_search;
					
				}
				// calculate probabilities
				float cum_prob = 0;
				for (typename boost::container::vector<zone_interface*>::iterator t_itr = temp_zones.begin(); t_itr != temp_zones.end(); ++t_itr)
				{
					cum_prob += (*t_itr)->template employment_total<float>() / employment;
					temp_zone_probabilities.push_back(cum_prob);
				}

				//=========================================================
				// select zone
				//---------------------------------------------------------
				float r = Uniform_RNG.template Next_Rand<float>();
				zone_interface* selected_zone = nullptr;
				typename boost::container::vector<zone_interface*>::iterator t_itr;
				boost::container::vector<float>::iterator p_itr;
				for (t_itr = temp_zones.begin(), p_itr = temp_zone_probabilities.begin(); t_itr != temp_zones.end(); ++t_itr, ++p_itr)
				{
					if (r<*p_itr) 
					{
						selected_zone = *t_itr;
						break;
					}
				}
				
				//=========================================================
				// Select location from within the zone
				//---------------------------------------------------------
				// set work location to home if no valid locations found
				if (selected_zone == nullptr)
				{
					pthis->template Work_Location<int>(pthis->template Home_Location<int>());
					//THROW_WARNING("WARNING: no valid work zone found for person id: " << pthis->uuid<int>() << ", setting work location to home location.");
				}

				// select work location from within available work locations in the zone
				else
				{
					locations_container_interface* work_locations = selected_zone->template work_locations<locations_container_interface*>();
					float size = work_locations->size();

					if (size == 0)
					{
						pthis->template Work_Location<int>(pthis->template Home_Location<int>());
					}
					else
					{
						int index = (int)(Uniform_RNG.template Next_Rand<float>()*size);
						location_interface* work_loc = (*work_locations)[index];

						if (work_loc == nullptr)
						{
							pthis->template Work_Location<int>(pthis->template Home_Location<int>());
						}
						else
						{
							pthis->template Work_Location<int>(work_loc->template internal_id<int>());
						}
					}
				}
				POP_FROM_STACK;
			}
			tag_feature_as_available(Choose_Work_Location_Based_On_Census);

			template<typename TargetType> void Choose_School_Location()
			{
				PUSH_TO_STACK("Choose_School_Location");

				// interface to this
				this_itf* pthis = (this_itf*)this;
				zone_interface* selected_zone = nullptr;


				//=========================================================
				// first, make sure person is student, if not exit
				//---------------------------------------------------------
				SCHOOL_ENROLLMENT status = _Static_Properties->template School_Enrollment<SCHOOL_ENROLLMENT>();
				if (status != SCHOOL_ENROLLMENT::ENROLLMENT_PUBLIC && status != SCHOOL_ENROLLMENT::ENROLLMENT_PRIVATE)
				{
					pthis->template School_Location<int>(-1);
					POP_FROM_STACK;
					return;
				}

				//=========================================================
				// Find available zones within the specified target range of the given work travel time
				//---------------------------------------------------------
				zones_container_interface* zones = network_reference<network_reference_interface*>()->template zones_container<zones_container_interface*>();
				typename zones_container_interface::iterator z_itr;
				boost::container::vector<zone_interface*> temp_zones;
				boost::container::vector<float> temp_zone_probabilities;
				zone_interface* orig = pthis->template Home_Location<zone_interface*>();

				//=========================================================
				// if origin zone has school locations, select, else search
				//---------------------------------------------------------
				if (orig->template school_locations<locations_container_interface*>()->size() > 0)
				{
					selected_zone = orig;
				}
				else
				{
					// loop through all zones, store those within +- 2 min of estimated work travel time that have available work locations
					float time_range_to_search = 15.0;
					int school_locations = 0;
					while (temp_zones.size() == 0)
					{
						for (z_itr = zones->begin(); z_itr != zones->end(); ++z_itr)
						{
							zone_interface* zone = z_itr->second;
//TODO
//							Time_Minutes t = network_reference<network_reference_interface*>()->template Get_TTime<Target_Type<NT,Time_Minutes,zone_interface*,Vehicle_Components::Types::Vehicle_Type_Keys,Time_Hours> >(orig,zone, Vehicle_Components::Types::SOV,9);
							if (t < time_range_to_search && zone->template school_locations<locations_container_interface*>()->size() > 0)
							{
								school_locations += (int)zone->template school_locations<locations_container_interface*>()->size();
								temp_zones.push_back(zone);
							}
						}
						// expand the search time radius if no available zones found
						if (time_range_to_search >= 60) break;
						time_range_to_search += time_range_to_search;
					
					}

					if (temp_zones.size()==0)
					{
						pthis->template School_Location<int>(pthis->template Home_Location<int>());
						POP_FROM_STACK;
						return;
					}

					// calculate probabilities
					float cum_prob = 0;
					for (typename boost::container::vector<zone_interface*>::iterator t_itr = temp_zones.begin(); t_itr != temp_zones.end(); ++t_itr)
					{
						cum_prob += (*t_itr)->template school_locations<locations_container_interface*>()->size() / school_locations;
						temp_zone_probabilities.push_back(cum_prob);
					}

					//=========================================================
					// select zone
					//---------------------------------------------------------
					float r = Uniform_RNG.template Next_Rand<float>();
					zone_interface* selected_zone = nullptr;
					typename boost::container::vector<zone_interface*>::iterator t_itr;
					boost::container::vector<float>::iterator p_itr;
					for (t_itr = temp_zones.begin(), p_itr = temp_zone_probabilities.begin(); t_itr != temp_zones.end(); ++t_itr, ++p_itr)
					{
						if (r<*p_itr) 
						{
							selected_zone = *t_itr;
							break;
						}
					}
				}
				
				//=========================================================
				// Select location from within the zone
				//---------------------------------------------------------
				// set school location to home if no valid locations found
				if (selected_zone == nullptr)
				{
					pthis->template School_Location<int>(pthis->template Home_Location<int>());
				}

				// select school location from within available school locations in the zone
				else
				{
					locations_container_interface* school_locations = selected_zone->template school_locations<locations_container_interface*>();
					float size = school_locations->size();
					int index = (int)(Uniform_RNG.template Next_Rand<float>()*size);
					location_interface* loc = (*school_locations)[index];

					if (loc != nullptr) pthis->template School_Location<int>(loc->template internal_id<int>());
					else pthis->template School_Location<int>(pthis->template Home_Location<int>());
				}

				POP_FROM_STACK;
			}
			tag_feature_as_available(Choose_School_Location);

			template<typename TargetType> void arrive_at_destination()
			{

			}
			tag_feature_as_available(arrive_at_destination);


			//=======================================================================================================================================================================
			// PASS THROUGH FEATURES -> dispatched to member sub-objects
			//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
			template<typename TargetType> TargetType current_movement_plan()
			{
			}

			//=======================================================================================================================================================================
			// Record sorting
			//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
			static bool record_comparer(typename MasterType::activity_record_type* act1, typename MasterType::activity_record_type* act2)
			{
				//typedef Activity_Components::Prototypes::Activity_Planner<typename remove_pointer<typename  type_of(Activity_Container)::value_type>::type> Activity_Plan;
				//typedef Back_Insertion_Sequence< type_of(Activity_Container),Activity_Plan*> Activity_Plans;

				Activity_Record* act1_itf = (Activity_Record*)act1;
				Activity_Record* act2_itf = (Activity_Record*)act2;
				return (act1_itf->template Start_Time<Time_Minutes>() < act2_itf->template Start_Time<Time_Minutes>());
			}
			template<typename TargetType> TargetType Sort_Activity_Records()
			{
				Activity_Records* records = this->Activity_Record_Container<Activity_Records*>();
				boost::container::list<typename MasterType::activity_record_type*>* recs = (list<typename MasterType::activity_record_type*>*)records;
				recs->sort(record_comparer);
			}tag_feature_as_available(Sort_Activity_Records);
		};

	}
}
