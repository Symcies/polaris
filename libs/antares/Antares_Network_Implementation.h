//*********************************************************
//	Graphical_Network_Implementation.h - Graphical Network Variant
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "Traffic_Simulator\Network_Implementation.h"
#include "Canvas.h"
#include "Information_Panel.h"

namespace Network_Components
{
	namespace Implementations
	{
#pragma pack(push,1)
		template<typename MasterType>
		struct Intersection_Polygon
		{
			Intersection_Polygon()
			{
				vertices=new Point_3D<MasterType>[8];
				num_vertices=8;
			}
			void* data;
			True_Color_RGBA<NT> color;
			int num_vertices;
			Point_3D<MasterType>* vertices;
		};
#pragma pack(pop)

		struct Extended_MOE_Data : public MOE_Data
		{
			float network_vmt;
			float network_vht;
			int network_cumulative_loaded_vehicles;
			int network_cumulative_departed_vehicles;
			int network_in_network_vehicles;
			int network_cumulative_arrived_vehicles;
		};
		//---------------------------------------------------------
		//	Antares_Network_Implementation - network class definition
		//---------------------------------------------------------
#ifdef IntegratedModelApplication
		implementation struct Antares_Network_Implementation:public Integrated_Network_Implementation<MasterType,INHERIT(Antares_Network_Implementation)>
#else
		implementation struct Antares_Network_Implementation:public Network_Implementation<MasterType,INHERIT(Antares_Network_Implementation)>
#endif
		{
			static float _intersection_radius;
			boost::container::vector<Point_2D<MasterType>> _network_vmt_cache;
			boost::container::vector<Point_2D<MasterType>> _network_vht_cache;
			boost::container::vector<Point_2D<MasterType>> _network_cumulative_loaded_vehicles_cache;
			boost::container::vector<Point_2D<MasterType>> _network_cumulative_departed_vehicles_cache;
			boost::container::vector<Point_2D<MasterType>> _network_in_network_vehicles_cache;
			boost::container::vector<Point_2D<MasterType>> _network_cumulative_arrived_vehicles_cache;

			boost::container::vector<Point_2D<MasterType>> _network_avg_link_travel_time_cache;
			boost::container::vector<Point_2D<MasterType>> _network_avg_link_speed_cache;
			boost::container::vector<Point_2D<MasterType>> _network_avg_link_density_cache;
			boost::container::vector<Point_2D<MasterType>> _network_avg_link_in_volume_cache;
			boost::container::vector<Point_2D<MasterType>> _network_avg_link_out_volume_cache;
			boost::container::vector<Point_2D<MasterType>> _network_avg_link_travel_time_ratio_cache;
			boost::container::vector<Point_2D<MasterType>> _network_avg_link_speed_ratio_cache;
			boost::container::vector<Point_2D<MasterType>> _network_avg_link_density_ratio_cache;
			boost::container::vector<Point_2D<MasterType>> _network_avg_link_queue_length_cache;

			boost::container::vector<Point_2D<MasterType>> _reference_network_vmt_cache;
			boost::container::vector<Point_2D<MasterType>> _reference_network_vht_cache;
			boost::container::vector<Point_2D<MasterType>> _reference_network_cumulative_loaded_vehicles_cache;
			boost::container::vector<Point_2D<MasterType>> _reference_network_cumulative_departed_vehicles_cache;
			boost::container::vector<Point_2D<MasterType>> _reference_network_in_network_vehicles_cache;
			boost::container::vector<Point_2D<MasterType>> _reference_network_cumulative_arrived_vehicles_cache;

			boost::container::vector<Point_2D<MasterType>> _reference_network_avg_link_travel_time_cache;
			boost::container::vector<Point_2D<MasterType>> _reference_network_avg_link_speed_cache;
			boost::container::vector<Point_2D<MasterType>> _reference_network_avg_link_density_cache;
			boost::container::vector<Point_2D<MasterType>> _reference_network_avg_link_in_volume_cache;
			boost::container::vector<Point_2D<MasterType>> _reference_network_avg_link_out_volume_cache;
			boost::container::vector<Point_2D<MasterType>> _reference_network_avg_link_travel_time_ratio_cache;
			boost::container::vector<Point_2D<MasterType>> _reference_network_avg_link_speed_ratio_cache;
			boost::container::vector<Point_2D<MasterType>> _reference_network_avg_link_density_ratio_cache;
			boost::container::vector<Point_2D<MasterType>> _reference_network_avg_link_queue_length_cache;

			m_prototype(Antares_Layer<typename type_of(MasterType::antares_layer)>,network_vmt_layer, NONE, NONE);
			m_prototype(Antares_Layer<typename type_of(MasterType::antares_layer)>,network_vht_layer, NONE, NONE);
			m_prototype(Antares_Layer<typename type_of(MasterType::antares_layer)>,network_cumulative_loaded_vehicles_layer, NONE, NONE);
			m_prototype(Antares_Layer<typename type_of(MasterType::antares_layer)>,network_cumulative_departed_vehicles_layer, NONE, NONE);
			m_prototype(Antares_Layer<typename type_of(MasterType::antares_layer)>,network_in_network_vehicles_layer, NONE, NONE);
			m_prototype(Antares_Layer<typename type_of(MasterType::antares_layer)>,network_cumulative_arrived_vehicles_layer, NONE, NONE);
			m_prototype(Antares_Layer<typename type_of(MasterType::antares_layer)>,network_cumulative_combined_layer, NONE, NONE);

			m_prototype(Antares_Layer<typename type_of(MasterType::antares_layer)>,network_avg_link_travel_time_layer, NONE, NONE);
			m_prototype(Antares_Layer<typename type_of(MasterType::antares_layer)>,network_avg_link_speed_layer, NONE, NONE);
			m_prototype(Antares_Layer<typename type_of(MasterType::antares_layer)>,network_avg_link_density_layer, NONE, NONE);
			m_prototype(Antares_Layer<typename type_of(MasterType::antares_layer)>,network_avg_link_in_volume_layer, NONE, NONE);
			m_prototype(Antares_Layer<typename type_of(MasterType::antares_layer)>,network_avg_link_out_volume_layer, NONE, NONE);
			m_prototype(Antares_Layer<typename type_of(MasterType::antares_layer)>,network_avg_link_travel_time_ratio_layer, NONE, NONE);
			m_prototype(Antares_Layer<typename type_of(MasterType::antares_layer)>,network_avg_link_speed_ratio_layer, NONE, NONE);
			m_prototype(Antares_Layer<typename type_of(MasterType::antares_layer)>,network_avg_link_density_ratio_layer, NONE, NONE);
			m_prototype(Antares_Layer<typename type_of(MasterType::antares_layer)>,network_avg_link_queue_length_layer, NONE, NONE);

			m_prototype(Antares_Layer<typename type_of(MasterType::antares_layer)>,historic_link_travel_time_layer, NONE, NONE);
			m_prototype(Antares_Layer<typename type_of(MasterType::antares_layer)>,historic_link_speed_layer, NONE, NONE);
			m_prototype(Antares_Layer<typename type_of(MasterType::antares_layer)>,historic_link_density_layer, NONE, NONE);
			m_prototype(Antares_Layer<typename type_of(MasterType::antares_layer)>,historic_link_in_volume_layer, NONE, NONE);
			m_prototype(Antares_Layer<typename type_of(MasterType::antares_layer)>,historic_link_out_volume_layer, NONE, NONE);
			m_prototype(Antares_Layer<typename type_of(MasterType::antares_layer)>,historic_link_travel_time_ratio_layer, NONE, NONE);
			m_prototype(Antares_Layer<typename type_of(MasterType::antares_layer)>,historic_link_speed_ratio_layer, NONE, NONE);
			m_prototype(Antares_Layer<typename type_of(MasterType::antares_layer)>,historic_link_density_ratio_layer, NONE, NONE);
			m_prototype(Antares_Layer<typename type_of(MasterType::antares_layer)>,historic_link_queue_length_layer, NONE, NONE);

			boost::container::vector<struct Extended_MOE_Data> reference_td_network_moe_data_array;

			template<typename TargetType> void set_network_bounds()
			{
				_network_bounds.reboost::container::set<type_of(network_bounds),Antares_Network_Implementation,NULLTYPE>();
				typename _Intersections_Container_Interface::iterator intersection_itr;
				for(intersection_itr = _intersections_container.begin(); intersection_itr != _intersections_container.end(); intersection_itr++)
				{
					_Intersection_Interface* intersection = (_Intersection_Interface*)(*intersection_itr);

					if(intersection->template x_position<float>() < _network_bounds._xmin)
					{
						_network_bounds._xmin = intersection->template x_position<float>();
					}
					else if(intersection->template x_position<float>() > _network_bounds._xmax)
					{
						_network_bounds._xmax = intersection->template x_position<float>();
					}
					
					if(intersection->template y_position<float>() < _network_bounds._ymin)
					{
						_network_bounds._ymin = intersection->template y_position<float>();
					}
					else if(intersection->template y_position<float>() > _network_bounds._ymax)
					{
						_network_bounds._ymax = intersection->template y_position<float>();
					}
				}
			}

			template<typename TargetType> void initialize_antares_layers()
			{
				initialize_network_moe_plotting_layers<TargetType>();
				initialize_network_map_layers<TargetType>();
				initialize_reference_data<TargetType>();				
			}

			template<typename TargetType> void initialize_reference_data()
			{
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				reference_td_network_moe_data_array.clear();
				fstream& network_moe_reference_file = ((_Scenario_Interface*)_global_scenario)->template reference_realtime_network_moe_file<fstream&>();
				if (network_moe_reference_file.is_open())
				{
					struct Extended_MOE_Data moe_data;
					boost::container::vector<string> tokens;
					string line;
					int token_size = 20;
					getline(network_moe_reference_file,line); // skip the first line
					while (network_moe_reference_file.good())
					{
						getline(network_moe_reference_file,line);
						string_split(tokens, line, token_size);
						if (tokens[0].length() == 0)
						{
							break;
						}
						int time = stoi(tokens[1]);
						if (time < ((_Scenario_Interface*)_global_scenario)->template simulation_start_time<int>())
						{
							continue;
						}
						else if (time > ((_Scenario_Interface*)_global_scenario)->template simulation_end_time<int>() + ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>())
						{
							break;
						}
						else
						{
							moe_data.num_loaded_vehicles = stoi(tokens[2]);
							moe_data.num_departed_vehicles = stoi(tokens[3]);
							moe_data.num_arrived_vehicles = stoi(tokens[4]);
							moe_data.network_avg_link_travel_time = stof(tokens[5]);
							moe_data.network_avg_link_speed = stof(tokens[6]);
							moe_data.network_avg_link_density = stof(tokens[7]);
							moe_data.network_avg_link_in_volume = stof(tokens[8]);
							moe_data.network_avg_link_out_volume = stof(tokens[9]);
							moe_data.network_avg_link_travel_time_ratio = stof(tokens[10]);
							moe_data.network_avg_link_speed_ratio = stof(tokens[11]);
							moe_data.network_avg_link_density_ratio = stof(tokens[12]);
							moe_data.network_avg_link_queue_length = stof(tokens[13]);
							moe_data.network_vmt = int(stof(tokens[14]));
							moe_data.network_vht = int(stof(tokens[15]));
							moe_data.network_cumulative_loaded_vehicles = stoi(tokens[16]);
							moe_data.network_cumulative_departed_vehicles = stoi(tokens[17]);
							moe_data.network_in_network_vehicles = stoi(tokens[18]);
							moe_data.network_cumulative_arrived_vehicles = stoi(tokens[19]);
							reference_td_network_moe_data_array.push_back(moe_data);
						}
					}
				}
			}


			template<typename TargetType> void initialize_network_moe_plotting_layers()
			{
				// configure plot layer
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;

				Antares_Layer_Configuration pcfg;

				pcfg.Configure_Plot();
				pcfg.storage_period = ((_Scenario_Interface*)_global_scenario)->simulation_interval_length<int>();
				pcfg.storage_offset = ((_Scenario_Interface*)_global_scenario)->simulation_interval_length<int>() - 1;
				pcfg.targetsub_iteration() = Scenario_Components::Types::END_OF_ITERATION + 1;
				pcfg.grouped = true;
				pcfg.group_color = true;

				pcfg.x_label = "time (second)";
				
				pcfg.y_label = "vechile-miles traveled";
//TODO
//				_network_vmt_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(network_vmt_layer),Antares_Network_Implementation>*, string& > >(string("VMT"));
				_network_vmt_layer->Initialize<NULLTYPE>(pcfg);

				pcfg.y_label = "vechiles-hours traveled";
//TODO
//				_network_vht_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(network_vht_layer),Antares_Network_Implementation>*, string& > >(string("VHT"));
				_network_vht_layer->Initialize<NULLTYPE>(pcfg);

				//pcfg.y_label = "cumulative loaded vehicles";
//TODO
//				//_network_cumulative_loaded_vehicles_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(network_cumulative_loaded_vehicles_layer),Antares_Network_Implementation>*, string& > >(string("cumulative loaded"));
				//_network_cumulative_loaded_vehicles_layer->Initialize<NULLTYPE>(pcfg);

				pcfg.y_label = "cumulative departed vehicles";
//TODO
//				_network_cumulative_departed_vehicles_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(network_cumulative_departed_vehicles_layer),Antares_Network_Implementation>*, string& > >(string("cumulative departed"));
				_network_cumulative_departed_vehicles_layer->Initialize<NULLTYPE>(pcfg);

				pcfg.y_label = "cumulative arrived vehicles";
//TODO
//				_network_cumulative_arrived_vehicles_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(network_cumulative_arrived_vehicles_layer),Antares_Network_Implementation>*, string& > >(string("cumulative arrived"));
				_network_cumulative_arrived_vehicles_layer->Initialize<NULLTYPE>(pcfg);

				pcfg.y_label = "number of vehicles";
//TODO
//				_network_cumulative_combined_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(network_cumulative_combined_layer),Antares_Network_Implementation>*, string& > >(string("cumulative curves"));
				_network_cumulative_combined_layer->Initialize<NULLTYPE>(pcfg);
				
				pcfg.y_label = "in-network vehicles";
//TODO
//				_network_in_network_vehicles_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(network_in_network_vehicles_layer),Antares_Network_Implementation>*, string& > >(string("in-network"));
				_network_in_network_vehicles_layer->Initialize<NULLTYPE>(pcfg);

				pcfg.y_label = "link travel time (minute)";
//TODO
//				_network_avg_link_travel_time_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(network_avg_link_travel_time_layer),Antares_Network_Implementation>*, string& > >(string("network avg link travel time"));
				_network_avg_link_travel_time_layer->Initialize<NULLTYPE>(pcfg);

				pcfg.y_label = "link speed (miles/hour)";
//TODO
//				_network_avg_link_speed_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(network_avg_link_speed_layer),Antares_Network_Implementation>*, string& > >(string("network avg link speed"));
				_network_avg_link_speed_layer->Initialize<NULLTYPE>(pcfg);

				pcfg.y_label = "link density (vehiles/mile/lane)";
//TODO
//				_network_avg_link_density_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(network_avg_link_density_layer),Antares_Network_Implementation>*, string& > >(string("network avg link density"));
				_network_avg_link_density_layer->Initialize<NULLTYPE>(pcfg);

				pcfg.y_label = "link travel time (minute)";
//TODO
//				_historic_link_travel_time_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(historic_link_travel_time_layer),Antares_Network_Implementation>*, string& > >(string("link travel time"));
				_historic_link_travel_time_layer->Initialize<NULLTYPE>(pcfg);

				pcfg.y_label = "link speed (miles/hour)";
//TODO
//				_historic_link_speed_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(historic_link_speed_layer),Antares_Network_Implementation>*, string& > >(string("link speed"));
				_historic_link_speed_layer->Initialize<NULLTYPE>(pcfg);

				pcfg.y_label = "link density (vehiles/mile/lane)";
//TODO
//				_historic_link_density_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(historic_link_density_layer),Antares_Network_Implementation>*, string& > >(string("link density"));
				_historic_link_density_layer->Initialize<NULLTYPE>(pcfg);

				//pcfg.y_label = "link travel time ratio";
//TODO
//				//_network_avg_link_travel_time_ratio_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(network_avg_link_travel_time_ratio_layer),Antares_Network_Implementation>*, string& > >(string("avg link travel time ratio"));
				//_network_avg_link_travel_time_ratio_layer->Initialize<NULLTYPE>(pcfg);

				//pcfg.y_label = "link speed ratio";
//TODO
//				//_network_avg_link_speed_ratio_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(network_avg_link_speed_ratio_layer),Antares_Network_Implementation>*, string& > >(string("avg link speed ratio"));
				//_network_avg_link_speed_ratio_layer->Initialize<NULLTYPE>(pcfg);

				//pcfg.y_label = "link density ratio";
//TODO
//				//_network_avg_link_density_ratio_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(network_avg_link_density_ratio_layer),Antares_Network_Implementation>*, string& > >(string("avg link density ratio"));
				//_network_avg_link_density_ratio_layer->Initialize<NULLTYPE>(pcfg);

				//pcfg.y_label = "link queue length ratio";
//TODO
//				//_network_avg_link_queue_length_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(network_avg_link_queue_length_layer),Antares_Network_Implementation>*, string& > >(string("avg link queue length"));
				//_network_avg_link_queue_length_layer->Initialize<NULLTYPE>(pcfg);

				//pcfg.y_label = "link travel time ratio";
//TODO
//				//_historic_link_travel_time_ratio_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(historic_link_travel_time_ratio_layer),Antares_Network_Implementation>*, string& > >(string("historic link travel time ratio"));
				//_historic_link_travel_time_ratio_layer->Initialize<NULLTYPE>(pcfg);

				//pcfg.y_label = "link speed ratio";
//TODO
//				//_historic_link_speed_ratio_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(historic_link_speed_ratio_layer),Antares_Network_Implementation>*, string& > >(string("historic link speed ratio"));
				//_historic_link_speed_ratio_layer->Initialize<NULLTYPE>(pcfg);

				//pcfg.y_label = "link density ratio";
//TODO
//				//_historic_link_density_ratio_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(historic_link_density_ratio_layer),Antares_Network_Implementation>*, string& > >(string("historic link density ratio"));
				//_historic_link_density_ratio_layer->Initialize<NULLTYPE>(pcfg);

				//pcfg.y_label = "link queue length ratio";
//TODO
//				//_historic_link_queue_length_layer=Allocate_New_Plot_Layer< typename MasterType::type_of(information_panel),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(historic_link_queue_length_layer),Antares_Network_Implementation>*, string& > >(string("historic link queue length"));
				//_historic_link_queue_length_layer->Initialize<NULLTYPE>(pcfg);
			}

			template<typename TargetType> void initialize_network_map_layers()
			{
				initialize_intersection_map_layers<TargetType>();
				initialize_link_map_layers<TargetType>();
			}

			template<typename TargetType> void initialize_link_map_layers()
			{
//TODO
//				_link_lines=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(link_lines),Antares_Network_Implementation>*, string& > >(string("Links"));

				Antares_Layer_Configuration cfg;
				cfg.Configure_Static_Lines();
				cfg.draw=true;
				cfg.primitive_color=true;
				cfg.storage_period = 1;

				cfg.selection_callback = &MasterType::type_of(link)::on_select;
				cfg.head_accent_size_value = 3;
				
				//cfg.head_size_value = 
				//cfg.attributes_callback = (attributes_callback_type)&MasterType::type_of(link)::fetch_attributes;
				//cfg.attributes_schema = string("Id,Type,Travel time,Travel delay,Speed,Density,In-flow rate,Out-flow rate,Travel time ratio,Speed ratio,Density ratio,In-flow ratio,Out-flow ratio");

				//cfg.attributes_schema.push_back("Id");
				//cfg.attributes_schema.push_back("Type");
				//cfg.attributes_schema.push_back("Length");
				//cfg.attributes_schema.push_back("Number of lanes");
				//cfg.attributes_schema.push_back("Upstream node");
				//cfg.attributes_schema.push_back("Downstream node");
				//cfg.attributes_schema.push_back("Free-flow speed");
				//cfg.attributes_schema.push_back("Travel time");
				//cfg.attributes_schema.push_back("Speed");
				//cfg.attributes_schema.push_back("Density");
				//cfg.attributes_schema.push_back("Travel time ratio");
				//cfg.attributes_schema.push_back("Speed ratio");
				//cfg.attributes_schema.push_back("Density ratio");
				//cfg.attributes_schema.push_back("Queue length");

				_link_lines->Initialize<NULLTYPE>(cfg);

				typename _Links_Container_Interface::iterator link_itr;
				for (link_itr = _links_container.begin(); link_itr != _links_container.end(); link_itr++)
				{
					_Link_Interface* link = (_Link_Interface*)(*link_itr);
					link->template configure_displayed_line<NT>();
					_link_lines->Push_Element<Regular_Element>(link->template displayed_line<Link_Line<MasterType>*>());
				}
			}


			template<typename TargetType> void initialize_intersection_map_layers()
			{
//TODO
//				_intersection_polygons=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(intersection_polygons),Antares_Network_Implementation>*, string& > >(string("Intersections"));
				Antares_Layer_Configuration cfg;
				cfg.Configure_Static_Polygons();
				cfg.draw=false;

				cfg.selection_callback = &MasterType::type_of(intersection)::on_select;
				//cfg.attributes_callback = (attributes_callback_type)&MasterType::type_of(intersection)::fetch_attributes;
				//cfg.attributes_schema = string("Id,Type,Travel time,Travel delay,Speed,Density,In-flow rate,Out-flow rate,Travel time ratio,Speed ratio,Density ratio,In-flow ratio,Out-flow ratio");

				//cfg.attributes_schema.push_back("Id");
				//cfg.attributes_schema.push_back("Type");
				
				_intersection_polygons->Initialize<NULLTYPE>(cfg);

				Intersection_Polygon<MasterType> polygon;

				typename _Intersections_Container_Interface::iterator intersection_itr;
				for (intersection_itr = _intersections_container.begin(); intersection_itr != _intersections_container.end(); intersection_itr++)
				{
					_Intersection_Interface* intersection = (_Intersection_Interface*)(*intersection_itr);

					Point_3D<MasterType> v_node;
					v_node._x = intersection->x_position<float>();
					v_node._y = intersection->y_position<float>();
//TODO
//					Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NULLTYPE,void,Point_3D<MasterType>&>>(v_node);

					float x = v_node._x;
					float y = v_node._y;
					float sin_alpha = sin(45.0/2.0*3.14159265/180.0);
					float cos_alpha = cos(45.0/2.0*3.14159265/180.0);
					float r = _intersection_radius;

					polygon.vertices[0]._x = x - r * sin_alpha;
					polygon.vertices[0]._y = y - r * cos_alpha;
					polygon.vertices[0]._z = 0.5;
					
					polygon.vertices[1]._x = x + r * sin_alpha;
					polygon.vertices[1]._y = y - r * cos_alpha;
					polygon.vertices[1]._z = 0.5;

					polygon.vertices[2]._x = x + r * cos_alpha;
					polygon.vertices[2]._y = y - r * sin_alpha;
					polygon.vertices[2]._z = 0.5;

					polygon.vertices[3]._x = x + r * cos_alpha;
					polygon.vertices[3]._y = y + r * sin_alpha;
					polygon.vertices[3]._z = 0.5;

					polygon.vertices[4]._x = x + r * sin_alpha;
					polygon.vertices[4]._y = y + r * cos_alpha;
					polygon.vertices[4]._z = 0.5;

					polygon.vertices[5]._x = x - r * sin_alpha;
					polygon.vertices[5]._y = y + r * cos_alpha;
					polygon.vertices[5]._z = 0.5;

					polygon.vertices[6]._x = x - r * cos_alpha;
					polygon.vertices[6]._y = y + r * sin_alpha;
					polygon.vertices[6]._z = 0.5;

					polygon.vertices[7]._x = x - r * cos_alpha;
					polygon.vertices[7]._y = y - r * sin_alpha;
					polygon.vertices[7]._z = 0.5;

					True_Color_RGBA<NT> color;
					color = get_intersection_color_by_type(intersection->template intersection_type<int>());
					polygon.color = color;
					polygon.data = (void*)(intersection);
					_intersection_polygons->Push_Element<Regular_Element>(&polygon);
				}
			}

			True_Color_RGBA<NT> get_intersection_color_by_type(int intersection_type)
			{
				True_Color_RGBA<NT> color;
				switch(intersection_type)
				{
				case Intersection_Components::Types::Intersection_Type_Keys::YIELD_SIGN:
				case Intersection_Components::Types::Intersection_Type_Keys::ALL_WAY_STOP_SIGN:
				case Intersection_Components::Types::Intersection_Type_Keys::TWO_WAY_STOP_SIGN:
				case Intersection_Components::Types::Intersection_Type_Keys::PRE_TIMED_SIGNAL_CONTROL:
				case Intersection_Components::Types::Intersection_Type_Keys::ACTUATED_SIGNAL_CONTROL:
				case Intersection_Components::Types::Intersection_Type_Keys::ADAPTIVE_SIGNAL_CONTROL:
					color._r = 200;
					color._g = 200;
					color._b = 200;
					color._a = 255;
					return color;
				default:
					color._r = 200;
					color._g = 200;
					color._b = 200;
					color._a = 255;
					return color;
				}
			}		

			template<typename TargetType> void printResults() 
			{
				((Network_Implementation<MasterType,INHERIT(Integrated_Network_Implementation)>*)this)->printResults<TargetType>();
				plot_network_moes();
				plot_reference_network_moes();
			}

			void plot_vehicle_number()
			{
#pragma pack(push,1)
				struct Plot_Element
				{
					int num_primitives;
					Point_2D<MasterType>* points;
				};
#pragma pack(pop)
				Point_2D<MasterType> submission;
				submission._x=iteration();
				submission._y=MasterType::vehicle_type::_vehicles_counter;

				MasterType::vehicle_type::_num_vehicles_cache.push_back(submission);

				Plot_Element element;

				element.num_primitives = MasterType::vehicle_type::_num_vehicles_cache.size();
				element.points = &MasterType::vehicle_type::_num_vehicles_cache.front();

				MasterType::vehicle_type::_num_vehicles->Push_Element<Regular_Element>((void*)&element);

				MasterType::vehicle_type::_vehicles_counter=0;
			}

			void plot_network_moes()
			{

				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
#pragma pack(push,1)
				struct Plot_Element
				{
					True_Color_RGBA<NT> color;
					int num_primitives;
					Point_2D<MasterType>* points;
				};
#pragma pack(pop)
				Point_2D<MasterType> submission;
				Plot_Element element;
				element.color._r = 255;
				element.color._g = 0;
				element.color._b = 0;
				element.color._a = 255;
				submission._x=iteration();

				// plot vmt
				submission._y=_network_vmt;
				_network_vmt_cache.push_back(submission);
				element.num_primitives = _network_vmt_cache.size();
				element.points = &_network_vmt_cache.front();
				_network_vmt_layer->Push_Element<Regular_Element>((void*)&element);

				// plot vht
				submission._y=_network_vht;
				_network_vht_cache.push_back(submission);
				element.num_primitives = _network_vht_cache.size();
				element.points = &_network_vht_cache.front();
				_network_vht_layer->Push_Element<Regular_Element>((void*)&element);

				//// plot loaded vehicles
				//submission._y=((_Scenario_Interface*)_global_scenario)->template network_cumulative_loaded_vehicles<int>();
				//_network_cumulative_loaded_vehicles_cache.push_back(submission);
				//element.num_primitives = _network_cumulative_loaded_vehicles_cache.size();
				//element.points = &_network_cumulative_loaded_vehicles_cache.front();
				//_network_cumulative_loaded_vehicles_layer->Push_Element<Regular_Element>((void*)&element);

				// plot departed vehicles
				submission._y=((_Scenario_Interface*)_global_scenario)->template network_cumulative_departed_vehicles<int>();
				_network_cumulative_departed_vehicles_cache.push_back(submission);
				element.num_primitives = _network_cumulative_departed_vehicles_cache.size();
				element.points = &_network_cumulative_departed_vehicles_cache.front();
				_network_cumulative_departed_vehicles_layer->Push_Element<Regular_Element>((void*)&element);

				// plot arrived vehicles
				submission._y=((_Scenario_Interface*)_global_scenario)->template network_cumulative_arrived_vehicles<int>();
				_network_cumulative_arrived_vehicles_cache.push_back(submission);
				element.num_primitives = _network_cumulative_arrived_vehicles_cache.size();
				element.points = &_network_cumulative_arrived_vehicles_cache.front();
				_network_cumulative_arrived_vehicles_layer->Push_Element<Regular_Element>((void*)&element);

				// plot in_network vehicles
				submission._y=((_Scenario_Interface*)_global_scenario)->template network_in_network_vehicles<int>();
				_network_in_network_vehicles_cache.push_back(submission);
				element.num_primitives = _network_in_network_vehicles_cache.size();
				element.points = &_network_in_network_vehicles_cache.front();
				_network_in_network_vehicles_layer->Push_Element<Regular_Element>((void*)&element);

				// plot cumulative combined
				element.num_primitives = _network_cumulative_departed_vehicles_cache.size();
				element.points = &_network_cumulative_departed_vehicles_cache.front();
				_network_cumulative_combined_layer->Push_Element<Regular_Element>((void*)&element);
				Plot_Element element2;
				
				element2.num_primitives = _network_cumulative_arrived_vehicles_cache.size();
				element2.points = &_network_cumulative_arrived_vehicles_cache.front();
				_network_cumulative_combined_layer->Push_Element<Regular_Element>((void*)&element2);

				// plot network_avg_link_travel_time
				submission._y=realtime_network_moe_data.network_avg_link_travel_time;
				_network_avg_link_travel_time_cache.push_back(submission);
				element.num_primitives = _network_avg_link_travel_time_cache.size();
				element.points = &_network_avg_link_travel_time_cache.front();
				_network_avg_link_travel_time_layer->Push_Element<Regular_Element>((void*)&element);

				// plot _network_avg_link_speed_cache
				submission._y=realtime_network_moe_data.network_avg_link_speed;
				_network_avg_link_speed_cache.push_back(submission);
				element.num_primitives = _network_avg_link_speed_cache.size();
				element.points = &_network_avg_link_speed_cache.front();
				_network_avg_link_speed_layer->Push_Element<Regular_Element>((void*)&element);


				// plot _network_avg_link_density_cache
				submission._y=realtime_network_moe_data.network_avg_link_density;
				_network_avg_link_density_cache.push_back(submission);
				element.num_primitives = _network_avg_link_density_cache.size();
				element.points = &_network_avg_link_density_cache.front();
				_network_avg_link_density_layer->Push_Element<Regular_Element>((void*)&element);

				//// plot _network_avg_link_travel_time_ratio
				//submission._y=realtime_network_moe_data.network_avg_link_travel_time_ratio;
				//_network_avg_link_travel_time_ratio_cache.push_back(submission);
				//element.num_primitives = _network_avg_link_travel_time_ratio_cache.size();
				//element.points = &_network_avg_link_travel_time_ratio_cache.front();
				//_network_avg_link_travel_time_ratio_layer->Push_Element<Regular_Element>((void*)&element);


				//// plot _network_avg_link_speed_ratio_cache
				//submission._y=realtime_network_moe_data.network_avg_link_speed_ratio;
				//_network_avg_link_speed_ratio_cache.push_back(submission);
				//element.num_primitives = _network_avg_link_speed_ratio_cache.size();
				//element.points = &_network_avg_link_speed_ratio_cache.front();
				//_network_avg_link_speed_ratio_layer->Push_Element<Regular_Element>((void*)&element);


				//// plot _network_avg_link_density_ratio_cache
				//submission._y=realtime_network_moe_data.network_avg_link_density_ratio;
				//_network_avg_link_density_ratio_cache.push_back(submission);
				//element.num_primitives = _network_avg_link_density_ratio_cache.size();
				//element.points = &_network_avg_link_density_ratio_cache.front();
				//_network_avg_link_density_ratio_layer->Push_Element<Regular_Element>((void*)&element);

				//// plot _network_avg_link_queue_length_cache
				//submission._y=realtime_network_moe_data.network_avg_link_queue_length;
				//_network_avg_link_queue_length_cache.push_back(submission);
				//element.num_primitives = _network_avg_link_queue_length_cache.size();
				//element.points = &_network_avg_link_queue_length_cache.front();
				//_network_avg_link_queue_length_layer->Push_Element<Regular_Element>((void*)&element);
			}

			void plot_reference_network_moes()
			{
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				typedef Network_Components::Prototypes::Network<typename MasterType::network_type> _Network_Interface;

				int time_index = ((_Network_Interface*)this)->template current_simulation_interval_index<int>();
				if (time_index >= (int)(reference_td_network_moe_data_array.size())) 
				{
					return;
				}
				Extended_MOE_Data reference_moe_data = reference_td_network_moe_data_array[time_index];
				if (time_index < (int)(reference_td_network_moe_data_array.size() - 1))
				{
					reference_moe_data.network_vmt =  reference_td_network_moe_data_array[time_index+1].network_vmt;
					reference_moe_data.network_vht =  reference_td_network_moe_data_array[time_index+1].network_vht;
				}
#pragma pack(push,1)
				struct Plot_Element
				{
					True_Color_RGBA<NT> color;
					int num_primitives;
					Point_2D<MasterType>* points;
				};
#pragma pack(pop)
				Point_2D<MasterType> submission;
				Plot_Element element;
				element.color._r = 128;
				element.color._g = 128;
				element.color._b = 128;
				element.color._a = 255;
				submission._x=iteration();

				// plot vmt
				submission._y=reference_moe_data.network_vmt;
				_reference_network_vmt_cache.push_back(submission);
				element.num_primitives = _reference_network_vmt_cache.size();
				element.points = &_reference_network_vmt_cache.front();
				_network_vmt_layer->Push_Element<Regular_Element>((void*)&element);

				// plot vht
				submission._y=reference_moe_data.network_vht;
				_reference_network_vht_cache.push_back(submission);
				element.num_primitives = _reference_network_vht_cache.size();
				element.points = &_reference_network_vht_cache.front();
				_network_vht_layer->Push_Element<Regular_Element>((void*)&element);

				//// plot loaded vehicles
				//submission._y=reference_moe_data.network_cumulative_loaded_vehicles;
				//_reference_network_cumulative_loaded_vehicles_cache.push_back(submission);
				//element.num_primitives = _reference_network_cumulative_loaded_vehicles_cache.size();
				//element.points = &_reference_network_cumulative_loaded_vehicles_cache.front();
				//_network_cumulative_loaded_vehicles_layer->Push_Element<Regular_Element>((void*)&element);

				// plot departed vehicles
				submission._y=reference_moe_data.network_cumulative_departed_vehicles;
				_reference_network_cumulative_departed_vehicles_cache.push_back(submission);
				element.num_primitives = _reference_network_cumulative_departed_vehicles_cache.size();
				element.points = &_reference_network_cumulative_departed_vehicles_cache.front();
				_network_cumulative_departed_vehicles_layer->Push_Element<Regular_Element>((void*)&element);

				// plot arrived vehicles
				submission._y=reference_moe_data.network_cumulative_arrived_vehicles;
				_reference_network_cumulative_arrived_vehicles_cache.push_back(submission);
				element.num_primitives = _reference_network_cumulative_arrived_vehicles_cache.size();
				element.points = &_reference_network_cumulative_arrived_vehicles_cache.front();
				_network_cumulative_arrived_vehicles_layer->Push_Element<Regular_Element>((void*)&element);

				// plot in_network vehicles
				submission._y=reference_moe_data.network_in_network_vehicles;
				_reference_network_in_network_vehicles_cache.push_back(submission);
				element.num_primitives = _reference_network_in_network_vehicles_cache.size();
				element.points = &_reference_network_in_network_vehicles_cache.front();
				_network_in_network_vehicles_layer->Push_Element<Regular_Element>((void*)&element);

				// plot network_avg_link_travel_time
				submission._y=reference_moe_data.network_avg_link_travel_time;
				_reference_network_avg_link_travel_time_cache.push_back(submission);
				element.num_primitives = _reference_network_avg_link_travel_time_cache.size();
				element.points = &_reference_network_avg_link_travel_time_cache.front();
				_network_avg_link_travel_time_layer->Push_Element<Regular_Element>((void*)&element);

				// plot _network_avg_link_speed_cache
				submission._y=reference_moe_data.network_avg_link_speed;
				_reference_network_avg_link_speed_cache.push_back(submission);
				element.num_primitives = _reference_network_avg_link_speed_cache.size();
				element.points = &_reference_network_avg_link_speed_cache.front();
				_network_avg_link_speed_layer->Push_Element<Regular_Element>((void*)&element);


				// plot _network_avg_link_density_cache
				submission._y=reference_moe_data.network_avg_link_density;
				_reference_network_avg_link_density_cache.push_back(submission);
				element.num_primitives = _reference_network_avg_link_density_cache.size();
				element.points = &_reference_network_avg_link_density_cache.front();
				_network_avg_link_density_layer->Push_Element<Regular_Element>((void*)&element);

				//// plot _network_avg_link_travel_time_ratio
				//submission._y=reference_moe_data.network_avg_link_travel_time_ratio;
				//_reference_network_avg_link_travel_time_ratio_cache.push_back(submission);
				//element.num_primitives = _reference_network_avg_link_travel_time_ratio_cache.size();
				//element.points = &_reference_network_avg_link_travel_time_ratio_cache.front();
				//_network_avg_link_travel_time_ratio_layer->Push_Element<Regular_Element>((void*)&element);


				//// plot _network_avg_link_speed_ratio_cache
				//submission._y=reference_moe_data.network_avg_link_speed_ratio;
				//_reference_network_avg_link_speed_ratio_cache.push_back(submission);
				//element.num_primitives = _reference_network_avg_link_speed_ratio_cache.size();
				//element.points = &_reference_network_avg_link_speed_ratio_cache.front();
				//_network_avg_link_speed_ratio_layer->Push_Element<Regular_Element>((void*)&element);


				//// plot _network_avg_link_density_ratio_cache
				//submission._y=reference_moe_data.network_avg_link_density_ratio;
				//_reference_network_avg_link_density_ratio_cache.push_back(submission);
				//element.num_primitives = _reference_network_avg_link_density_ratio_cache.size();
				//element.points = &_reference_network_avg_link_density_ratio_cache.front();
				//_network_avg_link_density_ratio_layer->Push_Element<Regular_Element>((void*)&element);

				//// plot _network_avg_link_queue_length_cache
				//submission._y=reference_moe_data.network_avg_link_queue_length;
				//_reference_network_avg_link_queue_length_cache.push_back(submission);
				//element.num_primitives = _reference_network_avg_link_queue_length_cache.size();
				//element.points = &_reference_network_avg_link_queue_length_cache.front();
				//_network_avg_link_queue_length_layer->Push_Element<Regular_Element>((void*)&element);
			}

			void read_historic_link_moe()
			{
				typedef  Link_Components::Prototypes::Link<typename remove_pointer<typename  type_of(links_container)::value_type>::type>  _Link_Interface;
				typedef  Random_Access_Sequence< type_of(links_container), _Link_Interface*> _Links_Container_Interface;

				typedef Network_Components::Prototypes::Network<typename MasterType::network_type> _Network_Interface;
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;

				fstream& historic_link_moe_file = ((_Scenario_Interface*)_global_scenario)->template historic_link_moe_file<fstream&>();
				if (!historic_link_moe_file.is_open())
				{
					THROW_EXCEPTION(endl << "Link MOE reference file cannot be opened" << endl);
				}
				struct Link_MOE_Data link_moe_data;
				boost::container::vector<string> tokens;
				string line;
				int current_time = ((_Network_Interface*)this)->template start_of_current_simulation_interval_absolute<int>();
				int record_time;
				typename MasterType::link_type* link;
				Network_Components::Types::Link_ID_Dir link_id_dir;
				for (int i = 0; i < (int)_links_container.size(); i++)
				{
					getline(historic_link_moe_file, line); 
					string_split(tokens, line);
					if (tokens.size() == 0)
					{
						break;
					}
					record_time = stoi(tokens[1]);
					if (record_time < current_time)
					{
						// skip until current time
						while(historic_link_moe_file.good())
						{
							getline(historic_link_moe_file, line);
							string_split(tokens, line);
							record_time = stoi(tokens[1]);
							if (record_time >= current_time)
							{
								break;
							}
						}
					}
					link_id_dir.id = stol(tokens[3]);
					link_id_dir.dir = stol(tokens[4]);
					link = (typename MasterType::link_type*)(_link_dbid_dir_to_ptr_map[link_id_dir.id_dir]);
					int offset = 8;
					link_moe_data.link_travel_time = stof(tokens[offset++]);
					link_moe_data.link_travel_time_standard_deviation = stof(tokens[offset++]);
					link_moe_data.link_queue_length = stof(tokens[offset++]);
					link_moe_data.link_travel_delay = stof(tokens[offset++]);
					link_moe_data.link_travel_delay_standard_deviation = stof(tokens[offset++]);
					link_moe_data.link_speed = stof(tokens[offset++]);
					link_moe_data.link_density = stof(tokens[offset++]);
					link_moe_data.link_in_flow_rate = stof(tokens[offset++]);
					link_moe_data.link_out_flow_rate = stof(tokens[offset++]);
					link_moe_data.link_in_volume = stof(tokens[offset++]);
					link_moe_data.link_out_volume = stof(tokens[offset++]);
					link_moe_data.link_speed_ratio = stof(tokens[offset++]);
					link_moe_data.link_in_flow_ratio = stof(tokens[offset++]);
					link_moe_data.link_out_flow_ratio = stof(tokens[offset++]);
					link_moe_data.link_density_ratio = stof(tokens[offset++]);
					link_moe_data.link_travel_time_ratio = stof(tokens[offset++]);
					link_moe_data.num_vehicles_in_link = stof(tokens[offset++]);
					link->historic_link_moe_data = link_moe_data;
				}
			}
			m_data(Rectangle_XY<MasterType>, network_bounds, NONE, NONE);
			
			static m_prototype(Antares_Layer<typename type_of(MasterType::antares_layer)>,link_lines, NONE, NONE);
			static m_prototype(Antares_Layer<typename type_of(MasterType::antares_layer)>,intersection_polygons, NONE, NONE);
	
			typedef  Intersection_Components::Prototypes::Intersection<typename remove_pointer<typename  type_of(intersections_container)::value_type>::type>  _Intersection_Interface;
			typedef  Random_Access_Sequence< type_of(intersections_container), _Intersection_Interface*> _Intersections_Container_Interface;

			typedef  Link_Components::Prototypes::Link<typename remove_pointer<typename  type_of(links_container)::value_type>::type>  _Link_Interface;
			typedef  Random_Access_Sequence< type_of(links_container), _Link_Interface*> _Links_Container_Interface;

		};

		template<typename MasterType,typename InheritanceList>
		float Antares_Network_Implementation<MasterType,InheritanceList>::_intersection_radius = 50;

		template<typename MasterType,typename InheritanceList>
		Antares_Layer<typename type_of(MasterType::antares_layer)>* Antares_Network_Implementation<MasterType,InheritanceList>::_link_lines;
		
		template<typename MasterType,typename InheritanceList>
		Antares_Layer<typename type_of(MasterType::antares_layer)>* Antares_Network_Implementation<MasterType,InheritanceList>::_intersection_polygons;
	}
}

using namespace Network_Components::Implementations;
