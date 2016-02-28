#pragma once
#include <SLGE.h>
#include "Bar.h"
using namespace SLGE;


//Base class for all entities
class Entity_t : public Object_t
{
public:

	enum Direction_t
	{
		Down,
		Right,
		Left,
		Up,

		DownRight,
		DownLeft,
		UpRight,
		UpLeft
	};

	enum AnimateType_t
	{
		aIdle,
		aMoving,
		aAttacking
	};

protected:

	struct Attack_t
	{
		double Damage;
		double Speed;
		double Range;
		bool Active;

		void ClearData()
		{
			Damage = 0;
			Speed = 0;
			Range = 0;
			Active = false;
		}
	};

	class Animation_t
	{
	protected:

		struct AnimateDirection_t
		{
		friend class Entity_t;
		private:

			double TimeDisplayed; //In milliseconds

		public:

			int StartPosition;
			int TotalNumber;
			double Duration; //In milliseconds

			void ClearData()
			{
				StartPosition = 0;
				TotalNumber = 0;
				Duration = 0;
				TimeDisplayed = 0;
			}

		} Direction[8];

	public:

		bool Active;

		void ClearData()
		{
			for (int i = 0; i < 8; i++)
				Direction[i].ClearData();

			Active = false;
		}

		AnimateDirection_t &operator[](Direction_t in_Direction)
		{
			if (in_Direction >= 0 && in_Direction < 8)
				return Direction[in_Direction];

			return Direction[0];
		}

		const AnimateDirection_t  &operator[](Direction_t in_Direction) const
		{
			if (in_Direction >= 0 && in_Direction < 8)
				return Direction[in_Direction];

			return Direction[0];
		}

		void SetAll(int in_StartPosition, double in_Duration, int in_TotalFrames)
		{
			SetTotalFramesAll(in_TotalFrames);
			SetDurationAll(in_Duration);
			SetStartPositionAll(in_StartPosition);
		}

		void SetTotalFramesAll(int in_TotalNumber)
		{
			for (int i = 0; i < 8; i++)
				Direction[i].TotalNumber = in_TotalNumber;
		}

		void SetDurationAll(double in_Duration)
		{
			for (int i = 0; i < 8; i++)
				Direction[i].Duration = in_Duration;
		}

		void SetStartPositionAll(int in_StartPosition)
		{
			int StartOffset = 0;

			for (int i = 0; i < 8; i++)
			{
				Direction[i].StartPosition = in_StartPosition + StartOffset;
				StartOffset += Direction[i].TotalNumber;
			}
		}
	};

	//Movement
	double MoveSpeed; //In Pixels per second
	double DestinationX;
	double DestinationY;
	double LastX;
	double LastY;

	//Attack
	double CurrentHealth;
	double InitialHealth;

	//Animation
	Direction_t Facing;
	int AnimationLayer;

	Animation_t Idle;
	Animation_t Moving;
	Animation_t Attacking;
	Animation_t TakingDamage;

	virtual void ClearData()
	{
		MoveSpeed = 0; //In Pixels per second
		DestinationX = 0;
		DestinationY = 0;
		LastX = 0;
		LastY = 0;

		//Attack
		MainWeapon.ClearData();
		CurrentHealth = 0;
		InitialHealth = 0;

		//Animation
		Facing = Down;
		AnimationLayer = 0;

		Idle.ClearData();
		Moving.ClearData();
		Attacking.ClearData();
		TakingDamage.ClearData();

		Object_t::ClearData();
	}

public:

	Attack_t MainWeapon;
	Bar_t HealthBar;

	Entity_t() : Object_t()
	{
		ClearData();
	}

	Entity_t(Window_t *in_Window) : Entity_t()
	{
		Register(in_Window);
	}

	virtual int Register(Window_t *in_Window)
	{
		int Ret = 0;

		if (Ret = Object_t::Register(in_Window))
			return Ret;
		if (Ret = HealthBar.Register(in_Window))
			return Ret;

		return Ret;
	}

