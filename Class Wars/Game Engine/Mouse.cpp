#include "SLGE.h"

#define DLL_API __declspec(dllexport) 
using namespace SLGE;


int DLL_API Window_t::Mouse_t::GetX()
{
	return X;
}

int DLL_API Window_t::Mouse_t::GetY()
{
	return Y;
}

int DLL_API Window_t::Mouse_t::GetLastX()
{
	return LastX;
}

int DLL_API Window_t::Mouse_t::GetLastY()
{
	return LastY;
}

int DLL_API Window_t::Mouse_t::GetRelativeX()
{
	return X - LastX;
}

int DLL_API Window_t::Mouse_t::GetRelativeY()
{
	return Y - LastY;
}


bool DLL_API Window_t::Mouse_t::HasMovedHorizontally()
{
	if (LastX != X)
		return true;
	else
		return false;
}

bool DLL_API Window_t::Mouse_t::HasMovedVertically()
{
	if (LastY != Y)
		return true;
	else
		return false;
}

bool DLL_API Window_t::Mouse_t::HasMoved()
{
	if (HasMovedHorizontally() || HasMovedVertically())
		return true;
	else
		return false;
}

bool DLL_API Window_t::Mouse_t::IsWithin(SDL_Rect in_Area)
{
	if (X >= in_Area.x - in_Area.w / 2 &&
		X <= in_Area.x + in_Area.w / 2 &&
		Y >= in_Area.y - in_Area.h / 2 &&
		Y <= in_Area.y + in_Area.h / 2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool DLL_API Window_t::Mouse_t::IsWithin(Object_t* in_Object)
{
	if (X >= in_Object->GetX() - in_Object->GetW() / 2 &&
		X <= in_Object->GetX() + in_Object->GetW() / 2 &&
		Y >= in_Object->GetY() - in_Object->GetH() / 2 &&
		Y <= in_Object->GetY() + in_Object->GetH() / 2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

const Window_t::Mouse_t::Button_t DLL_API Window_t::Mouse_t::operator[](BUTTON::Buttons in_Button) const
{
	return Button[in_Button];
}	

const Window_t::Mouse_t::Wheel_t DLL_API Window_t::Mouse_t::operator[](WHEEL::Wheels Wheel) const
{
	return this->Wheel;
}

#undef DLL_API