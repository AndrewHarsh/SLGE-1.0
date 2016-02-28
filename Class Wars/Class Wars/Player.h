#pragma once
#include <SLGE.h>
#include "Entity.h"
using namespace SLGE;

const double PI = 3.141592653589793238463;	
const double RADIANS_TO_DEGREES = 180 / PI;							//Multiply radians by this constant to get degrees, divide degrees by this to get radians


//Base class for all players
class Player_t : public Entity_t
{
protected:
	
	class Reticule_t : public Object_t
	{
	protected:

		virtual void ClearData()
		{
			Object_t::ClearData();
		}

	public:

		Reticule_t() : Object_t()
		{
			ClearData();
		}

		Reticule_t(Window_t *in_Window) : Reticule_t()
		{
			Register(in_Window);
		}

		int Display()
		{
			int MouseX, MouseY;
			WindowHandle->GetMouseState(&MouseX, &MouseY);

			double Angle = atan((MouseY - Y) / (MouseX - X)) * RADIANS_TO_DEGREES;

			if (MouseX > X)
				Angle -= 180;

			SetCoords(X, Y - 30);

			(*Image)[0].Rotate(Angle - 90, { (*Image)[0].W() / 2, (*Image)[0].H() + 30});

			return 0;
		}

	};

	virtual void ClearData()
	{
		Entity_t::ClearData();
	}

public:

	Reticule_t Arc;
							   
	Player_t() : Entity_t()
	{
		ClearData();
	}			

	Player_t(Window_t *in_Window) : Player_t()
	{
		Register(in_Window);  
	}

	virtual int Register(Window_t *in_Window)
	{
		int Ret = 0;

		if (Ret = Entity_t::Register(in_Window))
			return Ret;

		if (Ret = Arc.Register(in_Window))
			return Ret;

		return Ret;
	}

	virtual int Display()
	{
		int Ret = 0;

		if (Ret = Entity_t::Display())
			return Ret;

		if (Ret = Arc.Display())
			return Ret;

		PerFrameFunctions();

		return Ret;
	}

	virtual void Init()
	{
		W = 16;
		H = 38;
		Facing = Down;
		MoveSpeed = 100;


		//Animations
		Moving.SetAll(1, 0.1, 8);
		Attacking.SetAll(33, 0.1, 8);
		Idle.SetAll(1, 0.1, 8);


		//Images
		AddLayer("Collision Box.png", { 0, 0, (int)W, (int)H }, { 255, 0, 153, 0 });
		for (int i = 0; i < 20; i++)	
		{
			for (int ii = 0; ii < 8; ii++)
				AddImage("Player.png", { (ii * 100) + 1, (i * 100) + 1, 98, 98 }, { 255, 255, 255, 0 });
		}
		AnimationLayer = 1;				

		//Weapon
		MainWeapon.Range = 35;
		MainWeapon.Damage = 200;

		Arc.AddImage("Sword Arc.png", { NULL }, { 0xFF, 0xFF, 0xFF, 0xA0 });

		//Healthbar
		HealthBar.Register(WindowHandle);
		HealthBar.AddImage("Health.png", { 0, 50, 200, 20 }, { NULL });
		HealthBar.AddLayer("Health.png", { 0, 100, 200, 20 }, { NULL });
		HealthBar.SetCoords(HealthBar.GetW() / 2 + 10, HealthBar.GetH() / 2 + 10);
		HealthBar.Init();

		CurrentHealth = 100;
		InitialHealth = 100;
	}

	virtual void HandleEvents()
	{
		Moving.Active = false;

		if (WindowHandle->GetKeyState(SDL_SCANCODE_W))
			Move(Up);
		if (WindowHandle->GetKeyState(SDL_SCANCODE_S))
			Move(Down);
		if (WindowHandle->GetKeyState(SDL_SCANCODE_A))
			Move(Left);
		if (WindowHandle->GetKeyState(SDL_SCANCODE_D))
			Move(Right);

		int MouseX, MouseY;

		if (WindowHandle->GetMouseState(&MouseX, &MouseY) & SDL_BUTTON(SDL_BUTTON_LEFT))
			Attacking.Active = true;
		else
			Attacking.Active = false;

		double XDist = MouseX - X;
		double YDist = MouseY - Y;

		if (XDist > YDist && XDist < -YDist)
			Facing = Up;
		else if (XDist < YDist && XDist > -YDist)
			Facing = Down;

		else if (XDist < YDist && XDist < -YDist)
			Facing = Left;
		else if (XDist > YDist && XDist > -YDist)
			Facing = Right;
	}
};				 