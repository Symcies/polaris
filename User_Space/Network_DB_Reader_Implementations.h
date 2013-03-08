#pragma once
#include "Network_DB_Reader_Prototype.h"

namespace Network_Components
{
	namespace Types
	{
		union Link_ID_Dir
		{
			struct
			{
				long id;
				long dir;
			};
			long long id_dir;
		};
	}
	
	namespace Implementations
	{
		implementation struct Network_DB_Reader_Implementation : public Polaris_Component<APPEND_CHILD(Network_DB_Reader_Implementation),MasterType,Execution_Object,ParentType>
		{
			// Pointer to the Parent class
			member_prototype(Prototypes::Network_Prototype, network_reference, typename MasterType::network_type, none, none);
			member_prototype(Scenario_Components::Prototypes::Scenario_Prototype, scenario_reference, typename MasterType::scenario_type, none, none);

			//==================================================================================================================
			/// read from database
			//------------------------------------------------------------------------------------------------------------------

			feature_implementation void read_network_data(Network_Components::Types::Network_IO_Maps& net_io_maps)
			{
				using namespace odb;
				using namespace polaris::io;
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type,ComponentType> _Scenario_Interface;
				_scenario_reference = _network_reference->scenario_reference<_Scenario_Interface*>();
				string name(_scenario_reference->template database_name<string&>());
				auto_ptr<database> db (open_sqlite_database (name));
				transaction t(db->begin());

				read_intersection_data<ComponentType,CallerType,TargetType>(db, net_io_maps);
				read_link_data<ComponentType,CallerType,TargetType>(db, net_io_maps);
				read_turn_movement_data<ComponentType,CallerType,TargetType>(db, net_io_maps);
				read_zone_data<ComponentType,CallerType,TargetType>(db, net_io_maps);
				read_activity_location_data<ComponentType,CallerType,TargetType>(db, net_io_maps);
			}

			feature_implementation void read_intersection_data(auto_ptr<odb::database>& db, Network_Components::Types::Network_IO_Maps& net_io_maps)
			{
				using namespace odb;
				using namespace polaris::io;

				define_container_and_value_interface_unqualified_container(_Intersections_Container_Interface, _Intersection_Interface, typename type_of(network_reference)::type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				_Intersections_Container_Interface* intersections_container_ptr=_network_reference->intersections_container<_Intersections_Container_Interface*>();
				typename type_of(network_reference)::type_of(intersections_container)& intersections_container_monitor=(typename type_of(network_reference)::type_of(intersections_container)&)(*intersections_container_ptr);
				define_component_interface(_Intersection_Control_Interface, typename _Intersection_Interface::get_type_of(intersection_control), Intersection_Control_Components::Prototypes::Intersection_Control_Prototype, ComponentType);
				define_container_and_value_interface(_Control_Plans_Container_Interface, _Control_Plan_Interface, typename _Intersection_Control_Interface::get_type_of(control_plan_data_array), Random_Access_Sequence_Prototype, Intersection_Control_Components::Prototypes::Control_Plan_Prototype, ComponentType);
				
				intersections_container_ptr->clear();

				result<Node> node_result=db->template query<Node>(query<Node>::true_expr);

				net_io_maps.intersection_id_to_ptr.set_empty_key(-1);
				net_io_maps.intersection_id_to_ptr.set_deleted_key(-2);

				_Intersection_Interface* intersection;

				int counter=-1;

				cout << "Reading Nodes" << endl;

				for(result<Node>::iterator db_itr = node_result.begin (); db_itr != node_result.end (); ++db_itr)
				{
					counter++;
					if(counter%10000==0) cout << "\t" << counter << endl;

					intersection=(_Intersection_Interface*)Allocate<typename _Intersection_Interface::Component_Type>();

					intersection->template uuid<int>( db_itr->getNode() );
					intersection->template internal_id<int>(counter);
					intersection->template x_position<float>( _scenario_reference->template meterToFoot<NULLTYPE>(db_itr->getX()));
					intersection->template y_position<float>( _scenario_reference->template meterToFoot<NULLTYPE>(db_itr->getY()));
					intersection->template intersection_control<_Intersection_Control_Interface*>((_Intersection_Control_Interface*)nullptr);
					
					net_io_maps.intersection_id_to_ptr[db_itr->getNode()]=intersection;

					if (_scenario_reference->template intersection_control_flag<int>() == 0) 
					{
						intersection->template intersection_type<int>(Intersection_Components::Types::NO_CONTROL);

						_Intersection_Control_Interface* intersection_control = (_Intersection_Control_Interface*)Allocate<typename _Intersection_Control_Interface::Component_Type>();
						intersection_control->template intersection<_Intersection_Interface*>(intersection);
					
						_Control_Plan_Interface* control_plan = (_Control_Plan_Interface*)Allocate<typename _Control_Plan_Interface::Component_Type>();
						control_plan->template control_plan_index<int>(0.0);
						control_plan->template control_type<int>(Intersection_Components::Types::NO_CONTROL);
						control_plan->template starting_time<int>(0.0);
						control_plan->template ending_time<int>(24*60*60);
						intersection_control->template control_plan_data_array<_Control_Plans_Container_Interface&>().push_back(control_plan);
						intersection->template intersection_control<_Intersection_Control_Interface*>(intersection_control);
					}
					intersections_container_ptr->push_back(intersection);
				}
			}

			feature_implementation void read_link_data(auto_ptr<odb::database>& db, Network_Components::Types::Network_IO_Maps& net_io_maps)
			{
				using namespace odb;
				using namespace polaris::io;

				const float link_length = 5280.0; // in foot
				const float speed_limit = 60.0; // in miles per hour
				const float speed_limit_ramp = 30.0; 
				const float maximum_flow_rate = 2200.0; // in vehicles per hour per lane
				const float maximum_flow_rate_ramp = 600.0; // 
				const float maximum_flow_rate_arterial = 900;
				const float jam_density = 220.0; // in vehiles per mile per lane
				const float backward_wave_speed = 12.0;
				const float distance_factor = 1.5;				

				_network_reference->max_free_flow_speed<float>(-1);
				define_container_and_value_interface_unqualified_container(_Intersections_Container_Interface, _Intersection_Interface, typename type_of(network_reference)::type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				define_container_and_value_interface_unqualified_container(_Links_Container_Interface, _Link_Interface, typename type_of(network_reference)::type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				_Links_Container_Interface* links_container_ptr=_network_reference->links_container<_Links_Container_Interface*>();
				typename type_of(network_reference)::type_of(links_container)& links_container_monitor=(typename type_of(network_reference)::type_of(links_container)&)(*links_container_ptr);				
				/*typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario = scenario_reference<ComponentType,CallerType,_Scenario_Interface*>();*/				

				Types::Link_ID_Dir link_id_dir;
				
				result<Link> link_result=db->template query<Link>(query<Link>::true_expr);

				net_io_maps.link_id_dir_to_ptr.set_empty_key(-1);
				net_io_maps.link_id_dir_to_ptr.set_deleted_key(-2);

				_Link_Interface* link;
				
				int counter=0;

				cout << "Reading Links" << endl;
				
				int link_counter=-1;

				for(result<Link>::iterator db_itr = link_result.begin (); db_itr != link_result.end (); ++db_itr)
				{
					counter++;
					if(counter%10000==0) cout << "\t" << counter << endl;

					if(db_itr->getLanes_Ab()>0)
					{
						link=(_Link_Interface*)Allocate<typename _Link_Interface::Component_Type>();
						
						link_id_dir.id=db_itr->getLink();
						link_id_dir.dir=0;

						net_io_maps.link_id_dir_to_ptr[link_id_dir.id_dir]=link;



						link->template upstream_intersection<_Intersection_Interface*>((_Intersection_Interface*)net_io_maps.intersection_id_to_ptr[db_itr->getNode_A()->getNode()]);
						link->template downstream_intersection<_Intersection_Interface*>((_Intersection_Interface*)net_io_maps.intersection_id_to_ptr[db_itr->getNode_B()->getNode()]);

											
						link->template internal_id<int>(++link_counter);
						link->template uuid<int>(link_id_dir.id /*link_counter*/);

						link->template num_lanes<int>(db_itr->getLanes_Ab());
						
						link->template length<float>(_scenario_reference->template meterToFoot<NULLTYPE>(db_itr->getLength()));
						link->template speed_limit<float>(_scenario_reference->template mepsToMiph<NULLTYPE>(db_itr->getSpeed_Ab()));
						
						link->template num_left_turn_bays<int>(db_itr->getLeft_Ab());
						link->template num_right_turn_bays<int>(db_itr->getRight_Ab());
						
						link->template left_turn_bay_length<float>(_scenario_reference->template meterToFoot<NULLTYPE>(0.0));
						link->template right_turn_bay_length<float>(_scenario_reference->template meterToFoot<NULLTYPE>(0.0));

						
						const std::string& facility_type=db_itr->getType()->getLink_Type();

						if(facility_type=="FREEWAY")
						{
							link->template link_type<Link_Components::Types::Link_Type_Keys>(Link_Components::Types::FREEWAY);
						}
						else if(facility_type=="EXPRESSWAY")
						{
							link->template link_type<Link_Components::Types::Link_Type_Keys>(Link_Components::Types::EXPRESSWAY);
						}
						else if(facility_type=="RAMP")
						{
							link->template link_type<Link_Components::Types::Link_Type_Keys>(Link_Components::Types::ON_RAMP);
						}
						else
						{
							link->template link_type<Link_Components::Types::Link_Type_Keys>(Link_Components::Types::ARTERIAL);
						}

						link->template free_flow_speed<float>(_scenario_reference->template mepsToMiph<NULLTYPE>(db_itr->getSpeed_Ab()) + 10.0);
						link->template maximum_flow_rate<float>(maximum_flow_rate);
						link->template backward_wave_speed<float>(backward_wave_speed);
						link->template jam_density<float>(jam_density);
						link->template original_free_flow_speed<float>(link->template free_flow_speed<float>());


						_network_reference->max_free_flow_speed<float>(max(_network_reference->max_free_flow_speed<float>(),link->template free_flow_speed<float>()));

						link->template upstream_intersection<_Intersection_Interface*>()->template outbound_links<_Links_Container_Interface&>().push_back(link);
						link->template downstream_intersection<_Intersection_Interface*>()->template inbound_links<_Links_Container_Interface&>().push_back(link);

						links_container_ptr->push_back(link);
					}

					if(db_itr->getLanes_Ba()>0)
					{
						link=(_Link_Interface*)Allocate<typename _Link_Interface::Component_Type>();

						link_id_dir.id=db_itr->getLink();
						link_id_dir.dir=1;

						net_io_maps.link_id_dir_to_ptr[link_id_dir.id_dir]=link;

						link->template upstream_intersection<_Intersection_Interface*>((_Intersection_Interface*)net_io_maps.intersection_id_to_ptr[db_itr->getNode_B()->getNode()]);
						link->template downstream_intersection<_Intersection_Interface*>((_Intersection_Interface*)net_io_maps.intersection_id_to_ptr[db_itr->getNode_A()->getNode()]);

						
						link->template uuid<int>(++link_counter);
						link->template internal_id<int>(link_counter);

						link->template num_lanes<int>(db_itr->getLanes_Ba());
						
						link->template length<float>(_scenario_reference->template meterToFoot<NULLTYPE>(db_itr->getLength()));
						link->template speed_limit<float>(_scenario_reference->template mepsToMiph<NULLTYPE>(db_itr->getSpeed_Ba()));
						
						link->template num_left_turn_bays<int>(db_itr->getLeft_Ba());
						link->template num_right_turn_bays<int>(db_itr->getRight_Ba());
						
						link->template left_turn_bay_length<float>(_scenario_reference->template meterToFoot<NULLTYPE>(0.0));
						link->template right_turn_bay_length<float>(_scenario_reference->template meterToFoot<NULLTYPE>(0.0));

						
						const std::string& facility_type=db_itr->getType()->getLink_Type();

						if(facility_type=="FREEWAY")
						{
							link->template link_type<Link_Components::Types::Link_Type_Keys>(Link_Components::Types::FREEWAY);
						}
						else if(facility_type=="EXPRESSWAY")
						{
							link->template link_type<Link_Components::Types::Link_Type_Keys>(Link_Components::Types::EXPRESSWAY);
						}
						else if(facility_type=="RAMP")
						{
							link->template link_type<Link_Components::Types::Link_Type_Keys>(Link_Components::Types::ON_RAMP);
						}
						else
						{
							link->template link_type<Link_Components::Types::Link_Type_Keys>(Link_Components::Types::ARTERIAL);
						}

						link->template free_flow_speed<float>(_scenario_reference->template mepsToMiph<NULLTYPE>(db_itr->getSpeed_Ba()) + 10.0);
						link->template maximum_flow_rate<float>(maximum_flow_rate);
						link->template backward_wave_speed<float>(backward_wave_speed);
						link->template jam_density<float>(jam_density);



						_network_reference->max_free_flow_speed<float>(max(_network_reference->max_free_flow_speed<float>(),link->template free_flow_speed<float>()));

						link->template upstream_intersection<_Intersection_Interface*>()->template outbound_links<_Links_Container_Interface&>().push_back(link);
						link->template downstream_intersection<_Intersection_Interface*>()->template inbound_links<_Links_Container_Interface&>().push_back(link);

						links_container_ptr->push_back(link);
					}
				}
			}

			feature_implementation void read_turn_movement_data(auto_ptr<odb::database>& db, Network_Components::Types::Network_IO_Maps& net_io_maps)
			{
				using namespace odb;
				using namespace polaris::io;

				Types::Link_ID_Dir link_id_dir;

				define_container_and_value_interface_unqualified_container(_Intersections_Container_Interface, _Intersection_Interface, typename type_of(network_reference)::type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				define_container_and_value_interface_unqualified_container(_Links_Container_Interface, _Link_Interface, typename type_of(network_reference)::type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface_unqualified_container(_Turn_Movements_Container_Interface, _Turn_Movement_Interface, typename type_of(network_reference)::type_of(turn_movements_container), Random_Access_Sequence_Prototype, Turn_Movement_Components::Prototypes::Movement_Prototype, ComponentType);
				_Turn_Movement_Interface* turn_movement;
				typename type_of(network_reference)::type_of(turn_movements_container)& turn_movements_monitor=_network_reference->turn_movements_container<typename type_of(network_reference)::type_of(turn_movements_container)&>();
				_Turn_Movements_Container_Interface& turn_movements_container = _network_reference->turn_movements_container<_Turn_Movements_Container_Interface&>();
				_Intersections_Container_Interface& intersections_container = _network_reference->intersections_container<_Intersections_Container_Interface&>();
				result<Connect> connect_result=db->template query<Connect>(query<Connect>::true_expr);
				
				int counter=-1;

				cout << "Reading Connections" << endl;
				_Link_Interface* link;
				for(result<Connect>::iterator db_itr = connect_result.begin (); db_itr != connect_result.end (); ++db_itr)
				{

					if(counter%10000==0) cout << "\t" << counter << endl;

					turn_movement = (_Turn_Movement_Interface*)Allocate<typename _Turn_Movement_Interface::Component_Type>();

					link_id_dir.id=db_itr->getLink()->getLink();
					link_id_dir.dir=db_itr->getDir();
					
					assert(net_io_maps.link_id_dir_to_ptr.count(link_id_dir.id_dir));
					
					counter++;
					link=(_Link_Interface*)net_io_maps.link_id_dir_to_ptr[link_id_dir.id_dir];

					turn_movement->template inbound_link<_Link_Interface*>(link);

					int target_intersection_id=access(_Intersection_Interface,link->template downstream_intersection).internal_id<int>();


					link_id_dir.id=db_itr->getTo_Link()->getLink();
					link_id_dir.dir=0;

					if(net_io_maps.link_id_dir_to_ptr.count(link_id_dir.id_dir))
					{
						link=(_Link_Interface*)net_io_maps.link_id_dir_to_ptr[link_id_dir.id_dir];

						if(access(_Intersection_Interface,link->template upstream_intersection).internal_id<int>()!=target_intersection_id)
						{
							link_id_dir.dir=1;
							assert(net_io_maps.link_id_dir_to_ptr.count(link_id_dir.id_dir));
							link=(_Link_Interface*)net_io_maps.link_id_dir_to_ptr[link_id_dir.id_dir];
						}
					}
					else
					{
						link_id_dir.dir=1;
						assert(net_io_maps.link_id_dir_to_ptr.count(link_id_dir.id_dir));
						link=(_Link_Interface*)net_io_maps.link_id_dir_to_ptr[link_id_dir.id_dir];
					}

					turn_movement->template outbound_link<_Link_Interface*>(link);

					turn_movement->template uuid<int>(db_itr->getConn());
					turn_movement->template internal_id<int>(counter);

					const string& connect_type=db_itr->getType();

					if(connect_type=="THRU")
					{
						turn_movement->template movement_type<Turn_Movement_Components::Types::Turn_Movement_Type_Keys>(Turn_Movement_Components::Types::THROUGH_TURN);
					}
					else if(connect_type=="LEFT" || connect_type=="L_SPLIT" || connect_type=="L_MERGE")
					{
						turn_movement->template movement_type<Turn_Movement_Components::Types::Turn_Movement_Type_Keys>(Turn_Movement_Components::Types::LEFT_TURN);
					}
					else if(connect_type=="RIGHT" || connect_type=="R_SPLIT" || connect_type=="R_MERGE")
					{
						turn_movement->template movement_type<Turn_Movement_Components::Types::Turn_Movement_Type_Keys>(Turn_Movement_Components::Types::RIGHT_TURN);
					}
					else if(connect_type=="UTURN")
					{
						turn_movement->template movement_type<Turn_Movement_Components::Types::Turn_Movement_Type_Keys>(Turn_Movement_Components::Types::U_TURN);
					}
					else
					{
						assert(false);
						break;
					}

					
					turn_movement->template movement_rule<Turn_Movement_Components::Types::Turn_Movement_Rule_Keys>(Turn_Movement_Components::Types::ALLOWED);


					turn_movements_container.push_back(turn_movement);
				}

				cout << "Configuring Connections" << endl;

				typename _Turn_Movements_Container_Interface::iterator turn_movements_itr;
				for(turn_movements_itr = turn_movements_container.begin(); turn_movements_itr != turn_movements_container.end(); turn_movements_itr++)
				{
					_Turn_Movement_Interface* turn_mvmt=(_Turn_Movement_Interface*)(*turn_movements_itr);

					//outbound turn movement of an inbound link
					_Link_Interface* inbound_link = turn_mvmt->template inbound_link<_Link_Interface*>();
					inbound_link->template outbound_turn_movements<_Turn_Movements_Container_Interface&>().push_back(turn_mvmt);
					//inbound turn movement of an outbound link
					_Link_Interface* outbound_link = turn_mvmt->template outbound_link<_Link_Interface*>();
					outbound_link->template inbound_turn_movements<_Turn_Movements_Container_Interface&>().push_back(turn_mvmt);
				}


				define_container_and_value_interface(_Outbound_Inbound_Movements_Container_Interface, _Outbound_Inbound_Movements_Interface, _Intersection_Interface::get_type_of(outbound_inbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Outbound_Inbound_Movements_Prototype, ComponentType);			

				// configure outbound_inbound_movements
				typename _Intersections_Container_Interface::iterator intersections_itr;

				cout << "Configuring Outbound Inbound Movements" << endl;

				for(intersections_itr = intersections_container.begin(); intersections_itr != intersections_container.end(); intersections_itr++)
				{
					_Intersection_Interface* intersection = (_Intersection_Interface*)(*intersections_itr);
					type_of(network_reference)::type_of(intersections_container)::type_of(unqualified_value)& intersection_monitor=(type_of(network_reference)::type_of(intersections_container)::type_of(unqualified_value)&)*intersection;

					_Links_Container_Interface& outbound_links = intersection->template outbound_links<_Links_Container_Interface&>();
					
					// skip intersections_container that do not have any outbound links
					if (outbound_links.size() == 0) continue;
					
					typename _Links_Container_Interface::iterator links_itr;
					
					for (links_itr = outbound_links.begin(); links_itr != outbound_links.end(); links_itr++)
					{
						_Link_Interface* outboundLink = (_Link_Interface*)(*links_itr);
						
						_Outbound_Inbound_Movements_Interface* outboundInboundMovements = (_Outbound_Inbound_Movements_Interface*)Allocate<typename _Outbound_Inbound_Movements_Interface::Component_Type>();
						
						outboundInboundMovements->template outbound_link_reference<_Link_Interface*>(outboundLink);
						_Turn_Movements_Container_Interface& inboundTurnMovements = outboundLink->template inbound_turn_movements<_Turn_Movements_Container_Interface&>();
						
						for (int k = 0; k < (int)inboundTurnMovements.size(); k++)
						{
							outboundInboundMovements->template inbound_movements<_Turn_Movements_Container_Interface&>().push_back(inboundTurnMovements[k]);
						}

						intersection->template outbound_inbound_movements<_Outbound_Inbound_Movements_Container_Interface&>().push_back(outboundInboundMovements);
					}
				}



				define_container_and_value_interface(_Inbound_Outbound_Movements_Container_Interface, _Inbound_Outbound_Movements_Interface, _Intersection_Interface::get_type_of(inbound_outbound_movements), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Inbound_Outbound_Movements_Prototype, ComponentType);

				cout << "Configuring Inbound Outbound Movements" << endl;

				// configure inbound_outbound_movements
				for (int i = 0; i < (int)intersections_container.size(); i++)
				{
					_Intersection_Interface* intersection = (_Intersection_Interface*)intersections_container[i];
					
					// skip intersections_container that do not have any inbound links
					if (intersection->template inbound_links<_Links_Container_Interface&>().size() == 0) continue;
					
					for (int j = 0; j < (int)intersection->template inbound_links<_Links_Container_Interface&>().size(); j++) 
					{
						_Inbound_Outbound_Movements_Interface* inboundOutboundMovements = (_Inbound_Outbound_Movements_Interface*)Allocate<typename _Inbound_Outbound_Movements_Interface::Component_Type>();
						_Link_Interface* inboundLink = (_Link_Interface*)intersection->template inbound_links<_Links_Container_Interface&>()[j];
						inboundOutboundMovements->template inbound_link_reference<_Link_Interface*>(inboundLink);
						_Turn_Movements_Container_Interface& outboundTurnMovements = inboundLink->template outbound_turn_movements<_Turn_Movements_Container_Interface&>();

						for (int k = 0; k < (int)outboundTurnMovements.size(); k++)
						{
							inboundOutboundMovements->template outbound_movements<_Turn_Movements_Container_Interface&>().push_back(outboundTurnMovements[k]);
						}
						intersection->template inbound_outbound_movements<_Inbound_Outbound_Movements_Container_Interface&>().push_back(inboundOutboundMovements);
					}
				}
			}

			feature_implementation void read_zone_data(auto_ptr<odb::database>& db, Network_Components::Types::Network_IO_Maps& net_io_maps)
			{
				using namespace odb;
				using namespace polaris::io;

				//Types::Link_ID_Dir link_id_dir;

				cout << "Reading Zones" << endl;

				int counter=-1;

				define_container_and_value_interface_unqualified_container(_Links_Container_Interface, _Link_Interface, typename type_of(network_reference)::type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface_unqualified_container(_Zones_Container_Interface, _Zone_Interface, typename type_of(network_reference)::type_of(zones_container), Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
				define_container_and_value_interface_unqualified_container(_Activity_Locations_Container_Interface, _Activity_Location_Interface, typename type_of(network_reference)::type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
				_Zones_Container_Interface& zones_container = _network_reference->zones_container<_Zones_Container_Interface&>();
				// initialzie zone hash_map
				zones_container.set_empty_key(-1);
				zones_container.set_deleted_key(-2);
				typename _Zones_Container_Interface::iterator zone_itr;
			
				// get zones from database
				_Zone_Interface* zone;
				result<Zone> zone_result=db->template query<Zone>(query<Zone>::true_expr);	

				int zone_count = 0;
				for(result<Zone>::iterator db_itr = zone_result.begin (); db_itr != zone_result.end (); ++db_itr, ++zone_count)
				{
					zone = (_Zone_Interface*)Allocate<typename _Zone_Interface::Component_Type>();
					zone->template Initialize<NULLTYPE>();
					zone->template uuid<int>(db_itr->getZone());
					zone->template internal_id<int>(zone_count);
					zone->template X<double>(db_itr->getX());
					zone->template Y<double>(db_itr->getY());
					zones_container.insert(pair<int,_Zone_Interface*>(zone->template uuid<int>(), zone));
				}

				result<ZoneLandUse> zone_lu_result=db->template query<ZoneLandUse>(query<ZoneLandUse>::true_expr);	
			
				for(result<ZoneLandUse>::iterator db_itr = zone_lu_result.begin (); db_itr != zone_lu_result.end (); ++db_itr)
				{
					int zone_id = db_itr->getZone_Id();
					zone_itr = zones_container.find(zone_id);

					if (zone_itr != zones_container.end())
					{
						zone = zone_itr->second;
						zone->template population<int>(db_itr->getPop_Per());
						zone->template employment<int>(db_itr->getEmp_Tot());
					}
				}
			}
		
			feature_implementation void read_activity_location_data(auto_ptr<odb::database>& db, Network_Components::Types::Network_IO_Maps& net_io_maps)
			{
				using namespace odb;
				using namespace polaris::io;

				Types::Link_ID_Dir link_id_dir;

				hash_map<int,int> uuid_to_index;

				cout << "Reading Activity Locations " << endl;

				int counter=-1;

				define_container_and_value_interface_unqualified_container(_Links_Container_Interface, _Link_Interface, typename type_of(network_reference)::type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface_unqualified_container(_Zones_Container_Interface, _Zone_Interface, typename type_of(network_reference)::type_of(zones_container), Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
				define_container_and_value_interface_unqualified_container(_Activity_Locations_Container_Interface, _Activity_Location_Interface, typename type_of(network_reference)::type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
				_Activity_Locations_Container_Interface& activity_locations_container = _network_reference->activity_locations_container<_Activity_Locations_Container_Interface&>();

				activity_locations_container.clear();

				result<Location> location_result=db->template query<Location>(query<Location>::true_expr);
				
				_Zones_Container_Interface* zones = _network_reference->zones_container<_Zones_Container_Interface*>();
				_Zones_Container_Interface::iterator zone_itr;

				_Activity_Location_Interface* activity_location;
				int skipped_counter=0;
				_Link_Interface* link;
				for(result<Location>::iterator db_itr = location_result.begin (); db_itr != location_result.end (); ++db_itr)
				{

					link_id_dir.id=db_itr->getLink()->getLink();
					link_id_dir.dir=db_itr->getDir();

					if(!net_io_maps.link_id_dir_to_ptr.count(link_id_dir.id_dir))
					{
						//cout << endl <<"Skipped location: "<<db_itr->getPrimaryKey() << ", " << skipped_counter << " locations skipped";
						if(++skipped_counter%1000==0)
						{
							cout << skipped_counter << " locations skipped" << endl;
						}
						continue;				
					}

					++counter;
					if(counter%10000==0) cout << "\t" << counter << endl;

					// get the zone id and pull interface to zone from zone container
					int zone_id = db_itr->getZone()->getZone();
					if ((zone_itr=zones->find(zone_id)) == zones->end()) THROW_EXCEPTION("ERROR, zone id: "<<zone_id<<" was not found.");
					_Zone_Interface* zone = zone_itr->second;

					activity_location = (_Activity_Location_Interface*)Allocate<typename _Activity_Location_Interface::Component_Type>();
					assert(net_io_maps.link_id_dir_to_ptr.count(link_id_dir.id_dir));
					link=(_Link_Interface*)net_io_maps.link_id_dir_to_ptr[link_id_dir.id_dir];

					activity_location->template origin_links<_Links_Container_Interface&>().push_back(link);
				
					activity_location->template destination_links<_Links_Container_Interface&>().push_back(link);
				
					activity_location->template zone<_Zone_Interface*>(zone);
					activity_location->template uuid<int>(db_itr->getPrimaryKey());
					activity_location->template internal_id<int>(counter);

					uuid_to_index.insert(pair<int,int>(db_itr->getPrimaryKey(),counter));

					zone->template origin_activity_locations<_Activity_Locations_Container_Interface&>().push_back(activity_location);
					zone->template destination_activity_locations<_Activity_Locations_Container_Interface&>().push_back(activity_location);

					
					activity_locations_container.push_back(activity_location);
				}

				cout << "Reading Activity Location Data."<< endl ;
				// get activity location data
				result<LocationData> loc_data_result=db->template query<LocationData>(query<LocationData>::true_expr);	
				int data_counter=0;
				for(result<LocationData>::iterator db_itr = loc_data_result.begin (); db_itr != loc_data_result.end (); ++db_itr)
				{
					try
					{
						
						int loc_id = db_itr->getPrimaryKey();
						hash_map<int,int>::iterator itr = uuid_to_index.find(loc_id);
						if (itr == uuid_to_index.end()) continue;

						int loc_idx = itr->second;
						activity_location = activity_locations_container[loc_idx];
						
						stringstream s;
						s << db_itr->getCensus_Zone();

						long long zone_id;
						s >> zone_id;
						activity_location->census_zone_id<long long>(zone_id);
					}
					catch (std::exception ex)
					{
						cerr<< ex.what()<<" at data item "<<data_counter << endl ;
					}
					data_counter++;
				}
			}

		};
	}
}

using namespace Network_Components::Implementations;