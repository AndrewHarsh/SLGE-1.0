#include "SLGE.h"

#define DLL_API __declspec(dllexport) 
using namespace SLGE;


DLL_API Window::Window()
{
	WindowHandle = nullptr;
	HScreen = nullptr;
	Screen = nullptr;
	ScreenObjects = nullptr;
	Caption = new std::string;

	ClearData();
}

DLL_API Window::Window(const int in_Width, const int in_Height) : Window()
{
	Init(in_Width, in_Height);
}

DLL_API Window::Window(const int in_Width, const int in_Height, const std::string in_Caption) : Window()
{
	Init(in_Width, in_Height, in_Caption, false);
}

DLL_API Window::Window(const int in_Width, const int in_Height, const std::string in_Caption, const bool in_Flags) : Window()
{
	Init(in_Width, in_Height, in_Caption, in_Flags);
}

void DLL_API Window::ClearData()
{
	if (WindowHandle != nullptr)
	{
		SDL_DestroyWindow(WindowHandle);
		WindowHandle = nullptr;
	}

	if (HScreen != nullptr)
	{
		SDL_DestroyRenderer(HScreen);
		HScreen = nullptr;
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

	*Caption = "SDL Program";

	Width = 1080;
	Height = 720;
	WindowID = 0;

	NumberOfObjects = 0;
	Shown = true;
	MouseFocus = true;
	KeyboardFocus = false;
	Minimized = false;
	Running = false;
	HardwareAccelerated = false;
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

	//Create window
	WindowHandle = SDL_CreateWindow(Caption->c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Width, Height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);//SDL_WINDOW_FULLSCREEN);
		
	if (WindowHandle == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	WindowID = SDL_GetWindowID(WindowHandle);

	if (HardwareAccelerated)
	{
		//Create renderer for window
		HScreen = SDL_CreateRenderer(WindowHandle, -1, SDL_RENDERER_ACCELERATED);
		if (HScreen == NULL)
		{
			printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
			return EXIT_FAILURE;
		}

		//Initialize renderer color
		SDL_SetRenderDrawColor(HScreen, 0xFF, 0xFF, 0xFF, 0xFF);
	}

	else
		Screen = SDL_GetWindowSurface(WindowHandle);

	Running = true;
	return EXIT_SUCCESS;	
}

int DLL_API Window::Init(const int in_Width, const int in_Height)
{
	Width = in_Width;
	Height = in_Height;

	return Init();
}

int DLL_API Window::Init(const int in_Width, const int in_Height, const std::string in_Caption, const bool in_Flags)
{
	Width = in_Width;
	Height = in_Height;
	*Caption = in_Caption;
	HardwareAccelerated = in_Flags;

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
	if (HardwareAccelerated)
	{
		SDL_Rect Test1 = { 0, 0, Width, Height / 2 };
		//SDL_RenderSetViewport(HScreen, &Test1);
	}

	while (SDL_PollEvent(&Event))
	{
		for (int i = 0; i < NumberOfObjects; i++)
			ScreenObjects[i]->EventHandler(&Event);

		    //If an event was detected for this window
		if (Event.type == SDL_WINDOWEVENT && Event.window.windowID == WindowID)
		{
			switch (Event.window.event)
			{
				//Window appeared
				case SDL_WINDOWEVENT_SHOWN:
				Shown = true;
				break;

				//Window disappeared
				case SDL_WINDOWEVENT_HIDDEN:
				Shown = false;
				break;

				//Get new dimensions and repaint
				case SDL_WINDOWEVENT_SIZE_CHANGED:
				Width = Event.window.data1;
				Height = Event.window.data2;
				if (HardwareAccelerated)
					SDL_RenderPresent(HScreen);
				else
					SDL_UpdateWindowSurface(WindowHandle);
				break;

				//Repaint on expose
				case SDL_WINDOWEVENT_EXPOSED:
				if (HardwareAccelerated)
					SDL_RenderPresent(HScreen);
				else
					SDL_UpdateWindowSurface(WindowHandle);
				break;

				//Mouse enter
				case SDL_WINDOWEVENT_ENTER:
				MouseFocus = true;
				break;

				//Mouse exit
				case SDL_WINDOWEVENT_LEAVE:
				MouseFocus = false;
				break;

				//Keyboard focus gained
				case SDL_WINDOWEVENT_FOCUS_GAINED:
				KeyboardFocus = true;
				break;

				//Keyboard focus lost
				case SDL_WINDOWEVENT_FOCUS_LOST:
				KeyboardFocus = false;
				break;

				//Window minimized
				case SDL_WINDOWEVENT_MINIMIZED:
				Minimized = true;
				break;

				//Window maxized
				case SDL_WINDOWEVENT_MAXIMIZED:
				Minimized = false;
				break;

				//Window restored
				case SDL_WINDOWEVENT_RESTORED:
				Minimized = false;
				break;

				//Hide on close
				case SDL_WINDOWEVENT_CLOSE:
				SDL_HideWindow(WindowHandle);
				ClearData();
				break;
			}
		}
    }

	//TimerHandle.Benchmark("To ObjectEvent");

	for (int i = 0; i < NumberOfObjects; i++)
	{
		//std::string ID = "Object";
		//ID.append(std::to_string(i + 1));

		ScreenObjects[i]->PerFrameActions();
		ScreenObjects[i]->CollisionDetection();
		ScreenObjects[i]->Animate();
		ScreenObjects[i]->Display();
		//TimerHandle.Benchmark(ID.c_str());
	}

	//TimerHandle.Benchmark("To ObjectDisp");

	TimerHandle.DisplayFPS();
	TimerHandle.CapFPS();

	if (HardwareAccelerated)
	{
		//SDL_Rect Test = { 0, Height / 2, Width, Height / 2 };
		//SDL_RenderSetViewport(HScreen, &Test);

		//for (int i = 0; i < NumberOfObjects; i++)
		//	ScreenObjects[i]->Display();

		SDL_RenderPresent(HScreen);
		SDL_RenderClear(HScreen);
	}
	else
		return SDL_UpdateWindowSurface(WindowHandle);

	return 0;
}

#undef DLL_API