//*********************************************************
//	Antares_Sensor_Implementation.h - Graphical Sensors
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "User_Space\Sensor_Prototype.h"

namespace Sensor_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{

	}
	
	namespace Implementations
	{
		template<typename MasterType,typename ParentType=NULLTYPE,typename InheritanceList=NULLTYPELIST,template<class,class,class> class InheritanceTemplate=NULLTEMPLATE_3>
		struct Antares_Fixed_Sensor : public InheritanceTemplate<MasterType,ParentType,APPEND_CHILD(Antares_Fixed_Sensor)>
		{
			typedef typename InheritanceTemplate<MasterType,NT,APPEND_CHILD(Antares_Fixed_Sensor)>::ComponentType ComponentType;

#pragma pack(push,1)
			struct Link_Line_Segment
			{
				Point_3D<MasterType> a;
				Point_3D<MasterType> b;
			};
#pragma pack(pop)

#pragma pack(push,1)
			struct Link_Line_Group
			{
				void* object;
				True_Color_RGBA<MasterType> color;
				int num_primitives;
				Link_Line_Segment* segments;
			};
#pragma pack(pop)
			
			declare_feature_event_implementation(Sensor_Event)
			{
				InheritanceTemplate<MasterType,ParentType,APPEND_CHILD(Antares_Fixed_Sensor)>::Sensor_Event<ComponentType,CallerType,TargetType>(_this);


				ComponentType* pthis = (ComponentType*)_this;

				

				if( (_iteration + 1) % (((Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()*10) == 0 )
				{
					//int write_some = rand()%100;

					//if(write_some==0)
					//{
					//	cout << "Coloring: " << _iteration << endl;
					//}

					pthis->Color_Sensors();
				}
			}

			feature_implementation static void Initialize_Type(string& name)
			{
				//InheritanceTemplate<MasterType,NT,APPEND_CHILD(Antares_Depot)>::Initialize_Type<ComponentType,CallerType,NT>();

				_its_component_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NT,Antares_Layer<type_of(its_component_layer),ComponentType>*, string& > >(name);

				Antares_Layer_Configuration cfg;
				cfg.Configure_Dynamic_Lines();
				cfg.grouped=true;
				cfg.head_size_value=4;
				cfg.head_accent_size_value=6;
				cfg.selection_callback=&on_select;
				cfg.target_sub_iteration=0;
				cfg.storage_offset=((Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()*10-1;
				cfg.group_color=true;
				cfg.storage_period=((Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()*10;

				cfg.head_color._r = 0;
				cfg.head_color._g = 0;
				cfg.head_color._b = 255;
				cfg.head_color._a = 255;

				_its_component_layer->Initialize<NULLTYPE>(cfg);
			}

			void Color_Sensors()
			{
				if(_detector)
				{
					Link_Line_Segment* segments = new Link_Line_Segment[ 1 ];
				
					Link_Line_Group group;
					group.num_primitives = 1;
					group.segments = segments;
					group.object = (void*)((ComponentType*)this);
				
					if(_outlier_detected)
					{
						group.color._r = 200;
						group.color._g = 0;
						group.color._b = 0;
						group.color._a = 255;
					}
					else
					{
						group.color._r = 0;
						group.color._g = 200;
						group.color._b = 0;
						group.color._a = 255;
					}

					Link_Line_Segment* current_segment = group.segments;


					Link_Prototype<typename type_of(MasterType::link),ComponentType>* link = (Link_Prototype<typename type_of(MasterType::link),ComponentType>*)_covered_link;
				
					Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* intersection;
				
					intersection = link->upstream_intersection< Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* >();
				
					current_segment->a._x = intersection->x_position<float>();
					current_segment->a._y = intersection->y_position<float>();
					current_segment->a._z = 3;

					Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( current_segment->a );

					intersection = link->downstream_intersection< Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* >();

					current_segment->b._x = intersection->x_position<float>();
					current_segment->b._y = intersection->y_position<float>();
					current_segment->b._z = 3;

					Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( current_segment->b );


					_its_component_layer->Push_Element<Regular_Element>(&group);


				


					delete[] segments;
				}
			}

			feature_implementation void Initialize(TargetType configuration)
			{
				InheritanceTemplate<MasterType,NT,APPEND_CHILD(Antares_Fixed_Sensor)>::Initialize<ComponentType,CallerType,TargetType>(configuration);

				//Link_Line_Segment* segments = new Link_Line_Segment[ 1 ];
				//
				//Link_Line_Group group;
				//group.num_primitives = 1;
				//group.segments = segments;
				//group.object = (void*)((ComponentType*)this);
				//group.color._r = 0;
				//group.color._g = 200;
				//group.color._b = 0;
				//group.color._a = 255;

				//Link_Line_Segment* current_segment = group.segments;


				//Link_Prototype<typename type_of(MasterType::link),ComponentType>* link = (Link_Prototype<typename type_of(MasterType::link),ComponentType>*)_covered_link;
				//
				//Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* intersection;
				//
				//intersection = link->upstream_intersection< Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* >();
				//		
				//current_segment->a._x = intersection->x_position<float>();
				//current_segment->a._y = intersection->y_position<float>();
				//current_segment->a._z = 3;

				//Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( current_segment->a );

				//intersection = link->downstream_intersection< Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* >();

				//current_segment->b._x = intersection->x_position<float>();
				//current_segment->b._y = intersection->y_position<float>();
				//current_segment->b._z = 3;

				//Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( current_segment->b );


				//_its_component_layer->Push_Element<Regular_Element>(&group);

				//delete[] segments;
			}

			
			static void on_select(const list<void*>& removed,const list<void*>& added,const list<void*>& selected,vector<pair<string,string>>& bucket)
			{
				if(removed.size())
				{
					_its_component_layer->Clear_Accented<NT>();

					if(selected.size())
					{
						for(list<void*>::const_iterator itr=selected.begin();itr!=selected.end();itr++)
						{
							((ComponentType*)*itr)->Accent_Self<ComponentType,ComponentType,NT>();
						}
					}
				}
				else if(added.size())
				{
					for(list<void*>::const_iterator itr=added.begin();itr!=added.end();itr++)
					{
						((ComponentType*)*itr)->Accent_Self<ComponentType,ComponentType,NT>();
					}
				}
			}

			feature_implementation void Accent_Self()
			{
				Link_Line_Segment* segments = new Link_Line_Segment[ 1 ];
				
				Link_Line_Group group;
				group.num_primitives = 1;
				group.segments = segments;

				Link_Line_Segment* current_segment = group.segments;

				Link_Prototype<typename type_of(MasterType::link),ComponentType>* link = (Link_Prototype<typename type_of(MasterType::link),ComponentType>*)_covered_link;
				
				Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* intersection;
				
				intersection = link->upstream_intersection< Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* >();
				
				current_segment->a._x = intersection->x_position<float>();
				current_segment->a._y = intersection->y_position<float>();
				current_segment->a._z = 3;

				Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( current_segment->a );

				intersection = link->downstream_intersection< Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* >();

				current_segment->b._x = intersection->x_position<float>();
				current_segment->b._y = intersection->y_position<float>();
				current_segment->b._z = 3;

				Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( current_segment->b );
				
				_its_component_layer->Push_Element<Accented_Element>(&group);

				delete[] segments;
			}

			static member_prototype(Antares_Layer,its_component_layer,typename type_of(MasterType::antares_layer),none,none);
		};
		
		template<typename MasterType,typename ParentType,typename InheritanceList,template<class,class,class> class InheritanceTemplate>
		Antares_Layer<typename type_of(MasterType::antares_layer),typename Antares_Fixed_Sensor<MasterType,ParentType,InheritanceList,InheritanceTemplate>::ComponentType>* Antares_Fixed_Sensor<MasterType,ParentType,InheritanceList,InheritanceTemplate>::_its_component_layer;

		implementation struct Antares_Link_Sensor : public Antares_Fixed_Sensor<MasterType,NT,APPEND_CHILD(Antares_Link_Sensor),Link_Sensor>
		{
			feature_implementation static void Initialize_Type()
			{
				Antares_Fixed_Sensor::Initialize_Type<ComponentType,CallerType,NT>(string("Link Sensors"));
			}
		};
	}
}