#pragma once


//TEMPLATE CLASS
//	Use to create new classes
//	Replace "TEMPLATE_t" with "<Class Name>_t"
//	Replace "Object_t" with "<Inherited Class Name>_t" but ALL classes MUST ultimately inherit from "Object_t"
class TEMPLATE_t : public Object_t
{
protected:

	//Used to clear ALL data from a class (use wisely)
	void ClearData()
	{
		//Todo: Add code to clear all attributes of the class
		Object_t::ClearData();
	}

public:

	//Calls all the constructors that need to be called
	TEMPLATE_t() : Object_t()
	{
		ClearData();
	}

	//Associates the object with a window for later use. MUST be done either in a constructor (below) or by directly calling the "Register()" method
	TEMPLATE_t(Window_t *in_Window) : TEMPLATE_t()
	{
		Register(in_Window);
	}
};



//======================================//
//		        World					//
//======================================//

class World_t : public Object_t
{
protected:

	//Used to clear ALL data from a class (use wisely)
	void ClearData()
	{
		//Todo: Add code to clear all attributes of the class
		Object_t::ClearData();
	}

public:

	//Calls all the constructors that need to be called
	World_t() : Object_t()
	{
		ClearData();
	}

	//Associates the object with a window for later use. MUST be done either in a constructor (below) or by directly calling the "Register()" method
	World_t(Window_t *in_Window) : World_t()
	{
		Register(in_Window);
	}
};





//======================================//
//		          UI					//
//======================================//

//Base class for all User Interface objects
class UI_t : public Object_t
{
protected:

	void ClearData()
	{
		Object_t::ClearData();
	}

public:

	UI_t() : Object_t()
	{
		ClearData();
	}

	UI_t(Window_t *in_Window) : UI_t()
	{
		Register(in_Window);
	}

	virtual void Init()
	{

	}

	virtual void DisplayAll()
	{
		Display();
	}
};

//======================================

//Base class for all buttons
class Button_t : public UI_t
{
protected:

	bool ButtonPressed;
	std::string Message;
	TTF_Font *Font;

	void ClearData()
	{
		if (Font != nullptr)
		{
			TTF_CloseFont(Font);
			Font = nullptr;
		}

		Message.empty();

		UI_t::ClearData();
	}

public:

	Button_t() : UI_t()
	{
		ClearData();
	}

	Button_t(Window_t *in_Window) : Button_t()
	{
		Register(in_Window);
	}

	void Init(std::string in_Message)
	{
		Font = TTF_OpenFont("Cheeseburger.ttf", 29);

		if (Font == nullptr)
		{
			std::cout << "Could not load Cheeseburger.ttf." << std::endl;
			return;
		}

		AddText(in_Message, Font, { 0x01, 0x01, 0x01 });
		(*Image)[Image->size() - 1].SetCoords(X, Y);
		AddLayer(Image->size() - 1);
	}


	//Class specific methods
	bool IsPressed()
	{
		if (HoveringOver())
		{
			if (WindowHandle->GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT))
			{
				ButtonPressed = true;
				(*ImageToDisplay)[0] = 2;
			}
			else if (ButtonPressed)
				 return true;
		}
		else
		{
			(*ImageToDisplay)[0] = 0;
			ButtonPressed = false;
		}

		return false;
	}

	bool HoveringOver()
	{
		int MouseX = 0; 
		int MouseY = 0;

		WindowHandle->GetMouseState(&MouseX, &MouseY);

		if (IsOverlapping({ MouseX, MouseY, 1, 1 }))
		{
			(*ImageToDisplay)[0] = 1;
			return true;
		}
		else
		{
			return false;
		}
	}
};

//======================================

//Base class for all menus
class Menu_t : public UI_t
{
protected:

	bool Open;
	bool Moving;
	double Speed; //in pixels per second
	int UnitsKilled;

	TTF_Font *Font;

	void ClearData()
	{
		Open = false;
		Moving = false;
		Speed = 0;
	    UnitsKilled;

		UI_t::ClearData();
	}

public:

	UI_t KillCounter;

