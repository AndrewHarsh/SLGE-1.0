#include <SLGE.h>


using namespace SLGE;
const bool HARDWARE_ACCELERATED = true;
const int WIDTH = 640;
const int HEIGHT = 480;
const int ARRAY_SIZE = 10;
const double PI = 3.141592653589793238463;
const double RADIANS_TO_DEGREES = 180 / PI;



//TEMPLATE CLASS
//	Use to create new classes
//	Replace "TEMPLATE_t" with "<Class Name>_t"
//	Replace "Object_t" with "<Inherited Class Name>_t" but ALL classes MUST ultimately inherit from "Object_t"
class TEMPLATE_t : public Object_t
{
protected:

	void ClearData()
	{
		//Todo: Add code to clear all attributes of the class
	}

public:

	TEMPLATE_t() : Object_t()
	{
		ClearData();
	}

	TEMPLATE_t(Window_t *in_Window) : TEMPLATE_t()
	{
		Register(in_Window);
	}
};


//
//  Define your classes	(Can be in separate header or dll)
//
class Entity_t : public Object_t
{
protected:

	enum Direction
	{
		Up,
		UpRight,
		Right,
		DownRight,
		Down,
		DownLeft,
		Left,
		UpLeft
	};

	//Movement
	double MoveSpeed; //In Pixels per second
	double TargetX;
	double TargetY;
	double LastX;
	double LastY;

	//Attack
	double AttackDamage;
	double AttackSpeed;
	double CurrentHealth;
	double InitialHealth;

	//Animation
	Direction Facing;

	int Animate_IdleSpeed;
	int Animate_MovingSpeed;
	int Animate_AttackingSpeed;
	int Animate_HitSpeed;

	//Status
	bool Idle;
	bool Moving;
	bool Attacking;
	bool Hit;

	void ClearData()
	{
		MoveSpeed = 0; //In Pixels per second
		TargetX = 0;
		TargetY = 0;
		LastX = 0;
		LastY = 0;

		//Attack
		AttackDamage = 0;
		AttackSpeed = 0;
		CurrentHealth = 0;
		InitialHealth = 0;

		//Animation
		Facing = Down;

		Animate_IdleSpeed = 0;
		Animate_MovingSpeed = 0;
		Animate_AttackingSpeed = 0;
		Animate_HitSpeed = 0;

		//Status
		Idle = false;
		Moving = false;
		Attacking = false;
		Hit = false;

		Object_t::ClearData();
	}

public:

	Entity_t() : Object_t()
	{
		ClearData();
	}

	Entity_t(Window_t *in_Window) : Entity_t()
	{
		Register(in_Window);
	}

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
			Moving = false;
			return 0;
			break;
		}

		Moving = true;
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

		if (IsOverlapping({ in_X, in_Y, MoveSpeed / WindowHandle->TimerHandle.GetFPS(), MoveSpeed / WindowHandle->TimerHandle.GetFPS() }))
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

	void Attack(Entity_t *in_Victim)
	{
		in_Victim->TakeDamage(AttackDamage);
	}

	void TakeDamage(double in_Damage)
	{
		CurrentHealth -= in_Damage;
	}

	virtual void Animate()
	{
		
	}
};

class Player_t : public Entity_t
{
	int Looking;
	double ImagesPerSecond;
	double AnimateCounter;
	bool Moving;

public:

	class Sword_t : public Object_t
	{
	protected:

		void ClearData()
		{
			//Todo: Add code to clear all attributes of the class
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
			WindowHandle->GetMouseState(MouseX, MouseY);

			double Angle = atan((MouseY - Y) / (MouseX - X)) * RADIANS_TO_DEGREES;

			if (MouseX > X)
				Angle -= 180;

			(*Image)[0].SetImageProp(Angle - 90, { (*Image)[0].W() / 2, (*Image)[0].H() + 30 }, SDL_FLIP_NONE);
		}

	} Sword;

	friend class Sword_t;

	Player_t()
	{
	}

	Player_t(Window_t *in_Window)
	{
		Register(in_Window);  

		Looking = 0;
		ImagesPerSecond = 0.1;
		AnimateCounter = 0;
		Moving = false;

		Sword.Register(in_Window);
		Sword.AddImage("Sword Arc.png", { NULL }, { 0xFF, 0xFF, 0xFF });
	}

