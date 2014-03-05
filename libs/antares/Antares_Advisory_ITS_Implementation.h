//*********************************************************
//	Antares_Advisory_ITS_Implementation.h - Graphical Adv ITS
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "Antares_Link_Implementation.h"

namespace Advisory_ITS_Components
{
	namespace Types{}

	namespace Concepts{}
	
	namespace Implementations
	{
		template<typename MasterType,typename InheritanceList=NULLTYPELIST,template<class,class> class InheritanceTemplate=NULLTEMPLATE_2>
		struct Antares_Advisory_ITS : public InheritanceTemplate<MasterType,INHERIT(Antares_Advisory_ITS)>
		{
			typedef typename InheritanceTemplate<MasterType,INHERIT(Antares_Advisory_ITS)>::ComponentType ComponentType;

			typedef Link<typename type_of(MasterType::link)> Link_Interface;
			typedef Intersection<typename type_of(MasterType::intersection)> Intersection_Interface;

			typedef Link_Components::Implementations::Link_Line<MasterType> Link_Line;

//#pragma pack(push,1)
//			struct Link_Line_Segment
//			{
//				Point_3D<MasterType> a;
//				Point_3D<MasterType> b;
//			};
//#pragma pack(pop)
//
//#pragma pack(push,1)
//			struct Link_Line_Group
//			{
//				int num_primitives;
//				Link_Line_Segment* segments;
//			};
//#pragma pack(pop)

#pragma pack(push,1)
			struct ITS_Location
			{
				void* object;
				Point_3D<MasterType> position;
			};
#pragma pack(pop)

			template<typename TargetType> void Initialize(TargetType configuration)
			{
				InheritanceTemplate<MasterType,INHERIT(Antares_Advisory_ITS)>::Initialize<TargetType>(configuration);

				if(_covered_links.size())
				{
					float xmax = - (FLT_MAX/2.0);
					float ymax = - (FLT_MAX/2.0);
					float xmin = (FLT_MAX/2.0);
					float ymin = (FLT_MAX/2.0);

					for(boost::container::vector<Link_Interface*>::iterator itr = _covered_links.begin(); itr != _covered_links.end(); itr++)
					{
						Link_Interface* link = *itr;

						Intersection_Interface* intersection;

						intersection = link->upstream_intersection<Intersection_Interface*>();

						if(intersection->x_position<float>() < xmin)
						{
							xmin = intersection->x_position<float>();
						}
						
						if(intersection->x_position<float>() > xmax)
						{
							xmax = intersection->x_position<float>();
						}

						if(intersection->y_position<float>() < ymin)
						{
							ymin = intersection->y_position<float>();
						}
						
						if(intersection->y_position<float>() > ymax)
						{
							ymax = intersection->y_position<float>();
						}

						
						
						intersection = link->downstream_intersection<Intersection_Interface*>();

						if(intersection->x_position<float>() < xmin)
						{
							xmin = intersection->x_position<float>();
						}
						
						if(intersection->x_position<float>() > xmax)
						{
							xmax = intersection->x_position<float>();
						}

						if(intersection->y_position<float>() < ymin)
						{
							ymin = intersection->y_position<float>();
						}
						
						if(intersection->y_position<float>() > ymax)
						{
							ymax = intersection->y_position<float>();
						}
					}

					ITS_Location its_location;

					its_location.object = (void*)((ComponentType*)this);

					its_location.position._x = (xmax + xmin)/2.0f;
					its_location.position._y = (ymax + ymin)/2.0f;
					its_location.position._z = 5;
					
//TODO
//					Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( its_location.position );

					_its_component_layer->Push_Element<Regular_Element>(&its_location);

					//Link_Line_Segment* segments = new Link_Line_Segment[ _covered_links.size() ];
					//
					//Link_Line_Group group;
					//group.num_primitives = _covered_links.size();
					//group.segments = segments;
					//group.object = (void*)((ComponentType*)this);

					//Link_Line_Segment* current_segment = group.segments;

					//for(boost::container::vector<Link<typename type_of(MasterType::link)>*>::iterator itr = _covered_links.begin(); itr != _covered_links.end(); itr++)
					//{
					//	Link<typename type_of(MasterType::link)>* link = (Link<typename type_of(MasterType::link)>*)(*itr);
					//	
					//	Intersection<typename type_of(MasterType::intersection)>* intersection;
					//	
					//	intersection = link->upstream_intersection< Intersection<typename type_of(MasterType::intersection)>* >();
					//	
					//	current_segment->a._x = intersection->x_position<float>();
					//	current_segment->a._y = intersection->y_position<float>();
					//	current_segment->a._z = 3;

//TODO
//					//	Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( current_segment->a );

					//	intersection = link->downstream_intersection< Intersection<typename type_of(MasterType::intersection)>* >();

					//	current_segment->b._x = intersection->x_position<float>();
					//	current_segment->b._y = intersection->y_position<float>();
					//	current_segment->b._z = 3;

//TODO
//					//	Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( current_segment->b );

					//	++current_segment;
					//}
					//
					//_its_component_layer->Push_Element<Regular_Element>(&group);

					//delete[] segments;
				}
			}

