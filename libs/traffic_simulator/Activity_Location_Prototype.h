#pragma once
#include "User_Space_Includes.h"

namespace Activity_Location_Components
{
	namespace Types
	{
		enum LAND_USE
		{
			LU_AGRICULTURE,
			LU_ALL,
			LU_BUSINESS,
			LU_CIVIC_RELIGIOUS,
			LU_CULTURAL,
			LU_EDUCATION,
			LU_INDUSTRIAL,
			LU_SHOPPING,
			LU_MEDICAL,
			LU_MIXED_USE,
			LU_NONE,
			LU_RESIDENTIAL,
			LU_RECREATION,
			LU_SPECIAL_GENERATOR,
			TRANSIT_STOP
		};

	}

	namespace Concepts
	{
		concept struct Is_Activity_Location_Prototype 
		{
			check_template_method_name(has_origin_links, Component_Type::origin_links);
			check_template_method_name(has_destination_links, Component_Type::destination_links);
			check_template_method_name(has_zone, Component_Type::zone); 
			check_template_method_name(has_uuid, Component_Type::uuid); 
			check_template_method_name(has_internal_id, Component_Type::internal_id); 
			define_default_check(has_origin_links && has_destination_links && has_zone && has_uuid && has_internal_id);
		};
		concept struct Is_Activity_Location
		{
			check_template_method_name(has_origin_links, origin_links);
			check_template_method_name(has_destination_links, destination_links);
			check_template_method_name(has_zone, zone); 
			check_template_method_name(has_uuid, uuid); 
			check_template_method_name(has_internal_id, internal_id); 

			check_concept(is_activity_location_prototype, Is_Activity_Location_Prototype, T, V);
			define_sub_check(is_activity_location, has_origin_links && has_destination_links && has_zone && has_uuid && has_internal_id);
			define_default_check(is_activity_location || is_activity_location_prototype);
		};
	}
	
	namespace Prototypes
	{
		prototype struct Activity_Location ADD_DEBUG_INFO
		{
			tag_as_prototype;

			accessor(origin_links, NONE, NONE);
			accessor(destination_links, NONE, NONE);
			accessor(origin_link_choice_cdfs, NONE, NONE);
			accessor(destination_link_choice_cdfs, NONE, NONE);
			accessor(zone, NONE, NONE);
			accessor(census_zone_id, NONE, NONE);
			accessor(uuid, NONE, NONE);
			accessor(internal_id, NONE, NONE);
			accessor(land_use_type, NONE, NONE);

			accessor(x_position, NONE, NONE);
			accessor(y_position, NONE, NONE);
			accessor(z_position, NONE, NONE);

			template<typename TargetType> bool Has_Valid_Origin_Link()
			{
				typedef Link_Components::Prototypes::Link<typename remove_pointer<typename get_type_of(origin_links)::value_type>::type> link_itf;
				typedef Random_Access_Sequence<typename get_type_of(origin_links),link_itf*> links_itf;

				typedef Turn_Movement_Components::Prototypes::Movement<typename remove_pointer<typename link_itf::get_type_of(outbound_turn_movements)::value_type>::type> turn_itf;
				typedef Random_Access_Sequence<typename link_itf::get_type_of(outbound_turn_movements),turn_itf*> turns_itf;

				
				typename links_itf::iterator link_itr = this->origin_links<links_itf*>()->begin();
				if (link_itr != this->origin_links<links_itf*>()->end()	)
				{
					link_itf* link = *link_itr;
					if (link->template outbound_turn_movements<turns_itf*>()->size() > 0) return true;
					else return false;
				}
				else return false;

			}
			template<typename TargetType> bool Has_Valid_Destination_Link()
			{
				typedef Link_Components::Prototypes::Link<typename remove_pointer<typename get_type_of(destination_links)::value_type>::type> link_itf;
				typedef Random_Access_Sequence<typename get_type_of(destination_links),link_itf*> links_itf;

				typedef Turn_Movement_Components::Prototypes::Movement<typename remove_pointer<typename link_itf::get_type_of(outbound_turn_movements)::value_type>::type> turn_itf;
				typedef Random_Access_Sequence<typename link_itf::get_type_of(outbound_turn_movements),turn_itf*> turns_itf;

				
				typename links_itf::iterator link_itr = this->destination_links<links_itf*>()->begin();
				if (link_itr != this->destination_links<links_itf*>()->end()	)
				{
					link_itf* link = *link_itr;
					if (link->template outbound_turn_movements<turns_itf*>()->size() > 0) return true;
					else return false;
				}
				else return false;

			}
			template<typename TargetType> bool Is_Routable_Location()
			{
				// Location is routable only if it has a valid origin/destination link and is not a transit stop
				bool routable_location = Has_Valid_Origin_Link<NT>() /*&& Has_Valid_Destination_Link<NT>()*/ && land_use_type<Types::LAND_USE>() != Types::TRANSIT_STOP && land_use_type<Types::LAND_USE>() != Types::LU_NONE;
				//if (!routable_location) cout << "Warning: location id "<<this->uuid<int>() <<", is not a routable activity location."<<endl;
				return routable_location;
			}
			template<typename TargetType> bool is_work_location()
			{
				if (this->land_use_type<Types::LAND_USE>() == Types::LAND_USE::LU_NONE || this->land_use_type<Types::LAND_USE>() == Types::LU_RESIDENTIAL) return false;
				else return true;
			}
			template<typename TargetType> bool is_school_location()
			{
				if (this->land_use_type<Types::LAND_USE>() == Types::LAND_USE::LU_ALL || this->land_use_type<Types::LAND_USE>() == Types::LU_EDUCATION || this->land_use_type<Types::LAND_USE>() == Types::LU_MIXED_USE) return true;
				else return false;
			}
			template<typename TargetType> bool is_residential_location()
			{
				if (this->land_use_type<Types::LAND_USE>() == Types::LAND_USE::LU_ALL || this->land_use_type<Types::LAND_USE>() == Types::LU_RESIDENTIAL || this->land_use_type<Types::LAND_USE>() == Types::LU_MIXED_USE) return true;
				else return false;
			}
		};
	}
}

using namespace Activity_Location_Components::Prototypes;
