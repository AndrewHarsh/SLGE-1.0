#include <SLGE.h>

using namespace SLGE;

SDL_Event Event;


Window Window1(1080, 720, "Cool Program", true);
Window Window2(640, 480, "Window 2", true);

//Must initialize each object with a Window
Object Background(&Window1);
Entity Person(&Window1);
NPC Animation(&Window1);
UI Button(&Window1);

const int ARRAY_SIZE = 30; //MAX: 4000 (as of right now)

//Arrays cannot be initialized with a window in the constructor
Object Array[ARRAY_SIZE];

Object Background2(&Window2);


extern "C" int SDL_main(int argc, char* argv[])
{
	srand(unsigned(time(NULL)));

	for (int ii = 0; ii < ARRAY_SIZE; ii++)
	{
		//Must register arrays / anything that was not registered when it was declared
		Array[ii].Register(&Window2);
		
		for (int i = 0; i < 160; i += 32)
			Array[ii].OpenImage("1.png", { i, 0, 32, 32 }, { 255, 0, 153, 0 });
		for (int i = 0; i < 160; i += 32)
			Array[ii].OpenImage("1.png", { i, 32, 32, 32 }, { 255, 0, 153, 0 });

		Array[ii].SetCoords((rand() % (Window2.GetWidth() - 32)) + 16, (rand() % (Window2.GetHeight() / 2 - 32)) + 16, 32, 32);
	}

	//Load the images 
	Background.OpenImage("Image.png", { NULL }, { NULL });
	Background2.OpenImage("Image.png", { NULL }, { NULL });

	//Multiple images can be loaded to one object
	Person.OpenImage("2.png", {0, 0, 32, 32}, { 255, 0, 153, 0 });
	Person.OpenImage("2.png", {32, 0, 32, 32}, { 255, 0, 153, 0 });

	//First parameter is filename, second is a Clipping rect, and the last is a RGBA ColorKey
	for (int i = 0; i < 160; i += 32)
		Animation.OpenImage("1.png", { i, 0, 32, 32 }, { 255, 0, 153, 0 });
	for (int i = 0; i < 160; i += 32)
		Animation.OpenImage("1.png", { i, 32, 32, 32 }, { 255, 0, 153, 0 });

	Button.OpenImage("3.png", { 0, 0, 32, 32 }, { 255, 0, 153, 0 });
	Button.OpenImage("2.png", { 32, 0, 32, 32 }, { 255, 0, 153, 0 });
	Button.OpenImage("1.png", { 64, 0, 32, 32 }, { 255, 0, 153, 0 });

	//Set the Coordinates for X, Y, W, H (Width and Height are not the width and height of the image)
	Background.SetCoords(540, 360, 1080, 720);
	Person.SetCoords(540, 360, 32, 32);
	Animation.SetCoords(100, 100, 32, 32);
	Button.SetCoords(300, 500, 32, 32);

	Background2.SetCoords(320, 240, 640, 480);
	

	int Counter = 0;
	int Counter2 = 0;

	while (Window1.IsRunning() || Window2.IsRunning())
	{
		Counter++;

		if (Counter % 10 == 9)
		{
			Counter2++;
			
			//SelectImage tells the object which image out of the array to get
			Animation.SelectImage(Counter2 % Animation.GetNumberOfImages());
			Person.SelectImage(Counter2 % Person.GetNumberOfImages());

			for (int i = 0; i < ARRAY_SIZE; i++)
				Array[i].SelectImage(Counter2 % Array[i].GetNumberOfImages());
		}

		//Window1.TimerHandle.Benchmark("To Animation");

		//Refreshed the window
		Window1.Refresh();
		Window2.Refresh();

		Button.IsClicked();
	}

	//Exit:
	return 0;
}
