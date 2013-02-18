//*********************************************************
//	Information_Panel.h - Information and Plotting
//*********************************************************

#pragma once
#include "Antares_Includes.h"

//---------------------------------------------------------
//	Information_Panel - information panel class definition
//---------------------------------------------------------

prototype struct Information_Panel
{
	tag_as_prototype;
	
	feature_prototype void Render()
	{
		this_component()->Render<CallerType,TargetType>();
	}
	
	feature_prototype typename TargetType::ReturnType Allocate_New_Layer(typename TargetType::ParamType name)
	{
		return (TargetType::ReturnType)this_component()->Allocate_New_Layer<CallerType,TargetType>(name);
	}

	//feature_accessor(plotwindow,none,none);
};