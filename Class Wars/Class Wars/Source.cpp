#include <SLGE.h>


using namespace SLGE;
const int ARRAY_SIZE = 10;

Window_t Window1(640, 480, "Cooler Program", false);


class Player_TT : public Object_t
{
	friend class DynamicClass <Player_TT>;

public:

	Player_TT()
	{
	};

	Player_TT(Window_t *in_Window)
	{
		WindowHandle = in_Window;
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


DynamicClass <Object_t> Background;
DynamicClass <Object_t> Array;
DynamicClass <Player_TT, Object_t> Player;




void SpawnGame()
{
	Background.Spawn(1, Window1);
	Array.Spawn(10, Window1);
	//Player.template Spawn <Player_TT> (1, Window1);
	Player.Spawn(1, Window1);
	//Other.Spawn(1, Window1);

	Background[0].OpenImage("Image.png", { NULL }, { NULL });
	Background[0].SetCoords(0, 0, Window1.GetWidth(), Window1.GetHeight());

	for (int i = 0; i < 10; i++)
	{
		Array[i].OpenImage("1.png", { 0, 0, 32, 32 }, { 255, 0, 153, 0 });
		Array[i].SetCoords(i * 50, 240, 32, 32);
	}

	for (int i = 0; i < 20; i++)
	{
		for (int ii = 0; ii < 8; ii++)
			Player[0].OpenImage("Player.png", { (ii * 100) + 1, (i * 100) + 1, 98, 98 }, { 255, 255, 255, 0 });
	}

	for (int i = 0; i < 20; i++)
	{
		for (int ii = 0; ii < 8; ii++)
			Player[1].OpenImage("Player.png", { (ii * 100) + 1, (i * 100) + 1, 98, 98 }, { 255, 255, 255, 0 });
	}

	Player[0].SetCoords(300, 500, 32, 32);
	Player[1].SetCoords(100, 100, 32, 32);
}

void RunGame()
{
	//std::vector <Object_t> Array1;

	//for (int i = 0; i < Array1.size(); i++)
	//	Array[i].Display();



	//Player[0].EventHandler();
	DynamicClass <Player_TT>::All(&Player_TT::EventHandler);

	DynamicClass <Object_t>::All(&Object_t::Display);
}

void DespawnGame()
{
	Background.Despawn();
	Array.Despawn();
	Player.Despawn();
}


extern "C" int SDL_main(int argc, char* argv[])
{
	srand(unsigned(time(NULL)));

	Window1.Run(SpawnGame, RunGame, DespawnGame);

	return 0;
}