	Menu_t() : UI_t()
	{
		ClearData();
	}

	Menu_t(Window_t *in_Window) : Menu_t()
	{
		Register(in_Window);
	}

	void DisplayAll()
	{
		Display();
		KillCounter.Display();
	}

	void Init(double in_Speed)
	{
		Speed = in_Speed;
		(*Image)[0].SetTransparency(0.75);

		Font = TTF_OpenFont("Cheeseburger.ttf", 30);

		KillCounter.Register(WindowHandle);
		KillCounter.AddText("Units Killed: " + std::to_string(UnitsKilled), Font, { 0, 0, 0, 255 });
		KillCounter.SetCoords(X, Y + 100);

		//this->Button.Spawn(1, in_Window);
		//OpenButton.Register(in_Window);
		//OpenButton.AddImage("Arrow button.png", { 0, 0, 30, 64 }, { 0xff, 0xff, 0xff});	
		//OpenButton.AddImage("Arrow button.png", { 0, 0, 30, 64 }, { 0xff, 0xff, 0xff});
		//OpenButton.AddImage("Arrow button.png", { 0, 0, 30, 64 }, { 0xff, 0xff, 0xff});
		//OpenButton.SetCoords(OpenButton.GetW() / 2, WindowHandle->GetHeight() / 2);
	}

	void AddToKilledUnits()
	{
		UnitsKilled++;
	}

	void HandleEvents()
	{
		if (WindowHandle->GetKeyState(SDL_SCANCODE_TAB) && !Moving)
		{
			Open = Open ? false : true;
			Moving = true;	 
		}

		if (Moving)
		{
			if (X > WindowHandle->GetWidth() - W / 2 && Open)
			{
				X -= Speed / WindowHandle->TimerHandle.GetFPS();

				if (X < WindowHandle->GetWidth() - W / 2 )
					X = WindowHandle->GetWidth() - W / 2 ;
			}
			else if (X < WindowHandle->GetWidth() + W / 2 && !Open)
			{
				X += Speed / WindowHandle->TimerHandle.GetFPS();
	
				if (X > WindowHandle->GetWidth() + W / 2 )
					X = WindowHandle->GetWidth() + W / 2 ;
			}
			else
				Moving = false;
		}
		else if (!Open)
			SetCoords(WindowHandle->GetWidth() + W / 2, Y);

		SDL_Color TextColor = { 0, 0, 0, 0 };
		KillCounter.GetImageAtIndex(0)->Load("Units Killed: " + std::to_string(UnitsKilled), Font, TextColor);
		KillCounter.SetCoords(X, Y + 100);
	}
};

//======================================

//Base class for all Heads Up Displays
class HUD_t : public UI_t
{
protected:

	int SelectorPosition;

	void ClearData()
	{
		SelectorPosition = 0;
		UI_t::ClearData();
	}

public:

	Object_t Selector;

	HUD_t() : UI_t()
	{
		ClearData();
	}

	HUD_t(Window_t *in_Window) : HUD_t()
	{
		Register(in_Window);
	}

	void Init()
	{
		Selector.Register(WindowHandle);
		Selector.AddLayer("HUD2.png", { 640, 0, 81, 0 }, { NULL });
		SelectorPosition = 0;
	}

	void DisplayAll()
	{
		Display();
		Selector.Display();
	}

	void HandleEvents()
	{
		int HUD_BOX_SIZE = (int) (W / 8);

		SetCoords(WindowHandle->GetWidth() / 2, WindowHandle->GetHeight() - 30);

		if (WindowHandle->GetKeyState(SDL_SCANCODE_1))
			SelectorPosition = 0;
		if (WindowHandle->GetKeyState(SDL_SCANCODE_2))
			SelectorPosition = 1;
		if (WindowHandle->GetKeyState(SDL_SCANCODE_3))
			SelectorPosition = 2;
		if (WindowHandle->GetKeyState(SDL_SCANCODE_4))
			SelectorPosition = 3;

		Selector.SetCoords(X - W / 2 + Selector.GetW() / 2 + HUD_BOX_SIZE * SelectorPosition + 160, Y);
	}
};

