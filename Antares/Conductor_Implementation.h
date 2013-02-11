//*********************************************************
//	Conductor_Implementation.h - Controls Visualization Progression
//*********************************************************

#pragma once
#include "Conductor.h"

//---------------------------------------------------------
//	Conductor_Implementation - conductor class definition
//---------------------------------------------------------

implementation class Conductor_Implementation : public Polaris_Component_Class<Conductor_Implementation,MasterType,Execution_Object,ParentType>
{
public:
	define_component_interface(Graphical_Network_Interface,typename MasterType::type_of(graphical_network),Network_Prototype,Conductor_Implementation);

	member_prototype(Canvas,canvas,typename MasterType::type_of(canvas),Conductor_Implementation,none,none);
	member_prototype(Information_Panel,information_panel,typename MasterType::type_of(information_panel),Conductor_Implementation,none,none);
	//member_prototype(Graphical_Network,graphical_network,typename MasterType::type_of(graphical_network),Conductor_Implementation,none,none);

	feature_implementation void Initialize()
	{
		Load_Register<Conductor_Implementation>(&advance_simulation_condition<NULLTYPE>,&advance_simulation<NULLTYPE>,0,Scenario_Components::Types::END_OF_ITERATION+1);
		_pause=true;
	}

	declare_feature_conditional(advance_simulation_condition)
	{
		response.result=true;
		response.next._iteration=_iteration+1;
		response.next._sub_iteration=Scenario_Components::Types::END_OF_ITERATION+1;
	}

	declare_feature_event(advance_simulation)
	{
		Conductor_Implementation* pthis=(Conductor_Implementation*)_this;

		if(pthis->_pause)
		{
			while(pthis->_pause) Sleep(10);
		}
		else
		{
			//Sleep(100);
		}

		Graphical_Network_Interface* graphical_net=pthis->_canvas->graphical_network<Graphical_Network_Interface*>();

		cout << _iteration << endl;
		
		graphical_net->submit_num_vehicles<NULLTYPE>();

		pthis->_canvas->Refresh<NULLTYPE>();

		pthis->_information_panel->Render<NULLTYPE>();
	}

	member_data(bool,pause,none,none);

};