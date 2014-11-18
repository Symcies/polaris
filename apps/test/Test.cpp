#define EXCLUDE_DEMAND

#include "Polaris_PCH.h"
#include "Application_Includes.h"
#include "Network_Skimming_Prototype.h"


using namespace polaris;


struct MasterType
{
	typedef MasterType M;

	//==============================================================================================
	#pragma region Network Types
	//----------------------------------------------------------------------------------------------
	#ifdef ANTARES
	typedef Conductor_Implementation<M> conductor_type;
	typedef Control_Panel_Implementation<M> control_panel_type;
	typedef Time_Panel_Implementation<M> time_panel_type;
	typedef Information_Panel_Implementation<M> information_panel_type;
	typedef Canvas_Implementation<M> canvas_type;
	typedef Antares_Layer_Implementation<M> antares_layer_type;
	typedef Layer_Options_Implementation<M> layer_options_type;
	typedef Attributes_Panel_Implementation<M> attributes_panel_type;
	typedef Control_Dialog_Implementation<M> control_dialog_type;
	typedef Information_Page_Implementation<MasterType> information_page_type;
	typedef Splash_Panel_Implementation<M> splash_panel_type;

	typedef Antares_Network_Implementation<M> network_type;
	typedef Antares_Link_Implementation<M> link_type;
	typedef Vehicle_Components::Implementations::Antares_Vehicle_Implementation<M> vehicle_type;
	//typedef Vehicle_Components::Implementations::Antares_Vehicle_Implementation<M> basic_vehicle_type;
	//typedef Vehicle_Components::Implementations::Polaris_Base_Vehicle_Implementation<M> vehicle_type;
	typedef NULLTYPE visual_vehicle_type;

	typedef Zone_Components::Implementations::Graphical_Zone_Implementation<M> zone_type;
	//typedef Zone_Components::Implementations::Zone_Implementation<M> zone_type;

	typedef Antares_Intersection_Implementation<M> intersection_type;
	typedef Zone_Components::Implementations::Graphical_Zone_Group_Implementation<M> graphical_zone_group_type;
	#else
	typedef Network_Components::Implementations::Network_Implementation<M> network_type;
	typedef Link_Components::Implementations::Link_Implementation<M> link_type;
	typedef Intersection_Components::Implementations::Intersection_Implementation<M> intersection_type;
	typedef Vehicle_Components::Implementations::Vehicle_Implementation<M> vehicle_type;
	typedef Zone_Components::Implementations::Zone_Implementation<M> zone_type;
	#endif

	typedef Scenario_Components::Implementations::Scenario_Implementation<M> scenario_type;
	typedef Network_Components::Implementations::Network_DB_Reader_Implementation<M> network_db_reader_type;
	typedef Turn_Movement_Components::Implementations::Movement_Implementation<M> movement_type;

	typedef Turn_Movement_Components::Implementations::Movement_Implementation<M> turn_movement_type;
	typedef Routing_Components::Implementations::Routable_Network_Implementation<M> routable_network_type;
	typedef Routing_Components::Implementations::Routing_Implementation<M> routing_type;
	typedef Routing_Components::Implementations::Skim_Routing_Implementation<M> skim_routing_type;
	//typedef Intersection_Components::Implementations::Routable_Intersection_Implementation<M> routable_intersection_type;
	//typedef Link_Components::Implementations::Routable_Link_Implementation<M> routable_link_type;
	typedef Activity_Location_Components::Implementations::Activity_Location_Implementation<M> activity_location_type;
	typedef Traveler_Components::Implementations::Traveler_Implementation<M> traveler_type;
	typedef Vehicle_Components::Implementations::Switch_Decision_Data_Implementation<MasterType> switch_decision_data_type;
	typedef Intersection_Components::Implementations::Inbound_Outbound_Movements_Implementation<M> inbound_outbound_movements_type;
	typedef Intersection_Components::Implementations::Outbound_Inbound_Movements_Implementation<M> outbound_inbound_movements_type;
	//typedef Intersection_Components::Implementations::Routable_Inbound_Outbound_Movements_Implementation<M> routable_inbound_outbound_movements_type;
	//typedef Intersection_Components::Implementations::Routable_Outbound_Inbound_Movements_Implementation<M> routable_outbound_inbound_movements_type;
	//typedef Intersection_Components::Implementations::Routable_Movement_Implementation<M> routable_movement_type;
	typedef Operation_Components::Implementations::Operation_Implementation<M> operation_type;
	typedef Intersection_Control_Components::Implementations::Intersection_Control_Implementation<M> intersection_control_type;
	typedef Intersection_Control_Components::Implementations::Control_Plan_Implementation<M> control_plan_type;
	typedef Intersection_Control_Components::Implementations::Phase_Implementation<M> phase_type;
	typedef Intersection_Control_Components::Implementations::Phase_Movement_Implementation<M> phase_movement_type;
	typedef Intersection_Control_Components::Implementations::Approach_Implementation<M> approach_type;
	