//======================================

//Base class for all bars
class Bar_t : public UI_t
{
protected:

	int StartWidth;

	void ClearData()
	{
		StartWidth = 0;

		UI_t::ClearData();
	}

	void SetImage(int in_ImageIndex)
	{
		if (in_ImageIndex != 1)
			Object_t::SetImage(in_ImageIndex);
		else if (in_ImageIndex >= 0 && in_ImageIndex < (int) Image->size())
		{
			(*Image)[in_ImageIndex].SetCoords(X - W / 2, Y - (*Image)[in_ImageIndex].H() / 2);
			(*Image)[in_ImageIndex].SetSize((*Image)[in_ImageIndex].W(), (*Image)[in_ImageIndex].H());
		}
	}

public:

	Bar_t() : UI_t()
	{
		ClearData();
	}

	Bar_t(Window_t *in_Window) : Bar_t()
	{
		Register(in_Window);
	}

	void Init()
	{
		StartWidth = (*Image)[0].W();
	}

	void SetPercentage(double in_Percentage)
	{ 
		if (in_Percentage >= 0 && in_Percentage <= 1)
			(*Image)[1].SetSize((int) (StartWidth * in_Percentage), (int) H);
	}
};
 

//======================================//
//				Entities				//
//======================================//

//Base class for all entities
class Entity_t : public Object_t
{
protected:

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
			int StartPosition;
			int TotalNumber;
			double Duration; //In milliseconds
			double TimeDisplayed; //In milliseconds

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

	void ClearData()
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

	virtual void Init()
	{
		HealthBar.Init();
	}

	virtual void PerFrameFunctions()
	{
		LastX = X;
		LastY = Y;
	}

	virtual void DisplayAll()
	{
		Display();
		PerFrameFunctions();
	}

	virtual void LoopRefresh()
	{
		LastX = X;
		LastY = Y;
	}

	virtual void Animate()
	{
		//if (++(*ImageToDisplay)[0] > (*Image).size())
		//	(*ImageToDisplay)[0] = 0;

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


	//Class specific methods
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
		
			default:
			Facing = Down;
			Moving.Active = false;
			return 0;
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

	virtual bool CanAttack(Entity_t *in_Victim)
	{
		SDL_Rect Temp;

		Temp.x = (int) X;
		Temp.y = (int) Y;
		Temp.w = (int) (W + (2 * MainWeapon.Range));
		Temp.h = (int) (H + (2 * MainWeapon.Range));

		//{ in_Victim->GetX(), in_Victim->GetY(), in_Victim->GetW() + 2 * AttackRange, in_Victim->GetH() + 2 * AttackRange }

		if (in_Victim->IsOverlapping(Temp) && IsFacing(in_Victim))
			return true;
		else
			return false;
	}

	void Attack(Entity_t *in_Victim, Attack_t Weapon)
	{
		in_Victim->TakeDamage(Weapon.Damage / WindowHandle->TimerHandle.GetFPS());
	}

	virtual void TakeDamage(double in_Damage)
	{
		CurrentHealth -= in_Damage;
		
		HealthBar.SetPercentage((double) CurrentHealth / (double)InitialHealth);
		//HealthBar.Image->operator[](1).SetCoords(HealthBar.GetX(), HealthBar.GetY(), (CurrentHealth / InitialHealth) * 50, 0);
	}
};

//======================================

//Base class for all players
class Player_t : public Entity_t
{

protected:

	void ClearData()
	{
		Entity_t::ClearData();
	}

public:

	class Sword_t : public Object_t
	{
	protected:

		void ClearData()
		{
			Object_t::ClearData();
		}

	public:

		Sword_t() : Object_t()
		{
			ClearData();
		}

		Sword_t(Window_t *in_Window) : Sword_t()
		{
			Register(in_Window);
		}

		void EventHandler()
		{
			int MouseX, MouseY;
			WindowHandle->GetMouseState(&MouseX, &MouseY);

			double Angle = atan((MouseY - Y) / (MouseX - X)) * RADIANS_TO_DEGREES;

			if (MouseX > X)
				Angle -= 180;

			(*Image)[0].Rotate(Angle - 90, { (*Image)[0].W() / 2, (*Image)[0].H() + 30});
		}

	} Sword;
							   
