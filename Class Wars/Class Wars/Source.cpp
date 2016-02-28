#include <SLGE.h>


using namespace SLGE;
const int ARRAY_SIZE = 10;


//
//	Declare your classes and window (Can be in separate header or dll but must come after classes are defined)
//

class AI_t;
class Player_t;
class Other_t;
class UI_t;
class Menu_t;
class HUD_t;


Window_t Window1(640, 480, "Cooler Program", false);

DynamicClass <Object_t> Background;
DynamicClass <AI_t, Object_t> Array;
DynamicClass <Player_t, Object_t> Player;
DynamicClass <Other_t, Player_t, Object_t> Other;
DynamicClass <UI_t, Object_t> Button;
DynamicClass <Menu_t, UI_t, Object_t> Inventory;
DynamicClass <HUD_t, Object_t> HUD;


//
//  Define your classes	(Can be in separate header or dll)
//

class AI_t : public Object_t
{
public:

	AI_t()
	{
	}

	AI_t(Window_t *in_Window)
	{
		WindowHandle = in_Window;
		OpenImage("1.png", { 0, 0, 32, 32 }, { 255, 0, 153, 0 });
	}
};

class Player_t : public Object_t
{
	friend class DynamicClass <Player_t>;

public:

	Player_t()
	{
	}

	Player_t(Window_t *in_Window)
	{
		WindowHandle = in_Window;

		for (int i = 0; i < 20; i++)
		{
			for (int ii = 0; ii < 8; ii++)
				OpenImage("Player.png", { (ii * 100) + 1, (i * 100) + 1, 98, 98 }, { 255, 255, 255, 0 });
		}
	}

	void EventHandler()
	{
		if (WindowHandle->GetKeyState(SDL_SCANCODE_W))
			Y -= 100 / Window1.TimerHandle.GetFPS();
		if (WindowHandle->GetKeyState(SDL_SCANCODE_S))
			Y += 100 / Window1.TimerHandle.GetFPS();
		if (WindowHandle->GetKeyState(SDL_SCANCODE_A))
			X -= 100 / Window1.TimerHandle.GetFPS();
		if (WindowHandle->GetKeyState(SDL_SCANCODE_D))
			X += 100 / Window1.TimerHandle.GetFPS();

		if (WindowHandle->GetKeyState(SDL_SCANCODE_Q))
		{
			if (Array.NumberOfObjects() > 5)
				Array.Despawn(0, 1);
		}
		if (WindowHandle->GetKeyState(SDL_SCANCODE_E))
		{
			Array.Spawn(1, &Window1);
			Array[Array.NumberOfObjects() - 1].OpenImage("1.png", { 0, 0, 32, 32 }, { NULL });
			Array[Array.NumberOfObjects() - 1].SetCoords(rand() % Window1.GetWidth(), rand() % Window1.GetHeight(), 32, 32);
		}
	}

	void Animate()
	{
		if (++ImageToDisplay >= NumberOfImages)
			ImageToDisplay = 0;
	}
};

class Other_t : public Player_t
{
public:

	Other_t()
	{
	}

	Other_t(Window_t *in_Window)
	{
		WindowHandle = in_Window;
		OpenImage("1.png", { 0, 0, 32, 32 }, { 255, 0, 153, 0 });
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

		OpenImage("2.png", { 0, 0, 32, 32 }, { 255, 0, 153, 0 });
		OpenImage("1.png", { 0, 0, 32, 32 }, { 255, 0, 153, 0 });
		OpenImage("3.png", { 0, 0, 32, 32 }, { 255, 0, 153, 0 });
	}

	bool IsPressed()
	{
		int null;

		if (HoveringOver())
		{
			if (WindowHandle->GetMouseState(null, null) & SDL_BUTTON(SDL_BUTTON_LEFT))
			{
				ImageToDisplay = 1;
				return true;
			}
		}
		else
			ImageToDisplay = 0;

		return false;
	}

	bool HoveringOver()
	{
		int MouseX = 0; 
		int MouseY = 0;

		WindowHandle->GetMouseState(MouseX, MouseY);

		if (IsOverlapping({ MouseX, MouseY, 1, 1 }))
		{
			ImageToDisplay = 2;
			return true;
		}
		else
		{
			return false;
		}
	}
};

class Menu_t : public UI_t
{
	bool Open;
	bool Moving;
	double Speed; //in pixels per second

public:

	DynamicClass <UI_t, Object_t> Button;

	Menu_t()
	{
	}

	Menu_t(Window_t *in_Window)
	{
		Open = false;

		Register(in_Window);

		Button.Spawn(1, in_Window);

		OpenImage("Menu.png", { NULL }, { NULL });
		OpenImage("Menu.png", { NULL }, { NULL });
		OpenImage("Menu.png", { NULL }, { NULL });
		SetCoords(-250, WindowHandle->GetHeight() / 2, 500, 400);

		//Button[0].OpenImage("3.png", { 0, 0, 32, 32 }, { 255, 0, 153, 0 });
		Button[0].SetCoords(32, WindowHandle->GetHeight() / 2, 32, 32);

		Speed = 1000;
	}