			static bool on_submit(const boost::container::list<void*>& selected,const boost::container::vector<string>& attribute_choices,const boost::container::vector<string>& dropdown_choices)
			{
				string user_event_choice = dropdown_choices[0];
				bool update_successful = false;

				for(boost::container::list<void*>::const_iterator sitr=selected.begin();sitr!=selected.end();sitr++)
				{
					ComponentType* its_component=(ComponentType*) (*sitr);

					for(boost::container::vector< Network_Event< typename type_of(MasterType::base_network_event) >* >::iterator itr=its_component->_current_events.begin();itr!=its_component->_current_events.end();itr++)
					{
						if(user_event_choice == (*itr)->notes<string&>())
						{
							its_component->_displayed_events.clear();
							its_component->_displayed_events.push_back( *itr );

							update_successful = true;
						}
					}
				}

				if(update_successful) return true;
				else return false;
			}

			static void on_double_click(const boost::container::list<void*>& selected,boost::container::vector<pair<string,string>>& attributes,boost::container::vector<boost::container::vector<string>>& dropdowns)
			{
				ComponentType* its_component=(ComponentType*)selected.back();

				dropdowns.resize(1);

				for(boost::container::vector< Network_Event< typename type_of(MasterType::base_network_event) >* >::iterator itr=its_component->_current_events.begin();itr!=its_component->_current_events.end();itr++)
				{
					dropdowns[0].push_back( (*itr)->notes<string&>() );
				}
			}

			static void on_select(const boost::container::list<void*>& removed,const boost::container::list<void*>& added,const boost::container::list<void*>& selected,boost::container::vector<pair<string,string>>& bucket)
			{
				if(removed.size())
				{
					_its_component_layer->Clear_Accented<NT>();
					MasterType::network_type::_link_lines->Clear_Accented<NT>();

					if(selected.size())
					{
						for(boost::container::list<void*>::const_iterator itr=selected.begin();itr!=selected.end();itr++)
						{
							((ComponentType*)*itr)->Accent_Self<NT>();
						}
					}
				}
				else if(added.size())
				{
					for(boost::container::list<void*>::const_iterator itr=added.begin();itr!=added.end();itr++)
					{
						((ComponentType*)*itr)->Accent_Self<NT>();
					}
				}

				if(selected.size())
				{
					((ComponentType*) (selected.back()))->Display_Attributes<NT>(bucket);
				}
			}
			
			template<typename TargetType> void Accept_Displayed_Network_Events(boost::container::vector<Network_Event_Components::Prototypes::Network_Event<typename type_of(MasterType::base_network_event)>*>& network_events)
			{
			}

			template<typename TargetType> void Display_Attributes(boost::container::vector<pair<string,string>>& bucket)
			{
				pair<string,string> key_value_pair;
				
				key_value_pair.first="Displayed";

				if(_displayed_events.size())
				{
					key_value_pair.second=_displayed_events[0]->notes<string&>();
				}
				else
				{
					key_value_pair.second="None Displayed";
				}
				
				bucket.push_back(key_value_pair);
			}

