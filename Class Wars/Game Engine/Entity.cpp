#include "SLGE.h"

#define DLL_API __declspec(dllexport) 
using namespace SLGE;


//Private Methods
DLL_API Entity::Entity()
{
	ClearData();
}

DLL_API Entity::Entity(Window *in_WindowHandle) : Entity()
{
	Register(in_WindowHandle);
}

int DLL_API Entity::DetectCollisions(Object* in_Object)
{
	if (IsCollidingWith(in_Object))
	{
		if (	LastX < in_Object->GetX() && 
				LastY + H / 2 > in_Object->GetY() - in_Object->GetH() / 2 && 
				LastY - H / 2 < in_Object->GetY() + in_Object->GetH() / 2)
			SetCoords(in_Object->GetX() - in_Object->GetW() / 2 - W / 2, Y);

		else if (LastX > in_Object->GetX() && 
				LastY + H / 2 > in_Object->GetY() - in_Object->GetH() / 2 && 
				LastY - H / 2 < in_Object->GetY() + in_Object->GetH() / 2)
			SetCoords(in_Object->GetX() + in_Object->GetW() / 2 + W / 2, Y);

		else if (LastY < in_Object->GetY() && 
				 LastX + W / 2 > in_Object->GetX() - in_Object->GetW() / 2 && 
				 LastX - W / 2 < in_Object->GetX() + in_Object->GetW() / 2)
			SetCoords(X, in_Object->GetY() - in_Object->GetH() / 2 - H / 2);

		else if (LastY > in_Object->GetY() && 
				 LastX + W / 2 > in_Object->GetX() - in_Object->GetW() / 2 && 
				 LastX - W / 2 < in_Object->GetX() + in_Object->GetW() / 2)
			SetCoords(X, in_Object->GetY() + in_Object->GetH() / 2 + H / 2);

		return 0;
	}

	else
		return 0;
}



//Status Methods
double DLL_API Entity::GetSpeed()
{
	return Speed;
}

double DLL_API Entity::GetHealth()
{
	return CurrentHealth;
}

double DLL_API Entity::GetAttackDamage() 
{
	return AttackDamage;
}

bool DLL_API Entity::IsMoving()
{
	return Moving;
}

bool DLL_API Entity::IsAttacking()
{
	return Attacking;
}

bool DLL_API Entity::IsBeingHit()
{
	return BeingHit;
}

//Collision detection
bool DLL_API Entity::IsWithinAttackRange(Object *in_Object)
{
	if (X + W / 2 + AttackDistance >= in_Object->GetX() - in_Object->GetW() / 2 && 
		X - W / 2 - AttackDistance <= in_Object->GetX() + in_Object->GetW() / 2 && 
		Y + H / 2 + AttackDistance >= in_Object->GetY() - in_Object->GetH() / 2 && 
		Y - H / 2 - AttackDistance <= in_Object->GetY() + in_Object->GetH() / 2)
		return true;
	else
		return false;
}

bool DLL_API Entity::IsFacing(Object *in_Object)
{
	if (in_Object->GetX() < X && Facing == Left)
		return true;
	if (in_Object->GetX() > X && Facing == Right)
		return true;
	if (in_Object->GetY() < Y && Facing == Up)
		return true;
	if (in_Object->GetY() > Y && Facing == Down)
		return true;
	else
		return false;
}

bool DLL_API Entity::IsCollidingWith(Object *in_Object)
{
	if (IsOverlapping(in_Object) && Moving)
		return true;
	else
		return false;
}


//Setting Methods
int DLL_API Entity::SetTraits(const double in_Speed, const double in_Health, const double in_AttackDamage)
{
	Speed = in_Speed;
	MaxHealth = in_Health;
	CurrentHealth = in_Health;
	AttackDamage = in_AttackDamage;

	DoDynamicDepth = true;
	return 0;
}
					
int DLL_API Entity::SetAnimation(const double in_Speed)
{
	AnimateSpeed = in_Speed;
	return 0;
}


