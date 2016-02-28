#pragma once
#include <SLGE.h>
#include "User Interface.h"
#include "Button.h"
using namespace SLGE;


class buttonGrey_t : public Button_t
{
protected:

	//Used to clear ALL data from a class (use wisely)
	virtual void ClearData()
	{
		//Todo: Add code to clear all attributes of the class
		Button_t::ClearData();
	}

public:

	//Calls all the constructors that need to be called
	buttonGrey_t() : Button_t()
	{
		ClearData();
	}

	//Associates the object with a window for later use. MUST be done either in a constructor (below) or by directly calling the "Register()" method
	buttonGrey_t(Window_t *in_Window) : buttonGrey_t()
	{
		Register(in_Window);
	}

	void Init(std::string in_Message)
	{
		AddImage("Main Menu Button.png", { 0, 0, 300, 50 }, { 255, 255, 255 });
		AddImage("Main Menu Button.png", { 300, 0, 300, 50 }, { 255, 255, 255 });
		AddImage("Main Menu Button.png", { 600, 0, 300, 50 }, { 255, 255, 255 });

		Button_t::Init(in_Message);
	}
};

