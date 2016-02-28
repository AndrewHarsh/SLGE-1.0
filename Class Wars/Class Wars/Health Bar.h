#pragma once
#include <SLGE.h>
#include "User Interface.h"
#include "Bar.h"
using namespace SLGE;


class HealthBar_t : public Bar_t
{
protected:

	virtual void ClearData()
	{
		StartWidth = 0;

		Bar_t::ClearData();
	}

public:

	HealthBar_t() : Bar_t()
	{
		ClearData();
	}

	HealthBar_t(Window_t *in_Window) : HealthBar_t()
	{
		Register(in_Window);
	}
};