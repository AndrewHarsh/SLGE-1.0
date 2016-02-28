#include "SLGE.h"

#define DLL_API __declspec(dllexport) 
using namespace SLGE;


DLL_API NPC::NPC()
{
	ClearData();
}

DLL_API NPC::NPC(Window *in_WindowHandle)
{
	WindowHandle = in_WindowHandle;

	NPC::NPC();
}

void DLL_API NPC::ClearData()
{
	Entity::ClearData();
}


int DLL_API NPC::Wander(const SDL_Rect in_WanderArea, const double in_RestTime)
{
	return 0;
}
		
int DLL_API NPC::Follow(const Object *in_Leader, const double in_Distance)
{
	return 0;
}

#undef DLL_API