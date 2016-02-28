#include <SLGE.h>


using namespace SLGE;
const int ARRAY_SIZE = 10;


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
	};

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
			DisplayClip[ImageToDisplay].y -= 1;
		if (WindowHandle->GetKeyState(SDL_SCANCODE_S))
			DisplayClip[ImageToDisplay].y += 1;
		if (WindowHandle->GetKeyState(SDL_SCANCODE_A))
			DisplayClip[ImageToDisplay].x -= 1;
		if (WindowHandle->GetKeyState(SDL_SCANCODE_D))
			DisplayClip[ImageToDisplay].x += 1;
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
	};

	Other_t(Window_t *in_Window)
	{
		WindowHandle = in_Window;
		OpenImage("1.png", { 0, 0, 32, 32 }, { 255, 0, 153, 0 });
	}
};


//
//	Declare your classes and window (Can be in separate header or dll but must come after classes are defined)
//

Window_t Window1(640, 480, "Cooler Program", false);

DynamicClass <Object_t> Background;
DynamicClass <AI_t, Object_t> Array;
DynamicClass <Player_t, Object_t> Player;
DynamicClass <Other_t, Player_t, Object_t> Other;

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

	//Set the objects in their place
	Background[0].OpenImage("Image.png", { NULL }, { NULL });
	Background[0].SetCoords(0, 0, Window1.GetWidth(), Window1.GetHeight());

	for (int i = 0; i < Array.NumberOfObjects(); i++)
		Array[i].SetCoords(i * 50, 240, 32, 32);

	Player[0].SetCoords(300, 500, 32, 32);
	Player[1].SetCoords(100, 100, 32, 32);

	for (int i = 0; i < Other.NumberOfObjects(); i++)
		Other[i].SetCoords(i * 100 + 100, 400, 32, 32);
}

void RunGame()
{
	DynamicClass <Player_t>::All(&Player_t::EventHandler);

	DynamicClass <Object_t>::All(&Object_t::Display);
}

void DespawnGame()
{
	Background.Despawn();
	Array.Despawn();
	Player.Despawn();
	Other.Despawn();
}


//
//  Main program loop will look simple
//

extern "C" int SDL_main(int argc, char* argv[])
{
	srand(unsigned(time(NULL)));

	//Do Window1.Run(init, loop, despawn) on any new screen
	Window1.Run(SpawnGame, RunGame, DespawnGame);

	return 0;
}
