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
	/*
	if (Image != nullptr)
	{
		for (int i = 0; i < NumberOfImages; i++)
			SDL_FreeSurface(Image[i]);

		Image = nullptr;
	}

	if (HImage != nullptr)
	{
		for (int i = 0; i < NumberOfImages; i++)
			SDL_DestroyTexture(HImage[i]);

		HImage = nullptr;
	}

	if (Clip != nullptr)
	{
		delete[] Clip;
		Clip = nullptr;
	}

	NumberOfImages = 0;
	ImageToDisplay = 0;

	X = 0;
	Y = 0;
	W = 0;
	H = 0;

	Speed = 0;
	Health = 0;
	AttackDamage = 0;
	*/

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