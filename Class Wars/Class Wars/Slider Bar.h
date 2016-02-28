#pragma once
#include <SLGE.h>
#include "User Interface.h"
#include "Bar.h"
using namespace SLGE;


class SliderBar_t : public Bar_t
{
protected:

	virtual void ClearData()
	{
		StartWidth = 0;

		Bar_t::ClearData();
	}

public:

	SliderBar_t() : Bar_t()
	{
		ClearData();
	}

	SliderBar_t(Window_t *in_Window) : SliderBar_t()
	{
		Register(in_Window);
	}


};