	void EventHandler()
	{
		Moving = false;

		if (WindowHandle->GetKeyState(SDL_SCANCODE_W))
		{
			Y -= 100 / WindowHandle->TimerHandle.GetFPS();
			Looking = 3;
			Moving = true;
		}
		if (WindowHandle->GetKeyState(SDL_SCANCODE_S))
		{
			Y += 100 / WindowHandle->TimerHandle.GetFPS();
			Looking = 0;
			Moving = true;
		}
		if (WindowHandle->GetKeyState(SDL_SCANCODE_A))
		{
			X -= 100 / WindowHandle->TimerHandle.GetFPS();
			Looking = 2;
			Moving = true;
		}
		if (WindowHandle->GetKeyState(SDL_SCANCODE_D))
		{
			X += 100 / WindowHandle->TimerHandle.GetFPS();
			Looking = 1;
			Moving = true;
		}

		Sword.SetCoords(X, Y - 30);//- 15);
		Sword.EventHandler();
	}

	void Animate()
	{
		//if (++ImageToDisplay >= NumberOfImages)
		//	ImageToDisplay = 0;

		if (Moving)
		{
			AnimateCounter += 1 / WindowHandle->TimerHandle.GetFPS();

			while (AnimateCounter > ImagesPerSecond)
			{
				AnimateCounter -= ImagesPerSecond;

				if (++(*ImageToDisplay)[0] >= (Looking + 1) * 8 || (*ImageToDisplay)[0] < Looking * 8)
					(*ImageToDisplay)[0] = Looking * 8;

			}
		}
		else
			(*ImageToDisplay)[0] = Looking * 8;
	}
};

class Monster_t : public Entity_t
{
protected:

	Uint32 Clock_Wander;

	void ClearData()
	{
		Clock_Wander = 0;
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
		TargetX = X;
		TargetY = Y;
	}

	//moves object to random position in rectangle with sides WIDTH and HEIGHT and middle of rectangle located at (XMiddle, YMiddle). object stays at random position for TIME seconds
	void Wander(SDL_Rect in_WanderArea, double in_WaitTime)
	{
		/*
		if (SDL_GetTicks() - Clock_Wander > in_WaitTime * 1000)//sets random points within the rectangle and begins the failure timer
		{
			TargetX = rand() % in_WanderArea.w + (in_WanderArea.x - in_WanderArea.w / 2);//sets random x
			TargetY = rand() % in_WanderArea.h + (in_WanderArea.y - in_WanderArea.h / 2);//sets random y
			Clock_Wander = SDL_GetTicks();// +10000 * in_WaitTime;//this keeps the object from getting stuck while moving towards a non-reachable coordinate. 
		}
		*/

		if (IsWithin({ TargetX, TargetY, MoveSpeed, MoveSpeed }))
		{
			TargetX = rand() % (in_WanderArea.w + (in_WanderArea.x - in_WanderArea.w / 2));//sets random x
			TargetY = rand() % (in_WanderArea.h + (in_WanderArea.y - in_WanderArea.h / 2));//sets random y
		}
		else
			Move(TargetX, TargetY);
	}

};



class UI_t : public Object_t
{
public:

	UI_t()
	{
	}

	UI_t(Window_t *in_Window)
	{
		Register(in_Window);
	}

	void Init()
	{
		AddImage("2.png", { 0, 0, 32, 32 }, { 255, 0, 153, 0 });
		AddImage("1.png", { 0, 0, 32, 32 }, { 255, 0, 153, 0 });
		AddImage("3.png", { 0, 0, 32, 32 }, { 255, 0, 153, 0 });
	}

	bool IsPressed()
	{
		int null;

		if (HoveringOver())
		{
			if (WindowHandle->GetMouseState(null, null) & SDL_BUTTON(SDL_BUTTON_LEFT))
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

		WindowHandle->GetMouseState(MouseX, MouseY);

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
	}

public:

	Button_t() //: UI_t()
	{
	}

	Button_t(Window_t *in_Window) //: UI_t()
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
};

class Menu_t : public Object_t
{
protected:

	bool Open;
	bool Moving;
	double Speed; //in pixels per second

public:

