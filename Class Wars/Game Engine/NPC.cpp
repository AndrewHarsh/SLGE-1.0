#include "SLGE.h"

#define DLL_API __declspec(dllexport) 
using namespace SLGE;


DLL_API NPC::NPC()
{
	ClearData();
}

DLL_API NPC::NPC(Window *in_WindowHandle) : NPC()
{
	Register(in_WindowHandle);
}

void DLL_API NPC::ClearData()
{	 
	Entity::ClearData();
}


int DLL_API NPC::Wander(const SDL_Rect in_WanderArea, const double in_RestTime)
{
	return 0;
}
		
int DLL_API NPC::Follow(Object *in_Leader, const double in_Distance)
{
	if (in_Leader->GetX() < X - in_Distance)
		Move(Left);
	else if (in_Leader->GetX() > X + in_Distance)
		Move(Right);
	if (in_Leader->GetY() < Y - in_Distance)
		Move(Up);
	else if (in_Leader->GetY() > Y + in_Distance)
		Move(Down);


	/*
	if (in_Leader->GetX() < X + in_Distance)
	{
		if (in_Leader->GetY() < Y + in_Distance)
			Move(UpLeft);
		else if (in_Leader->GetY() > Y - in_Distance)
			Move(DownLeft);
		else
			Move(Left);
	}
	else if (in_Leader->GetX() > X - in_Distance)
	{
		if (in_Leader->GetY() < Y + in_Distance)
			Move(UpRight);
		else if (in_Leader->GetY() > Y - in_Distance)
			Move(DownRight);
		else
			Move(Right);
	}
	
	if (in_Leader->GetY() < Y + in_Distance)
		Move(Up);
	else if (in_Leader->GetY() > Y - in_Distance)
		Move(Down);
	*/

	return 0;
}

#undef DLL_API