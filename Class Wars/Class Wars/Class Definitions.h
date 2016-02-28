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
		(*Image)[Image->size() - 1].SetCoords(X, Y, 0, 0);
		AddLayer(Image->size() - 1);
	}


	//Class specific methods
	bool IsPressed()
	{
		if (HoveringOver())
		{
			if (WindowHandle->GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT))
			{
				(*ImageToDisplay)[0] = 2;
				return true;
			}
		}
		else
			(*ImageToDisplay)[0] = 0;

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

	void ClearData()
	{
		Open = false;
		Moving = false;
		Speed = 0;

		UI_t::ClearData();
	}

public:

	Menu_t() : UI_t()
	{
		ClearData();
	}

	Menu_t(Window_t *in_Window) : Menu_t()
	{
		Register(in_Window);
	}

	void Init(double in_Speed)
	{
		Speed = in_Speed;

		//this->Button.Spawn(1, in_Window);
		//OpenButton.Register(in_Window);
		//OpenButton.AddImage("Arrow button.png", { 0, 0, 30, 64 }, { 0xff, 0xff, 0xff});	
		//OpenButton.AddImage("Arrow button.png", { 0, 0, 30, 64 }, { 0xff, 0xff, 0xff});
		//OpenButton.AddImage("Arrow button.png", { 0, 0, 30, 64 }, { 0xff, 0xff, 0xff});
		//OpenButton.SetCoords(OpenButton.GetW() / 2, WindowHandle->GetHeight() / 2);
	}

	void HandleEvents()
	{
		if (WindowHandle->GetKeyState(SDL_SCANCODE_TAB) && !Moving)
		{
			if (Open)
				Open = false;
			else
				Open = true;

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
	}
};

//======================================

//Base class for all Heads Up Displays
class HUD_t : public UI_t
{
protected:

	void ClearData()
	{
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
	}

	void DisplayAll()
	{
		Display();
		Selector.Display();
	}

