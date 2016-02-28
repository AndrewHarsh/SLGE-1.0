#include "SLGE.h"

#define DLL_API __declspec(dllexport) 
using namespace SLGE;

DLL_API Window::Window()
{
	WindowHandle = nullptr;
	Screen = nullptr;

	ScreenObjects = nullptr;
	ScreenEntities = nullptr;
	ScreenNPCs = nullptr;
	ScreenUIs = nullptr;

	Width = 1080;
	Height = 720;

	NumberOfObjects = 0;
	NumberOfEntities = 0;
	NumberOfNPCs = 0;
	NumberOfUIs = 0;

	Init();
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

	if (ScreenEntities != nullptr)
	{
		delete[] ScreenEntities;
		ScreenEntities = nullptr;
	}

	if (ScreenNPCs != nullptr)
	{
		delete[] ScreenNPCs;
		ScreenNPCs = nullptr;
	}

	if (ScreenUIs != nullptr)
	{
		delete[] ScreenUIs;
		ScreenUIs = nullptr;
	}

	Width = 1080;
	Height = 720;

	NumberOfObjects = 0;
	NumberOfEntities = 0;
	NumberOfNPCs = 0;
	NumberOfUIs = 0;
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

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
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
		WindowHandle = SDL_CreateWindow("3D Test Program", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Width, Height, NULL);//SDL_WINDOW_FULLSCREEN);
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


int DLL_API Window::Display(const Object *in_Object)
{
	if (in_Object->Image == nullptr)
		return 1;

	SDL_Rect Offset;

	Offset.x = static_cast <int> (in_Object->X >= 0 ? in_Object->X + 0.5 : in_Object->X - 0.5);
	Offset.y = static_cast <int> (in_Object->Y >= 0 ? in_Object->Y + 0.5 : in_Object->Y - 0.5);

	return SDL_BlitSurface(in_Object->Image[in_Object->ImageToDisplay], &in_Object->Clip[in_Object->ImageToDisplay], Screen, &Offset);
}

int DLL_API Window::AddToScreen(Object *in_ScreenObject)
{
	Object **TempArray = new Object*[NumberOfObjects + 1];

	for (int i = 0; i < NumberOfObjects; i++)
		TempArray[i] = ScreenObjects[i];

	TempArray[NumberOfObjects++] = in_ScreenObject;

	if (ScreenObjects != nullptr)
		delete[] ScreenObjects;

	ScreenObjects = TempArray;

	return 0;
}

int DLL_API Window::AddToScreen(Entity *in_ScreenEntitiy)
{
	Entity **TempArray = new Entity*[NumberOfEntities + 1];

	for (int i = 0; i < NumberOfEntities; i++)
		TempArray[i] = ScreenEntities[i];

	TempArray[NumberOfEntities++] = in_ScreenEntitiy;
	
	if (ScreenEntities != nullptr)
		delete[] ScreenEntities;

	ScreenEntities = TempArray;

	return 0;
}

int DLL_API Window::AddToScreen(NPC *in_ScreenNPC)
{
	NPC **TempArray = new NPC*[NumberOfNPCs + 1];

	for (int i = 0; i < NumberOfNPCs; i++)
		TempArray[i] = ScreenNPCs[i];

	TempArray[NumberOfNPCs++] = in_ScreenNPC;
	
	if (ScreenNPCs != nullptr)
		delete[] ScreenNPCs;
	
	ScreenNPCs = TempArray;

	return 0;
}

int DLL_API Window::AddToScreen(UI *in_ScreenUI)
{
	UI **TempArray = new UI*[NumberOfUIs + 1];

	for (int i = 0; i < NumberOfUIs; i++)
		TempArray[i] = ScreenUIs[i];

	TempArray[NumberOfUIs++] = in_ScreenUI;
	
	if (ScreenUIs != nullptr)
		delete[] ScreenUIs;
	
	ScreenUIs = TempArray;

	return 0;
}


int DLL_API Window::Refresh()
{
	for (int i = 0; i < NumberOfObjects; i++)
	{
		Display(ScreenObjects[i]);
	}

	for (int i = 0; i < NumberOfEntities; i++)
	{
		Display(ScreenEntities[i]);
	}

	for (int i = 0; i < NumberOfNPCs; i++)
	{
		Display(ScreenNPCs[i]);
	}

	for (int i = 0; i < NumberOfUIs; i++)
	{
		Display(ScreenUIs[i]);
	}

	return SDL_UpdateWindowSurface(WindowHandle);
}

#undef DLL_API