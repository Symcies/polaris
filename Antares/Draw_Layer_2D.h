//*********************************************************
//	Draw_Layer.h - Function for drawing a general layer
//*********************************************************

#pragma once
#include "Information_Page_Implementation.h"

//---------------------------------------------------------
//	Draw_Layer
//--------------------------------------------------------

template<typename MasterType,typename ParentType,typename InheritanceList>
template<typename ComponentType,typename CallerType,typename TargetType>
void Information_Page_Implementation<MasterType,ParentType,InheritanceList>::Draw_Layer(const int start_iteration, const int end_iteration)
{
	const bool draw=_layer->draw<bool>();

	// skip hidden layers
	if(!draw) return;

	const PrimitiveType primitive_type=_layer->primitive_type<PrimitiveType>();	
	
	assert(primitive_type==_PLOT);


	PLFLT xmin, xmax, ymin, ymax;
	xmin = ymin = FLT_MAX;
	xmax = ymax = -FLT_MAX;

	const True_Color_RGBA<MasterType> head_color=_layer->head_color<True_Color_RGBA<MasterType>>();
	const int head_size_value=_layer->head_size_value<int>();
	
	const bool grouped=_layer->grouped<bool>();
	const bool group_color=_layer->group_color<bool>();

	const int primitive_stride = _layer->primitive_stride<int>();

	const int vert_size = _layer->vert_size<int>();
	const int vert_stride = _layer->vert_stride<int>();

	const int data_stride=_layer->data_stride<int>();
	
	const Dynamic_Multi_Buffer< vector<unsigned char>[_num_antares_threads] >& storage=_layer->storage<Dynamic_Multi_Buffer< vector<unsigned char>[_num_antares_threads] >&>();

	//---- determine bounds for main layer ----

	int current_iteration=start_iteration;
	
	PLFLT point_value;

	while(current_iteration <= end_iteration)
	{
		const vector<unsigned char> (&geometry_by_thread)[_num_antares_threads] = storage[current_iteration];

		for(int i=0;i<_num_antares_threads;i++)
		{
			const unsigned char* geometry_itr = &geometry_by_thread[i].front();
			const unsigned char* const geometry_end = geometry_itr+geometry_by_thread[i].size();

			while(geometry_itr != geometry_end)
			{
				geometry_itr += data_stride;

				if(group_color)
				{
					geometry_itr += sizeof(True_Color_RGBA<MasterType>);
				}

				const int num_group_primitives=*((int*)geometry_itr);

				geometry_itr += sizeof(int);

				const unsigned char* const group_x_end = geometry_itr + (sizeof(PLFLT) * num_group_primitives);

				//Scan x points
				while( geometry_itr != group_x_end )
				{
					point_value = *((PLFLT*)geometry_itr);

					xmin = ( ( xmin ) < ( point_value ) ? ( xmin ) : ( point_value ) );
					xmax = ( ( xmax ) < ( point_value ) ? ( point_value ) : ( xmax ) );

					geometry_itr += sizeof(PLFLT);
				}

				const unsigned char* const group_y_end = geometry_itr + (sizeof(PLFLT) * num_group_primitives);

				//Scan y points
				while( geometry_itr != group_y_end )
				{
					point_value = *((PLFLT*)geometry_itr);

					ymin = ( ( ymin ) < ( point_value ) ? ( ymin ) : ( point_value ) );
					ymax = ( ( ymax ) < ( point_value ) ? ( point_value ) : ( ymax ) );

					geometry_itr += sizeof(PLFLT);
				}
			}
		}

		current_iteration++;
	}

	const Dynamic_Multi_Buffer< vector<unsigned char>[_num_antares_threads] >& accent_storage=_layer->accent_storage<Dynamic_Multi_Buffer< vector<unsigned char>[_num_antares_threads] >&>();

	//---- determine bounds for accents ----

	current_iteration=start_iteration;
	
	while(current_iteration <= end_iteration)
	{
		const vector<unsigned char> (&geometry_by_thread)[_num_antares_threads] = accent_storage[current_iteration];

		for(int i=0;i<_num_antares_threads;i++)
		{
			const unsigned char* geometry_itr = &geometry_by_thread[i].front();
			const unsigned char* const geometry_end = geometry_itr+geometry_by_thread[i].size();

			while(geometry_itr != geometry_end)
			{
				geometry_itr += data_stride;

				if(group_color)
				{
					geometry_itr += sizeof(True_Color_RGBA<MasterType>);
				}

				const int num_group_primitives=*((int*)geometry_itr);

				geometry_itr += sizeof(int);

				const unsigned char* const group_x_end = geometry_itr + (sizeof(PLFLT) * num_group_primitives);

				//Scan x points
				while( geometry_itr != group_x_end )
				{
					point_value = *((PLFLT*)geometry_itr);

					xmin = ( ( xmin ) < ( point_value ) ? ( xmin ) : ( point_value ) );
					xmax = ( ( xmax ) < ( point_value ) ? ( point_value ) : ( xmax ) );

					geometry_itr += sizeof(PLFLT);
				}

				const unsigned char* const group_y_end = geometry_itr + (sizeof(PLFLT) * num_group_primitives);

				//Scan y points
				while( geometry_itr != group_y_end )
				{
					point_value = *((PLFLT*)geometry_itr);

					ymin = ( ( ymin ) < ( point_value ) ? ( ymin ) : ( point_value ) );
					ymax = ( ( ymax ) < ( point_value ) ? ( point_value ) : ( ymax ) );

					geometry_itr += sizeof(PLFLT);
				}
			}
		}

		current_iteration++;
	}

	wxPLplotstream* pls = _plotwindow->GetStream();
	//wxPLplotstream* pls = GetStream();

	//pls->adv( 0 );
	//pls->scol0( 0, 255, 255, 255 );
	//pls->scol0( 15, 0, 0, 0 );

	xmax = max(xmax,1.0);

	ymax += 1;
	ymax *= 1.2;

	pls->col0( 1 );
	pls->env( xmin, xmax, ymin, ymax, 0, 0 );

	//pls->col0( 15 );
	//pls->lab( "Iteration", "Value", _layer->name<string&>().c_str() );

	pls->scol0( 3, head_color._r, head_color._g, head_color._b );
	pls->col0( 3 );

	pls->width( 2 );

	current_iteration=start_iteration;

	//---- draw main layer ----

	while(current_iteration <= end_iteration)
	{
		const vector<unsigned char> (&geometry_by_thread)[_num_antares_threads] = storage[current_iteration];

		for(int i=0;i<_num_antares_threads;i++)
		{
			const unsigned char* geometry_itr = &geometry_by_thread[i].front();
			const unsigned char* const geometry_end = geometry_itr+geometry_by_thread[i].size();

			while(geometry_itr != geometry_end)
			{
				geometry_itr += data_stride;

				if(group_color)
				{
					glColor4ubv((GLubyte*)geometry_itr);
					geometry_itr += sizeof(True_Color_RGBA<MasterType>);
				}

				const int num_group_primitives=*((int*)geometry_itr);

				geometry_itr += sizeof(int);

				const PLFLT* const x_points = (const PLFLT* const)geometry_itr;
				geometry_itr+=(sizeof(PLFLT) * num_group_primitives);

				const PLFLT* const y_points = (const PLFLT* const)geometry_itr;
				geometry_itr+=(sizeof(PLFLT) * num_group_primitives);

				pls->line( num_group_primitives, x_points, y_points );
			}
		}

		current_iteration++;
	}

	pls->scol0( 4, 255-head_color._r, 255-head_color._g, 255-head_color._b );
	pls->col0( 4 );

	current_iteration=start_iteration;

	//---- draw accented layer ----

	while(current_iteration <= end_iteration)
	{
		const vector<unsigned char> (&geometry_by_thread)[_num_antares_threads] = accent_storage[current_iteration];

		for(int i=0;i<_num_antares_threads;i++)
		{
			const unsigned char* geometry_itr = &geometry_by_thread[i].front();
			const unsigned char* const geometry_end = geometry_itr+geometry_by_thread[i].size();

			while(geometry_itr != geometry_end)
			{
				geometry_itr += data_stride;

				if(group_color)
				{
					glColor4ubv((GLubyte*)geometry_itr);
					geometry_itr += sizeof(True_Color_RGBA<MasterType>);
				}

				const int num_group_primitives=*((int*)geometry_itr);

				geometry_itr += sizeof(int);

				const PLFLT* const x_points = (const PLFLT* const)geometry_itr;
				geometry_itr+=(sizeof(PLFLT) * num_group_primitives);

				const PLFLT* const y_points = (const PLFLT* const)geometry_itr;
				geometry_itr+=(sizeof(PLFLT) * num_group_primitives);

				pls->line( num_group_primitives, x_points, y_points );
			}
		}

		current_iteration++;
	}

	//---- flush and display ----

	_plotwindow->RenewPlot();
	//RenewPlot();
}