//Action Methods
int DLL_API Entity::Move(const Direction in_Direction)
{
	if (WindowHandle->TimerHandle.GetFPS() <= 0)
		return 1;

	switch (in_Direction)
	{
		case Up:
		Y -= Speed / WindowHandle->TimerHandle.GetFPS();
		Facing = Up;
		break;

		case UpRight:
		X += Speed / WindowHandle->TimerHandle.GetFPS();
		Y -= Speed / WindowHandle->TimerHandle.GetFPS();
		Facing = UpRight;
		break;

		case Right:
		X += Speed / WindowHandle->TimerHandle.GetFPS();
		Facing = Right;
		break;

		case DownRight:
		X += Speed / WindowHandle->TimerHandle.GetFPS();
		Y += Speed / WindowHandle->TimerHandle.GetFPS();
		Facing = DownRight;
		break;

		case Down:
		Y += Speed / WindowHandle->TimerHandle.GetFPS();
		Facing = Down;
		break;

		case DownLeft:
		X -= Speed / WindowHandle->TimerHandle.GetFPS();
		Y += Speed / WindowHandle->TimerHandle.GetFPS();
		Facing = DownLeft;
		break;

		case Left:
		X -= Speed / WindowHandle->TimerHandle.GetFPS();
		Facing = Left;
		break;

		case UpLeft:
		X -= Speed / WindowHandle->TimerHandle.GetFPS();
		Y -= Speed / WindowHandle->TimerHandle.GetFPS();
		Facing = UpLeft;
		break;	   
		
		default:
		Moving = false;
		Facing = Down;
		return 0;
		break;
	}

	Moving = true;

	return 0;
}

int DLL_API Entity::MoveTo(const double in_X, const double in_Y)
{
	/*
	if (in_X > X)
	{
		if (in_Y > Y)
			Move(DownRight);
		else if (in_Y == Y)
			Move(Right);
		else if (in_Y < Y)
			Move(UpRight);
	}

	else if (in_X == X)
	{
		if (in_Y > Y)
			Move(Down);
		else if (in_Y == Y)
			return 0;
		else if (in_Y < Y)
			Move(Up);
	}

	else if (in_X < X)
	{
		if (in_Y > Y)
			Move(DownLeft);
		else if (in_Y == Y)
			Move(Left);
		else if (in_Y < Y)
			Move(UpLeft);
	}
	*/


	double DistanceX = in_X - X;
	double DistanceY = in_Y - Y;

	if (DistanceX == 0)
		return 0;

	double SpeedX = Speed * cos(atan(DistanceY / DistanceX)) / WindowHandle->TimerHandle.GetFPS();
	double SpeedY = Speed * sin(atan(DistanceY / DistanceX)) / WindowHandle->TimerHandle.GetFPS();

	if (IsOverlapping({ in_X, in_Y, Speed / WindowHandle->TimerHandle.GetFPS(), Speed / WindowHandle->TimerHandle.GetFPS() }))
	{
		SetCoords(in_X, in_Y);
		Moving = false;
		return 0;
	}

	if (DistanceX < 0)
		SetCoords(X - SpeedX, Y - SpeedY);
	else
		SetCoords(X + SpeedX, Y + SpeedY);

	// Moving Left
	if (DistanceX < 0)
	{		
		// Moving Up
		if (DistanceY < 0)
		{
			// More Up than Left
			if (abs(DistanceX * 2) < abs(DistanceY))
				ImageToDisplay = 2;

			// More	Left than Up
			else if (abs(DistanceY * 2) < abs(DistanceX))
				ImageToDisplay = 0;

			// Both Up and Left
			else
				ImageToDisplay = 4;
		}
		
		// Moving Down
		else
		{
			// More Down than Left
			if (abs(DistanceX * 2) < abs(DistanceY))
				ImageToDisplay = 3;

			// More	Left than Down
			else if (abs(DistanceY * 2) < abs(DistanceX))
				ImageToDisplay = 0;

			// Both Down and Left
			else
				ImageToDisplay = 7;
		}
	}

	// Moving Right
	else
	{
		// Moving Up
		if (DistanceY < 0)
		{
			// More Up than Right
			if (abs(DistanceX * 2) < abs(DistanceY))
				ImageToDisplay = 2;

			// More	Right than Up
			else if (abs(DistanceY * 2) < abs(DistanceX))
				ImageToDisplay = 1;

			// Both Up and Right
			else
				ImageToDisplay = 5;
		}
		
		// Moving Down
		else
		{
			// More Down than Right
			if (abs(DistanceX * 2) < abs(DistanceY))
				ImageToDisplay = 3;

			// More	Right than Down
			else if (abs(DistanceY * 2) < abs(DistanceX))
				ImageToDisplay = 1;

			// Both Down and Right
			else
				ImageToDisplay = 6;
		}
	}

	Moving = true;

	return 0;
}

int DLL_API Entity::TakeDamage(const double in_AttackDamage)
{
	CurrentHealth -= in_AttackDamage / WindowHandle->TimerHandle.GetFPS();

	if (CurrentHealth <= 0)
		CurrentHealth = 0;

	return 0;
}

int DLL_API Entity::DealDamage(Entity *out_Victim, const double in_AttackDamage)
{
	return out_Victim->TakeDamage(in_AttackDamage);
}

#undef DLL_API