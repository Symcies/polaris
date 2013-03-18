//*********************************************************
//	Graphical_Vehicle_Implementation.h - Graphical Vehicle Variant
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "User_Space\Vehicle_Prototype.h"

namespace Vehicle_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{

	}
	
	namespace Implementations
	{
		implementation struct Graphical_Vehicle_Implementation:public Polaris_Component<APPEND_CHILD(Graphical_Vehicle_Implementation),MasterType,Execution_Object,ParentType>
		{
			static vector<Point_2D<MasterType>> num_vehicles_cache;
			static vector<Point_2D<MasterType>> num_vehicles_cache_B;

#pragma pack(push,1)
			struct Plot_Element
			{
				int num_primitives;
				Point_2D<MasterType>* points;
			};
#pragma pack(pop)
			
			static void Initialize_Layer()
			{
				_vehicle_points=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NULLTYPE,Antares_Layer<type_of(vehicle_points),Graphical_Vehicle_Implementation>*, string& > >(string("Vehicles"));
				Antares_Layer_Configuration cfg;
				cfg.Configure_Points();

				cfg.attributes_schema = string("ID,Status,Current_Link");
				
				cfg.attributes_callback = (attributes_callback_type)&fetch_attributes;
				cfg.submission_callback = (attributes_callback_type)&submit_attributes;

				_vehicle_points->Initialize<NULLTYPE>(cfg);

				//// configure plot layer
				//_num_vehicles=_information_panel->Allocate_New_Layer< Target_Type< NULLTYPE,Antares_Layer<type_of(num_vehicles),Graphical_Network_Implementation>*, string& > >(string("Number of Vehicles"));
				//Antares_Layer_Configuration pcfg;
				//pcfg.Configure_Plot();
				//_num_vehicles->Initialize<NULLTYPE>(pcfg);


				//_num_vehicles_B=_information_panel->Allocate_New_Layer< Target_Type< NULLTYPE,Antares_Layer<type_of(num_vehicles_B),Graphical_Network_Implementation>*, string& > >(string("Number of Vehicles Green"));
				//pcfg.head_color._r=0;
				//pcfg.head_color._g=255;
				//_num_vehicles_B->Initialize<NULLTYPE>(pcfg);
			}


			feature_implementation void submit_num_vehicles()
			{
				Point_2D<MasterType> submission;
				submission._x=_iteration;
				submission._y=_vehicles_counter;

				vehicle_points.push_back(submission);

				submission._y=_vehicles_counter/2;
				
				vehicle_points_B.push_back(submission);

				Plot_Element element;

				element.num_primitives = vehicle_points.size();
				element.points = &vehicle_points.front();

				_num_vehicles->Push_Element<Regular_Element>((void*)&element);
				

				element.points = &vehicle_points.front();
				
				_num_vehicles_B->Push_Element<Regular_Element>((void*)&element);

				_vehicles_counter=0;
			}

			feature_implementation void accent_num_vehicles()
			{
				Plot_Element element;

				element.num_primitives = vehicle_points_B.size();
				element.points = &vehicle_points_B.front();

				_num_vehicles->Clear_Accented<NT>();

				_num_vehicles->Push_Element<Accented_Element>((void*)&element);
			}
			
			feature_implementation void accept_vehicle_coordinates()
			{
				_vehicles_counter++;
			}
			
			member_prototype(Antares_Layer,num_vehicles,typename type_of(MasterType::antares_layer),none,none);
			member_prototype(Antares_Layer,num_vehicles_B,typename type_of(MasterType::antares_layer),none,none);

			member_data(volatile int,vehicles_counter,none,none);

			
			
			
			member_data(Vehicle_Components::Types::Vehicle_Status_Keys, simulation_status, none, none);
			member_data(int, uuid, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, internal_id, none, none);

			member_prototype(Movement_Plan_Prototype, movement_plan, typename MasterType::movement_plan_type, none, none);

#ifndef EXCLUDE_DEMAND
			member_component(typename MasterType::person_type, traveler, none, none);
#else
			member_component(typename MasterType::traveler_type, traveler, none, none);
#endif

			define_component_interface(Link_Interface,typename type_of(MasterType::link),Link_Prototype,Graphical_Vehicle_Implementation);
			define_component_interface(Intersection_Interface,typename type_of(MasterType::intersection),Intersection_Prototype,Graphical_Vehicle_Implementation);

			declare_feature_conditional(compute_vehicle_position_condition)
			{
				response.result=true;
				response.next._iteration=_iteration+1;
				response.next._sub_iteration=Scenario_Components::Types::END_OF_ITERATION;
			}

			declare_feature_event(compute_vehicle_position)
			{
				Graphical_Vehicle_Implementation* pthis=(Graphical_Vehicle_Implementation*)_this;

				if(pthis->simulation_status<ComponentType,ComponentType,Types::Vehicle_Status_Keys>() != Types::Vehicle_Status_Keys::IN_NETWORK) return;

				if(pthis->_movement_plan == NULL)
				{
					cout << "verified!" << endl;
					return;
				}
				Link_Interface* link=pthis->_movement_plan->current_link<Link_Interface*>();

				if(link == nullptr)
				{
					cout << "verified!" << endl;
					return;
				}

				Intersection_Interface* upstream_intersection=link->upstream_intersection<Intersection_Interface*>();
				
				Canvas<typename MasterType::type_of(canvas),Graphical_Vehicle_Implementation>* my_canvas=((Canvas<typename MasterType::type_of(canvas),Graphical_Vehicle_Implementation>*)canvas);

#pragma pack(push,1)
				struct attribute_coordinate
				{
					void* ptr;
					Point_3D<MasterType> vertex;
				} coordinate;
#pragma pack(pop)

				coordinate.ptr = _this;

				coordinate.vertex._x=upstream_intersection->x_position<float>();
				coordinate.vertex._y=upstream_intersection->y_position<float>();
				coordinate.vertex._z=1;
				
				Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>(coordinate.vertex);

				_vehicle_points->Push_Element<Regular_Element>(&coordinate);

				pthis->_vehicles_counter++;
			}

			feature_implementation void load(requires(check_2(TargetType,Types::Load_To_Entry_Queue,is_same)))
			{
				_simulation_status = Types::Vehicle_Status_Keys::IN_ENTRY_QUEUE;
			}

			feature_implementation void load(requires(check_2(TargetType,Types::Load_To_Origin_Link,is_same)))
			{
				typedef Movement_Plan_Components::Prototypes::Movement_Plan_Prototype<typename MasterType::movement_plan_type,ComponentType> _Movement_Plan_Interface;
				_simulation_status =  Types::Vehicle_Status_Keys::IN_NETWORK;
				((_Movement_Plan_Interface*)_movement_plan)->template initialize_trajectory<NULLTYPE>();

				Load_Register<Graphical_Vehicle_Implementation>(&compute_vehicle_position_condition<NULLTYPE>,&compute_vehicle_position<NULLTYPE>,_iteration+1,Scenario_Components::Types::END_OF_ITERATION);
			}

			feature_implementation void load(requires(!check_2(TargetType,Types::Load_To_Origin_Link,is_same) && !check_2(TargetType,Types::Load_To_Entry_Queue,is_same)))
			{
				static_assert(false,"Unrecognized load type!");
			}

			feature_implementation void unload()
			{
				_simulation_status = Types::Vehicle_Status_Keys::OUT_NETWORK;
			}


			static bool fetch_attributes(Graphical_Vehicle_Implementation* _this,vector<string>& bucket)
			{
				//_this->_graphical_network->accent_num_vehicles<NT>();

				stringstream s;
				
				s << _this->_internal_id;
				bucket.push_back(s.str());
				s.str("");
				
				bucket.push_back(string("IN_NETWORK"));
				
				s << _this->_movement_plan->current_link<Link_Prototype<typename MasterType::type_of(link)>*>()->uuid<int>();
				bucket.push_back(s.str());

				return true;
			}
			
			static bool submit_attributes(Graphical_Vehicle_Implementation* _this,vector<string>& bucket)
			{
				vector<string>::iterator itr;

				int new_id=atoi(bucket[0].c_str());

				if(new_id%2==0)
				{
					_this->_internal_id = new_id;

					return true;
				}
				else
				{
					return false;
				}
			}

			static member_prototype(Antares_Layer,vehicle_points,typename type_of(MasterType::antares_layer),none,none);
		};

		template<typename MasterType,typename ParentType,typename InheritanceList>
		Antares_Layer<typename MasterType::type_of(antares_layer),Graphical_Vehicle_Implementation<MasterType,ParentType,InheritanceList>>* Graphical_Vehicle_Implementation<MasterType,ParentType,InheritanceList>::_vehicle_points;
	}

}

using namespace Vehicle_Components::Implementations;