	Button_t OpenButton;

	Menu_t() //: UI_t()
	{
	}

	Menu_t(Window_t *in_Window) //: UI_t()
	{
		Register(in_Window);
		Speed = 1000;
		Open = false;

		//this->Button.Spawn(1, in_Window);
		OpenButton.Register(in_Window);
		OpenButton.AddImage("Arrow button.png", { 0, 0, 30, 64 }, { 0xff, 0xff, 0xff});	
		OpenButton.AddImage("Arrow button.png", { 0, 0, 30, 64 }, { 0xff, 0xff, 0xff});
		OpenButton.AddImage("Arrow button.png", { 0, 0, 30, 64 }, { 0xff, 0xff, 0xff});
		OpenButton.SetCoords(OpenButton.GetW() / 2, WindowHandle->GetHeight() / 2);
	}

	void HandleEvents()
	{
		if (OpenButton.IsPressed() && !Moving)
		{
			if (Open)
				Open = false;
			else
				Open = true;

			Moving = true;
		}

		if (Moving)
		{
			if (X < W / 2 && Open)
			{
				X += Speed / WindowHandle->TimerHandle.GetFPS();
				OpenButton.SetCoords(OpenButton.GetX() + Speed / WindowHandle->TimerHandle.GetFPS(), OpenButton.GetY());

				if (X > W / 2)
				{
					X = W / 2;
					OpenButton.SetCoords(W + OpenButton.GetW() / 2, OpenButton.GetY());
				}
			}
			else if (X > -W / 2 && !Open)
			{
				X -= Speed / WindowHandle->TimerHandle.GetFPS();
				OpenButton.SetCoords(OpenButton.GetX() - Speed / WindowHandle->TimerHandle.GetFPS(), OpenButton.GetY());

				if (X < -W / 2)
				{
					X = -W / 2 ;
					OpenButton.SetCoords(OpenButton.GetW() / 2, OpenButton.GetY());
				}
			}
			else
				Moving = false;
		}
	}
};

class HUD_t : public Object_t
{

public:

	Object_t Selector;

	HUD_t() //: UI_t()
	{

	}

	HUD_t(Window_t *in_Window) //: UI_t()
	{
		Register(in_Window);

		Selector.Register(in_Window);
		Selector.AddImage("HUD Selector.png", { NULL }, { NULL });
		Selector.SetCoords(WIDTH / 2 - 441 / 2 + Selector.GetW() / 2, HEIGHT - 50);
	}

