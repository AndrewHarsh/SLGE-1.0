#pragma once
#include <SLGE.h>
#include "User Interface.h"
using namespace SLGE;


//Base class for all menus
class Menu_t : public UI_t
{
protected:

	virtual void ClearData()
	{
		UI_t::ClearData();
	}

public:

	Menu_t() : UI_t()
	{
		ClearData();
	}

	Menu_t(Window_t *in_Window) : Menu_t()
	{
		Register(in_Window);
	}

	virtual int Display()
	{
		int Ret = 0;

		if (Ret = UI_t::Display())
			return Ret;

		return Ret;
	}

	virtual void Init()
	{

	}
};