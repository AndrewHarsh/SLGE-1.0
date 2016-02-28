#include "SLGE.h"

#define DLL_API __declspec(dllexport) 
using namespace SLGE;


DLL_API Window_t::Window_t()
{
	WindowHandle = nullptr;
	HScreen = nullptr;
	Screen = nullptr;
	Caption = new std::string;

	KeyState = SDL_GetKeyboardState(NULL);

	ClearData();
}

DLL_API Window_t::Window_t(const int in_Width, const int in_Height) : Window_t()
{
	Init(in_Width, in_Height);
}

DLL_API Window_t::Window_t(const int in_Width, const int in_Height, const std::string in_Caption) : Window_t()
{
	Init(in_Width, in_Height, in_Caption, false);
}

DLL_API Window_t::Window_t(const int in_Width, const int in_Height, const std::string in_Caption, const bool in_Flags) : Window_t()
{
	Init(in_Width, in_Height, in_Caption, in_Flags);
}

void DLL_API Window_t::ClearData()
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

	//ZeroMemory(&Event[0], sizeof(Event[0]));
	//ZeroMemory(&Event[1], sizeof(Event[1]));

	*Caption = "SDL Program";

	Width = 1080;
	Height = 720;
	ID = 0;

	Shown = true;
	MouseFocus = true;
	KeyboardFocus = false;
	Minimized = false;
	Running = false;
	HardwareAccelerated = false;
}

int DLL_API Window_t::EventHandler()
{
	if (Event.type == SDL_KEYDOWN)
	{
		if (Event.key.keysym.sym == SDLK_ESCAPE)
		{
			//SDL_HideWindow(WindowHandle);
			//ClearData();
		}
	}

	if (Event.type == SDL_WINDOWEVENT && Event.window.windowID == ID)
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

	return 0;
}

int DLL_API Window_t::FetchEvents()
{
	//char *Event0 = reinterpret_cast <char*> (&Event);
	//char *Event1 = reinterpret_cast <char*> (&Event);

	while (SDL_PollEvent(&Event))
	{
		EventHandler();

		if (Event.type == SDL_TEXTINPUT)
		{
			std::string Text = Event.text.text;;
			bool hi = true;
		}
	};

	//KeyState = SDL_GetKeyboardState(NULL);
	//{
		//for (int i = 0; i < sizeof(Event); i++)
			//Event1[i] |= Event0[i];
	//}

	return 0;
}

int DLL_API Window_t::Refresh()
{
	TimerHandle.CapFPS(0);
	TimerHandle.DisplayFPS(100);

	/*
	if (HardwareAccelerated)
	{
		SDL_Rect Test1 = { 0, 0, Width, Height / 2 };
		SDL_RenderSetViewport(HScreen, &Test1);
	}
	*/

	//Screen title is FPS
	SDL_SetWindowTitle(WindowHandle, std::to_string((int)TimerHandle.GetFPS()).c_str());

	if (HardwareAccelerated)
	{
		/*
		SDL_Rect Test = { 0, Height / 2, Width, Height / 2 };
		SDL_RenderSetViewport(HScreen, &Test);

		for (int i = 0; i < NumberOfObjects; i++)
			ScreenObjects[i]->Display();
		*/

		SDL_RenderPresent(HScreen);
		SDL_RenderClear(HScreen);
	}
	else
		return SDL_UpdateWindowSurface(WindowHandle);

	return 0;
}

											  

int DLL_API Window_t::GetWidth()
{
	return Width;
}
	
int DLL_API Window_t::GetHeight()
{
	return Height;
}

int DLL_API Window_t::GetBPP()
{
	return BitsPerPixel;
}

const SDL_Event* Window_t::GetEvent() const
{
	return &Event;
}

const Uint8 Window_t::GetKeyState(int in_Key) const
{
	return KeyState[in_Key];
}

const Uint32 Window_t::GetMouseState(int *in_X, int *in_Y) const
{
	return SDL_GetMouseState(in_X, in_Y);
}


bool DLL_API Window_t::IsShown()
{
	return Shown;
}

bool DLL_API Window_t::IsMouseFocused()
{
	return MouseFocus;
}

bool DLL_API Window_t::IsKeyboardFocused()
{
	return KeyboardFocus;
}

bool DLL_API Window_t::IsMinimized()
{
	return Minimized;
}

bool DLL_API Window_t::IsHardwareAccelerated()
{
	return HardwareAccelerated;
}

bool DLL_API Window_t::IsRunning()
{
	return Running;
}



int DLL_API Window_t::Init()
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

	ID = SDL_GetWindowID(WindowHandle);

	if (HardwareAccelerated)
	{
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

int DLL_API Window_t::Init(const int in_Width, const int in_Height)
{
	Width = in_Width;
	Height = in_Height;

	return Init();
}

int DLL_API Window_t::Init(const int in_Width, const int in_Height, const std::string in_Caption, const bool in_Flags)
{
	Width = in_Width;
	Height = in_Height;
	*Caption = in_Caption;
	HardwareAccelerated = in_Flags;

	return Init();
}


int DLL_API Window_t::Run(Function in_Spawn, Function in_Loop, Function in_Despawn)
{
	if (!Running)
		return Exit;

	FunctionReturn Ret;
	Ret = in_Spawn();

	if (Ret != Continue)
	{
		in_Despawn();
		return Ret;
	}

	while (IsRunning())
	{
		FetchEvents();
		Ret = in_Loop();
	
		if (Ret != Continue)
		{
			in_Despawn();
			return Ret;
		}

		Refresh();
	}

	return in_Despawn();
}

#undef DLL_API