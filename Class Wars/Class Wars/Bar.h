#pragma once
#include <SLGE.h>
#include "User Interface.h"
using namespace SLGE;


//Base class for all bars
class Bar_t : public UI_t
{
protected:

	int StartWidth;

	virtual void ClearData()
	{
		StartWidth = 0;

		UI_t::ClearData();
	}

	void SetImage(int in_ImageIndex)
	{
		if (in_ImageIndex != 1)
			Object_t::SetImage(in_ImageIndex);
		else if (in_ImageIndex >= 0 && in_ImageIndex < (int) Image->size())
			(*Image)[in_ImageIndex].SetCoords(X - W / 2, Y - (*Image)[in_ImageIndex].H() / 2);
	}

public:

	Bar_t() : UI_t()
	{
		ClearData();
	}

	Bar_t(Window_t *in_Window) : Bar_t()
	{
		Register(in_Window);
	}

	virtual void Init()
	{
		StartWidth = (*Image)[0].W();
	}

	void SetPercentage(double in_Percentage)
	{ 
		if (in_Percentage >= 0 && in_Percentage <= 1)
			(*Image)[1].SetSize((int) (StartWidth * in_Percentage), (int) H);
	}
};
 