	friend class Sword_t;

	Player_t() : Entity_t()
	{
		ClearData();
	}			

	Player_t(Window_t *in_Window) : Player_t()
	{
		Register(in_Window);  
	}

	void Init()
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

		Sword.Register(WindowHandle);
		Sword.AddImage("Sword Arc.png", { NULL }, { 0xFF, 0xFF, 0xFF, 0xA0 });


		//Healthbar
		HealthBar.Register(WindowHandle);
		HealthBar.AddImage("Health.png", { 0, 50, 200, 20 }, { NULL });
		HealthBar.AddLayer("Health.png", { 0, 100, 200, 20 }, { NULL });
		HealthBar.SetCoords(HealthBar.GetW() / 2 + 10, HealthBar.GetH() / 2 + 10);
		HealthBar.Init();

		CurrentHealth = 100;
		InitialHealth = 100;
	}

	void EventHandler()
	{
		Moving.Active = false;

		if (WindowHandle->GetKeyState(SDL_SCANCODE_W))
		{
			Y -= MoveSpeed / WindowHandle->TimerHandle.GetFPS();
			//Looking = 3;
			Moving.Active = true;
		}
		if (WindowHandle->GetKeyState(SDL_SCANCODE_S))
		{
			Y += MoveSpeed / WindowHandle->TimerHandle.GetFPS();
			//Looking = 0;
			Moving.Active = true;
		}
		if (WindowHandle->GetKeyState(SDL_SCANCODE_A))
		{
			X -= MoveSpeed / WindowHandle->TimerHandle.GetFPS();
			//Looking = 2;
			Moving.Active = true;
		}
		if (WindowHandle->GetKeyState(SDL_SCANCODE_D))
		{
			X += MoveSpeed / WindowHandle->TimerHandle.GetFPS();
			//Looking = 1;
			Moving.Active = true;
		}

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

		Sword.SetCoords(X, Y - 30);//- 15);
		Sword.EventHandler();
	}


	//Virtual overrides
	void DisplayAll()
	{
		Display();
		Sword.Display();
		HealthBar.DisplayAll();
		PerFrameFunctions();
	}


	//Object specific methods
};

//======================================

//Bass class for all monsters
class Monster_t : public Entity_t
{
protected:

	Uint32 Clock_Wander;

	void ClearData()
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

	void Init(int in_X, int in_Y, int in_MoveSpeed)
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


