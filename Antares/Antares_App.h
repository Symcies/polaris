//*********************************************************
//	Antares_App.h - Antares Application Initialization
//*********************************************************

#pragma once
#include "Antares_Frame_Implementation.h"

//---------------------------------------------------------
//	START_UI - macro to start the UI
//---------------------------------------------------------

#define START_UI(ARGC,ARGV,MASTER_TYPE) \
	antares=(void*)new Antares_App<MASTER_TYPE>();\
	canvas=nullptr;\
	((Antares_App<MASTER_TYPE>*)antares)->Start_UI(ARGC,ARGV);\

//---------------------------------------------------------
//	Wx_Loop - declare wx loop function
//---------------------------------------------------------

DWORD WINAPI Wx_Loop(LPVOID _app);

//---------------------------------------------------------
//	Antares_App - antares application class
//---------------------------------------------------------

implementation class Antares_App: public wxApp
{
public:
	//---------------------------------------------------------
	//	OnInit - build Antares frame, called after Start_UI
	//---------------------------------------------------------
	
	virtual bool OnInit()
	{
		//if(argc!=2)
		//{
		//	wxMessageBox("Database File Not Provided!");
		//	exit(0);
		//}
		//else
		//{
		//	db_name=argv[1];

		//	ifstream test;
		//	test.open(db_name);

		//	if(!test.is_open())
		//	{
		//		wxMessageBox("Invalid Database File Provided!");
		//		exit(0);
		//	}

		//	test.close();
		//}

		_thread_id = _num_threads;

		main=new Antares_Implementation<MasterType,Antares_App>(nullptr,db_name);

		canvas = main->_canvas;

		main->Maximize();
		main->Show();

		main->_canvas->Initialize<NULLTYPE>();

		return true;
	}
	
	//---------------------------------------------------------
	//	Start_UI - Initialize wx, spin thread to handle events
	//---------------------------------------------------------
	
	void Start_UI(int argc, char** argv)
	{
		wxDISABLE_DEBUG_SUPPORT();

		wxEntryStart(argc,argv);

		if(argc!=2)
		{
			wxMessageBox("Database File Not Provided!");
			exit(0);
		}
		else
		{
			db_name=argv[1];

			//ifstream test;
			//test.open(db_name);

			//if(!test.is_open())
			//{
			//	wxMessageBox("Invalid Database File Provided!");
			//	exit(0);
			//}

			//test.close();
		}

		CreateThread(NULL,0,Wx_Loop<MasterType>,this,0,NULL);
	}

	Antares_Implementation<MasterType,Antares_App>* main;
	string db_name;
};

//---------------------------------------------------------
//	Wx_Loop - define wx loop function
//---------------------------------------------------------

template<typename MasterType>
DWORD WINAPI Wx_Loop(LPVOID _app)
{
	Antares_App<MasterType>* app=(Antares_App<MasterType>*)_app;

	app->CallOnInit();
	app->MainLoop();
	return 1;
}