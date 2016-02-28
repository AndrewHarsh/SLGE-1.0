#pragma once
#include <SLGE.h>
#include "User Interface.h"
#include "Menu.h"
using namespace SLGE;


class menuGameOptions_t : public Menu_t
{
protected:

	//Used to clear ALL data from a class (use wisely)
	virtual void ClearData()
	{
		//Todo: Add code to clear all attributes of the class
		Menu_t::ClearData();
	}

public:

	buttonGrey_t Button[6];

	//Calls all the constructors that need to be called
	menuGameOptions_t() : Menu_t()
	{
		ClearData();
	}

	//Associates the object with a window for later use. MUST be done either in a constructor (below) or by directly calling the "Register()" method
	menuGameOptions_t(Window_t *in_Window) : menuGameOptions_t()
	{
		Register(in_Window);

		for (int i = 0; i < 6; i++)
			Button[i].Register(in_Window);
	}

	virtual void Init()
	{
		//Menu_t::Init();

		for (int i = 0; i < 6; i++)
			Button[i].SetCoords(WindowHandle->GetWidth() / 2, WindowHandle->GetHeight() / 2 + 70 * i - 170);

		Button[0].Init("Fencer");
		Button[1].Init("Barbarian");
		Button[2].Init("Archer");
		Button[3].Init("Paladin");
		Button[4].Init("Mage");
		Button[5].Init("Back");
	}

	virtual int Display()
	{
		int Ret = 0;

		for (int i = 0; i < 6; i++)
		{
			if (Ret = Button[i].Display())
				return Ret;
		}

		return Ret;
	}
};