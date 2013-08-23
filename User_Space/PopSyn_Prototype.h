#pragma once

#include "Person_Prototype.h"
#include "Household_Prototype.h"
#include "User_Space_Includes.h"
#include "linker.h"
//#include "Synthesis_Region_Implementation.h"
//#include "Network_Prototype.h"
//#include "Scenario_Prototype.h"

namespace PopSyn
{
	namespace Types
	{

	} using namespace Types;

	namespace Concepts
	{
		concept struct Uses_Linker_File
		{
			check_named_member(Has_Use_Linker_File_Defined, _linker_file_path);
			define_default_check(Has_Use_Linker_File_Defined);
		};
	}

	namespace Prototypes
	{
		prototype struct Population_Synthesizer_Prototype ADD_DEBUG_INFO
		{
			tag_as_prototype; // Declare class as a polaris prototype

			//----------------------------------------------------------------
			// Schedules the first event from above
			feature_prototype void Initialize(typename TargetType::ParamType network, typename TargetType::Param2Type scenario, requires(check(TargetType,Is_Target_Type_Struct) && check(typename TargetType::ParamType, Network_Components::Concepts::Is_Transportation_Network) && check(typename TargetType::Param2Type, Scenario_Components::Concepts::Has_Popsyn_Configuration_Data)))
			{
				// Allocate IPF Solver and get Settings from scenario reference
				define_component_interface(solver_itf,typename get_type_of(Solution_Settings),PopSyn::Prototypes::Solver_Settings_Prototype,NULLTYPE);
				solver_itf* solver = (solver_itf*)Allocate<typename get_type_of(Solution_Settings)>();
				solver->template Initialize<Target_Type<NULLTYPE,void,double,int>>(scenario->template ipf_tolerance<double>(),scenario->template percent_to_synthesize<double>(),scenario->template maximum_iterations<int>());

				// get output control params from scenario
				this->write_marginal_output_flag<bool>(scenario->template write_marginal_output<bool>());
				this->write_full_output_flag<bool>(scenario->template write_full_output<bool>());
				this->linker_file_path<string>(scenario->template popsyn_control_file_name<string>());

				// Add references to other objects to the population synthesizer
				this->Solution_Settings<solver_itf*>(solver);
				this->network_reference<typename TargetType::ParamType>(network);
				this->scenario_reference<typename TargetType::Param2Type>(scenario);

				// set up output files
				stringstream pop_filename("");
				pop_filename << scenario->template output_dir_name<string>();
				pop_filename << "full_population.csv";
				this->Output_Stream<ofstream&>().open(pop_filename.str(),ios_base::out);

				stringstream log_filename("");
				log_filename << scenario->template output_dir_name<string>();
				log_filename << "popsyn_log.csv";
				this->Log_File<ofstream&>().open(log_filename.str(),ios_base::out);

				stringstream marg_filename("");
				marg_filename << scenario->template output_dir_name<string>();
				marg_filename << "marginals_and_distributions.csv";
				this->Log_File<ofstream&>().open(marg_filename.str(),ios_base::out);		
		

				this_component()->template Initialize<ComponentType,CallerType,TargetType>();

				load_event(ComponentType,Start_Popsyn_Conditional,Start_Popsyn_Event,POPSYN_ITERATIONS::MAIN_INITIALIZE,POPSYN_SUBITERATIONS::INITIALIZE,NULLTYPE);
			}
			feature_prototype void Initialize(typename TargetType::ParamType network, typename TargetType::Param2Type scenario, requires(!check(TargetType,Is_Target_Type_Struct) || !check(typename TargetType::ParamType, Network_Components::Concepts::Is_Transportation_Network) || !check(typename TargetType::Param2Type, Scenario_Components::Concepts::Has_Popsyn_Configuration_Data)))
			{
				assert_check(TargetType, Is_Target_Type_Struct,"Error, the TargetType must be passed as a Target_Type struct to this function.");
				assert_check(typename TargetType::ParamType, Network_Components::Concepts::Is_Transportation_Network, "Error, the specified TargetType ParamType is not a valid Transportation network.");
				assert_check(typename TargetType::Param2Type, Scenario_Components::Concepts::Has_Popsyn_Configuration_Data, "Error, the specified TargetType Param2Type is not a valid Scenario reference.");
			}
			
			//----------------------------------------------------------------
			// Main analysis loop events, used to separate operations into different timesteps
			//----------------------------------------------------------------
			// 1.) Startup Event - Reads inputs and allocates analysis objects (at Iteration = 0, timestep 1)
			declare_feature_conditional(Start_Popsyn_Conditional)
			{
				ComponentType* pthis = (ComponentType*)_this;
				if (_iteration == 0 || _iteration == 1)
				{
					switch (_sub_iteration)
					{
					case POPSYN_SUBITERATIONS::INITIALIZE:
						response.result = true;
						response.next._iteration = _iteration;
						response.next._sub_iteration = POPSYN_SUBITERATIONS::START_TIMING;
						break;
					case POPSYN_SUBITERATIONS::START_TIMING:
						response.result = true;
						pthis->Swap_Event(&Start_Main_Timer<NULLTYPE>);
						response.next._iteration = POPSYN_ITERATIONS::MAIN_PROCESS;
						response.next._sub_iteration = POPSYN_SUBITERATIONS::STOP_TIMING;
						break;
					case POPSYN_SUBITERATIONS::STOP_TIMING:
						response.result = true;
						pthis->Swap_Event(&Stop_Main_Timer<NULLTYPE>);
						response.next._iteration = _iteration;
						response.next._sub_iteration = POPSYN_SUBITERATIONS::OUTPUT;
						break;
					case POPSYN_SUBITERATIONS::OUTPUT:
						response.result = true;
						pthis->Swap_Event(&Output_Popsyn_Event<NULLTYPE>);
						response.next._iteration = 3;
						response.next._sub_iteration = 0;
						break;
					default:
						response.result = false;
						response.next._iteration = END;
						response.next._sub_iteration = 0;
					}
				}
				else if (_iteration == 3)
				{
					response.result = true;
					pthis->Swap_Event(&Output_Popsyn_To_DB_Event<NULLTYPE>);
					response.next._iteration = END;
					response.next._sub_iteration = 0;
				}
				else
				{
					response.result = false;
					response.next._iteration = END;
					response.next._sub_iteration = 0;
				}
				
				
			}
			declare_feature_event(Start_Popsyn_Event)
			{
				((Population_Synthesizer_Prototype<ComponentType,CallerType>*) _this)->template Start_Popsyn<NULLTYPE>();
			}
			feature_prototype bool Start_Popsyn(requires(check(ComponentType,Concepts::Uses_Linker_File)))
			{
				//------------------------
				// TIMER
				cout<<endl<<"====================================================="<<endl<<"Starting synthetic population generation:"<<endl;
				Counter timer;
				timer.Start();
				//------------------------

				int ndim, ans;
				Linker linker = Linker();
				File_IO::File_Writer fw, fw_sample;
				ofstream& out = this->Output_Stream<ofstream&>();
				
				//===============================================================================================================
				// Initialize settings
				//---------------------------------------------------------------------------------------------------------------
				// CREATE RNG for later use
				define_component_interface(Rand_Interface,typename ComponentType::Master_Type::RNG,RNG_Prototype,NULLTYPE);
				Rand_Interface* rand = (Rand_Interface*)Allocate<typename ComponentType::Master_Type::RNG>();
				rand->template Initialize<double>(0.0);

				// IPF Solver Settings
				define_component_interface(solver_itf,typename get_type_of(Solution_Settings),PopSyn::Prototypes::Solver_Settings_Prototype,ComponentType);
				solver_itf* solver = this->Solution_Settings<solver_itf*>();

				//===============================================================================================================
				// Initialize file linker
				//---------------------------------------------------------------------------------------------------------------
				vector<int> dims;
				linker.Init_Linker(dims, this->linker_file_path<string>().c_str());
				ndim = (int)dims.size();

				// Define iterators and get pointer to the region collection
				typedef typename get_type_of(Synthesis_Regions_Collection)				region_collection_type;
				typedef typename region_collection_type::unqualified_value_type			region_type;
				typedef typename region_type::Sample_Data_type							sample_collection_type;
				typedef typename sample_collection_type::unqualified_value_type			sample_type;
				typedef typename region_type::Temporary_Sample_Data_type				temporary_sample_collection_type;
				typedef typename region_type::Synthesis_Zone_Collection_type			zone_collection_type;
				typedef typename zone_collection_type::unqualified_value_type			zone_type;
				typedef typename region_type::get_type_of(Target_Joint_Distribution)	joint_dist_type;
				typedef typename region_type::get_type_of(Target_Marginal_Distribution)	marg_dist_type;

				define_container_and_value_interface_unqualified_container(regions_itf,region_itf,region_collection_type,Associative_Container_Prototype,PopSyn::Prototypes::Synthesis_Region_Prototype,ComponentType);
				define_container_and_value_interface_unqualified_container(zones_itf,zone_itf,zone_collection_type,Associative_Container_Prototype,PopSyn::Prototypes::Synthesis_Zone_Prototype,ComponentType);
				define_container_and_value_interface_unqualified_container(sample_data_itf,pop_unit_itf,sample_collection_type,Associative_Container_Prototype,Household_Components::Prototypes::Household_Properties,ComponentType);
				define_container_and_value_interface_unqualified_container(temp_sample_data_itf,temp_pop_unit_itf,temporary_sample_collection_type,Associative_Container_Prototype,Household_Components::Prototypes::Household_Properties,ComponentType);
				define_container_and_value_interface_unqualified_container(person_sample_data_itf,person_unit_itf,typename pop_unit_itf::get_type_of(Persons_Container),Containers::Random_Access_Sequence_Prototype,Person_Components::Prototypes::Person_Properties,ComponentType);
				define_simple_container_interface(joint_itf,joint_dist_type,Multidimensional_Random_Access_Array_Prototype, typename joint_dist_type::unqualified_value_type ,NULLTYPE);
				define_simple_container_interface(marginal_itf,marg_dist_type,Multidimensional_Random_Access_Array_Prototype, typename marg_dist_type::unqualified_value_type ,NULLTYPE);
	
				
				regions_itf* regions = this->Synthesis_Regions_Collection<regions_itf*>();
				typename regions_itf::iterator region_itr;
				this->Synthesis_Regions_Collection<regions_itf*>(regions);

				typename joint_itf::index_type dimensions;
				typename joint_itf::index_type index;
				for (vector<int>::iterator i = dims.begin(); i != dims.end(); ++i) 
				{
					dimensions.push_back(*i);
					index.push_back(0);
				}


				//===============================================================================================================
				// read region household file, fill sample data
				//---------------------------------------------------------------------------------------------------------------
				File_IO::File_Reader fr;
				if (!fr.Open(*linker.Sample_File_Path())) 
				{
					cout<<endl<<"Could not open region sample file path. Press any key."<<endl; cin>>ans; return EXIT_FAILURE;
				}

				// references to the various region components
				region_itf* new_region;
				joint_itf* dist;
				marginal_itf* marg,  *regional_marg;
				sample_data_itf* sample;
				temp_sample_data_itf* temp_sample;

				while(fr.Read())
				{
					typename regions_itf::key_type ID;
		
					// read the region data from input file
					if(!fr.Get_Data<typename regions_itf::key_type>(ID,linker.region_id_col)) break; 

					if ((region_itr = regions->find(ID)) == regions->end())
					{
						// create new region
						new_region = (region_itf*)Allocate<region_type>();
						new_region->template Initialize<NULLTYPE>();
						new_region->parent_reference(this);
						dist = new_region->template Target_Joint_Distribution<joint_itf*>();
						marg = new_region->template Target_Marginal_Distribution<marginal_itf*>();
						Rand_Interface* my_rand = (Rand_Interface*)Allocate<typename zone_itf::get_type_of(Rand)>(); // ALLOCATION TEST
						my_rand->template Initialize<double>(ID);
						new_region->template Output_Stream<ostream&>(out);
						((zone_itf*)new_region)->template Rand<Rand_Interface*>(my_rand);

						//-----------------------------------------------------------------------------------------
						// Initialize the distribution and marginals
						dist->resize(dimensions,0.0);
						marg->resize(dimensions,0.0);

						new_region->template ID<int>(ID);
						solver_itf* region_solver = (solver_itf*)Allocate<typename get_type_of(Solution_Settings)>(); // ALLOCATION TEST

						region_solver->template Initialize<Target_Type<NULLTYPE,void,double,int>>(solver->template Tolerance<double>(),solver->template Percentage_to_synthesize<double>(),solver->template Iterations<int>());
						new_region->template Solver_Settings<solver_itf*>(region_solver);

						// add new region to the list
						pair<typename regions_itf::key_type, region_itf*> item = pair<typename regions_itf::key_type, region_itf*>(ID, new_region);
						//regions->insert(pair<typename regions_itf::key_type, region_itf*>(ID, new_region));
						regions->insert(item);
					}
					else
					{
						new_region = region_itr->second;
					}
					// next add the sample data
					dist = new_region->template Target_Joint_Distribution<joint_itf*>();
					marg = new_region->template Target_Marginal_Distribution<marginal_itf*>();
					sample = new_region->template Sample_Data<sample_data_itf*>();

					
					double x;
					for (int i=0; i<ndim; i++)
					{
						fr.Get_Data<double>(x,linker.get_pums_column(i));
						index[i] = linker.find_index_in_dimension(i,x);
					}

					typename sample_type::ID_type sample_id;
					typename sample_type::Weight_type weight;
					fr.Get_Data<typename sample_type::ID_type>(sample_id,linker.sample_id_col);
					fr.Get_Data<typename sample_type::Weight_type>(weight, linker.sample_weight_col);
					vector<double> data;
					fr.Get_Data<double>(data,linker.get_pums_data_cols());

					pop_unit_itf* p = (pop_unit_itf*)Allocate<sample_type>();
					p->ID(sample_id);
					p->Index(new_region->template Get_1D_Index<Target_Type<NULLTYPE,typename joint_itf::size_type,typename joint_itf::index_type>>(index));
					p->Weight(weight);
					p->template Characteristics<Target_Type<NT,void,vector<double>*> >(&data);

					// Update the sample and joint distribution with the current population unit
					sample->insert(p->template Index<typename sample_collection_type::key_type>(), p);
					(*dist)[index] += weight;
					
					// also add to temporary sample data, so that the person file records can attach to household records
					//temp_sample->insert((typename temporary_sample_collection_type::key_type)sample_id,p);
					pair<typename temporary_sample_collection_type::key_type,pop_unit_itf*> item = pair<typename temporary_sample_collection_type::key_type,pop_unit_itf*>(sample_id,p);
					new_region->template Temporary_Sample_Data<temp_sample_data_itf*>()->insert(item);
				}
				fr.Close();



				//===============================================================================================================
				// read region person file, fill sample data
				//---------------------------------------------------------------------------------------------------------------
				if (!fr.Open(*linker.Person_File_Path())) 
				{
					cout<<endl<<"Could not open person file path. Press any key."<<endl; cin>>ans; return EXIT_FAILURE;
				}

				// references to the various region components
				typename temp_sample_data_itf::iterator sample_itr;

				while(fr.Read())
				{
					typename regions_itf::key_type ID;
		
					// read the region data from input file
					if(!fr.Get_Data<typename regions_itf::key_type>(ID,linker.person_region_id_col)) break; 

					if ((region_itr = regions->find(ID)) == regions->end())
					{
						THROW_WARNING("Sample from person file has non-existent region id.");
					}
					else
					{
						new_region = region_itr->second;

						// next add the sample data
						temp_sample = new_region->Temporary_Sample_Data<temp_sample_data_itf*>();

						// get characteristics from file
						typename person_unit_itf::get_type_of(ID) sample_id;
						fr.Get_Data<typename person_unit_itf::get_type_of(ID)>(sample_id,linker.person_sample_id_col);
						vector<double> data;
						fr.Get_Data<double>(data,linker.get_person_data_cols());

						// create new person unit
						person_unit_itf* p = (person_unit_itf*)Allocate<typename person_unit_itf::Component_Type>();
						p->ID(sample_id);
						p->template Characteristics<Target_Type<NT,void,vector<double>*> >(&data);

						// find the household that the person belongs to and add
						sample_itr = temp_sample->find(sample_id);
						if (sample_itr != temp_sample->end())
						{
							pop_unit_itf* pop_unit = sample_itr->second;
							pop_unit->Persons_Container<person_sample_data_itf*>()->push_back(p);
						}
						else
						{
							THROW_WARNING("Warning: person sample unit refers to non-existent household sample unit, observation ignored.");
						}
						
					}
				}
				fr.Close();



				//===============================================================================================================
				// Read zone file, fill marginal data
				//---------------------------------------------------------------------------------------------------------------
				File_IO::File_Reader zone_fr;
				if (!zone_fr.Open(*linker.Marg_File_Path())) 
				{
					cout<<endl<<"Ending. Press any key."<<endl;	cin>>ans; return EXIT_FAILURE;
				}
				while(zone_fr.Read())
				{
					typename zone_type::ID_type ID;
					typename regions_itf::key_type RID;
					double x;
		
					// get ID values for ZONE and REGION
					if(!zone_fr.Get_Data<typename zone_type::ID_type>(ID,linker.zone_id_col))
					{
						cout<<"ERROR: could not retrieve zone id from zone file"; return EXIT_FAILURE;
					}
					if(!zone_fr.Get_Data<typename regions_itf::key_type>(RID,linker.region_in_zone_id_col))
					{
						cout<<"ERROR: could not retrieve region id from zone file"; return EXIT_FAILURE;
					}

					// get region iterator from hash map
					if ((region_itr = regions->find(RID))==regions->end())
					{
						cout<<"ERROR: Region ID not found in map.  Press a key to end.";
						cin>>RID;
						return EXIT_FAILURE;
					}
					region_itf* region = region_itr->second;
					regional_marg = region->template Target_Marginal_Distribution<marginal_itf*>();

					// Read marginal data from file and add to ZONE
					zone_itf* zone = (zone_itf*)Allocate<zone_type>();
					zone->template Initialize<NULLTYPE>();
					zone->parent_reference(region);

					zone->ID(ID);
					solver_itf* zone_solver = (solver_itf*)Allocate<typename get_type_of(Solution_Settings)>(); 

					zone_solver->Initialize<Target_Type<NULLTYPE,void,double,int>>(solver->template Tolerance<double>(),solver->template Percentage_to_synthesize<double>(), solver->template Iterations<int>());
					zone->template Solver_Settings<solver_itf*>(zone_solver);
					joint_itf* mway = zone->template Target_Joint_Distribution<joint_itf*>();
					marginal_itf* marg = zone->template Target_Marginal_Distribution<marginal_itf*>();
					mway->resize(dimensions,0);
					marg->resize(dimensions,0);
					Rand_Interface* my_rand = (Rand_Interface*)Allocate<typename zone_itf::get_type_of(Rand)>();
					my_rand->template Initialize<double>(rand->template Next_Rand<double>()*(double)SHRT_MAX);
					

					zone->template Rand<Rand_Interface*>(my_rand);

					for (typename marginal_itf::size_type i=0; i<dimensions.size(); i++)
					{
						for (typename marginal_itf::size_type j=0; j<dimensions[i]; j++)
						{
							if (!zone_fr.Get_Data<double>(x,linker.get_sf3_column(i,j))) break;
							(*marg)[pair<typename marginal_itf::size_type,typename marginal_itf::size_type>(i,j)] = x;
							(*regional_marg)[pair<typename marginal_itf::size_type,typename marginal_itf::size_type>(i,j)] += x;
						}

					}
					pair<typename zone_type::ID_type,zone_itf*> item = pair<typename zone_type::ID_type,zone_itf*>(ID,zone);
					region->template Synthesis_Zone_Collection<zones_itf*>()->insert(item);
				}
				zone_fr.Close();


				//===============================================================================================================
				// Fill zonal activity_locations list from network reference
				//---------------------------------------------------------------------------------------------------------------
				define_component_interface(network_itf,typename get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
				define_container_and_value_interface(locations_container_itf, location_itf,typename network_itf::get_type_of(activity_locations_container),Containers::Random_Access_Sequence_Prototype,Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
				define_simple_container_interface(location_id_container_itf,typename zone_itf::get_type_of(Activity_Locations_Container),Containers::Random_Access_Sequence_Prototype,int,ComponentType);
				network_itf* network = this->network_reference<network_itf*>();
				locations_container_itf* locations = network->template activity_locations_container<locations_container_itf*>();
				location_itf* location;
				region_itf* region;
				zone_itf* zone;
				typename locations_container_itf::iterator loc_itr;
				typename zones_itf::iterator zone_itr;

				for (loc_itr = locations->begin(); loc_itr != locations->end(); ++loc_itr)
				{
					location = *loc_itr;
					long long zone_id = location->template census_zone_id<long long >();

					for (region_itr = regions->begin(); region_itr != regions->end(); ++region_itr)
					{
						region = region_itr->second;

						zones_itf* zones = region->template Synthesis_Zone_Collection<zones_itf*>();
						
						zone_itr = zones->find(zone_id);

						if (zone_itr != zones->end())
						{
							zone = zone_itr->second;
							zone->template Activity_Locations_Container<location_id_container_itf*>()->push_back(location->template internal_id<int>());
							break;
						}
					}
				}


				//------------------------
				// TIMER
				cout <<"Setup Runtime (ms): "<<timer.Stop();
				//------------------------

				return true;
			}
			feature_prototype bool Start_Popsyn(requires(check(ComponentType,!Concepts::Uses_Linker_File)))
			{
				assert_check(ComponentType,Concepts::Uses_Linker_File,"This popsyn type does not use linker file setup.");
			}
			
			// 2.) Start timing event - called before individual objects begin processing (at Iteration = 0, timestep 3)
			declare_feature_event(Start_Main_Timer)
			{
				Population_Synthesizer_Prototype<ComponentType,CallerType>* pthis = (Population_Synthesizer_Prototype<ComponentType,CallerType>*)_this;
				pthis->Start_Timer<NULLTYPE>();
			}
			feature_prototype void Start_Timer()
			{
				this->timer<Counter&>().Start();
			}
			
			// 3.) Stop timing event - called after individual objects end processing (at Iteration = 0, timestep 5)
			declare_feature_event(Stop_Main_Timer)
			{
				Population_Synthesizer_Prototype<ComponentType,CallerType>* pthis = (Population_Synthesizer_Prototype<ComponentType,CallerType>*)_this;
				pthis->Stop_Timer<NULLTYPE>();
			}
			feature_prototype void Stop_Timer()
			{
				cout << endl<<"Main Algorithm run-time: " << this->timer<Counter&>().Stop();
				//cout << endl<<"freq: " << this->timer<Counter&>().get_freq_value() << ", start: "<<this->timer<Counter&>().get_start_value() <<", l: "<<this->timer<Counter&>().get_l_value();
			}
			
			// 4.) Output results event - called after timing is stopped (at Iteration = 0, timestep 7)
			declare_feature_event(Output_Popsyn_Event)
			{
				Population_Synthesizer_Prototype<ComponentType,CallerType>* pthis = (Population_Synthesizer_Prototype<ComponentType,CallerType>*)_this;
				ofstream& sample_out = pthis->Output_Stream<ofstream&>();
				ofstream& marg_out = pthis->Marginal_Output_Stream<ofstream&>();
				ofstream& popsyn_log = pthis->Log_File<ofstream&>();

				//---------------------------------------------------------------------------------------------
				// Type defines for sub_objects
				typedef typename get_type_of(Synthesis_Regions_Collection)				region_collection_type;
				typedef typename region_collection_type::unqualified_value_type			region_type;
				typedef typename region_type::Sample_Data_type							sample_collection_type;
				typedef typename sample_collection_type::unqualified_value_type			sample_type;
				typedef typename region_type::Synthesis_Zone_Collection_type			zone_collection_type;
				typedef typename zone_collection_type::unqualified_value_type			zone_type;
				typedef typename zone_type::get_type_of(Synthetic_Households_Container)	household_collection_type;
				typedef typename household_collection_type::unqualified_value_type		household_type;
				typedef typename region_type::get_type_of(Target_Joint_Distribution)	joint_dist_type;
				typedef typename region_type::get_type_of(Target_Marginal_Distribution)	marg_dist_type;
				//---------------------------------------------------------------------------------------------
				// Interface defines for sub_objects
				define_container_and_value_interface_unqualified_container(regions_itf,region_itf,region_collection_type,Associative_Container_Prototype,PopSyn::Prototypes::Synthesis_Region_Prototype,ComponentType);
				define_container_and_value_interface_unqualified_container(zones_itf,zone_itf,zone_collection_type,Associative_Container_Prototype,PopSyn::Prototypes::Synthesis_Zone_Prototype,ComponentType);
				define_container_and_value_interface_unqualified_container(sample_data_itf,pop_unit_itf,sample_collection_type,Associative_Container_Prototype,Household_Components::Prototypes::Household_Properties,ComponentType);
				define_container_and_value_interface(person_sample_data_itf,person_unit_itf,typename pop_unit_itf::get_type_of(Persons_Container),Random_Access_Sequence_Prototype,Household_Components::Prototypes::Household_Properties,ComponentType);
				define_simple_container_interface(joint_itf,joint_dist_type,Multidimensional_Random_Access_Array_Prototype, typename joint_dist_type::unqualified_value_type ,NULLTYPE);
				define_simple_container_interface(marginal_itf,marg_dist_type,Multidimensional_Random_Access_Array_Prototype, typename marg_dist_type::unqualified_value_type ,NULLTYPE);
				define_container_and_value_interface_unqualified_container(household_collection_itf, household_itf,household_collection_type,Random_Access_Sequence_Prototype,Household_Components::Prototypes::Household,ComponentType);
				define_container_and_value_interface(person_collection_itf, person_itf,typename household_itf::get_type_of(Persons_Container),Random_Access_Sequence_Prototype,Person_Components::Prototypes::Person,ComponentType);
				define_simple_container_interface(activity_location_ids_itf,typename zone_itf::get_type_of(Activity_Locations_Container),Containers::Random_Access_Sequence_Prototype,int,ComponentType);				
				define_component_interface(network_itf,typename get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
				define_component_interface(scenario_itf,typename get_type_of(scenario_reference),Scenario_Components::Prototypes::Scenario_Prototype,ComponentType);
				define_container_and_value_interface(activity_locations_itf, activity_location_itf,typename network_itf::get_type_of(activity_locations_container),Containers::Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
				define_container_and_value_interface(_Zone_Container_Interface, _Zone_Interface,typename network_itf::get_type_of(zones_container),Containers::Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype,ComponentType);
				
				regions_itf* regions = pthis->Synthesis_Regions_Collection<regions_itf*>();
				network_itf* network = pthis->network_reference<network_itf*>();
				scenario_itf* scenario = pthis->scenario_reference<scenario_itf*>();
				activity_locations_itf* activity_locations = network->template activity_locations_container<activity_locations_itf*>();
				

				typename regions_itf::iterator r_itr;
				typename zones_itf::iterator z_itr;
				typename household_collection_itf::iterator p_itr;

				// initialize all of the synthesized individuals and assign unique ids
				long uuid = 0; // globally unique person id

				int counter = 0;

				cout << endl << endl;

				// Loop through all regions
				for (r_itr = regions->begin(); r_itr != regions->end(); ++r_itr)
				{
					region_itf* region = r_itr->second;
					zones_itf* zones = region->template Synthesis_Zone_Collection<zones_itf*>();
					// loop through zones in each region
					for (z_itr = zones->begin(); z_itr != zones->end(); ++z_itr)
					{
						zone_itf* zone = z_itr->second;
						activity_location_ids_itf* loc_indices = zone->template Activity_Locations_Container<activity_location_ids_itf*>();

						// loop through each synthesized person
						household_collection_itf* households = zone->template Synthetic_Households_Container<household_collection_itf*>();
						for (p_itr = households->begin(); p_itr != households->end(); ++p_itr)
						{
							// update synthesizing persons counter
							if (counter % 10000 == 0) cout << '\r' << "Initializing Agents: " << counter;
							household_itf* hh = *p_itr;

							// initialize the hh - allocates all hh subcomponents
							hh->template Initialize<Target_Type<NT,void,long,zone_itf*, network_itf*, scenario_itf*> >(uuid, zone, network, scenario);

							person_collection_itf* persons = hh->Persons_Container<person_collection_itf*>();

							long perid=0;
							for (typename person_collection_itf::iterator p_itr = persons->begin(); p_itr != persons->end(); ++p_itr)
							{		
								person_itf* person = (person_itf*)(*p_itr);
								person->template Initialize<Target_Type<NT,void,long,zone_itf*, network_itf*, scenario_itf*> >(perid, zone, network, scenario);
								++perid;
							}

							++uuid;
							++counter;
						}
					}
				}
				cout <<endl<<endl<<"Total Households Synthesized: "<<uuid<<endl<<endl;


				// Handle file output if needed
				if (pthis->write_marginal_output_flag<bool>() == true || pthis->write_full_output_flag<bool>() == true)
				{

					Counter timer;
					timer.Start();

					for (r_itr = regions->begin(); r_itr != regions->end(); ++r_itr)
					{
						region_itf* region = r_itr->second;
						zones_itf* zones = region->template Synthesis_Zone_Collection<zones_itf*>();
						for (z_itr = zones->begin(); z_itr != zones->end(); ++z_itr)
						{
							zone_itf* zone = z_itr->second;

							// write the marginal results
							if (pthis->write_marginal_output_flag<bool>() == true )
							{
								marg_out <<endl<<endl<<"ZONE_ID: "<<zone->template ID<long long int>();
								//zone->template Target_Joint_Distribution<joint_itf*>()->write(marg_out);
								marg_out <<endl;
								zone->template Target_Marginal_Distribution<marginal_itf*>()->write(marg_out);
								marg_out <<endl;
							}

							// write th full population results
							if (pthis->write_full_output_flag<bool>() == true)
							{
								household_collection_itf* sample = zone->template Synthetic_Households_Container<household_collection_itf*>();	
								household_itf* hh;
								for (typename household_collection_itf::iterator s_itr = sample->begin(); s_itr != sample->end(); ++s_itr)
								{
									hh = *s_itr;
									pop_unit_itf* p = hh->template Static_Properties<pop_unit_itf*>();
									sample_out << "ZONE_ID: "<<zone->template ID<long long int>() << endl << "ID: " << hh->template uuid<uint>() << ", ACS_ID: " << p->template ID<double>() << ",  weight: "<<p->template Weight<float>() <<", index: "<<p->template Index<uint>() << ", Gender: "<<p->template Household_type<Household_Components::Types::HHTYPE>();
								}
							}
						}
					}

					cout <<endl<<"File I/O Runtime: "<<timer.Stop();
				}
				sample_out.close();
				marg_out.close();
				popsyn_log.close();
			}
			
			// 5.) Write output to database (at Iteration 2)
			declare_feature_event(Output_Popsyn_To_DB_Event)
			{
				Population_Synthesizer_Prototype<ComponentType,CallerType>* pthis = (Population_Synthesizer_Prototype<ComponentType,CallerType>*)_this;

				//---------------------------------------------------------------------------------------------
				// Type defines for sub_objects
				typedef typename get_type_of(Synthesis_Regions_Collection)				region_collection_type;
				typedef typename region_collection_type::unqualified_value_type			region_type;
				typedef typename region_type::Sample_Data_type							sample_collection_type;
				typedef typename sample_collection_type::unqualified_value_type			sample_type;
				typedef typename region_type::Synthesis_Zone_Collection_type			zone_collection_type;
				typedef typename zone_collection_type::unqualified_value_type			zone_type;
				typedef typename zone_type::get_type_of(Synthetic_Households_Container)	household_collection_type;
				typedef typename household_collection_type::unqualified_value_type		household_type;
				typedef typename region_type::get_type_of(Target_Joint_Distribution)	joint_dist_type;
				typedef typename region_type::get_type_of(Target_Marginal_Distribution)	marg_dist_type;
				//---------------------------------------------------------------------------------------------
				// Interface defines for sub_objects
				define_container_and_value_interface_unqualified_container(regions_itf,region_itf,region_collection_type,Associative_Container_Prototype,PopSyn::Prototypes::Synthesis_Region_Prototype,ComponentType);
				define_container_and_value_interface_unqualified_container(zones_itf,zone_itf,zone_collection_type,Associative_Container_Prototype,PopSyn::Prototypes::Synthesis_Zone_Prototype,ComponentType);
				define_container_and_value_interface_unqualified_container(sample_data_itf,pop_unit_itf,sample_collection_type,Associative_Container_Prototype,Household_Components::Prototypes::Household_Properties,ComponentType);
				define_container_and_value_interface(person_sample_data_itf,person_unit_itf,typename pop_unit_itf::get_type_of(Persons_Container),Random_Access_Sequence_Prototype,Household_Components::Prototypes::Household_Properties,ComponentType);
				define_simple_container_interface(joint_itf,joint_dist_type,Multidimensional_Random_Access_Array_Prototype, typename joint_dist_type::unqualified_value_type ,NULLTYPE);
				define_simple_container_interface(marginal_itf,marg_dist_type,Multidimensional_Random_Access_Array_Prototype, typename marg_dist_type::unqualified_value_type ,NULLTYPE);
				define_container_and_value_interface_unqualified_container(household_collection_itf, household_itf,household_collection_type,Random_Access_Sequence_Prototype,Household_Components::Prototypes::Household,ComponentType);
				define_container_and_value_interface(person_collection_itf, person_itf,typename household_itf::get_type_of(Persons_Container),Random_Access_Sequence_Prototype,Person_Components::Prototypes::Person,ComponentType);
				define_simple_container_interface(activity_location_ids_itf,typename zone_itf::get_type_of(Activity_Locations_Container),Containers::Random_Access_Sequence_Prototype,int,ComponentType);				
				define_component_interface(network_itf,typename get_type_of(network_reference),Network_Components::Prototypes::Network_Prototype,ComponentType);
				define_component_interface(scenario_itf,typename get_type_of(scenario_reference),Scenario_Components::Prototypes::Scenario_Prototype,ComponentType);
				define_container_and_value_interface(activity_locations_itf, activity_location_itf,typename network_itf::get_type_of(activity_locations_container),Containers::Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);
				define_container_and_value_interface(_Zone_Container_Interface, _Zone_Interface,typename network_itf::get_type_of(zones_container),Containers::Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype,ComponentType);
				
				regions_itf* regions = pthis->Synthesis_Regions_Collection<regions_itf*>();
				network_itf* network = pthis->network_reference<network_itf*>();
				scenario_itf* scenario = pthis->scenario_reference<scenario_itf*>();
				activity_locations_itf* activity_locations = network->template activity_locations_container<activity_locations_itf*>();
				

				// EXIT if no request to write the demand to database
				if (!scenario->template write_demand_to_database<bool>()) return;

				// Start database transaction
				try
				{
					string name(scenario->output_demand_database_name<string&>());
					unique_ptr<odb::database> db (open_sqlite_database_single<unique_ptr<odb::database> >(name));
					odb::transaction t(db->begin());
				

					typename regions_itf::iterator r_itr;
					typename zones_itf::iterator z_itr;
					typename household_collection_itf::iterator p_itr;
					int counter = 0;

					// Loop through all regions
					for (r_itr = regions->begin(); r_itr != regions->end(); ++r_itr)
					{
						region_itf* region = r_itr->second;
						zones_itf* zones = region->template Synthesis_Zone_Collection<zones_itf*>();
						// loop through zones in each region
						for (z_itr = zones->begin(); z_itr != zones->end(); ++z_itr)
						{
							zone_itf* zone = z_itr->second;
							activity_location_ids_itf* loc_indices = zone->template Activity_Locations_Container<activity_location_ids_itf*>();

							// loop through each synthesized person
							household_collection_itf* households = zone->template Synthetic_Households_Container<household_collection_itf*>();
							for (p_itr = households->begin(); p_itr != households->end(); ++p_itr)
							{
								// update synthesizing persons counter
								if (counter % 10000 == 0) cout << '\r' << "Writing Agents to database:           " << counter;
								household_itf* hh = *p_itr;
								pop_unit_itf* hh_unit = hh->Static_Properties<pop_unit_itf*>();
							
								// create household record
								shared_ptr<polaris::io::Household> hh_rec(new polaris::io::Household());
								hh_rec->setHhold(hh->uuid<int>());
								hh_rec->setPersons(hh_unit->Household_size<int>());
								hh_rec->setWorkers(hh_unit->Number_of_workers<int>());
								hh_rec->setVehicles(hh_unit->Number_of_vehicles<int>());
								hh_rec->setLocation(hh->Home_Location<activity_location_itf*>()->uuid<int>());
								//push to database
								db->persist(hh_rec);

								person_collection_itf* persons = hh->Persons_Container<person_collection_itf*>();

								for (typename person_collection_itf::iterator p_itr = persons->begin(); p_itr != persons->end(); ++p_itr)
								{		
									person_itf* person = (person_itf*)(*p_itr);

									shared_ptr<polaris::io::Person> per_rec(new polaris::io::Person());
									per_rec->setId(person->uuid<int>());
									if (person->School_Location<int>() >= 0)
										per_rec->setSchool_Location_Id(person->School_Location<activity_location_itf*>()->uuid<int>());
									if (person->Work_Location<int>() >= 0)
										per_rec->setWork_Location_Id(person->Work_Location<activity_location_itf*>()->uuid<int>());
									per_rec->setHousehold(hh_rec);
									//push to database
									db->persist(per_rec);

									counter++;
								}

							}
						}
					}
					t.commit();
				}
				catch (odb::sqlite::database_exception ex)
				{
					cout << endl << ex.what();
				}

				
			}

			//----------------------------------------------------------------
			// Required Features - necessary for any synthesis routine
			feature_accessor(Synthesis_Regions_Collection,none,none);
			feature_accessor(Solution_Settings,none,none);
			feature_accessor(scenario_reference, none, none);
			feature_accessor(network_reference, check(ReturnValueType,Network_Components::Concepts::Is_Transportation_Network), check(SetValueType,Network_Components::Concepts::Is_Transportation_Network));
			feature_accessor(timer,none,none);
			feature_accessor(write_marginal_output_flag,none,none);
			feature_accessor(write_full_output_flag,none,none);

			//----------------------------------------------------------------
			// Optional Features - used for specific solution types
			feature_accessor(linker_file_path, none,none);
			
			//----------------------------------------------------------------
			// Output features - optional, used to write intermediate and final results
			feature_accessor(Output_Stream,check_2(strip_modifiers(ReturnValueType),ofstream, is_same), check_2(strip_modifiers(SetValueType),ofstream, is_same));
			feature_accessor(Marginal_Output_Stream,check_2(strip_modifiers(ReturnValueType),ofstream, is_same), check_2(strip_modifiers(SetValueType),ofstream, is_same));
			feature_accessor(Log_File,check_2(strip_modifiers(ReturnValueType),ofstream, is_same), check_2(strip_modifiers(SetValueType),ofstream, is_same));
		};
	}
}
