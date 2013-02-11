//*********************************************************
//	Information_Panel_Implementation.h - Information and Plotting
//*********************************************************

#pragma once
#include "Information_Panel.h"

//---------------------------------------------------------
//	Information_Panel_Implementation - information panel class definition
//---------------------------------------------------------

implementation class Information_Panel_Implementation : public Polaris_Component_Class<Information_Panel_Implementation,MasterType>,public wxPanel
{
public:
	Information_Panel_Implementation(wxFrame* parent);
	virtual ~Information_Panel_Implementation(void){};
	
	typedef Antares_Layer<typename type_of(MasterType::antares_layer),Information_Panel_Implementation> Antares_Layer_Interface;

	feature_implementation Antares_Layer_Interface* Allocate_New_Layer(string& name)
	{
		Antares_Layer_Interface* new_layer=(Antares_Layer_Interface*)Allocate<typename type_of(MasterType::antares_layer)>();

		_2D_layers.push_back(new_layer);

		new_layer->list_index<int>(_2D_layers.size() - 1);
		new_layer->name<string&>(name);

		return new_layer;
	}
	
	feature_implementation void Render();
	void Draw_Layer(const int start_iteration, const int end_iteration, Antares_Layer_Interface* layer);
	void OnResize(wxSizeEvent& event);

	member_pointer(wxPLplotwindow,plotwindow,none,none);
	member_pointer(wxBoxSizer,box,none,none);
	
	member_data(int,cached_iteration,none,none);

	list<Antares_Layer_Interface*> _2D_layers;
};

//---------------------------------------------------------
//	Information_Panel - information initialization
//---------------------------------------------------------

template<typename MasterType,typename ParentType>
Information_Panel_Implementation<MasterType,ParentType>::Information_Panel_Implementation(wxFrame* parent) : wxPanel(parent,-1,wxDefaultPosition,wxDefaultSize,wxCLIP_CHILDREN )
{
	//---- miscellaneous initialization ----

	_box = new wxBoxSizer( wxVERTICAL );

	_plotwindow = new wxPLplotwindow( this, -1, wxDefaultPosition, wxSize(1920,1080), wxWANTS_CHARS, wxPLPLOT_BACKEND_GC | wxPLPLOT_DRAW_TEXT );

	_plotwindow->SetMaxSize( wxSize(1920,1080) );

	_box->Add( _plotwindow, 1, wxEXPAND );
	
	SetSizer(_box);

	Connect(wxEVT_SIZE,wxSizeEventHandler(Information_Panel_Implementation::OnResize));
}

template<typename MasterType,typename ParentType>
void Information_Panel_Implementation<MasterType,ParentType>::OnResize(wxSizeEvent& event)
{
	_box->SetDimension(wxPoint(0,0),GetSize());
}

//class MyPlotwindow : public wxPLplotwindow
//{
//public:
//    MyPlotwindow( wxWindow* parent, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition,
//                  const wxSize& size = wxDefaultSize, long style = 0,
//                  int pl_style = wxPLPLOT_NONE ) :
//	wxPLplotwindow( parent, id, pos, size, style, pl_style ){};
//};


//template<typename MasterType,typename ParentType>
//void Information_Panel_Implementation<MasterType,ParentType>::Plot()
//{
//	const size_t np = 500;
//	PLFLT x[np], y[np];
//	PLFLT xmin, xmax;
//	PLFLT ymin = 1e30, ymax = 1e-30;
//
//	xmin = 0;
//	xmax = _iteration + 5;
//
//	for ( size_t i = 0; i < np; i++ )
//	{
//		x[i] = ( xmax - xmin ) * i / np + xmin;
//		y[i] = 1.0;
//		if ( x[i] != 0.0 ) y[i] = sin( x[i] ) / x[i] + x[i];
//		
//		ymin = ( ( ymin ) < ( y[i] ) ? ( ymin ) : ( y[i] ) );
//		ymax = ( ( ymax ) < ( y[i] ) ? ( y[i] ) : ( ymax ) );
//	}
//
//	wxPLplotstream* pls = _plotwindow->GetStream();
//
//	pls->adv( 0 );
//	pls->scol0( 0, 255, 255, 255 );
//	pls->scol0( 15, 0, 0, 0 );
//
//	pls->col0( 1 );
//	pls->env( xmin, xmax, ymin, ymax, 0, 0 );
//	pls->col0( 15 );
//	pls->lab( "Iteration", "Value", "Vehicles In Network" );
//
//	pls->col0( 3 );
//	pls->width( 2 );
//
//	//pls->hist(np,y,ymin,ymax,10,PL_HIST_DEFAULT);
//
//	pls->line( np, x, y );
//
//	_plotwindow->RenewPlot();
//}





//template<typename MasterType,typename ParentType>
//void Information_Panel_Implementation<MasterType,ParentType>::Plot()
//{
//    const size_t np = 500;
//    PLFLT x[np], y[np];
//    PLFLT xmin, xmax;
//    PLFLT ymin = 1e30, ymax = 1e-30;
//
//    xmin = -2.0;
//    xmax = 10.0;
//    for ( size_t i = 0; i < np; i++ )
//    {
//        x[i] = ( xmax - xmin ) * i / np + xmin;
//        y[i] = 1.0;
//        if ( x[i] != 0.0 ) y[i] = sin( x[i] ) / x[i];
//
//		ymin = ( ( ymin ) < ( y[i] ) ? ( ymin ) : ( y[i] ) );
//		ymax = ( ( ymax ) < ( y[i] ) ? ( y[i] ) : ( ymax ) );
//    }
//    
//	wxPLplotstream* pls = _plotwindow->GetStream();
//
//    pls->adv( 0 );
//
//    pls->scol0( 0, 255, 255, 255 );
//    pls->scol0( 15, 0, 0, 0 );
//
//    pls->col0( 1 );
//    pls->env( xmin, xmax, ymin, ymax, 0, 0 );
//    pls->col0( 2 );
//    pls->lab( "x", "y", "sin(x)/x" );
//
//    pls->col0( 3 );
//    pls->width( 2 );
//    pls->line( np, x, y );
//
//    _plotwindow->RenewPlot();
//}