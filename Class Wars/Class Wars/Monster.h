#pragma once
#include <SLGE.h>
#include "Bar.h"
using namespace SLGE;

//Bass class for all monsters
class Monster_t : public Entity_t
{
protected:

	Uint32 Clock_Wander;

	virtual void ClearData()
	{
		Clock_Wander = 0;

		Entity_t::ClearData();
	}

public:

	Monster_t() : Entity_t()
	{
		ClearData();
	}

	Monster_t(Window_t *in_Window) : Monster_t()
	{
		Register(in_Window);
	}

	virtual void Init(int in_X, int in_Y, int in_MoveSpeed)
	{
		SetCoords(in_X, in_Y);
		//SetSize((*Image)[0].ClipW(), (*Image)[0].ClipH());
		SetSize(16, 16);
		MoveSpeed = in_MoveSpeed;
		DestinationX = X;
		DestinationY = Y;
		InitialHealth = 100;
		CurrentHealth = 100;

		Moving.SetAll(0, 0.1, 10);

		MainWeapon.Range = 5;
		MainWeapon.Damage = 5;
		MainWeapon.Speed = 0.5;

		HealthBar.Register(WindowHandle);
		HealthBar.AddImage("Health.png", { 0, 50, 50, 5 }, { NULL });
		HealthBar.AddLayer("Health.png", { 0, 100, 50, 5 }, { NULL });
		HealthBar.Init();

		AddLayer("Collision Box.png", { 0, 0, (int)W, (int)H }, { 255, 0, 153, 0 });
		MoveLayer(ImageToDisplay->size() - 1, 0);
		AnimationLayer++;
	}

	virtual int Display()
	{
		int Ret = 0;

		if (Ret = Entity_t::Display())
			return Ret;

		HealthBar.SetCoords(X, Y - (*Image)[(*ImageToDisplay)[0]].H() / 2);
		
		if (Ret = HealthBar.Display())
			return Ret;

		PerFrameFunctions();

		return Ret;
	}

	virtual bool CanAttack(Entity_t *in_Victim)
	{
		SDL_Rect Temp;

		Temp.x = (int) X;
		Temp.y = (int) Y;
		Temp.w = (int) (W + (2 * MainWeapon.Range));
		Temp.h = (int) (H + (2 * MainWeapon.Range));

		//{ in_Victim->GetX(), in_Victim->GetY(), in_Victim->GetW() + 2 * AttackRange, in_Victim->GetH() + 2 * AttackRange }

		if (in_Victim->IsOverlapping(Temp))
			return true;
		else
			return false;
	}


	//Class specific methods
	void Wander(SDL_Rect in_WanderArea, double in_WaitTime)
	{
		/*
		if (SDL_GetTicks() - Clock_Wander > in_WaitTime * 1000)//sets random points within the rectangle and begins the failure timer
		{
			DestinationX = rand() % in_WanderArea.w + (in_WanderArea.x - in_WanderArea.w / 2);//sets random x
			DestinationY = rand() % in_WanderArea.h + (in_WanderArea.y - in_WanderArea.h / 2);//sets random y
			Clock_Wander = SDL_GetTicks();// +10000 * in_WaitTime;//this keeps the object from getting stuck while moving towards a non-reachable coordinate. 
		}
		*/

		if (IsWithin({ (int) DestinationX, (int) DestinationY, (int) MoveSpeed, (int) MoveSpeed }))
		{
			DestinationX = rand() % (in_WanderArea.w + (in_WanderArea.x - in_WanderArea.w / 2));//sets random x
			DestinationY = rand() % (in_WanderArea.h + (in_WanderArea.y - in_WanderArea.h / 2));//sets random y
		}
		else
			MoveTo((int) DestinationX, (int) DestinationY);
	}
};
