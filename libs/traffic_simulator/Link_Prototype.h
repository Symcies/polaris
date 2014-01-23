#pragma once
#include "User_Space_Includes.h"

namespace Link_Components
{
	namespace Types
	{
		enum Link_Type_Keys
		{
			FREEWAY=0,
			ON_RAMP,
			OFF_RAMP,
			EXPRESSWAY,
			ARTERIAL,
			LOCAL,
			LIGHT_RAILWAY,
			HEAVY_RAILWAY,
			WALKWAY,
			BIKEWAY,
			WATERWAY,
		};
	}

	namespace Concepts
	{
		concept struct Is_Basic_Link_Prototype
		{
			check_template_method_name(has_upstream_intersection, Component_Type::upstream_intersection);
			check_template_method_name(has_downstream_intersection, Component_Type::downstream_intersection);
			check_template_method_name(has_network_reference, Component_Type::network_reference);
			check_template_method_name(has_link_type, Component_Type::link_type);  
			check_template_method_name(has_uuid, Component_Type::uuid); 
			check_template_method_name(has_internal_id, Component_Type::internal_id); 
			define_default_check(has_upstream_intersection && has_downstream_intersection && has_network_reference && has_link_type  && has_uuid && has_internal_id);
		};
		concept struct Is_Basic_Link
		{
			check_template_method_name(has_upstream_intersection, upstream_intersection);
			check_template_method_name(has_downstream_intersection, downstream_intersection);
			check_template_method_name(has_network_reference, network_reference);
			check_template_method_name(has_link_type, link_type); 
			check_template_method_name(has_uuid, uuid); 
			check_template_method_name(has_internal_id, internal_id); 

			check_concept(is_basic_link_prototype, Is_Basic_Link_Prototype, T, V);
			define_sub_check(is_basic_link, has_upstream_intersection && has_downstream_intersection && has_network_reference && has_link_type  && has_uuid && has_internal_id);
			define_default_check(is_basic_link || is_basic_link_prototype);
		};
	}
	