	//Virtual overrides
	void DisplayAll()
	{
		Display();

		HealthBar.SetCoords(X, Y - (*Image)[(*ImageToDisplay)[0]].H() / 2);
		HealthBar.Display();

		PerFrameFunctions();
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





class Fencer_t : public Player_t
{
protected:

	//Used to clear ALL data from a class (use wisely)
	void ClearData()
	{
		//Todo: Add code to clear all attributes of the class
		Player_t::ClearData();
	}

public:

	//Calls all the constructors that need to be called
	Fencer_t() : Player_t()
	{
		ClearData();
	}

	//Associates the object with a window for later use. MUST be done either in a constructor (below) or by directly calling the "Register()" method
	Fencer_t(Window_t *in_Window) : Fencer_t()
	{
		Register(in_Window);
	}

	void Init()
	{		
		W = 16;
		H = 38;
		Facing = Down;
		MoveSpeed = 200;


		//Animations
		Moving.SetAll(1, 0.05, 8);
		Attacking.SetAll(33, 0.1, 8);
		Idle.SetAll(1, 0.1, 8);


		//Images
		AddLayer("Collision Box.png", { 0, 0, (int)W, (int)H }, { 255, 0, 153, 0 });
		for (int i = 0; i < 20; i++)	
		{
			for (int ii = 0; ii < 8; ii++)
				AddImage("Player.png", { (ii * 100) + 1, (i * 100) + 1, 98, 98 }, { 255, 255, 255, 0 });
		}
		AddLayer("Archer Hat.png", { NULL }, { 0xFF, 0xFF, 0xFF, 0 });
		AnimationLayer = 1;

		//Weapon
		MainWeapon.Range = 35;
		MainWeapon.Damage = 200;

		Sword.Register(WindowHandle);
		Sword.AddImage("Sword Arc.png", { NULL }, { 0xFF, 0xFF, 0xFF, 0xA0 });


		//Healthbar
		HealthBar.Register(WindowHandle);
		HealthBar.AddImage("Health.png", { 0, 50, 200, 20 }, { NULL });
		HealthBar.AddLayer("Health.png", { 0, 100, 200, 20 }, { NULL });
		HealthBar.SetCoords(HealthBar.GetW() / 2 + 10, HealthBar.GetH() / 2 + 10);
		HealthBar.Init();

		CurrentHealth = 100;
		InitialHealth = 100;
	}

	void Fence()
	{

	}
};

class Barbarian_t : public Player_t
{
protected:

	//Used to clear ALL data from a class (use wisely)
	void ClearData()
	{
		//Todo: Add code to clear all attributes of the class
		Player_t::ClearData();
	}

public:

	//Calls all the constructors that need to be called
	Barbarian_t() : Player_t()
	{
		ClearData();
	}

	//Associates the object with a window for later use. MUST be done either in a constructor (below) or by directly calling the "Register()" method
	Barbarian_t(Window_t *in_Window) : Barbarian_t()
	{
		Register(in_Window);
	}

	void Init()
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
		AddLayer("Barb Hat.png", { NULL }, { 0xFF, 0xFF, 0xFF, 0 });
		AnimationLayer = 1;

		//Weapon
		MainWeapon.Range = 35;
		MainWeapon.Damage = 200;

		Sword.Register(WindowHandle);
		Sword.AddImage("Sword Arc.png", { NULL }, { 0xFF, 0xFF, 0xFF, 0xA0 });


		//Healthbar
		HealthBar.Register(WindowHandle);
		HealthBar.AddImage("Health.png", { 0, 50, 200, 20 }, { NULL });
		HealthBar.AddLayer("Health.png", { 0, 100, 200, 20 }, { NULL });
		HealthBar.SetCoords(HealthBar.GetW() / 2 + 10, HealthBar.GetH() / 2 + 10);
		HealthBar.Init();

		CurrentHealth = 100;
		InitialHealth = 100;
	}
};

class Archer_t : public Player_t
{
protected:

	//Used to clear ALL data from a class (use wisely)
	void ClearData()
	{
		//Todo: Add code to clear all attributes of the class
		Player_t::ClearData();
	}

public:

	//Calls all the constructors that need to be called
	Archer_t() : Player_t()
	{
		ClearData();
	}

	//Associates the object with a window for later use. MUST be done either in a constructor (below) or by directly calling the "Register()" method
	Archer_t(Window_t *in_Window) : Archer_t()
	{
		Register(in_Window);
	}

	void Init()
	{		
		W = 16;
		H = 38;
		Facing = Down;
		MoveSpeed = 150;


		//Animations
		Moving.SetAll(1, 0.075, 8);
		Attacking.SetAll(33, 0.1, 8);
		Idle.SetAll(1, 0.1, 8);


		//Images
		AddLayer("Collision Box.png", { 0, 0, (int)W, (int)H }, { 255, 0, 153, 0 });
		for (int i = 0; i < 20; i++)	
		{
			for (int ii = 0; ii < 8; ii++)
				AddImage("Player.png", { (ii * 100) + 1, (i * 100) + 1, 98, 98 }, { 255, 255, 255, 0 });
		}
		AddLayer("Archer Hat.png", { NULL }, { 0xFF, 0xFF, 0xFF, 0 });
		AnimationLayer = 1;

		//Weapon
		MainWeapon.Range = 35;
		MainWeapon.Damage = 200;

		Sword.Register(WindowHandle);
		Sword.AddImage("Sword Arc.png", { NULL }, { 0xFF, 0xFF, 0xFF, 0xA0 });


		//Healthbar
		HealthBar.Register(WindowHandle);
		HealthBar.AddImage("Health.png", { 0, 50, 200, 20 }, { NULL });
		HealthBar.AddLayer("Health.png", { 0, 100, 200, 20 }, { NULL });
		HealthBar.SetCoords(HealthBar.GetW() / 2 + 10, HealthBar.GetH() / 2 + 10);
		HealthBar.Init();

		CurrentHealth = 100;
		InitialHealth = 100;
	}
};

class Paladin_t : public Player_t
{
protected:

