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
	virtual int Limit_Shape_Selection()
	{
		std::vector <int> Shape_Limitations;

		srand(time(NULL));

		//Select between 0 and 3 random to be excluded from use on the floor
		for (int f = 0; f < (rand() % 3); f++)
		{
			Shape_Limitations.push_back(rand());
		}

		return 0;
	}

	int Create_L(int a)
	{
		std::cout << "Creating L";
		return a;
	}

	int Create_T(int a)
	{
		return a;
	}

	int Create_Hallway(int a)
	{
		return a;
	}

	int Create_Courtyard(int a)
	{
		return a;
	}

	int Create_BanquetHall(int a)
	{
		return a;
	}

	int Create_FigureEight(int a)
	{
		return a;
	}

	int Create_BridgeRoom(int a)
	{
		return a;
	}



	//Selects room's shape type
	virtual int Select_Shape()
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
		
		return 0;
	}
};