	void HandleEvents()
	{
		if (Button[0].IsPressed() && !Moving)
		{
			if (Open)
				Open = false;
			else
				Open = true;

			Moving = true;
		}

		if (Moving)
		{
			if (X < 250 && Open)
			{
				X += Speed / WindowHandle->TimerHandle.GetFPS();
				Button[0].SetCoords(Button[0].GetX() + Speed / WindowHandle->TimerHandle.GetFPS(), Button[0].GetY());
			}
			else if (X > -250 && !Open)
			{
				X -= Speed / WindowHandle->TimerHandle.GetFPS();
				Button[0].SetCoords(Button[0].GetX() - Speed / WindowHandle->TimerHandle.GetFPS(), Button[0].GetY());
			}
			else
				Moving = false;
		}
	}
};

class HUD_t : public UI_t
{

public:

	DynamicClass <Object_t> Selector;

	HUD_t()
	{

	}

	HUD_t(Window_t *in_Window)
	{
		Register(in_Window);
		
		OpenImage("HUD.png", { NULL }, { NULL });
		SetCoords(Window1.GetWidth() / 2, Window1.GetHeight() - 50, 450, 50);
	}

	void HandleEvents()
	{
		if (WindowHandle->GetKeyState(SDL_SCANCODE_1))
			Selector[0].SetCoords(X - W / 2 + Selector[0].GetW() / 2, Y);
		if (WindowHandle->GetKeyState(SDL_SCANCODE_2))
			Selector[0].SetCoords(X - W / 2 + Selector[0].GetW() / 2 + 48, Y);
		if (WindowHandle->GetKeyState(SDL_SCANCODE_3))
			Selector[0].SetCoords(X - W / 2 + Selector[0].GetW() / 2 + 97, Y);
		if (WindowHandle->GetKeyState(SDL_SCANCODE_4))
			Selector[0].SetCoords(X - W / 2 + Selector[0].GetW() / 2 + 145, Y);
		if (WindowHandle->GetKeyState(SDL_SCANCODE_5))
			Selector[0].SetCoords(X - W / 2 + Selector[0].GetW() / 2 + 193, Y);
		if (WindowHandle->GetKeyState(SDL_SCANCODE_6))
			Selector[0].SetCoords(X - W / 2 + Selector[0].GetW() / 2 + 241, Y);
		if (WindowHandle->GetKeyState(SDL_SCANCODE_7))
			Selector[0].SetCoords(X - W / 2 + Selector[0].GetW() / 2 + 290, Y);
		if (WindowHandle->GetKeyState(SDL_SCANCODE_8))
			Selector[0].SetCoords(X - W / 2 + Selector[0].GetW() / 2 + 340, Y);
		if (WindowHandle->GetKeyState(SDL_SCANCODE_9))
			Selector[0].SetCoords(X - W / 2 + Selector[0].GetW() / 2 + 390, Y);
	}
};


//
//  Declare the game init, loop, and despawn code (Can be in separate header or dll but must come after classes are initialized)
//

void SpawnGame()
{
	//Spawn
	Background.Spawn(1, &Window1);
	Array.Spawn(10, &Window1);
	Player.Spawn(2, &Window1);
	Other.Spawn(5, &Window1);
	Button.Spawn(3, &Window1);
	Inventory.Spawn(1, &Window1);
	HUD.Spawn(1, &Window1);

	HUD[0].Selector.Spawn(1, &Window1);
	HUD[0].Selector[0].OpenImage("HUD Selector.png", { NULL }, { NULL });
	HUD[0].Selector[0].SetCoords(HUD[0].GetX() - HUD[0].GetW() / 2 + 442 / 2, HUD[0].GetY(), 441, 50);

	//Set the objects in their place
	Background[0].OpenImage("Image.png", { NULL }, { NULL });
	Background[0].SetCoords(0, 0, Window1.GetWidth(), Window1.GetHeight());


	for (int i = 0; i < Array.NumberOfObjects(); i++)
		Array[i].SetCoords(i * 50, 240, 32, 32);

	Player[0].SetCoords(300, 500, 32, 32);
	Player[1].SetCoords(100, 100, 32, 32);

	for (int i = 0; i < Other.NumberOfObjects(); i++)
		Other[i].SetCoords(i * 100 + 100, 400, 32, 32);

	for (int i = 0; i < Button.NumberOfObjects(); i++)
		Button[i].SetCoords(Window1.GetWidth() / 2, i * 100 + 100, 32, 32);
}

void RunGame()
{
	DynamicClass <Player_t>::All(&Player_t::EventHandler);
	DynamicClass <Menu_t>::All(&Menu_t::HandleEvents);
	DynamicClass <HUD_t>::All(&HUD_t::HandleEvents);

	DynamicClass <UI_t>::All(&UI_t::IsPressed);

	DynamicClass <Object_t>::All(&Object_t::Display);
}

void DespawnGame()
{
	std::cout << "Number of Objects on Screen: " << Array.NumberOfObjects() << std::endl;

	Background.Despawn();
	Array.Despawn();
	Player.Despawn();
	Other.Despawn();
	Inventory.Despawn();
	HUD.Despawn();
}


//
//  Main program loop will look simple
//

extern "C" int SDL_main(int argc, char* argv[])
{
	srand(unsigned(time(NULL)));

	//Do Window1.Run(init, loop, despawn) on any new screen
	Window1.Run(SpawnGame, RunGame, DespawnGame);

	system("PAUSE");

	return 0;
}