	virtual void Init()
	{
		HealthBar.Init();
	}

	virtual void PerFrameFunctions()
	{
		LastX = X;
		LastY = Y;
	}

	virtual int Display()
	{
		int Ret = 0;

		if (Ret = Object_t::Display())
			return Ret;

		if (Ret = HealthBar.Display())
			return Ret;

		PerFrameFunctions();
		
		return Ret;
	}

	virtual void Animate()
	{
		if (Attacking.Active)
		{
			Attacking[Facing].TimeDisplayed += 1 / WindowHandle->TimerHandle.GetFPS();

			while (Attacking[Facing].TimeDisplayed > Attacking[Facing].Duration &&
				   Attacking[Facing].Duration > 0)
			{
				Attacking[Facing].TimeDisplayed -= Attacking[Facing].Duration;
				(*ImageToDisplay)[AnimationLayer]++;

				if ((*ImageToDisplay)[AnimationLayer] >= Attacking[Facing].StartPosition +  Attacking[Facing].TotalNumber ||
					(*ImageToDisplay)[AnimationLayer] < Attacking[Facing].StartPosition)
				{
					(*ImageToDisplay)[AnimationLayer] = Attacking[Facing].StartPosition;
				}
			}
		}
		else if (Moving.Active)
		{
			Moving[Facing].TimeDisplayed += 1 / WindowHandle->TimerHandle.GetFPS();

			while (Moving[Facing].TimeDisplayed > Moving[Facing].Duration &&
				   Moving[Facing].Duration > 0)
			{
				Moving[Facing].TimeDisplayed -= Moving[Facing].Duration;
				(*ImageToDisplay)[AnimationLayer]++;

				if ((*ImageToDisplay)[AnimationLayer] >= Moving[Facing].StartPosition + Moving[Facing].TotalNumber ||
					(*ImageToDisplay)[AnimationLayer] < Moving[Facing].StartPosition)
				{
					(*ImageToDisplay)[AnimationLayer] = Moving[Facing].StartPosition;
				}
			}
		}
		else
			(*ImageToDisplay)[AnimationLayer] = Idle[Facing].StartPosition;
	}

	virtual bool CanAttack(Entity_t *in_Victim)
	{
		SDL_Rect AttackRect;

		AttackRect.x = (int) X;
		AttackRect.y = (int) Y;
		AttackRect.w = (int) (W + (2 * MainWeapon.Range));
		AttackRect.h = (int) (H + (2 * MainWeapon.Range));

		if (in_Victim->IsOverlapping(AttackRect) && IsFacing(in_Victim))
			return true;
		else
			return false;
	}

	virtual void TakeDamage(double in_Damage)
	{
		CurrentHealth -= in_Damage;
		HealthBar.SetPercentage((double) CurrentHealth / (double)InitialHealth);
	}


	bool IsDead()
	{
		if (CurrentHealth <= 0)
			return true;
		else
			return false;
	}

	double GetSpeed()
	{
		return MoveSpeed;
	}

	int Move(Direction_t in_Direction)
	{
		if (WindowHandle->TimerHandle.GetFPS() <= 0)
			return 1;

		switch (in_Direction)
		{
			case Up:
			Y -= MoveSpeed / WindowHandle->TimerHandle.GetFPS();
			Facing = Up;
			break;

			case UpRight:
			X += MoveSpeed / WindowHandle->TimerHandle.GetFPS();
			Y -= MoveSpeed / WindowHandle->TimerHandle.GetFPS();
			Facing = UpRight;
			break;

			case Right:
			X += MoveSpeed / WindowHandle->TimerHandle.GetFPS();
			Facing = Right;
			break;

			case DownRight:
			X += MoveSpeed / WindowHandle->TimerHandle.GetFPS();
			Y += MoveSpeed / WindowHandle->TimerHandle.GetFPS();
			Facing = DownRight;
			break;

			case Down:
			Y += MoveSpeed / WindowHandle->TimerHandle.GetFPS();
			Facing = Down;
			break;

			case DownLeft:
			X -= MoveSpeed / WindowHandle->TimerHandle.GetFPS();
			Y += MoveSpeed / WindowHandle->TimerHandle.GetFPS();
			Facing = DownLeft;
			break;

			case Left:
			X -= MoveSpeed / WindowHandle->TimerHandle.GetFPS();
			Facing = Left;
			break;

			case UpLeft:
			X -= MoveSpeed / WindowHandle->TimerHandle.GetFPS();
			Y -= MoveSpeed / WindowHandle->TimerHandle.GetFPS();
			Facing = UpLeft;
			break;	   
		}

		Moving.Active = true;
		return 0;
	}

