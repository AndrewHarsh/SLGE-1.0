#pragma once
#include <SLGE.h>
using namespace SLGE;


class Floor_t : public Object_t
{
protected:

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
	};

	//Used to clear ALL data from a class (use wisely)
	void ClearData()
	{
		//Todo: Add code to clear all attributes of the class
		Object_t::ClearData();
	}

public:

	Room_t Rooms[10][10];

	//Calls all the constructors that need to be called
	Floor_t() : Object_t()
	{
		ClearData();
	}

	//Associates the object with a window for later use. MUST be done either in a constructor (below) or by directly calling the "Register()" method
	Floor_t(Window_t *in_Window) : Floor_t()
	{
		Register(in_Window);
	}

	virtual int Register(Window_t *in_Window)
	{
		int Ret = 0;

		if (Ret = Object_t::Register(in_Window))
			return Ret;

		for (int i = 0; i < 10; i++)
		{
			for (int ii = 0; ii < 10; ii++)
			{
				if (Ret = Rooms[i][ii].Register(in_Window))
					return Ret;
			}
		}

		return Ret;
	}

	virtual int Display()
	{
		int Ret = 0; 

		if (Ret = Object_t::Display())
			return Ret;

		for (int i = 0; i < 10; i++)
		{
			for (int ii = 0; ii < 10; ii++)
			{
				if (Ret = Rooms[i][ii].Display())
					return Ret;
			}
		}

		return Ret;
	}
};




