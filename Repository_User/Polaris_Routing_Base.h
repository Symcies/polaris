#pragma once
#include "Repository_User.h"
#include "Routable_Intersection_Base.h"
#include "Routable_Link_Base.h"
#include "Polaris_Vehicle_Base.h"

namespace Routing_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Bases
	{

		template<typename MasterType>
		struct Routable_Network_Base
		{

			typedef set<pair<float, void*>> scan_list_type;
			member_data_basic(scan_list_type, scan_list);
			member_data_basic(float, max_free_flow_speed);
			
			//========================================================================================================
			//start implementation of member_data(vecotr<void *>, intersections)
			//--------------------------------------------------------------------------------------------------------
		public:
			typedef typename MasterType::routable_intersection_type intersection_type;
			typedef intersection_type intersections_container_element_type;
			typedef vector<void*> intersections_container_type;

			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType intersections(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)_intersections;} tag_getter(intersections);
		
			intersections_container_type _intersections;
			//--------------------------------------------------------------------------------------------------------
			
			//========================================================================================================
			//start implementation of member_data(vecotr<void *>, intersections)
			//--------------------------------------------------------------------------------------------------------
			typedef typename MasterType::routable_link_type link_type;
			typedef link_type links_container_element_type;
			typedef vector<void*> links_container_type;

			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType links(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)_links;} tag_getter(links);
		
			links_container_type _links;
			//--------------------------------------------------------------------------------------------------------

			//========================================================================================================
			//start implementation of member_data(vecotr<void *>, intersections)
			//--------------------------------------------------------------------------------------------------------
			typedef typename MasterType::link_type network_link_type;
			typedef network_link_type reversed_path_container_element_type; 
			typedef vector<void*> reversed_path_container_type;
			
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType reversed_path_container(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)_reversed_path_container;} tag_getter(reversed_path_container);
		
			reversed_path_container_type _reversed_path_container;
			//--------------------------------------------------------------------------------------------------------

			typedef typename MasterType::network_type regular_network_type;
			
			typedef typename MasterType::link_type regular_links_container_element_type;
			typedef vector<void*> regular_links_container_type;
			
			typedef typename MasterType::intersection_type regular_intersections_container_element_type;
			typedef vector<void*> regular_intersections_container_type;

			typedef typename MasterType::routable_link_type routable_links_container_element_type;
			typedef vector<void*> routable_links_container_type;

			typedef typename MasterType::routable_intersection_type routable_intersections_container_element_type;
			typedef vector<void*> routable_intersections_container_type;

			typedef typename MasterType::inbound_outbound_movements_type regular_inbound_outbound_movements_container_element_type;
			typedef typename vector<void*> regular_inbound_outbound_movements_container_type;

			typedef typename MasterType::routable_inbound_outbound_movements_type routable_inbound_outbound_movements_container_element_type;
			typedef typename vector<void*> routable_inbound_outbound_movements_container_type;

			typedef typename MasterType::movement_type regular_movements_container_element_type;
			typedef typename vector<void*> regular_movements_container_type;
			
			typedef typename MasterType::routable_movement_type routable_movements_container_element_type;
			typedef typename vector<void*> routable_movements_container_type;
		};

		template<typename MasterType>
		struct Polaris_Routing_Base
		{
			typedef typename MasterType::vehicle_type vehicle_type;

			template<typename ThisType, typename CallerType, typename TargetType>
			void vehicle(TargetType set_value, call_requirements(requires(ThisType,Is_Dispatched))){_vehilce = (void*)set_value;} tag_setter(vehicle);

			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType vehicle(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)_vehicle;} tag_getter(vehicle);
	
			void* _vehicle;


			//member_data_basic(void*, vehicle);

			typedef typename MasterType::routable_network_type routable_network_type;

			template<typename ThisType, typename CallerType, typename TargetType>
			void routable_network(TargetType set_value, call_requirements(requires(ThisType,Is_Dispatched))){_routable_network = (void*)set_value;} tag_setter(routable_network);

			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType routable_network(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)_routable_network;} tag_getter(routable_network);
	
			void* _routable_network;			
			
			typedef typename MasterType::routable_link_type routable_link_type;
			
			template<typename ThisType, typename CallerType, typename TargetType>
			void origin_link(TargetType set_value, call_requirements(requires(ThisType,Is_Dispatched))){_origin_link = (void*)set_value;} tag_setter(origin_link);

			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType origin_link(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)_origin_link;} tag_getter(origin_link);
	
			void* _origin_link;	
			
			template<typename ThisType, typename CallerType, typename TargetType>
			void destination_link(TargetType set_value, call_requirements(requires(ThisType,Is_Dispatched))){_destination_link = (void*)set_value;}	tag_setter(destination_link);

			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType destination_link(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)_destination_link;} tag_getter(destination_link);

			void* _destination_link;	
			
			typedef vector<void *> turn_movements_container_type;
		};
	}

	namespace Components
	{
		template<typename MasterType>
		struct Routable_Network_Component
		{
			typedef Polaris_Component<Routing_Components::Interfaces::Routable_Network_Interface, Routing_Components::Bases::Routable_Network_Base<MasterType>, NULLTYPE, MasterType> type;
		};

		template<typename MasterType>
		struct Polaris_Routing_Component
		{
			typedef Polaris_Component_Execution<Routing_Components::Interfaces::Routing_Interface, Routing_Components::Bases::Polaris_Routing_Base<MasterType>, NULLTYPE, MasterType> type;
	
		};	
	}	

}