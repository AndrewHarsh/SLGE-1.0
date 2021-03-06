#pragma once;
#include <SLGE.h>
#include "Walls.h"

using namespace SLGE;

class Room_t : public Object_t
{
protected:

	//Used to clear ALL data from a class (use wisely)
	void ClearData()
	{
		//Todo: Add code to clear all attributes of the class
		Object_t::ClearData();
	}

public:

	int Room_Selection;

	

	//Calls all the constructors that need to be called
	Room_t() : Object_t()
	{
		ClearData();
	}

	//Associates the object with a window for later use. MUST be done either in a constructor (below) or by directly calling the "Register()" method
	Room_t(Window_t *in_Window) : Room_t()
	{
		Register(in_Window);
	}

	//Takes room width, height, and predetermined shape type.
	virtual void init(int RW, int RH, int Shape)
	{
		
	}

	//Limits the selection of shape types available to the Select_Shape Function
	int Limit_Shape_Selection()
	{
		std::vector <int> Shape_Limitations;

		srand(unsigned(time(NULL)));

		//Select between 0 and 3 random to be excluded from use on the floor
		for (int f = 0; f < (rand() % 3); f++)
		{
			Shape_Limitations.push_back(rand());
		}

		Shape_Limitations[1];
	}

	int Create_L(int a)
	{
		
		printf("Creating L");
		return a;
	}

	int Create_T(int a)
	{
		printf("Creating T");
		return a;
	}

	int Create_Hallway(int a)
	{
		printf("Creating Hallway");
		return a;
	}

	int Create_Courtyard(int a)
	{
		printf("Creating Courtyard");
		return a;
	}

	int Create_BanquetHall(int a)
	{
		printf("Creating BanquetHall");
		return a;
	}

	int Create_FigureEight(int a)
	{
		printf("Creating Hallway");
		return a;
	}

	int Create_BridgeRoom(int a)
	{
		printf("Creating BridgeRoom");
		return a;
	}

	int Select_Shape(int a)
	{
		switch (a)
		{
		case 0:
			Create_L(1);
			break;
		case 1:
			Create_T(1);
			break;
		case 2:
			Create_Hallway(1);
			break;
		case 3:
			Create_Courtyard(1);
			break;
		case 4:
			Create_BanquetHall(1);
			break;
		case 5:
			Create_FigureEight(1);
			break;
		case 6:
			Create_BridgeRoom(1);
			break;

		}

		return 1;
	}
	
	//Selects room's shape type
	//Doesn't work
	/*
	virtual int Select_Shape(int selection)
	{
		typedef int (Room_t::*Array_of_Room_Creation_Functions) (int);
		


		Array_of_Room_Creation_Functions List_of_Shape_Functions[] = 
		{ 
			&Room_t::Create_L, 
			&Room_t::Create_T, 
			&Room_t::Create_Hallway, 
			&Room_t::Create_Courtyard, 
			&Room_t::Create_BanquetHall, 
			&Room_t::Create_FigureEight, 
			&Room_t::Create_BridgeRoom 
		};



		List_of_Shape_Functions[selection];
		printf("PRINT");
		return 1;
	}
	*/
};