			template<typename TargetType> void Accent_Self()
			{
				float xmax = - (FLT_MAX/2.0);
				float ymax = - (FLT_MAX/2.0);
				float xmin = (FLT_MAX/2.0);
				float ymin = (FLT_MAX/2.0);

				for(boost::container::vector<Link_Interface*>::iterator itr = _covered_links.begin(); itr != _covered_links.end(); itr++)
				{
					Link_Interface* link = *itr;

					Intersection_Interface* intersection;

					intersection = link->upstream_intersection<Intersection_Interface*>();

					if(intersection->x_position<float>() < xmin)
					{
						xmin = intersection->x_position<float>();
					}
						
					if(intersection->x_position<float>() > xmax)
					{
						xmax = intersection->x_position<float>();
					}

					if(intersection->y_position<float>() < ymin)
					{
						ymin = intersection->y_position<float>();
					}
						
					if(intersection->y_position<float>() > ymax)
					{
						ymax = intersection->y_position<float>();
					}

						
						
					intersection = link->downstream_intersection<Intersection_Interface*>();

					if(intersection->x_position<float>() < xmin)
					{
						xmin = intersection->x_position<float>();
					}
						
					if(intersection->x_position<float>() > xmax)
					{
						xmax = intersection->x_position<float>();
					}

					if(intersection->y_position<float>() < ymin)
					{
						ymin = intersection->y_position<float>();
					}
						
					if(intersection->y_position<float>() > ymax)
					{
						ymax = intersection->y_position<float>();
					}
				}

				ITS_Location its_location;

				its_location.object = (void*)((ComponentType*)this);

				its_location.position._x = (xmax + xmin)/2.0f;
				its_location.position._y = (ymax + ymin)/2.0f;
				its_location.position._z = 5;
					
//TODO
//				Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( its_location.position );

				_its_component_layer->Push_Element<Accented_Element>(&its_location);






				//Link_Line_Segment* segments = new Link_Line_Segment[ _covered_links.size() ];
				
				Link_Line link_line;

				link_line.color._r = 255;
				link_line.color._g = 50;
				link_line.color._b = 50;
				link_line.color._a = 200;

				link_line.data = nullptr;

				//Link_Line_Group group;
				//group.num_primitives = _covered_links.size();
				//group.segments = segments;

				//Link_Line_Segment* current_segment = group.segments;

				for(boost::container::vector<Link_Interface*>::iterator itr = _covered_links.begin(); itr != _covered_links.end(); itr++)
				{
					Link_Interface* link = (*itr);
					
					Intersection_Interface* intersection;
					
					intersection = link->upstream_intersection< Intersection_Interface* >();
					
					link_line.up_node._x = intersection->x_position<float>();
					link_line.up_node._y = intersection->y_position<float>();
					link_line.up_node._z = 3;

//TODO
//					Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( link_line.up_node );

					intersection = link->downstream_intersection< Intersection_Interface* >();

					link_line.down_node._x = intersection->x_position<float>();
					link_line.down_node._y = intersection->y_position<float>();
					link_line.down_node._z = 3;

//TODO
//					Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( link_line.down_node );

					MasterType::network_type::_link_lines->Push_Element<Accented_Element>(&link_line);
				}
				
				

				//delete[] segments;
			}

			static m_prototype(Antares_Layer<typename type_of(MasterType::antares_layer)>,its_component_layer, NONE, NONE);
			//static m_prototype(Antares_Layer<typename type_of(MasterType::antares_layer)>,its_coverage_layer, NONE, NONE);
		};
		
		template<typename MasterType,typename InheritanceList,template<class,class> class InheritanceTemplate>
		Antares_Layer<typename type_of(MasterType::antares_layer)>* Antares_Advisory_ITS<MasterType,InheritanceList,InheritanceTemplate>::_its_component_layer;
		
		//template<typename MasterType,typename InheritanceList,template<class,class,class> class InheritanceTemplate>
		//Antares_Layer<typename type_of(MasterType::antares_layer),typename Antares_Advisory_ITS<MasterType,InheritanceList,InheritanceTemplate>::ComponentType>* Antares_Advisory_ITS<MasterType,InheritanceList,InheritanceTemplate>::_its_coverage_layer;

	}
}