	namespace Prototypes
	{
		prototype struct Link ADD_DEBUG_INFO
		{
			tag_as_prototype;
			
			//==================================================================================================================
			/// basic link
			//------------------------------------------------------------------------------------------------------------------
			accessor(uuid, NONE, NONE);
			accessor(internal_id, NONE, NONE);
			accessor(network_reference, NONE, NONE);
			accessor(upstream_intersection, NONE, NONE);
			accessor(downstream_intersection, NONE, NONE);
			accessor(length, NONE, NONE);
			accessor(link_type, NONE, NONE);
			accessor(activity_locations, NONE, NONE);
			accessor(dbid, NONE, NONE);
			accessor(direction, NONE, NONE);
			//------------------------------------------------------------------------------------------------------------------

			//==================================================================================================================
			/// transportation link
			//------------------------------------------------------------------------------------------------------------------
			accessor(speed_limit, NONE, NONE);
			accessor(num_lanes, NONE, NONE);
			accessor(travel_time, NONE, NONE);
			accessor(realtime_travel_time, NONE, NONE);
			accessor(inbound_turn_movements, NONE, NONE);
			accessor(outbound_turn_movements, NONE, NONE);
			accessor(num_left_turn_bays, NONE, NONE);
			accessor(num_right_turn_bays, NONE, NONE);
			accessor(left_turn_bay_length, NONE, NONE);
			accessor(right_turn_bay_length, NONE, NONE);
			accessor(approach, NONE, NONE);
			accessor(num_inbound_turn_lanes, NONE, NONE);
			//------------------------------------------------------------------------------------------------------------------

			//==================================================================================================================
			/// link with control
			//------------------------------------------------------------------------------------------------------------------
			accessor(detector, NONE, NONE);
			accessor(lane_groups, NONE, NONE);
			accessor(ramp_meter, NONE, NONE);
			//------------------------------------------------------------------------------------------------------------------

			//==================================================================================================================
			/// routable link
			//------------------------------------------------------------------------------------------------------------------
			accessor(f_cost, NONE, NONE);
			accessor(h_cost, NONE, NONE);
			accessor(label_pointer, NONE, NONE);
			accessor(label_cost, NONE, NONE);
			accessor(scan_list_status, NONE, NONE);
			accessor(scan_list_position, NONE, NONE);
			accessor(reset_list_status, NONE, NONE);
			accessor(network_link_reference, NONE, NONE);
			accessor(replicas_container, NONE, NONE);
			accessor(realtime_replicas_container, NONE, NONE);
			//------------------------------------------------------------------------------------------------------------------

			//==================================================================================================================
			/// simulation link
			//------------------------------------------------------------------------------------------------------------------
			accessor(link_vmt, NONE, NONE);
			accessor(link_vht, NONE, NONE);
			//------------------------------------------------------------------------------------------------------------------

			//==================================================================================================================
			/// queueing link
			//------------------------------------------------------------------------------------------------------------------

			// fast way to check if any external entities are trying to access this feature
			// accessor(link_destination_cumulative_arrived_vehicles, requires(TargetType,check_2(ComponentType,is_same)), requires(check_2(ComponentType,is_same)));
			accessor(link_destination_cumulative_arrived_vehicles, NONE, NONE);			
			accessor(link_destination_arrived_vehicles, NONE, NONE);
			accessor(link_destination_vehicle_queue, NONE, NONE);
			accessor(current_vehicle_queue, NONE, NONE);	

			accessor(link_fftt, NONE, NONE);
			accessor(link_bwtt, NONE, NONE);
			accessor(link_fftt_cached_simulation_interval_size, NONE, NONE);
			accessor(link_bwtt_cached_simulation_interval_size, NONE, NONE);
			
			accessor(link_capacity, NONE, NONE);
			accessor(link_supply, NONE, NONE);
			
			accessor(link_upstream_arrived_vehicles, NONE, NONE);
			accessor(link_downstream_departed_vehicles, NONE, NONE);
			
			accessor(link_origin_arrived_vehicles, NONE, NONE);
			accessor(link_origin_departed_vehicles, NONE, NONE);
			accessor(link_origin_loaded_vehicles, NONE, NONE);
			accessor(link_origin_loaded_capacity_leftover, NONE, NONE);
		
			//cumulative - Mid-Trip
			accessor(link_upstream_cumulative_arrived_vehicles, NONE, NONE);
			//accessor(link_shifted_cumulative_arrived_vehicles, NONE, NONE);
			accessor(link_upstream_cumulative_vehicles, NONE, NONE);
			accessor(link_downstream_cumulative_vehicles, NONE, NONE);
			accessor(link_downstream_cumulative_arrived_vehicles, NONE, NONE);
			
			//cumulative - Begin/End-Trip
			accessor(link_origin_cumulative_arrived_vehicles, NONE, NONE);
			accessor(link_origin_cumulative_departed_vehicles, NONE, NONE);
			//accessor(cached_link_shifted_cumulative_arrived_vehicles, NONE, NONE);
			accessor(cached_link_upstream_cumulative_vehicles_array, NONE, NONE);
			accessor(cached_link_downstream_cumulative_vehicles_array, NONE, NONE);
			accessor(link_origin_vehicle_current_position, NONE, NONE);
			//accessor(link_origin_vehicle_array, NONE, NONE);
			accessor(link_origin_vehicle_queue, NONE, NONE);
			
			accessor(maximum_flow_rate, NONE, NONE);
			accessor(free_flow_speed, NONE, NONE);
			accessor(backward_wave_speed, NONE, NONE);
			accessor(jam_density, NONE, NONE);
			accessor(num_vehicles_under_jam_density, NONE, NONE);
			accessor(critical_density, NONE, NONE);
			accessor(link_num_vehicles_in_queue, NONE, NONE);
			accessor(num_vehicles_on_link, NONE, NONE);
			accessor(original_free_flow_speed, NONE, NONE);
			accessor(original_maximum_flow_rate, NONE, NONE);
			accessor(original_num_lanes, NONE, NONE);
			accessor(original_speed_limit, NONE, NONE);
			accessor(shoulder_opened, NONE, NONE);
			// event-aware link
			accessor(event_affected, NONE, NONE);
			accessor(advisory_radio, NONE, NONE);
			accessor(depot, NONE, NONE);
			accessor(variable_word_sign, NONE, NONE);
			accessor(variable_speed_sign, NONE, NONE);

			accessor(weather_event_to_process, NONE, NONE);
			accessor(current_weather_event, NONE, NONE);
			accessor(accident_event_to_process, NONE, NONE);
			accessor(current_accident_event, NONE, NONE);
			
			accessor(speed_adjustment_factor_due_to_weather, NONE, NONE);
			accessor(speed_adjustment_factor_due_to_accident, NONE, NONE);
			accessor(capacity_adjustment_factor_due_to_weather, NONE, NONE);
			accessor(capacity_adjustment_factor_due_to_accident, NONE, NONE);
			accessor(lane_adjustment_due_to_accident, NONE, NONE);

			// ITS-enabled link
			accessor(advisory_radio_events, NONE, NONE);
			
			template<typename TargetType> void Push_ITS(TargetType its)
			{
				this_component()->template Accept_ITS<TargetType>(its);
			}

			//visualized link
			accessor(displayed_line, NONE, NONE);

			template<typename TargetType> void push_vehicle(TargetType vehicle)
			{
				accept_vehicle<TargetType>(vehicle);
			}

			template<typename TargetType> void accept_vehicle(TargetType vehicle)
			{
				this_component()->template accept_vehicle<TargetType>(vehicle);
			}

			template<typename TargetType> void link_supply_update()
			{
				this_component()->template link_supply_update<TargetType>();
			}
			
			template<typename TargetType> void link_moving()
			{
				this_component()->template link_moving<TargetType>();
			}

			template<typename TargetType> void origin_link_loading()
			{
				this_component()->template origin_link_loading<TargetType>();
			}

			template<typename TargetType> void network_state_update()
			{
				this_component()->template network_state_update<TargetType>();
			}

			template<typename TargetType> void Initialize()
			{
				this_component()->template Initialize<TargetType>();
			}

			template<typename TargetType> void reset_routable_link()
			{
				this_component()->template reset_routable_link<TargetType>();
			}

			template<typename TargetType> void construct_routable_from_regular(TargetType regular_link)
			{
				this_component()->template construct_routable_from_regular<TargetType>(regular_link);
			}

			template<typename TargetType> void construct_realtime_routable_from_regular(TargetType regular_link)
			{
				this_component()->template construct_realtime_routable_from_regular<TargetType>(regular_link);
			}

			template<typename TargetType> void initialize_features(TargetType param)
			{
				this_component()->template initialize_features<TargetType>(param);
			}

			template<typename TargetType> void calculate_moe_for_simulation_interval()
			{
				this_component()->template calculate_moe_for_simulation_interval<TargetType>();
			}

			template<typename TargetType> void calculate_moe_for_assignment_interval()
			{
				this_component()->template calculate_moe_for_assignment_interval<TargetType>();
			}

			template<typename TargetType> void update_vmt_vht()
			{
				this_component()->template update_vmt_vht<TargetType>();
			}

			template<typename TargetType> void visualize_moe()
			{
				this_component()->template visualize_moe<TargetType>();
			}

			template<typename TargetType> void configure_displayed_line()
			{
				this_component()->template configure_displayed_line<TargetType>();
			}
			
			template<typename TargetType> void handle_events()
			{
				this_component()->template handle_events<TargetType>();
			}

			template<typename TargetType> void get_events_from_vms(TargetType events_set)
			{
				this_component()->template get_events_from_vms<TargetType>(events_set);
			}
			template<typename TargetType> void get_events_from_har(TargetType events_set)
			{
				this_component()->template get_events_from_har<TargetType>(events_set);
			}

			template<typename TargetType> void get_link_moe(int& start_time, int& end_time, int& volume, float& speed, float& density)
			{
				this_component()->template get_link_moe<TargetType>(start_time, end_time, volume, speed, density);
			}
			template<typename TargetType> void get_prevailing_link_moe(int& volume, float& speed, float& density)
			{
				this_component()->template get_prevailing_link_moe<TargetType>(volume, speed, density);
			}

			/*
			 * return true if normal day link moe is available in which case volume, speed, and density are filled up with corresponding data;
			 * return false if normal day link moe is unavailable.
			 */
			template<typename TargetType> bool get_normal_day_link_moe(int& volume, float& speed, float& density)
			{
				return this_component()->template get_normal_day_link_moe<TargetType>(volume, speed, density);
			}
			template<typename TargetType> void open_shoulder()
			{
				this_component()->template open_shoulder<TargetType>();
			}
			template<typename TargetType> void close_shoulder()
			{
				this_component()->template close_shoulder<TargetType>();
			}
			template<typename TargetType> void change_speed_limit(float speed_limit)
			{
				this_component()->template change_speed_limit<TargetType>(speed_limit);
			}
			template<typename TargetType> float free_flow_speed_estimate()
			{
				return this_component()->template free_flow_speed_estimate<TargetType>();
			}			
		};
	}
}

using namespace Link_Components::Prototypes;
using namespace Link_Components::Types;
