#pragma once
#include <SLGE.h>
#include "User Interface.h"
#include "Menu.h"
using namespace SLGE;


class menuMain_t : public Menu_t
{
protected:

	//Used to clear ALL data from a class (use wisely)
	virtual void ClearData()
	{
		//Todo: Add code to clear all attributes of the class
		//for (int i = 0; i < 4; i++)
		//	Button[i].ClearData();

		Menu_t::ClearData();
	}

public:

	buttonGrey_t Button[4];

	//Calls all the constructors that need to be called
	menuMain_t() : Menu_t()
	{
		ClearData();
	}

	//Associates the object with a window for later use. MUST be done either in a constructor (below) or by directly calling the "Register()" method
	menuMain_t(Window_t *in_Window) : menuMain_t()
	{
		Register(in_Window);

		for (int i = 0; i < 4; i++)
			Button[i].Register(in_Window);
	}

	void Init()
	{
		AddImage("Main Menu.png", { NULL }, { NULL });

		Button[0].Init("Play");
		Button[1].Init("Options");
		Button[2].Init("Mike's Game");
		Button[3].Init("Exit Game");

		for (int i = 0; i < 4; i++)
			Button[i].SetCoords(WindowHandle->GetWidth() / 2, (WindowHandle->GetHeight() / 2) + (70 * i) - 70);
	}
	
	virtual int Display()
	{
		int Ret = 0;

		if (Ret = Menu_t::Display())
			return Ret;

		for (int i = 0; i < 4; i++)
		{
			if (Ret = Button[i].Display())
				return Ret;
		}

		return Ret;
	}
};

