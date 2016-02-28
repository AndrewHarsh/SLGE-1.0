#include "SLGE.h"

#define DLL_API __declspec(dllexport) 
using namespace SLGE;


DLL_API Entity::Entity()
{
	ClearData();
}

DLL_API Entity::Entity(Window *in_WindowHandle)
{
	WindowHandle = in_WindowHandle;

	Entity::Entity();
}

void DLL_API Entity::ClearData()
{
	Speed = 0;
	Health = 0;
	AttackDamage = 0;

	Object::ClearData();
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

int DLL_API Entity::TakeDamage(const double in_AttackDamage)
{
	return 0;
}

int DLL_API Entity::DealDamage(Entity *out_Victim, const double in_AttackDamage)
{
	return 0;
}

#undef DLL_API