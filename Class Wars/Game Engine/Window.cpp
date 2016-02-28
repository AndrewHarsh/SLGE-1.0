#include "SLGE.h"

#define DLL_API __declspec(dllexport) 
using namespace SLGE;

DLL_API Window::Window()
{
	WindowHandle = nullptr;
	Screen = nullptr;

	ScreenObjects = nullptr;

	Width = 1080;
	Height = 720;

	char TempCaption[] = "SDL Program";
	Caption = new char[sizeof(TempCaption)];

	for (int i = 0; i < sizeof(TempCaption); i++)
		Caption[i] = TempCaption[i];

	NumberOfObjects = 0;
	Running = false;

	Init();
}

DLL_API Window::Window(const int in_Width, const int in_Height)
{
	Caption = nullptr;

	Init(in_Width, in_Height);
}

DLL_API Window::~Window()
{
	SDL_Quit();
}

void DLL_API Window::ClearData()
{
	if (WindowHandle != nullptr)
	{
		SDL_DestroyWindow(WindowHandle);
		WindowHandle = nullptr;
	}

	if (Screen != nullptr)
	{
		SDL_FreeSurface(Screen);
		Screen = nullptr;
	}

	if (ScreenObjects != nullptr)
	{
		delete[] ScreenObjects;
		ScreenObjects = nullptr;
	}

	Width = 1080;
	Height = 720;

	NumberOfObjects = 0;
	Running = false;
}


int DLL_API Window::GetWidth()
{
	return Width;
}
	
int DLL_API Window::GetHeight()
{
	return Height;
}

int DLL_API Window::GetBPP()
{
	return BitsPerPixel;
}

bool DLL_API Window::IsRunning()
{
	return Running;
}


int DLL_API Window::Init()
{
	if (WindowHandle != nullptr)
	{
		SDL_DestroyWindow(WindowHandle);
		WindowHandle = nullptr;
	}

	if (Screen != nullptr)
	{
		SDL_FreeSurface(Screen);
		Screen = nullptr;
	}

	//Initialization flag
	bool Success = true;

	if (SDL_WasInit(NULL))
		SDL_Quit();

	//Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		Success = false;
	}

	else
	{

#ifdef SDL_MIXER
		if (TTF_Init() < 0)
		{
			printf("SDL_Mixer could not initialize! SDL_Error: %s\n", SDL_GetError());
			Success = false;
		}

		else
		{
			if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
			{
				printf("Audio could not initialize! SDL_Error: %s\n", SDL_GetError());
				Success = false;
			}

			else
				Mix_AllocateChannels(2);
		}
#endif

		//create window
		WindowHandle = SDL_CreateWindow(Caption, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Width, Height, NULL);//SDL_WINDOW_FULLSCREEN);
		if (WindowHandle == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			Success = false;
		}

		else
		{
#ifdef SDL_IMAGE
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) & imgFlags))
			{
				printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
				Success = false;
			}
#endif
			//Get window surface
			Screen = SDL_GetWindowSurface(WindowHandle);
			Running = true;
		}
	}

	return Success;	
}

int DLL_API Window::Init(const int in_Width, const int in_Height)
{
	Width = in_Width;
	Height = in_Height;

	return Init();
}


int DLL_API Window::AddToScreen(Object *in_Object)
{
	in_Object->WindowHandle = this;

	Object **TempArray = new Object*[NumberOfObjects + 1];

	for (int i = 0; i < NumberOfObjects; i++)
		TempArray[i] = ScreenObjects[i];

	TempArray[NumberOfObjects++] = in_Object;

	if (ScreenObjects != nullptr)
		delete[] ScreenObjects;

	ScreenObjects = TempArray;

	return 0;
}

int DLL_API Window::ChangeScreenPosition(Object *in_Object, const int in_NewPosition)
{
	return 0;
}

int DLL_API Window::ChangeScreenPosition(const int in_OldPosition, const int in_NewPosition)
{
	return 0;
}

int DLL_API Window::RemoveFromScreen(Object *in_Object)
{
	Object **TempArray = new Object*[NumberOfObjects - 1];
	int Offset = 0;

	for (int i = 0; i < NumberOfObjects - 1; i++)
	{
		if (ScreenObjects[i] == in_Object)
			Offset++;

		TempArray[i] = ScreenObjects[i + Offset];
	}

	if (Offset != 0)
	{
		if (ScreenObjects != nullptr)
			delete[] ScreenObjects;

		ScreenObjects = TempArray;
		NumberOfObjects--;
	}
	else
	{
		delete[] TempArray;
	}

	return 0;
}

int DLL_API Window::RemoveFromScreen(const int in_Position) 
{
	if (in_Position < 0 || in_Position >= NumberOfObjects)
		return 1;

	Object **TempArray = new Object*[NumberOfObjects - 1];

	for (int i = 0; i < NumberOfObjects - 1; i++)
	{
		if (i < in_Position)
			TempArray[i] = ScreenObjects[i];
		else
			TempArray[i] = ScreenObjects[i + 1];
	}

	if (ScreenObjects != nullptr)
		delete[] ScreenObjects;

	ScreenObjects = TempArray;
	NumberOfObjects--;

	return 0;
}


int DLL_API Window::Refresh()
{
	while (SDL_PollEvent(&Event))
	{
		for (int i = 0; i < NumberOfObjects; i++)
			ScreenObjects[i]->EventHandler(&Event);

		if (Event.type == SDL_QUIT)
			Running = false;
	}

	TimerHandle.Benchmark("To ObjectEvent");

	for (int i = 0; i < NumberOfObjects; i++)
	{
		std::string ID = "Object";
		ID.append(std::to_string(i + 1));

		ScreenObjects[i]->PerFrameActions();
		ScreenObjects[i]->CollisionDetection();
		ScreenObjects[i]->Animate();
		ScreenObjects[i]->Display();
		TimerHandle.Benchmark(ID.c_str());
	}

	TimerHandle.Benchmark("To ObjectDisp");

	TimerHandle.DisplayFPS();
	TimerHandle.CapFPS();

	return SDL_UpdateWindowSurface(WindowHandle);
}

#undef DLL_API