	void HandleEvents()
	{
		int HUD_BOX_SIZE = W / 9;

		if (WindowHandle->GetKeyState(SDL_SCANCODE_1))
			Selector.SetCoords(X - W / 2 + Selector.GetW() / 2 + HUD_BOX_SIZE * 0, Y);
		if (WindowHandle->GetKeyState(SDL_SCANCODE_2))
			Selector.SetCoords(X - W / 2 + Selector.GetW() / 2 + HUD_BOX_SIZE * 1, Y);
		if (WindowHandle->GetKeyState(SDL_SCANCODE_3))
			Selector.SetCoords(X - W / 2 + Selector.GetW() / 2 + HUD_BOX_SIZE * 2, Y);
		if (WindowHandle->GetKeyState(SDL_SCANCODE_4))
			Selector.SetCoords(X - W / 2 + Selector.GetW() / 2 + HUD_BOX_SIZE * 3, Y);
		if (WindowHandle->GetKeyState(SDL_SCANCODE_5))
			Selector.SetCoords(X - W / 2 + Selector.GetW() / 2 + HUD_BOX_SIZE * 4, Y);
		if (WindowHandle->GetKeyState(SDL_SCANCODE_6))
			Selector.SetCoords(X - W / 2 + Selector.GetW() / 2 + HUD_BOX_SIZE * 5, Y);
		if (WindowHandle->GetKeyState(SDL_SCANCODE_7))
			Selector.SetCoords(X - W / 2 + Selector.GetW() / 2 + HUD_BOX_SIZE * 6, Y);
		if (WindowHandle->GetKeyState(SDL_SCANCODE_8))
			Selector.SetCoords(X - W / 2 + Selector.GetW() / 2 + HUD_BOX_SIZE * 7, Y);
		if (WindowHandle->GetKeyState(SDL_SCANCODE_9))
			Selector.SetCoords(X - W / 2 + Selector.GetW() / 2 + HUD_BOX_SIZE * 8, Y);
	}
};






//
//	Declare your class objects and window (Can be in separate header or dll but must come after classes are defined)
//


Window_t Window1(WIDTH, HEIGHT, "Dungeon!", HARDWARE_ACCELERATED);

DynamicClass <Object_t> Background;
DynamicClass <Monster_t, Entity_t, Object_t> DummyTarget;
DynamicClass <Player_t, Entity_t, Object_t> Player;

DynamicClass <Button_t, Object_t> Button;
DynamicClass <Menu_t, Object_t> Inventory;
DynamicClass <HUD_t, Object_t> HUD;



//
//  Declare the game init, loop, and despawn code (Can be in separate header or dll but must come after classes are initialized)
//

FunctionReturn SpawnMainMenu()
{
	Background.Spawn(1, &Window1);
	Button.Spawn(4, &Window1);

	Background[0].AddImage("Main Menu.png", { 0, 0, 640, 480}, { NULL });
	Background[0].SetCoords(WIDTH / 2, HEIGHT / 2, 0, 0);

	for (int i = 0; i < Button.NumberOfObjects(); i++)
	{
		Button[i].AddImage("Main Menu Button.png", { 0, 0, 300, 50 }, { 255, 255, 255 });
		Button[i].AddImage("Main Menu Button.png", { 300, 0, 300, 50 }, { 255, 255, 255 });
		Button[i].AddImage("Main Menu Button.png", { 600, 0, 300, 50 }, { 255, 255, 255 });
		Button[i].SetCoords(WIDTH / 2, HEIGHT / 2 - (Button.NumberOfObjects() / 2 * 70) + 70 * i + 70);
	}

	Button[0].Init("Play");
	Button[1].Init("Options");
	Button[3].Init("Exit Game");

	return Continue;
}

FunctionReturn RunMainMenu()
{
	if (Button[0].IsPressed())
		return Exit;
	if (Button[1].IsPressed())
		return Exit;
	if (Button[2].IsPressed())
		return Exit;
	if (Button[3].IsPressed())
		return Exit;

	DynamicClass <Object_t>::All(&Object_t::Display);

	return Continue;
}

FunctionReturn DespawnMainMenu()
{
	Background.Despawn();
	Button.Despawn();

	return Continue;
}




FunctionReturn SpawnGame()
{
	//Spawn
	Background.Spawn(1, &Window1);
	DummyTarget.Spawn(10, &Window1);
	Player.Spawn(1, &Window1);
	Inventory.Spawn(1, &Window1);
	HUD.Spawn(1, &Window1);


	//Set the objects in their place and load images
	Background[0].AddImage("Image.png", { NULL }, { NULL });
	Background[0].SetCoords(WIDTH / 2, HEIGHT / 2);

	for (int i = 0; i < DummyTarget.NumberOfObjects(); i++)
	{
		DummyTarget[i].AddImage("1.png", { 0, 0, 32, 32 }, { 255, 0, 153, 0 });
		DummyTarget[i].Init(rand() % WIDTH, rand() % HEIGHT, 50);
	}

	for (int i = 0; i < 20; i++)
	{
		for (int ii = 0; ii < 8; ii++)
			Player[0].AddImage("Player.png", { (ii * 100) + 1, (i * 100) + 1, 98, 98 }, { 255, 255, 255, 0 });
	}
	Player[0].SetCoords(WIDTH / 2, HEIGHT / 2, 0, 0);

	Inventory[0].AddImage("Menu.png", { NULL }, { NULL });
	Inventory[0].SetCoords(-212, HEIGHT / 2);

	HUD[0].AddImage("HUD.png", { NULL }, { NULL });
	HUD[0].SetCoords(WIDTH / 2, HEIGHT - 50);


	return Continue;
}

FunctionReturn RunGame()
{
	DynamicClass <Player_t>::All(&Player_t::EventHandler);
	DynamicClass <Menu_t>::All(&Menu_t::HandleEvents);
	DynamicClass <HUD_t>::All(&HUD_t::HandleEvents);


	if (Window1.GetKeyState(SDL_SCANCODE_Q))
	{
		DummyTarget.Despawn(0, 1);
	}
	if (Window1.GetKeyState(SDL_SCANCODE_E))
	{
		DummyTarget.Spawn(1, &Window1);
		DummyTarget[DummyTarget.NumberOfObjects() - 1].AddImage("1.png", { 0, 0, 32, 32 }, { NULL });
		DummyTarget[DummyTarget.NumberOfObjects() - 1].Init(rand() % Window1.GetWidth(), rand() % Window1.GetHeight(), 50);
	}


	//Scroll screen
	int ScrollSpeed = 150 / Window1.TimerHandle.GetFPS();
	int BoxSize = 50;

	if (Player[0].GetX() > WIDTH / 2 + BoxSize)
	{
		Background[0].SetCoords(Background[0].GetX() - ScrollSpeed, Background[0].GetY());
		Player[0].SetCoords(WIDTH / 2 + BoxSize, Player[0].GetY());

		for (int i = 0; i < DummyTarget.NumberOfObjects(); i++)
			DummyTarget[i].SetCoords(DummyTarget[i].GetX() - ScrollSpeed, DummyTarget[i].GetY());
	}

	if (Player[0].GetX() < WIDTH / 2 - BoxSize)
	{
		Background[0].SetCoords(Background[0].GetX() + ScrollSpeed, Background[0].GetY());
		Player[0].SetCoords(WIDTH / 2 - BoxSize, Player[0].GetY());

		for (int i = 0; i < DummyTarget.NumberOfObjects(); i++)
			DummyTarget[i].SetCoords(DummyTarget[i].GetX() + ScrollSpeed, DummyTarget[i].GetY());
	}

	if (Player[0].GetY() > HEIGHT / 2 + BoxSize)
	{
		Background[0].SetCoords(Background[0].GetX(), Background[0].GetY() - ScrollSpeed);
		Player[0].SetCoords(Player[0].GetX(), HEIGHT / 2 + BoxSize);

		for (int i = 0; i < DummyTarget.NumberOfObjects(); i++)
			DummyTarget[i].SetCoords(DummyTarget[i].GetX(), DummyTarget[i].GetY() - ScrollSpeed);
	}

	if (Player[0].GetY() < HEIGHT / 2 - BoxSize)
	{
		Background[0].SetCoords(Background[0].GetX(), Background[0].GetY() + ScrollSpeed);
		Player[0].SetCoords(Player[0].GetX(), HEIGHT / 2 - BoxSize);

		for (int i = 0; i < DummyTarget.NumberOfObjects(); i++)
			DummyTarget[i].SetCoords(DummyTarget[i].GetX(), DummyTarget[i].GetY() + ScrollSpeed);
	}


	SDL_Rect Temp = { WIDTH / 2, HEIGHT / 2, WIDTH, HEIGHT };
	DynamicClass <Monster_t>::All(&Monster_t::Wander, Temp, NULL);



	


	Player[0].Animate();

	//Last entity Displayed
	Background[0].Display();
	DynamicClass <Object_t>::All(&Object_t::Display);
	Player[0].Sword.Display();

	DynamicClass <Menu_t>::All(&Menu_t::Display);
	Inventory[0].OpenButton.Display();

	DynamicClass <HUD_t>::All(&HUD_t::Display);
	HUD[0].Selector.Display();

	return Continue;
}

FunctionReturn DespawnGame()
{
	//std::cout << "Number of Objects on Screen: " << DummyTarget.NumberOfObjects() << std::endl;

	Background.Despawn();
	DummyTarget.Despawn();
	Player.Despawn();
	Inventory.Despawn();
	HUD.Despawn();

	return Continue;
}


//
//  Main program loop will look simple
//

extern "C" int SDL_main(int argc, char* argv[])
{
	srand(unsigned(time(NULL)));

	if (Window1.Run(SpawnMainMenu, RunMainMenu, DespawnMainMenu) == Error)
		return 1;

	if (Window1.Run(SpawnGame, RunGame, DespawnGame) == Error)
		return 1;

	if (Window1.Run(SpawnMainMenu, RunMainMenu, DespawnMainMenu) == Error)
		return 1;

	return 0;
}