	typedef Analyze_Link_Group_Components::Implementations::Analyze_Link_Group_Implementation<MasterType> analyze_link_group_type;

	typedef Plan_Components::Implementations::Plan_Implementation<M> plan_type;

	typedef Movement_Plan_Components::Implementations::Movement_Plan_Implementation<M> movement_plan_type;
	typedef Movement_Plan_Components::Implementations::Movement_Plan_Record_Implementation<M> movement_plan_record_type;

	typedef Movement_Plan_Components::Implementations::Trajectory_Unit_Implementation<M> trajectory_unit_type;
	typedef Network_Skimming_Components::Implementations::Basic_Network_Skimming_Implementation<M> network_skim_type;
	typedef Network_Skimming_Components::Implementations::LOS_Value_Implementation<M> los_value_type;
	typedef Network_Skimming_Components::Implementations::LOS_Time_Invariant_Value_Implementation<M> los_invariant_value_type;
	//typedef Network_Components::Implementations::Network_Validation_Implementation<M> network_validation_type;
	//typedef Network_Components::Implementations::Network_Validation_Unit_Implementation<M> network_validation_unit_type;
	#pragma endregion
	//----------------------------------------------------------------------------------------------


	//==============================================================================================
	#pragma region TMC Types
	//----------------------------------------------------------------------------------------------
	typedef Traffic_Management_Center_Components::Implementations::Simple_TMC<MasterType> traffic_management_center_type;
	#ifdef ANTARES
	typedef Network_Event_Components::Implementations::Antares_Weather_Network_Event<MasterType> weather_network_event_type;
	typedef Network_Event_Components::Implementations::Antares_Accident_Network_Event<MasterType> accident_network_event_type;
	typedef Network_Event_Components::Implementations::Antares_Congestion_Network_Event<MasterType> congestion_network_event_type;
	typedef Network_Event_Components::Implementations::Antares_Lane_Closure_Network_Event<MasterType> lane_closure_network_event_type;

	typedef Link_Control_Components::Implementations::Antares_Lane_Link_Control<MasterType> link_control_type;
	typedef Depot_Components::Implementations::Antares_Tow_Truck_Depot<MasterType> depot_type;
	typedef Depot_Components::Implementations::Antares_Tow_Truck<MasterType> tow_truck_type;

	typedef Advisory_Radio_Components::Implementations::Antares_Advisory_Radio<MasterType> advisory_radio_type;
	typedef Variable_Message_Sign_Components::Implementations::Antares_Variable_Word_Sign<MasterType> variable_word_sign_type;
	typedef Variable_Message_Sign_Components::Implementations::Antares_Variable_Speed_Sign<MasterType> variable_speed_sign_type;

	typedef Sensor_Components::Implementations::Antares_Link_Sensor<MasterType> link_sensor_type;
	typedef Buildings_Components::Implementations::Antares_Buildings_Implementation<M> buildings_type;
	typedef Ramp_Metering_Components::Implementations::Antares_Ramp_Metering_Implementation<M> ramp_metering_type;
	#else
	typedef Network_Event_Components::Implementations::Weather_Network_Event<MasterType> weather_network_event_type;
	typedef Network_Event_Components::Implementations::Accident_Network_Event<MasterType> accident_network_event_type;
	typedef Network_Event_Components::Implementations::Congestion_Network_Event<MasterType> congestion_network_event_type;
	typedef Network_Event_Components::Implementations::Lane_Closure_Network_Event<MasterType> lane_closure_network_event_type;