	int MoveTo(int in_X, int in_Y)
	{
		Moving.Active = false;

		double DistanceX = in_X - X;
		double DistanceY = in_Y - Y;

		if (DistanceX == 0)
		{
			SetCoords(X, MoveSpeed / WindowHandle->TimerHandle.GetFPS());
			return 0;
		}

		double SpeedX = MoveSpeed * cos(atan(DistanceY / DistanceX)) / WindowHandle->TimerHandle.GetFPS();
		double SpeedY = MoveSpeed * sin(atan(DistanceY / DistanceX)) / WindowHandle->TimerHandle.GetFPS();

		if (IsOverlapping({ (int) in_X, (int) in_Y, (int) (MoveSpeed / WindowHandle->TimerHandle.GetFPS()), (int) (MoveSpeed / WindowHandle->TimerHandle.GetFPS()) }))
		{
			SetCoords(in_X, in_Y);
		}
		else
		{
			if (DistanceX < 0)
				SetCoords(X - SpeedX, Y - SpeedY);
			else
				SetCoords(X + SpeedX, Y + SpeedY);

			Moving.Active = true;
		}

		return 0;
	}

	bool HandleCollisionWithO(Object_t *in_Object)
	{
		Entity_t *in_Entity = this;

		if (static_cast <Entity_t*> (in_Object) == in_Entity)
			return false;

		if (in_Entity->IsOverlapping(in_Object) && in_Entity->Moving.Active)
		{	
			//in_Entity->TakeDamage(1);

			/*
			if (LastX < in_Object->GetX() &&
				LastY + H / 2 > in_Object->GetY() - in_Object->GetH() / 2 &&
				LastY - H / 2 < in_Object->GetY() + in_Object->GetH() / 2)
			{
				SetCoords(in_Object->GetX() - in_Object->GetW() / 2 - W / 2, Y);
			}
			else if (LastX > in_Object->GetX() &&
					 LastY + H / 2 > in_Object->GetY() - in_Object->GetH() / 2 &&
					 LastY - H / 2 < in_Object->GetY() + in_Object->GetH() / 2)
			{
				SetCoords(in_Object->GetX() + in_Object->GetW() / 2 + W / 2, Y);
			}
			else if (LastY < in_Object->GetY() &&
					 LastX + W / 2 > in_Object->GetX() - in_Object->GetW() / 2 &&
					 LastX - W / 2 < in_Object->GetX() + in_Object->GetW() / 2)
			{
				SetCoords(X, in_Object->GetY() - in_Object->GetH() / 2 - H / 2);
			}
			else if (LastY > in_Object->GetY() &&
					 LastX + W / 2 > in_Object->GetX() - in_Object->GetW() / 2 &&
					 LastX - W / 2 < in_Object->GetX() + in_Object->GetW() / 2)
			{
				SetCoords(X, in_Object->GetY() + in_Object->GetH() / 2 + H / 2);
			}
			//*/

			///*
			//Left side
			if (in_Entity->LastX < in_Object->GetX() && 
				in_Entity->LastY + in_Entity->H / 2 > in_Object->GetY() - in_Object->GetH() / 2 && 
				in_Entity->LastY - in_Entity->H / 2 < in_Object->GetY() + in_Object->GetH() / 2)
			{
				in_Entity->SetCoords(in_Object->GetX() - (in_Entity->W + in_Object->GetW()) / 2, in_Entity->Y);
			}
			//Right side
			else if (in_Entity->LastX < in_Object->GetX() && 
					in_Entity->LastY + in_Entity->H / 2 > in_Object->GetY() - in_Object->GetH() / 2 && 
					in_Entity->LastY - in_Entity->H / 2 < in_Object->GetY() + in_Object->GetH() / 2)
			{
				in_Entity->SetCoords(in_Object->GetX() + (in_Entity->W + in_Object->GetW()) / 2, in_Entity->Y);
			}
			//Top
			else if (in_Entity->LastY < in_Object->GetY() && 
					in_Entity->LastX + in_Entity->W / 2 > in_Object->GetX() - in_Object->GetW() / 2 && 
					in_Entity->LastX - in_Entity->W / 2 < in_Object->GetX() + in_Object->GetW() / 2)
			{
				in_Entity->SetCoords(in_Entity->X, in_Object->GetY() - (in_Entity->H + in_Object->GetH()) / 2);
			}
			//Bottom
			else if (in_Entity->LastY < in_Object->GetY() && 
					in_Entity->LastX + in_Entity->W / 2 > in_Object->GetX() - in_Object->GetW() / 2 && 
					in_Entity->LastX - in_Entity->W / 2 < in_Object->GetX() + in_Object->GetW() / 2)
			{
				in_Entity->SetCoords(in_Entity->X, in_Object->GetY() + (in_Entity->H + in_Object->GetH()) / 2);
			}
			//*/
		
			return true;
		}

		return false;
	}