	void HandleEvents()
	{
		int HUD_BOX_SIZE = (int) (W / 8);

		if (WindowHandle->GetKeyState(SDL_SCANCODE_1))
			Selector.SetCoords(X - W / 2 + Selector.GetW() / 2 + HUD_BOX_SIZE * 0 + 160, Y, 0, 0);
		if (WindowHandle->GetKeyState(SDL_SCANCODE_2))
			Selector.SetCoords(X - W / 2 + Selector.GetW() / 2 + HUD_BOX_SIZE * 1 + 160, Y, 0, 0);
		if (WindowHandle->GetKeyState(SDL_SCANCODE_3))
			Selector.SetCoords(X - W / 2 + Selector.GetW() / 2 + HUD_BOX_SIZE * 2 + 160, Y, 0, 0);
		if (WindowHandle->GetKeyState(SDL_SCANCODE_4))
			Selector.SetCoords(X - W / 2 + Selector.GetW() / 2 + HUD_BOX_SIZE * 3 + 160, Y, 0, 0);
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

	virtual void SetDisplay(int in_ImageIndex)
	{
		if (in_ImageIndex != 1)
			Object_t::SetDisplay(in_ImageIndex);
		else if (in_ImageIndex >= 0 && in_ImageIndex < (int) Image->size())
			(*Image)[in_ImageIndex].SetCoords(X - W / 2, Y - (*Image)[in_ImageIndex].H() / 2, (*Image)[in_ImageIndex].W(), (*Image)[in_ImageIndex].H());
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

	void SetPercentage(double in_Percentage)
	{ 
		//if (in_Percentage >= 0 && in_Percentage >= 1)
		//{
		//	(*Image)[1].SetCoords(X, Y, )
		//}
	}
};
 

//======================================//
//				Entities				//
//======================================//

//Base class for all entities
class Entity_t : public Object_t
{
protected:

	enum Direction
	{
		Down,
		Right,
		Left,
		Up,

		DownRight,
		DownLeft,
		UpRight,
		UpLeft,
	};

	struct Attack
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

	struct Animation
	{
		int StartPosition;
		int TotalNumber;
		double Duration; //In milliseconds
		double TimeDisplayed; //In milliseconds
		bool Active;

		void ClearData()
		{
			StartPosition = 0;
			TotalNumber = 0;
			Duration = 0;
			TimeDisplayed = 0;
			Active = 0;
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
	Direction Facing;

	Animation Idle;
	Animation Moving;
	Animation Attacking;
	Animation TakingDamage;

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

		Idle.ClearData();
		Moving.ClearData();
		Attacking.ClearData();
		TakingDamage.ClearData();

		Object_t::ClearData();
	}

public:

	Attack MainWeapon;

	Bar_t HealthBar;

	Entity_t() : Object_t()
	{
		ClearData();
	}

	Entity_t(Window_t *in_Window) : Entity_t()
	{
		Register(in_Window);
	}

	virtual void DisplayAll()
	{
		Display();
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
			Attacking.TimeDisplayed += 1 / WindowHandle->TimerHandle.GetFPS();

			while (Attacking.TimeDisplayed > Attacking.Duration)
			{
				Attacking.TimeDisplayed -= Attacking.Duration;

				if (++(*ImageToDisplay)[0] >= (Facing + Attacking.StartPosition + 1) * Attacking.TotalNumber ||
					(*ImageToDisplay)[0] < (Facing + Attacking.StartPosition) * Attacking.TotalNumber)
				{
					(*ImageToDisplay)[0] = (Facing + Attacking.StartPosition) * Attacking.TotalNumber + 1;
				}
			}
		}
		else if (Moving.Active)
		{
			Moving.TimeDisplayed += 1 / WindowHandle->TimerHandle.GetFPS();

			while (Moving.TimeDisplayed > Moving.Duration)
			{
				Moving.TimeDisplayed -= Moving.Duration;

				if (++(*ImageToDisplay)[0] >= (Facing + Moving.StartPosition + 1) * Moving.TotalNumber ||
					(*ImageToDisplay)[0] < (Facing + Moving.StartPosition) * Moving.TotalNumber)
				{
					(*ImageToDisplay)[0] = (Facing + Moving.StartPosition) * Moving.TotalNumber + 1;
				}
			}
		}
		else
			(*ImageToDisplay)[0] = (Facing + Idle.StartPosition) * Idle.TotalNumber;
	}


	//Class specific methods
	int Move(Direction in_Direction)
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

	int Move(int in_X, int in_Y)
	{
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
		}

		return 0;
	}

	void Attack(Entity_t *in_Victim, Attack Weapon)
	{
		in_Victim->TakeDamage(Weapon.Damage);
	}

	void TakeDamage(double in_Damage)
	{
		CurrentHealth -= in_Damage;
		
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

			(*Image)[0].SetImageProp(Angle - 90, { (*Image)[0].W() / 2, (*Image)[0].H() + 30 }, SDL_FLIP_NONE);
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
		Facing = Down;
		MainWeapon.Range = 30;
		MainWeapon.Damage = 10;

		Moving.Duration = 0.1;
		Moving.StartPosition = 0;
		Moving.TotalNumber = 8;
		
		Attacking.Duration = 0.1;
		Attacking.StartPosition = 4;
		Attacking.TotalNumber = 8;
	
		Sword.Register(WindowHandle);
		Sword.AddImage("Sword Arc.png", { NULL }, { 0xFF, 0xFF, 0xFF, 0xA0 });
	}

	void EventHandler()
	{
		Moving.Active = false;

		if (WindowHandle->GetKeyState(SDL_SCANCODE_W))
		{
			Y -= 100 / WindowHandle->TimerHandle.GetFPS();
			//Looking = 3;
			Moving.Active = true;
		}
		if (WindowHandle->GetKeyState(SDL_SCANCODE_S))
		{
			Y += 100 / WindowHandle->TimerHandle.GetFPS();
			//Looking = 0;
			Moving.Active = true;
		}
		if (WindowHandle->GetKeyState(SDL_SCANCODE_A))
		{
			X -= 100 / WindowHandle->TimerHandle.GetFPS();
			//Looking = 2;
			Moving.Active = true;
		}
		if (WindowHandle->GetKeyState(SDL_SCANCODE_D))
		{
			X += 100 / WindowHandle->TimerHandle.GetFPS();
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

		Sword.SetCoords(X, Y - MainWeapon.Range);//- 15);
		Sword.EventHandler();
	}


	//Virtual overrides
	void DisplayAll()
	{
		Display();
		Sword.Display();
	}


	//Object specific methods
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

	bool CanAttack(Entity_t *in_Victim)
	{
		SDL_Rect Temp;

		Temp.x = (int) X;
		Temp.y = (int) Y;
		Temp.w = (int) (W + 2 * MainWeapon.Range);
		Temp.h = (int) (H + 2 * MainWeapon.Range);

		//{ in_Victim->GetX(), in_Victim->GetY(), in_Victim->GetW() + 2 * AttackRange, in_Victim->GetH() + 2 * AttackRange }

		if (in_Victim->IsOverlapping(Temp) &&
			IsFacing(in_Victim))
			return true;
		else
			return false;
	}
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
		SetCoords(in_X, in_Y, 32, 32);
		MoveSpeed = in_MoveSpeed;
		DestinationX = X;
		DestinationY = Y;
		InitialHealth = 100;
		CurrentHealth = 100;

		HealthBar.Register(WindowHandle);
		HealthBar.AddImage("Health.png", { 0, 50, 50, 5 }, { NULL });
		HealthBar.AddLayer("Health.png", { 0, 100, 50, 5 }, { NULL });
	}


	//Virtual overrides
	void DisplayAll()
	{
		Display();

		HealthBar.SetCoords(X, Y - H / 2);

		HealthBar.Display();
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
			Move((int) DestinationX, (int) DestinationY);
	}
};
