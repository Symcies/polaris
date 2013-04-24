#pragma once

#include "Activity_Timing_Choice_Prototype.h"



namespace Person_Components
{
	namespace Implementations
	{
		implementation struct Activity_Timing_Chooser_Implementation : public Polaris_Component<APPEND_CHILD(Activity_Timing_Chooser_Implementation),MasterType,Data_Object,ParentType>
		{
			// local types
			typedef pair<Time_Minutes,Time_Minutes> data_type;
			typedef pair<float,data_type> record_type;
			typedef map<float,data_type> map_type;


			// pointer back to parent class
			member_prototype(Prototypes::Person_Planner,Parent_Planner, typename MasterType::person_planner_type, none,none);


			// static start time and duration lookup container for each activity type
			static member_associative_container(concat(hash_map<Activity_Components::Types::ACTIVITY_TYPES, map_type >), start_time_duration_container,none,none);
			

			//=======================================================================================================
			// Time of day and duration choice model
			//-------------------------------------------------------------------------------------------------------
			feature_implementation typename pair<typename TargetType::ReturnType,typename TargetType::ReturnType> Choose_Time_Of_Day_Period(typename TargetType::ParamType activity_ref)
			{
				// returns a pair for start and end times of the selected period
			}


			//=======================================================================================================
			// accessors for drawing random start time and duration for given activity record from static container
			//-------------------------------------------------------------------------------------------------------
			feature_implementation typename TargetType::ReturnType Get_Start_Time(typename TargetType::ParamType activity_ref)
			{
				pair<typename TargetType::ReturnType, typename TargetType::ReturnType> return_val = Get_Start_Time_and_Duration(activity_ref);
				return return_val.first;
			}
			tag_feature_signature_as_available(Get_Start_Time,1);
			feature_implementation typename TargetType::ReturnType Get_Start_Time(typename TargetType::ParamType activity_ref, typename TargetType::Param2Type range_start, typename TargetType::Param2Type range_end)
			{
				pair<typename TargetType::ReturnType, typename TargetType::ReturnType> return_val = Get_Start_Time_and_Duration(activity_ref,range_start,range_end);
				return return_val.first;
			}
			tag_feature_signature_as_available(Get_Start_Time,3);
			feature_implementation typename TargetType::ReturnType Get_Duration(typename TargetType::ParamType activity_ref)
			{
				pair<typename TargetType::ReturnType, typename TargetType::ReturnType> return_val = Get_Start_Time_and_Duration(activity_ref);
				return return_val.second;
			}
			tag_feature_signature_as_available(Get_Duration,1);
			feature_implementation pair<typename TargetType::ReturnType,typename TargetType::ReturnType> Get_Start_Time_and_Duration(typename TargetType::ParamType activity_ref)
			{
				// get interface to activity reference
				Activity_Components::Prototypes::Activity_Planner<strip_modifiers(TargetType::ParamType)::Component_Type>* act;
				act = (Activity_Components::Prototypes::Activity_Planner<strip_modifiers(TargetType::ParamType)::Component_Type>*) activity_ref;
				
				// draw random from uniform distribution
				float rand = GLOBALS::Uniform_RNG.Next_Rand<float>();

				// use upper bound to draw the start time / duration pair with first cumulative probability greater than rand
				map_type::iterator itr = _start_time_duration_container[act->Activity_Type<ACTIVITY_TYPES>()].upper_bound(rand);

				// make sure valid entry is found
				if (itr == _start_time_duration_container[act->Activity_Type<ACTIVITY_TYPES>()].end()) THROW_EXCEPTION("ERROR: no valid start-time / duration pair found for activity type '" << act->Activity_Type<ACTIVITY_TYPES>() <<"' and random value = " << rand);
			
				pair<typename TargetType::ReturnType,typename TargetType::ReturnType> return_val;
				return_val.first = GLOBALS::Time_Converter.Convert_Value<Target_Type<NT,TargetType::ReturnType,Time_Minutes> >(itr->second.first);
				return_val.second = GLOBALS::Time_Converter.Convert_Value<Target_Type<NT,TargetType::ReturnType,Time_Minutes> >(itr->second.second);
				return return_val;
			}
			tag_feature_signature_as_available(Get_Start_Time_and_Duration,1);
			feature_implementation pair<typename TargetType::ReturnType,typename TargetType::ReturnType> Get_Start_Time_and_Duration(typename TargetType::ParamType activity_ref, typename TargetType::Param2Type range_start, typename TargetType::Param2Type range_end)
			{
				assert(range_end > range_start);

				// get interface to activity reference
				Activity_Components::Prototypes::Activity_Planner<strip_modifiers(TargetType::ParamType)::Component_Type>* act;
				act = (Activity_Components::Prototypes::Activity_Planner<strip_modifiers(TargetType::ParamType)::Component_Type>*) activity_ref;

				// convert given range bounds to minutes
				Time_Minutes start = GLOBALS::Time_Converter.Convert_ValueTarget_Type<NT,Time_Minutes,TargetType::Param2Type> >(range_start);
				Time_Minutes end = GLOBALS::Time_Converter.Convert_ValueTarget_Type<NT,Time_Minutes,TargetType::Param2Type> >(range_end);
				
				// draw random from uniform distribution and get corresponding data item - loop while draw is outside of range
				int iter = 0;
				float rand = GLOBALS::Uniform_RNG.Next_Rand<float>();
				map_type::iterator itr = _start_time_duration_container[act->Activity_Type<ACTIVITY_TYPES>()].upper_bound(rand);
				while (itr->second.first < start || itr->second.first >= end)
				{
					rand = GLOBALS::Uniform_RNG.Next_Rand<float>();
					itr = _start_time_duration_container[act->Activity_Type<ACTIVITY_TYPES>()].upper_bound(rand);
					++iter;
					if (iter >= 100)
					{
						THROW_WARNING("Warning, could not find a start time value within given range: random start time has been selected.");
						break;
					}
				}

				// make sure valid entry is found
				if (itr == _start_time_duration_container[act->Activity_Type<ACTIVITY_TYPES>()].end()) THROW_EXCEPTION("ERROR: no valid start-time / duration pair found for activity type '" << act->Activity_Type<ACTIVITY_TYPES>() <<"' and random value = " << rand);
			
				pair<typename TargetType::ReturnType,typename TargetType::ReturnType> return_val;
				return_val.first = GLOBALS::Time_Converter.Convert_Value<Target_Type<NT,TargetType::ReturnType,Time_Minutes> >(itr->second.first);
				return_val.second = GLOBALS::Time_Converter.Convert_Value<Target_Type<NT,TargetType::ReturnType,Time_Minutes> >(itr->second.second);
				return return_val;
			}
			tag_feature_signature_as_available(Get_Start_Time_and_Duration,3);


			// Static initialization
			static member_data(bool, is_initialized,none,none);
			static void static_initializer(string filename)
			{
				// make sure this is only run once
				if (_is_initialized) return;
				_is_initialized = true;


				// Initialize hashmap
				_start_time_duration_container.insert(pair<ACTIVITY_TYPES,map_type>(ACTIVITY_TYPES::EAT_OUT_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<ACTIVITY_TYPES,map_type>(ACTIVITY_TYPES::ERRANDS_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<ACTIVITY_TYPES,map_type>(ACTIVITY_TYPES::HEALTHCARE_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<ACTIVITY_TYPES,map_type>(ACTIVITY_TYPES::LEISURE_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<ACTIVITY_TYPES,map_type>(ACTIVITY_TYPES::MAJOR_SHOPPING_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<ACTIVITY_TYPES,map_type>(ACTIVITY_TYPES::OTHER_SHOPPING_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<ACTIVITY_TYPES,map_type>(ACTIVITY_TYPES::PERSONAL_BUSINESS_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<ACTIVITY_TYPES,map_type>(ACTIVITY_TYPES::PICK_UP_OR_DROP_OFF_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<ACTIVITY_TYPES,map_type>(ACTIVITY_TYPES::RELIGIOUS_OR_CIVIC_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<ACTIVITY_TYPES,map_type>(ACTIVITY_TYPES::SERVICE_VEHICLE_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<ACTIVITY_TYPES,map_type>(ACTIVITY_TYPES::SOCIAL_ACTIVITY,map_type()));

				// add items
				ifstream data_file;
				data_file.open(filename.c_str(), ios::in);
				if (!data_file.is_open()) cout << endl << "Could not open 'start_time_duration_data.txt' in the working directory.  Check to make sure the start-time duration file exists.";

				string line;
				int act_type;
				float prob, start, dur;

				getline(data_file,line);

				while(data_file >> act_type >> start >> dur >> prob )
				{
					_start_time_duration_container[(ACTIVITY_TYPES)act_type].insert(record_type(prob,data_type(start,dur)));
				}
			}
		};

		static_member_initialization(Activity_Timing_Chooser_Implementation,is_initialized,false);
		static_member_definition(Activity_Timing_Chooser_Implementation, start_time_duration_container);
	}
}