	bool HandleCollisionWithE(Entity_t *in_Entity2)
	{
		Entity_t *in_Entity = this;

		if (static_cast <Entity_t*> (in_Entity2) == in_Entity)
			return false;

		if (in_Entity->IsOverlapping(in_Entity2) && in_Entity->Moving.Active)
		{	
			//in_Entity->TakeDamage(1);

			///*
			if (LastX < in_Entity2->GetX() &&
				LastY + H / 2 > in_Entity2->GetY() - in_Entity2->GetH() / 2 &&
				LastY - H / 2 < in_Entity2->GetY() + in_Entity2->GetH() / 2)
			{
				SetCoords(in_Entity2->GetX() - in_Entity2->GetW() / 2 - W / 2, Y);
			}
			else if (LastX > in_Entity2->GetX() &&
					 LastY + H / 2 > in_Entity2->GetY() - in_Entity2->GetH() / 2 &&
					 LastY - H / 2 < in_Entity2->GetY() + in_Entity2->GetH() / 2)
			{
				SetCoords(in_Entity2->GetX() + in_Entity2->GetW() / 2 + W / 2, Y);
			}
			else if (LastY < in_Entity2->GetY() &&
					 LastX + W / 2 > in_Entity2->GetX() - in_Entity2->GetW() / 2 &&
					 LastX - W / 2 < in_Entity2->GetX() + in_Entity2->GetW() / 2)
			{
				SetCoords(X, in_Entity2->GetY() - in_Entity2->GetH() / 2 - H / 2);
			}
			else if (LastY > in_Entity2->GetY() &&
					 LastX + W / 2 > in_Entity2->GetX() - in_Entity2->GetW() / 2 &&
					 LastX - W / 2 < in_Entity2->GetX() + in_Entity2->GetW() / 2)
			{
				SetCoords(X, in_Entity2->GetY() + in_Entity2->GetH() / 2 + H / 2);
			}
			//*/


			/*
			if (LastX < in_Entity2->GetX() &&
				LastY + H / 2 > in_Entity2->GetY() - in_Entity2->GetH() / 2 &&
				LastY - H / 2 < in_Entity2->GetY() + in_Entity2->GetH() / 2)
			{
				SetCoords(LastX, Y);
				in_Entity2->SetCoords(in_Entity2->LastX, in_Entity2->Y);
			}
			else if (LastX > in_Entity2->GetX() &&
					 LastY + H / 2 > in_Entity2->GetY() - in_Entity2->GetH() / 2 &&
					 LastY - H / 2 < in_Entity2->GetY() + in_Entity2->GetH() / 2)
			{
				SetCoords(LastX, Y);
				in_Entity2->SetCoords(in_Entity2->LastX, in_Entity2->Y);
			}
			else if (LastY < in_Entity2->GetY() &&
					 LastX + W / 2 > in_Entity2->GetX() - in_Entity2->GetW() / 2 &&
					 LastX - W / 2 < in_Entity2->GetX() + in_Entity2->GetW() / 2)
			{
				SetCoords(X, LastY);
				in_Entity2->SetCoords(in_Entity2->X, in_Entity2->LastY);
			}
			else if (LastY > in_Entity2->GetY() &&
					 LastX + W / 2 > in_Entity2->GetX() - in_Entity2->GetW() / 2 &&
					 LastX - W / 2 < in_Entity2->GetX() + in_Entity2->GetW() / 2)
			{
				SetCoords(X, LastY);
				in_Entity2->SetCoords(in_Entity2->X, in_Entity2->LastY);
			}
			//*/

			/*
			//Left side
			if (in_Entity->LastX < in_Entity2->LastX && 
				in_Entity->LastY + in_Entity->H / 2 > in_Entity2->LastY - in_Entity2->GetH() / 2 && 
				in_Entity->LastY - in_Entity->H / 2 < in_Entity2->LastY + in_Entity2->GetH() / 2)
			{
				in_Entity->SetCoords(in_Entity2->LastX - (in_Entity->W + in_Entity2->GetW()) / 2, in_Entity->LastY);
			}
			//Right side
			else if (in_Entity->LastX < in_Entity2->LastX && 
					in_Entity->LastY + in_Entity->H / 2 > in_Entity2->LastY - in_Entity2->GetH() / 2 && 
					in_Entity->LastY - in_Entity->H / 2 < in_Entity2->LastY + in_Entity2->GetH() / 2)
			{
				in_Entity->SetCoords(in_Entity2->LastX + (in_Entity->W + in_Entity2->GetW()) / 2, in_Entity->Y);
			}
			//Top
			else if (in_Entity->LastY < in_Entity2->LastY && 
					in_Entity->LastX + in_Entity->W / 2 > in_Entity2->LastX - in_Entity2->GetW() / 2 && 
					in_Entity->LastX - in_Entity->W / 2 < in_Entity2->LastX + in_Entity2->GetW() / 2)
			{
				in_Entity->SetCoords(in_Entity->X, in_Entity2->LastY - (in_Entity->H + in_Entity2->GetH()) / 2);
			}
			//Bottom
			else if (in_Entity->LastY < in_Entity2->LastY && 
					in_Entity->LastX + in_Entity->W / 2 > in_Entity2->LastX - in_Entity2->GetW() / 2 && 
					in_Entity->LastX - in_Entity->W / 2 < in_Entity2->LastX + in_Entity2->GetW() / 2)
			{
				in_Entity->SetCoords(in_Entity->X, in_Entity2->LastY + (in_Entity->H + in_Entity2->GetH()) / 2);
			}
			//*/
		
			return true;
		}

		return false;
	}

	bool IsFacing(Entity_t *in_Victim)
	{
		double XDist = in_Victim->GetX() - X;
		double YDist = in_Victim->GetY() - Y;

		if (XDist > YDist && XDist < -YDist && Facing == Up)
			return true;
		else if (XDist < YDist && XDist > -YDist && Facing == Down)
			return true;
		else if (XDist < YDist && XDist < -YDist && Facing == Left)
			return true;
		else if (XDist > YDist && XDist > -YDist && Facing == Right)
			return true;
		else
			return false;
	}

	void Attack(Entity_t *in_Victim, Attack_t Weapon)
	{
		in_Victim->TakeDamage(Weapon.Damage / WindowHandle->TimerHandle.GetFPS());
	}
};
