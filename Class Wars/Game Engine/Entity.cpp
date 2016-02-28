#include "SLGE.h"

#define DLL_API __declspec(dllexport) 
using namespace SLGE;


DLL_API Entity::Entity()
{
	ClearData();
}

DLL_API Entity::Entity(Window *in_WindowHandle) : Entity()
{
	Register(in_WindowHandle);
}

void DLL_API Entity::ClearData()
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
	*/
	Object::ClearData();

	Speed = 0;
	Health = 0;
	AttackDamage = 0;
}


double DLL_API Entity::GetSpeed()
{
	return Speed;
}

double DLL_API Entity::GetHealth()
{
	return Health;
}

double DLL_API Entity::GetAttackDamage() 
{
	return AttackDamage;
}


int DLL_API Entity::SetTraits(const double in_Speed, const double in_Health, const double in_AttackDamage)
{
	return 0;
}


int DLL_API Entity::Move(const Direction in_Direction)
{
	return 0;
}

int DLL_API Entity::MoveTo(const double in_X, const double in_Y)
{
	return 0;
}

int DLL_API Entity::TakeDamage(const double in_AttackDamage)
{
	return 0;
}

int DLL_API Entity::DealDamage(Entity *out_Victim, const double in_AttackDamage)
{
	return 0;
}

#undef DLL_API