	typedef Link_Control_Components::Implementations::Lane_Link_Control<MasterType> link_control_type;
	typedef Depot_Components::Implementations::Tow_Truck_Depot<MasterType> depot_type;
	typedef Depot_Components::Implementations::Tow_Truck_Implementation<MasterType> tow_truck_type;

	typedef Advisory_Radio_Components::Implementations::Highway_Advisory_Radio<MasterType> advisory_radio_type;
	typedef Variable_Message_Sign_Components::Implementations::Variable_Word_Sign<MasterType> variable_word_sign_type;
	typedef Variable_Message_Sign_Components::Implementations::Variable_Speed_Sign<MasterType> variable_speed_sign_type;

	typedef Sensor_Components::Implementations::Link_Sensor<MasterType> link_sensor_type;

	typedef Ramp_Metering_Components::Implementations::Ramp_Metering_Implementation<M> ramp_metering_type;
	#endif

	typedef Network_Event_Components::Implementations::Base_Network_Event<MasterType> base_network_event_type;
	typedef TYPELIST_4(weather_network_event_type,accident_network_event_type,congestion_network_event_type,lane_closure_network_event_type) network_event_types;
	typedef TYPELIST_5(link_control_type,depot_type,advisory_radio_type,variable_word_sign_type,variable_speed_sign_type) its_component_types;

	typedef Network_Event_Components::Implementations::Network_Event_Manager_Implementation<MasterType> network_event_manager_type;


	typedef Routable_Agent_Implementation<MasterType> routable_agent_type;
	typedef Tree_Agent_Implementation<MasterType> tree_agent_type;
	typedef Graph_Implementation<MasterType, NTL, Base_Edge_A_Star<MasterType>> base_graph_type;
	typedef Graph_Pool_Implementation<MasterType, NTL, base_graph_type> graph_pool_type;
	typedef Edge_Implementation<Routing_Components::Types::static_attributes<MasterType>> static_edge_type;
	typedef Graph_Implementation<MasterType, NTL, static_edge_type> static_graph_type;
	typedef Routing_Components::Types::static_to_static static_to_static_type;
	typedef Custom_Connection_Group<MasterType, static_graph_type, static_graph_type, static_to_static_type> static_to_static_connection_type;
	
	typedef Edge_Implementation<Routing_Components::Types::time_dependent_attributes<MasterType>> time_dependent_edge_type;
	typedef Graph_Implementation<MasterType, NTL, time_dependent_edge_type> time_dependent_graph_type;
	typedef Routing_Components::Types::time_dependent_to_time_dependent time_dependent_to_time_dependent_type;
	typedef Custom_Connection_Group<MasterType, time_dependent_graph_type, time_dependent_graph_type, time_dependent_to_time_dependent_type> time_dependent_to_time_dependent_connection_type;

	#pragma endregion
	//----------------------------------------------------------------------------------------------
};


