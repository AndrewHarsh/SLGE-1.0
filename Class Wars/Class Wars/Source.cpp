#include <SLGE.h>

using namespace SLGE;

SDL_Event Event;

Window Window1;

//Must initialize each object with the Window
Object Background;
Entity Person(&Window1);
Entity Animation(&Window1);


int main(int argc, char* argv[])
{
	//Must initialize Window first thing
	Window1.Init(1080, 720);

	//Must manually initialize any object with the window that was not initialized in the constructor
	Background.Init(&Window1);

	//Load the images 
	Background.OpenImage("Image.png", { NULL }, { NULL });

	//Multiple images can be loaded to one object
	Person.OpenImage("1.png", { NULL }, { 255, 0, 153, 0 });
	Person.OpenImage("2.png", { NULL }, { 255, 0, 153, 0 });
	Person.OpenImage("3.png", { NULL }, { 255, 0, 153, 0 });

	//First parameter is filename, second is a Clipping rect, and the last is a RGBA ColorKey
	for (int i = 0; i < 160; i += 32)
		Animation.OpenImage("1.png", { i, 0, 32, 32 }, { 255, 0, 153, 0 });
	for (int i = 0; i < 160; i += 32)
		Animation.OpenImage("1.png", { i, 32, 32, 32 }, { 255, 0, 153, 0 });

	//Set the Coordinates for X, Y, W, H (Width and Height are not the width and height of the image)
	Background.SetCoords(0, 0, 1080, 720);
	Person.SetCoords(540, 360, 200, 200);
	Animation.SetCoords(100, 100, 32, 32);

	//Puts the image on the screen
	Window1.AddToScreen(&Background);
	Window1.AddToScreen(&Person);
	Window1.AddToScreen(&Animation);
	

	int Counter = 0;
	int Counter2 = 0;

	while (true)
	{
		Counter++;

		if (Counter % 10 == 9)
		{
			Counter2++;
			
			//SelectImage tells the object which image out of the array to get
			Animation.SelectImage(Counter2 % Animation.GetNumberOfImages());
			Person.SelectImage(Counter2 % Person.GetNumberOfImages());
		}

		//Refreshed the window
		Window1.Refresh();

		//Event handler
		while (SDL_PollEvent(&Event))
		{ 
			if (Event.type == SDL_QUIT)//If the screen is exited
				goto Exit;

			if (Event.type == SDL_KEYDOWN)
			{
				if (Event.key.keysym.sym == SDLK_a)
					Person.SetCoords(300, 360, 200, 200);

				if (Event.key.keysym.sym == SDLK_d)
				{
					Person.SetCoords(800, 360, 200, 200);
					Window1.RemoveFromScreen(&Animation);
				}
			}
		} 
	}

	Exit:
	return 0;
}
