#pragma once
#include <SLGE.h>
using namespace SLGE;


//Base class for all User Interface objects
class UI_t : public Object_t
{
protected:

	virtual void ClearData()
	{
		Object_t::ClearData();
	}

public:

	UI_t() : Object_t()
	{
		ClearData();
	}

	UI_t(Window_t *in_Window) : UI_t()
	{
		Register(in_Window);
	}

	virtual void Init()
	{

	}
};

