#include <SLGE.h>

using namespace SLGE;
const int ARRAY_SIZE = 10;


Window Window1(640, 480, "Cool Program", false);

//Must initialize each object with a Window
Object Background(&Window1);

//Arrays cannot be initialized with a window in the constructor
NPC Array[ARRAY_SIZE];


class Player : public Entity
{
private:
	bool MoveUp;
	bool MoveDown;
	bool MoveLeft;
	bool MoveRight;

	int ResetLoopVariables()
	{
		LastX = X;
		LastY = Y;

		Moving = false;

		if (MoveUp)
			Move(Up);
		if (MoveDown)
			Move(Down);
		if (MoveLeft)
			Move(Left);
		if (MoveRight)
			Move(Right);

		return 0;
	}

	int HandleEvents(SDL_Event *Event)
	{
		/*
		const Uint8 *Keystate = SDL_GetKeyboardState(NULL);

		if (Keystate[SDL_SCANCODE_W])
		{
			std::cout << "W IS PRESSED" << std::endl;
			Move(Up);
		}
		if (Keystate[SDL_SCANCODE_S])
			Move(Down);
		if (Keystate[SDL_SCANCODE_A])
			Move(Left);
		if (Keystate[SDL_SCANCODE_D])
			Move(Right);

		std::cout << "END OF EVENTS" << std::endl;
		*/

		if (Event->type == SDL_KEYDOWN)
		{
			if (Event->key.keysym.sym == SDLK_w)
				MoveUp = true;
			if (Event->key.keysym.sym == SDLK_s)
				MoveDown = true;
			if (Event->key.keysym.sym == SDLK_a)
				MoveLeft = true;
			if (Event->key.keysym.sym == SDLK_d)
				MoveRight = true;
		}

		if (Event->type == SDL_KEYUP)
		{
			if (Event->key.keysym.sym == SDLK_w)
				MoveUp = false;
			if (Event->key.keysym.sym == SDLK_s)
				MoveDown = false;
			if (Event->key.keysym.sym == SDLK_a)
				MoveLeft = false;
			if (Event->key.keysym.sym == SDLK_d)
				MoveRight = false;
		}

		return 0;
	}

public:
	Player(Window* WindowHandle)
	{
		Register(WindowHandle);
	}

} Player1(&Window1);


void SetUp()
{
	srand(unsigned(time(NULL)));

	//Load the images 
	Background.OpenImage("Image.png", { NULL }, { NULL });

	//Set the Coordinates for X, Y, W, H (Width and Height are not the width and height of the image)
	Background.SetCoords(540, 360, 1080, 720);

	for (int ii = 0; ii < ARRAY_SIZE; ii++)
	{
		//Must register arrays / anything that was not registered when it was declared
		Array[ii].Register(&Window1);

		for (int i = 0; i < 160; i += 32)
			Array[ii].OpenImage("1.png", { i, 0, 32, 32 }, { NULL }); //{ 255, 0, 153, 0 });
		for (int i = 0; i < 160; i += 32)
			Array[ii].OpenImage("1.png", { i, 32, 32, 32 }, { NULL });// { 255, 0, 153, 0 });

		Array[ii].SetTraits(100, 100, 3);
		Array[ii].SetAnimation(10);
		Array[ii].SetCoords((rand() % (Window1.GetWidth() - 32)) + 16, (rand() % (Window1.GetHeight() - 32)) + 16, 32, 16);
	}

	for (int i = 0; i < 160; i += 32)
		Player1.OpenImage("1.png", { i, 0, 32, 32 }, { 255, 0, 153, 0 });
	for (int i = 0; i < 160; i += 32)
		Player1.OpenImage("1.png", { i, 32, 32, 32 }, { 255, 0, 153, 0 });

	Player1.SetCoords(540, 360, 32, 16);
	Player1.SetTraits(200, 200, 5);
	Player1.SetAnimation(10);
}

extern "C" int SDL_main(int argc, char* argv[])
{
	SetUp();						 

	while (Window1.IsRunning())
	{
		//if (Button.IsClicked())
			Array[ARRAY_SIZE - 1].Follow(&Player1, 20);

		//Refresh the window
		Window1.Refresh();
	}

	return 0;
}