//==================================================
// SKIMFILE READER APPLICATION
int main(int argc, char** argv)
{
	string input_filename, output_filename;
	bool Transit=false;
	bool Update=false;

	if (argc < 2)
	{
		cout << "Enter input skimfile name: ";
		cin >> input_filename;
	}
	else
	{
		input_filename = argv[1];
	}

	if (argc >= 3)
	{
		if (strcmp(argv[2],"-t")==0)
		{
			cout << argv[2]<<endl;
			Transit = true;
		}
		else if (strcmp(argv[2],"-tb")==0)
		{
			cout << argv[2]<<endl;
			Transit = true;
			Update = true;
		}
	}

	Simulation_Configuration cfg;
	cfg.Multi_Threaded_Setup(1, 1);
	INITIALIZE_SIMULATION(cfg);

	// Create interface
	typedef Network_Skimming_Components::Prototypes::Network_Skimming<MasterType::network_skim_type> skim_interface;
	typedef skim_interface::get_type_of(update_interval_endpoints) intervals_type;

	skim_interface* skimmer = (skim_interface*)Allocate<MasterType::network_skim_type>();
	
	int num_modes;
	int num_zones;

	ofstream outfile;
	if (!Update)
	{
		output_filename = input_filename + ".csv";
		outfile.open(output_filename);
	}

	File_IO::Binary_File_Writer binary_outfile;
	if (Update)
	{
		output_filename = input_filename + "_new.bin";
		binary_outfile.Open(output_filename);
	}

	if (Transit)
	{	
		cout <<"Do transit skim file read..."<<endl;

		if (!skimmer->transit_input_file<File_IO::Binary_File_Reader&>().Open(input_filename)) THROW_EXCEPTION("Error: input binary transit skim file '" << input_filename << "' could not be opened. Highway skims are required, application terminating.");
		File_IO::Binary_File_Reader& transit_infile = skimmer->transit_input_file<File_IO::Binary_File_Reader&>();


		transit_infile.Read_Value<int>(num_zones);
		
		//===========================================================================
		// create the time-invariant skim tables
		float* transit_ttime = new float[num_zones*num_zones]();
		float* transit_walk_access_time = new float[num_zones*num_zones]();
		float* auto_distance /*transit_sov_access_time */= new float[num_zones*num_zones]();
		float* transit_wait_time = new float[num_zones*num_zones]();
		float* transit_fare = new float[num_zones*num_zones]();

		if (!Update)
		{

			// Read transit input data if required
			transit_infile.Read_Array<float>(transit_ttime, num_zones*num_zones);
			outfile <<"Transit Time:"<<endl;
			for (int i =0; i < num_zones; i++)
			{
				for (int j=0; j < num_zones; j++) outfile << transit_ttime[i*num_zones + j] << "," ;
				outfile << endl;
			}

			transit_infile.Read_Array<float>(transit_walk_access_time, num_zones*num_zones);
			outfile <<endl<<"Transit Walk Access Time:"<<endl;
			for (int i =0; i < num_zones; i++)
			{
				for (int j=0; j < num_zones; j++) outfile << transit_walk_access_time[i*num_zones + j] << "," ;
				outfile << endl;
			}

			transit_infile.Read_Array<float>(auto_distance /*transit_sov_access_time*/, num_zones*num_zones);
			outfile <<endl<<"Auto Distance:"<<endl;
			for (int i =0; i < num_zones; i++)
			{
				for (int j=0; j < num_zones; j++) outfile << auto_distance[i*num_zones + j] << "," ;
				outfile << endl;
			}

			transit_infile.Read_Array<float>(transit_wait_time, num_zones*num_zones);
			outfile <<endl<<"Transit Wait TIme:"<<endl;
			for (int i =0; i < num_zones; i++)
			{
				for (int j=0; j < num_zones; j++) outfile << transit_wait_time[i*num_zones + j] << "," ;
				outfile << endl;
			}

			transit_infile.Read_Array<float>(transit_fare, num_zones*num_zones);
			outfile <<endl<<"Transit Fare:"<<endl;
			for (int i =0; i < num_zones; i++)
			{
				for (int j=0; j < num_zones; j++) outfile << transit_fare[i*num_zones + j] << "," ;
				outfile << endl;
			}
		}
		else
		{
			
			int new_zones = num_zones+1;
			binary_outfile.Write_Value<int>(new_zones);

			cout << "Read original data."<<endl;
			transit_infile.Read_Array<float>(transit_ttime, num_zones*num_zones);
			transit_infile.Read_Array<float>(transit_walk_access_time, num_zones*num_zones);
			transit_infile.Read_Array<float>(auto_distance, num_zones*num_zones);
			transit_infile.Read_Array<float>(transit_wait_time, num_zones*num_zones);
			transit_infile.Read_Array<float>(transit_fare, num_zones*num_zones);

			matrix<float>::index_type index, new_index;
			index.first=num_zones; index.second=num_zones;
			new_index.first=new_zones; new_index.second=new_zones;

			matrix<float> ttime = matrix<float>(new_index,0);
			matrix<float> walk = matrix<float>(new_index,0);
			matrix<float> distance= matrix<float>(new_index,0);
			matrix<float> wait= matrix<float>(new_index,0);
			matrix<float> fare= matrix<float>(new_index,0);
			

			cout << "Copy to new arrays."<<endl;
			for (int i =0; i < num_zones; i++)
			{
				for (int j=0; j < num_zones; j++)
				{
					ttime(i,j) = transit_ttime[i*num_zones+j];
					walk(i,j) = transit_walk_access_time[i*num_zones+j];
					distance(i,j) = auto_distance[i*num_zones+j];
					wait(i,j) = transit_wait_time[i*num_zones+j];
					fare(i,j) = transit_fare[i*num_zones+j];
				}
			}

			cout << "Update new zone."<<endl;
			//update new zone to match zone 256 accessibility
			for (int i =0; i < num_zones; i++)
			{
				ttime(i,new_zones-1) = transit_ttime[i*num_zones+256];
				ttime(new_zones-1,i) = transit_ttime[(256)*num_zones+i];
				walk(i,new_zones-1) = transit_walk_access_time[i*num_zones+256];
				walk(new_zones-1,i) = transit_walk_access_time[(256)*num_zones+i];
				distance(i,new_zones-1) = auto_distance[i*num_zones+256];
				distance(new_zones-1,i) = auto_distance[(256)*num_zones+i];
				wait(i,new_zones-1) = transit_wait_time[i*num_zones+256];
				wait(new_zones-1,i) = transit_wait_time[(256)*num_zones+i];
				fare(i,new_zones-1) = transit_fare[i*num_zones+256];
				fare(new_zones-1,i) = transit_fare[(256)*num_zones+i];
			}

			cout << "Write binary file."<<endl;
			binary_outfile.Write_Array<float>(ttime.get_data_pointer(),new_zones*new_zones);
			binary_outfile.Write_Array<float>(walk.get_data_pointer(),new_zones*new_zones);
			binary_outfile.Write_Array<float>(distance.get_data_pointer(),new_zones*new_zones);
			binary_outfile.Write_Array<float>(wait.get_data_pointer(),new_zones*new_zones);
			binary_outfile.Write_Array<float>(fare.get_data_pointer(),new_zones*new_zones);
			
		}
	}

	else
	{
		int update_increment;
		intervals_type intervals;

		if (!skimmer->highway_input_file<File_IO::Binary_File_Reader&>().Open(input_filename)) THROW_EXCEPTION("Error: input binary highway skim file '" << input_filename << "' could not be opened. Highway skims are required, application terminating.");

		skimmer->Read_Binary_Headers<intervals_type*>(num_modes, num_zones, &intervals, false);


		Simulation_Timestep_Increment start;
		//===========================================================================
		// create the skim_table time periods, for basic create only a single time period skim_table

		for (int i=0; i < skimmer->_update_interval_endpoints.size(); ++i)
		{
			if (skimmer->_update_interval_endpoints[i]>GLOBALS::Time_Converter.template Convert_Value<Time_Hours,Time_Minutes>(1440)) break;
			if (i > 0)
			{
				if (skimmer->_update_interval_endpoints[i] == skimmer->_update_interval_endpoints[i-1]) break;
			}

			float* data = new float[num_zones*num_zones];
			skimmer->Read_Binary_Data<float*>(data,skimmer->highway_input_file<File_IO::Binary_File_Reader&>(),num_zones);

			outfile << endl << "Skim matrix for hour: " << GLOBALS::Time_Converter.template Convert_Value<Time_Minutes,Time_Hours>(intervals[i])<<endl;

			for (int i =0; i < num_zones; i++)
			{
				for (int j=0; j < num_zones; j++)
				{
					outfile << data[i*num_zones + j] << "," ;
				}
				outfile << endl;
			}
		}
	}


	int test;
	cin >> test;
}