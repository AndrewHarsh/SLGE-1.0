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
	CurrentHealth = 0;
	MaxHealth = 0;
	AttackDamage = 0;
	AnimateSpeed = 0;
	AnimateFrame = 0;

	DoCollisionDetection = true;
}

int DLL_API Entity::PerFrameActions()
{
	LastX = X;
	LastY = Y;

	Moving = false;

	return 0;
}

int DLL_API Entity::EventHandler(SDL_Event *in_Event)
{
	return 0;
}

int DLL_API Entity::CollisionDetection(Object* in_Object)
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

int DLL_API Entity::Animate()
{
	AnimateFrame += 1000 / WindowHandle->TimerHandle.GetFPS(); //milliseconds per frame

	if (AnimateSpeed && static_cast <int> (AnimateFrame) >= AnimateSpeed)
	{
		//ImageToDisplay += abs(AnimateFrame / AnimateSpeed);

		AnimateFrame = 0;

		if (++ImageToDisplay >= NumberOfImages)
			ImageToDisplay = 0;
	}

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
bool DLL_API Entity::IsOverlapping(Object *in_Object)
{
	if (X + W / 2 >= in_Object->GetX() - in_Object->GetW() / 2 && 
		X - W / 2 <= in_Object->GetX() + in_Object->GetW() / 2 && 
		Y + H / 2 >= in_Object->GetY() - in_Object->GetH() / 2 && 
		Y - H / 2 <= in_Object->GetY() + in_Object->GetH() / 2)
		return true;
	else
		return false;
}

bool DLL_API Entity::IsWithin(Object *in_Object)
{
	if (X - W / 2 >= in_Object->GetX() - in_Object->GetW() / 2 && 
		X + W / 2 <= in_Object->GetX() + in_Object->GetW() / 2 && 
		Y - H / 2 >= in_Object->GetY() - in_Object->GetH() / 2 && 
		Y + H / 2 <= in_Object->GetY() + in_Object->GetH() / 2)
		return true;
	else
		return false;
}

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
	if (in_Object->GetX() < X && Facing == 2)
		return true;
	if (in_Object->GetX() > X && Facing == 1)
		return true;
	if (in_Object->GetY() < Y && Facing == 3)
		return true;
	if (in_Object->GetY() > Y && Facing == 0)
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

	DoCollisionDetection = true;
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
		X += Speed / WindowHandle->TimerHandle.GetFPS();
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

	double Rise = in_Y - Y;
	double Run = in_X - X;

	//double Hypotenuse = sqrt(Rise * Rise + Run * Run);

	double Slope = Rise / Run;

	//X *= Slope;

	return 0;
}

int DLL_API Entity::TakeDamage(const double in_AttackDamage)
{
	CurrentHealth = -in_AttackDamage;

	if (CurrentHealth <= 0)
		CurrentHealth = 0;

	return 0;
}

int DLL_API Entity::DealDamage(Entity *out_Victim, const double in_AttackDamage)
{
	return out_Victim->TakeDamage(in_AttackDamage);
}

#undef DLL_API