#ifndef IOResult
#define IOResult

#include "Shared.h"

namespace polaris
{
namespace io
{
//Forward declarations.
//
class Link_Delay;
class Performance;
class Problem;
class Skim;
class Transims_Event;
class link_travel;
class Path;
class Link_Probability;
class LinkMOE;


#pragma db value
class link_travel
{
public:
	// Default Constructor
	link_travel () {}        
	link_travel (int link_, bool dir_, int entering_time_, int travel_time_, int delayed_time_)
	: link (link_), dir (dir_), entering_time (entering_time_), travel_time (travel_time_), delayed_time (delayed_time_)
	{
	}
	//Accessors
	const int& getLink () const {return link;}
	void setLink (const int& link_) {link = link_;}
	const bool& getDir () const {return dir;}
	void setDir (const bool& dir_) {dir = dir_;}
	const int& getEntering_Time () const {return entering_time;}
	void setEntering_Time (const int& entering_time_) {entering_time = entering_time_;}
	const int& getTravel_Time () const {return travel_time;}
	void setTravel_Time (const int& travel_time_) {travel_time = travel_time_;}
	const int& getDelayed_Time () const {return delayed_time;}
	void setDelayed_Time (const int& delayed_time_) {delayed_time = delayed_time_;}
	//Data Fields
private:
	friend class odb::access;
	int link;
	bool dir;
	int entering_time;
	int travel_time;
	int delayed_time;
};


#pragma db object
class Path
{
public:
	// Default Constructor
	Path () {}        
	Path (int id_, int vehicle_, int origin_zone_, int destination_zone_, int origin_activity_location_, int destination_activity_location_, int origin_link_, int destination_link_, int num_links_, int departure_time_, int arrival_time_, int travel_time_, std::vector<link_travel > links_)
	: id (id_), vehicle (vehicle_), origin_zone (origin_zone_), destination_zone (destination_zone_), origin_activity_location (origin_activity_location_), destination_activity_location (destination_activity_location_), origin_link (origin_link_), destination_link (destination_link_), num_links (num_links_), departure_time (departure_time_), arrival_time (arrival_time_), travel_time (travel_time_), links (links_)
	{
	}
	//Accessors
	const int& getId () const {return id;}
	void setId (const int& id_) {id = id_;}
	const int& getVehicle () const {return vehicle;}
	void setVehicle (const int& vehicle_) {vehicle = vehicle_;}
	const int& getOrigin_Zone () const {return origin_zone;}
	void setOrigin_Zone (const int& origin_zone_) {origin_zone = origin_zone_;}
	const int& getDestination_Zone () const {return destination_zone;}
	void setDestination_Zone (const int& destination_zone_) {destination_zone = destination_zone_;}
	const int& getOrigin_Activity_Location () const {return origin_activity_location;}
	void setOrigin_Activity_Location (const int& origin_activity_location_) {origin_activity_location = origin_activity_location_;}
	const int& getDestination_Activity_Location () const {return destination_activity_location;}
	void setDestination_Activity_Location (const int& destination_activity_location_) {destination_activity_location = destination_activity_location_;}
	const int& getOrigin_Link () const {return origin_link;}
	void setOrigin_Link (const int& origin_link_) {origin_link = origin_link_;}
	const int& getDestination_Link () const {return destination_link;}
	void setDestination_Link (const int& destination_link_) {destination_link = destination_link_;}
	const int& getNum_Links () const {return num_links;}
	void setNum_Links (const int& num_links_) {num_links = num_links_;}
	const int& getDeparture_Time () const {return departure_time;}
	void setDeparture_Time (const int& departure_time_) {departure_time = departure_time_;}
	const int& getArrival_Time () const {return arrival_time;}
	void setArrival_Time (const int& arrival_time_) {arrival_time = arrival_time_;}
	const int& getTravel_Time () const {return travel_time;}
	void setTravel_Time (const int& travel_time_) {travel_time = travel_time_;}
	const std::vector<link_travel >& getLinks () const {return links;}
	void setLinks (const std::vector<link_travel >& links_) {links = links_;}
	void setLink (const link_travel  links_) {links.push_back(links_);}
	//Data Fields
private:
	friend class odb::access;
	#pragma db auto id
	int id;
	#pragma db not_null
	int vehicle;
	#pragma db not_null
	int origin_zone;
	#pragma db not_null
	int destination_zone;
	#pragma db not_null
	int origin_activity_location;
	#pragma db not_null
	int destination_activity_location;
	#pragma db not_null
	int origin_link;
	#pragma db not_null
	int destination_link;
	#pragma db not_null
	int num_links;
	#pragma db not_null
	int departure_time;
	#pragma db not_null
	int arrival_time;
	#pragma db not_null
	int travel_time;
	std::vector<link_travel > links;
};

#pragma db object //table("LINK_DELAY")
class Link_Delay
{
public:
	// Default Constructor
	Link_Delay () {}	
	//Constructor
	Link_Delay ( int link_, int dir_, int type_, double start_, double end_, double flow_, double time_, int out_link_, double out_flow_, double out_time_ )  
	: link (link_), dir (dir_), type (type_), start (start_), end (end_), flow (flow_), time (time_), out_link (out_link_), out_flow (out_flow_), out_time (out_time_)
	{
	}
	//Accessors
	const int& getLink () const {return link;}
	void setLink (const int& link_){link = link_;}
	const int& getDir () const {return dir;}
	void setDir (const int& dir_){dir = dir_;}
	const int& getType () const {return type;}
	void setType (const int& type_){type = type_;}
	const double& getStart () const {return start;}
	void setStart (const double& start_){start = start_;}
	const double& getEnd () const {return end;}
	void setEnd (const double& end_){end = end_;}
	const double& getFlow () const {return flow;}
	void setFlow (const double& flow_){flow = flow_;}
	const double& getTime () const {return time;}
	void setTime (const double& time_){time = time_;}
	const int& getOut_Link () const {return out_link;}
	void setOut_Link (const int& out_link_){out_link = out_link_;}
	const double& getOut_Flow () const {return out_flow;}
	void setOut_Flow (const double& out_flow_){out_flow = out_flow_;}
	const double& getOut_Time () const {return out_time;}
	void setOut_Time (const double& out_time_){out_time = out_time_;}
	const unsigned long& getPrimaryKey () const {return auto_id;}
	const unsigned long& getAuto_id () const {return auto_id;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long auto_id;
	int link;
	int dir;
	int type;
	double start;
	double end;
	double flow;
	double time;
	int out_link;
	double out_flow;
	double out_time;

};

#pragma db object //table("PERFORMANCE")
class Performance
{
public:
	// Default Constructor
	Performance () {}	
	//Constructor
	Performance ( double speed_, double delay_, double density_, double max_den_, double ratio_, double queue_, int max_que_, int fail_ )  
	: speed (speed_), delay (delay_), density (density_), max_den (max_den_), ratio (ratio_), queue (queue_), max_que (max_que_), fail (fail_)
	{
	}
	//Accessors
	const double& getSpeed () const {return speed;}
	void setSpeed (const double& speed_){speed = speed_;}
	const double& getDelay () const {return delay;}
	void setDelay (const double& delay_){delay = delay_;}
	const double& getDensity () const {return density;}
	void setDensity (const double& density_){density = density_;}
	const double& getMax_Den () const {return max_den;}
	void setMax_Den (const double& max_den_){max_den = max_den_;}
	const double& getRatio () const {return ratio;}
	void setRatio (const double& ratio_){ratio = ratio_;}
	const double& getQueue () const {return queue;}
	void setQueue (const double& queue_){queue = queue_;}
	const int& getMax_Que () const {return max_que;}
	void setMax_Que (const int& max_que_){max_que = max_que_;}
	const int& getFail () const {return fail;}
	void setFail (const int& fail_){fail = fail_;}
	const unsigned long& getPrimaryKey () const {return auto_id;}
	const unsigned long& getAuto_id () const {return auto_id;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long auto_id;
	double speed;
	double delay;
	double density;
	double max_den;
	double ratio;
	double queue;
	int max_que;
	int fail;

};


#pragma db object //table("PROBLEM")
class Problem
{
public:
	// Default Constructor
	Problem () {}	
	//Constructor
	Problem ( int problem_, double time_, int link_, int dir_, int lane_, double offset_, int route_, int survey_ )  
	: problem (problem_), time (time_), link (link_), dir (dir_), lane (lane_), offset (offset_), route (route_), survey (survey_)
	{
	}
	//Accessors
	const int& getProblem () const {return problem;}
	void setProblem (const int& problem_){problem = problem_;}
	const double& getTime () const {return time;}
	void setTime (const double& time_){time = time_;}
	const int& getLink () const {return link;}
	void setLink (const int& link_){link = link_;}
	const int& getDir () const {return dir;}
	void setDir (const int& dir_){dir = dir_;}
	const int& getLane () const {return lane;}
	void setLane (const int& lane_){lane = lane_;}
	const double& getOffset () const {return offset;}
	void setOffset (const double& offset_){offset = offset_;}
	const int& getRoute () const {return route;}
	void setRoute (const int& route_){route = route_;}
	const int& getSurvey () const {return survey;}
	void setSurvey (const int& survey_){survey = survey_;}
	const int& getPrimaryKey () const {return problem;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id
	int problem;
	double time;
	int link;
	int dir;
	int lane;
	double offset;
	int route;
	int survey;
	#pragma db index member(problem)

};


#pragma db object //table("SKIM")
class Skim
{
public:
	// Default Constructor
	Skim () {}	
	//Constructor
	Skim ( double time_, double walk_, double drive_, double transit_, double wait_, double other_, double length_, double cost_, int impedance_ )  
	: time (time_), walk (walk_), drive (drive_), transit (transit_), wait (wait_), other (other_), length (length_), cost (cost_), impedance (impedance_)
	{
	}
	//Accessors
	const double& getTime () const {return time;}
	void setTime (const double& time_){time = time_;}
	const double& getWalk () const {return walk;}
	void setWalk (const double& walk_){walk = walk_;}
	const double& getDrive () const {return drive;}
	void setDrive (const double& drive_){drive = drive_;}
	const double& getTransit () const {return transit;}
	void setTransit (const double& transit_){transit = transit_;}
	const double& getWait () const {return wait;}
	void setWait (const double& wait_){wait = wait_;}
	const double& getOther () const {return other;}
	void setOther (const double& other_){other = other_;}
	const double& getLength () const {return length;}
	void setLength (const double& length_){length = length_;}
	const double& getCost () const {return cost;}
	void setCost (const double& cost_){cost = cost_;}
	const int& getImpedance () const {return impedance;}
	void setImpedance (const int& impedance_){impedance = impedance_;}
	const unsigned long& getPrimaryKey () const {return auto_id;}
	const unsigned long& getAuto_id () const {return auto_id;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long auto_id;
	double time;
	double walk;
	double drive;
	double transit;
	double wait;
	double other;
	double length;
	double cost;
	int impedance;

};

#pragma db object //table("EVENT")
class Transims_Event
{
public:
	// Default Constructor
	Transims_Event () {}	
	//Constructor
	Transims_Event ( int hhold_, int person_, int tour_, int trip_, int mode_, int type_, double schedule_, double actual_, int link_, int dir_, int lane_, double offset_, int route_ )  
	: hhold (hhold_), person (person_), tour (tour_), trip (trip_), mode (mode_), type (type_), schedule (schedule_), actual (actual_), link (link_), dir (dir_), lane (lane_), offset (offset_), route (route_)
	{
	}
	//Accessors
	const int& getHhold () const {return hhold;}
	void setHhold (const int& hhold_){hhold = hhold_;}
	const int& getPerson () const {return person;}
	void setPerson (const int& person_){person = person_;}
	const int& getTour () const {return tour;}
	void setTour (const int& tour_){tour = tour_;}
	const int& getTrip () const {return trip;}
	void setTrip (const int& trip_){trip = trip_;}
	const int& getMode () const {return mode;}
	void setMode (const int& mode_){mode = mode_;}
	const int& getType () const {return type;}
	void setType (const int& type_){type = type_;}
	const double& getSchedule () const {return schedule;}
	void setSchedule (const double& schedule_){schedule = schedule_;}
	const double& getActual () const {return actual;}
	void setActual (const double& actual_){actual = actual_;}
	const int& getLink () const {return link;}
	void setLink (const int& link_){link = link_;}
	const int& getDir () const {return dir;}
	void setDir (const int& dir_){dir = dir_;}
	const int& getLane () const {return lane;}
	void setLane (const int& lane_){lane = lane_;}
	const double& getOffset () const {return offset;}
	void setOffset (const double& offset_){offset = offset_;}
	const int& getRoute () const {return route;}
	void setRoute (const int& route_){route = route_;}
	const unsigned long& getPrimaryKey () const {return auto_id;}
	const unsigned long& getAuto_id () const {return auto_id;}

//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	unsigned long auto_id;
	int hhold;
	int person;
	int tour;
	int trip;
	int mode;
	int type;
	double schedule;
	double actual;
	int link;
	int dir;
	int lane;
	double offset;
	int route;

};
#pragma db object
class Link_Probability
{
public:
	// Default Constructor
	Link_Probability () {}        
	Link_Probability (int id_, int dir_link_from_, int dir_link_to_, int count_, float prob_)
		: id (id_), dir_link_from (dir_link_from_), dir_link_to (dir_link_to_), count (count_), prob (prob_)
	{
	}
	//Accessors
	const int& getId () const {return id;}
	void setId (const int& id_) {id = id_;}
	const int& getDir_Link_From () const {return dir_link_from;}
	void setDir_Link_From (const int& dir_link_from_) {dir_link_from = dir_link_from_;}
	const int& getDir_Link_To () const {return dir_link_to;}
	void setDir_Link_To (const int& dir_link_to_) {dir_link_to = dir_link_to_;}
	const int& getCount () const {return count;}
	void setCount (const int& count_) {count = count_;}
	const float& getProb () const {return prob;}
	void setProb (const float& prob_) {prob = prob_;}
	//Data Fields
private:
	friend class odb::access;
#pragma db auto id
	int id;
	int dir_link_from;
	int dir_link_to;
	int count;
	float prob;
};
#pragma db object
class LinkMOE
{
public:
	// Default Constructor
	LinkMOE () {}        
	LinkMOE (int id_, int link_uid_, int link_type, float link_length, int start_time_, int end_time_, float link_travel_time_, float link_travel_time_standard_deviation_, float link_queue_length_, float link_travel_delay_, float link_travel_delay_standard_deviation_, float link_speed_, float link_density_, float link_in_flow_rate_, float link_out_flow_rate_, float link_in_volume_, float link_out_volume_, float link_speed_ratio_, float link_in_flow_ratio_, float link_out_flow_ratio_, float link_density_ratio_, float link_travel_time_ratio_, float num_vehicles_in_link_)
		: id (id_), link_uid (link_uid_), link_type (link_type), link_length (link_length), start_time (start_time_), end_time (end_time_), link_travel_time (link_travel_time_), link_travel_time_standard_deviation (link_travel_time_standard_deviation_), link_queue_length (link_queue_length_), link_travel_delay (link_travel_delay_), link_travel_delay_standard_deviation (link_travel_delay_standard_deviation_), link_speed (link_speed_), link_density (link_density_), link_in_flow_rate (link_in_flow_rate_), link_out_flow_rate (link_out_flow_rate_), link_in_volume (link_in_volume_), link_out_volume (link_out_volume_), link_speed_ratio (link_speed_ratio_), link_in_flow_ratio (link_in_flow_ratio_), link_out_flow_ratio (link_out_flow_ratio_), link_density_ratio (link_density_ratio_), link_travel_time_ratio (link_travel_time_ratio_), num_vehicles_in_link (num_vehicles_in_link_)
	{
	}
	//Accessors
	const int& getId () const {return id;}
	void setId (const int& id_) {id = id_;}
	const int& getLink_Uid () const {return link_uid;}
	void setLink_Uid (const int& link_uid_) {link_uid = link_uid_;}
	const int& getLink_Type () const {return link_type;}
	void setLink_Type (const int& link_type_) {link_type = link_type_;}
	const float& getLink_Length () const {return link_length;}
	void setLink_Length (const float& link_length_) {link_length = link_length_;}
	const int& getStart_Time () const {return start_time;}
	void setStart_Time (const int& start_time_) {start_time = start_time_;}
	const int& getEnd_Time () const {return end_time;}
	void setEnd_Time (const int& end_time_) {end_time = end_time_;}
	const float& getLink_Travel_Time () const {return link_travel_time;}
	void setLink_Travel_Time (const float& link_travel_time_) {link_travel_time = link_travel_time_;}
	const float& getLink_Travel_Time_Standard_Deviation () const {return link_travel_time_standard_deviation;}
	void setLink_Travel_Time_Standard_Deviation (const float& link_travel_time_standard_deviation_) {link_travel_time_standard_deviation = link_travel_time_standard_deviation_;}
	const float& getLink_Queue_Length () const {return link_queue_length;}
	void setLink_Queue_Length (const float& link_queue_length_) {link_queue_length = link_queue_length_;}
	const float& getLink_Travel_Delay () const {return link_travel_delay;}
	void setLink_Travel_Delay (const float& link_travel_delay_) {link_travel_delay = link_travel_delay_;}
	const float& getLink_Travel_Delay_Standard_Deviation () const {return link_travel_delay_standard_deviation;}
	void setLink_Travel_Delay_Standard_Deviation (const float& link_travel_delay_standard_deviation_) {link_travel_delay_standard_deviation = link_travel_delay_standard_deviation_;}
	const float& getLink_Speed () const {return link_speed;}
	void setLink_Speed (const float& link_speed_) {link_speed = link_speed_;}
	const float& getLink_Density () const {return link_density;}
	void setLink_Density (const float& link_density_) {link_density = link_density_;}
	const float& getLink_In_Flow_Rate () const {return link_in_flow_rate;}
	void setLink_In_Flow_Rate (const float& link_in_flow_rate_) {link_in_flow_rate = link_in_flow_rate_;}
	const float& getLink_Out_Flow_Rate () const {return link_out_flow_rate;}
	void setLink_Out_Flow_Rate (const float& link_out_flow_rate_) {link_out_flow_rate = link_out_flow_rate_;}
	const float& getLink_In_Volume () const {return link_in_volume;}
	void setLink_In_Volume (const float& link_in_volume_) {link_in_volume = link_in_volume_;}
	const float& getLink_Out_Volume () const {return link_out_volume;}
	void setLink_Out_Volume (const float& link_out_volume_) {link_out_volume = link_out_volume_;}
	const float& getLink_Speed_Ratio () const {return link_speed_ratio;}
	void setLink_Speed_Ratio (const float& link_speed_ratio_) {link_speed_ratio = link_speed_ratio_;}
	const float& getLink_In_Flow_Ratio () const {return link_in_flow_ratio;}
	void setLink_In_Flow_Ratio (const float& link_in_flow_ratio_) {link_in_flow_ratio = link_in_flow_ratio_;}
	const float& getLink_Out_Flow_Ratio () const {return link_out_flow_ratio;}
	void setLink_Out_Flow_Ratio (const float& link_out_flow_ratio_) {link_out_flow_ratio = link_out_flow_ratio_;}
	const float& getLink_Density_Ratio () const {return link_density_ratio;}
	void setLink_Density_Ratio (const float& link_density_ratio_) {link_density_ratio = link_density_ratio_;}
	const float& getLink_Travel_Time_Ratio () const {return link_travel_time_ratio;}
	void setLink_Travel_Time_Ratio (const float& link_travel_time_ratio_) {link_travel_time_ratio = link_travel_time_ratio_;}
	const float& getNum_Vehicles_In_Link () const {return num_vehicles_in_link;}
	void setNum_Vehicles_In_Link (const float& num_vehicles_in_link_) {num_vehicles_in_link = num_vehicles_in_link_;}
	//Data Fields
private:
	friend class odb::access;
#pragma db auto id
	int id;
#pragma db not_null
	int link_uid;
	int link_type;
	float link_length;
	int start_time;
	int end_time;
	float link_travel_time;
	float link_travel_time_standard_deviation;
	float link_queue_length;
	float link_travel_delay;
	float link_travel_delay_standard_deviation;
	float link_speed;
	float link_density;
	float link_in_flow_rate;
	float link_out_flow_rate;
	float link_in_volume;
	float link_out_volume;
	float link_speed_ratio;
	float link_in_flow_ratio;
	float link_out_flow_ratio;
	float link_density_ratio;
	float link_travel_time_ratio;
	float num_vehicles_in_link;
};
}//end of io namespace
}//end of polaris namespace
#endif // IOResult