	//Used to clear ALL data from a class (use wisely)
	void ClearData()
	{
		//Todo: Add code to clear all attributes of the class
		Player_t::ClearData();
	}

public:

	//Calls all the constructors that need to be called
	Paladin_t() : Player_t()
	{
		ClearData();
	}

	//Associates the object with a window for later use. MUST be done either in a constructor (below) or by directly calling the "Register()" method
	Paladin_t(Window_t *in_Window) : Paladin_t()
	{
		Register(in_Window);
	}

	void Init()
	{		
		W = 16;
		H = 38;
		Facing = Down;
		MoveSpeed = 50;


		//Animations
		Moving.SetAll(1, 0.2, 8);
		Attacking.SetAll(33, 0.1, 8);
		Idle.SetAll(1, 0.1, 8);	


		//Images
		AddLayer("Collision Box.png", { 0, 0, (int)W, (int)H }, { 255, 0, 153, 0 });
		for (int i = 0; i < 20; i++)	
		{
			for (int ii = 0; ii < 8; ii++)
				AddImage("Player.png", { (ii * 100) + 1, (i * 100) + 1, 98, 98 }, { 255, 255, 255, 0 });
		}
		AddLayer("Paladin Hat.png", { 0, 0, 100, 100 }, { 0xFF, 0xFF, 0xFF, 0 });
		AnimationLayer = 1;

		//Weapon
		MainWeapon.Range = 35;
		MainWeapon.Damage = 200;

		Sword.Register(WindowHandle);
		Sword.AddImage("Sword Arc.png", { NULL }, { 0xFF, 0xFF, 0xFF, 0xA0 });


		//Healthbar
		HealthBar.Register(WindowHandle);
		HealthBar.AddImage("Health.png", { 0, 50, 200, 20 }, { NULL });
		HealthBar.AddLayer("Health.png", { 0, 100, 200, 20 }, { NULL });
		HealthBar.SetCoords(HealthBar.GetW() / 2 + 10, HealthBar.GetH() / 2 + 10);
		HealthBar.Init();

		CurrentHealth = 100;
		InitialHealth = 100;
	}
};

class Mage_t : public Player_t
{
protected:

	//Used to clear ALL data from a class (use wisely)
	void ClearData()
	{
		//Todo: Add code to clear all attributes of the class
		Player_t::ClearData();
	}

public:

	//Calls all the constructors that need to be called
	Mage_t() : Player_t()
	{
		ClearData();
	}

	//Associates the object with a window for later use. MUST be done either in a constructor (below) or by directly calling the "Register()" method
	Mage_t(Window_t *in_Window) : Mage_t()
	{
		Register(in_Window);
	}

	void Init()
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
		AddLayer("Mage Hat.png", { NULL }, { 0xFF, 0xFF, 0xFF, 0 });
		AnimationLayer = 1;

		//Weapon
		MainWeapon.Range = 35;
		MainWeapon.Damage = 200;

		Sword.Register(WindowHandle);
		Sword.AddImage("Sword Arc.png", { NULL }, { 0xFF, 0xFF, 0xFF, 0xA0 });


		//Healthbar
		HealthBar.Register(WindowHandle);
		HealthBar.AddImage("Health.png", { 0, 50, 200, 20 }, { NULL });
		HealthBar.AddLayer("Health.png", { 0, 100, 200, 20 }, { NULL });
		HealthBar.SetCoords(HealthBar.GetW() / 2 + 10, HealthBar.GetH() / 2 + 10);
		HealthBar.Init();

		CurrentHealth = 100;
		InitialHealth = 100;
	}
};

