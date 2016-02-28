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

int DLL_API Window_t::Log(std::ofstream &File, std::string Precursor)
{
	if (!File.is_open())
		File.open("Log Data.txt", std::ios::app);

	if (File.is_open())
	{
		File << Precursor << "==================== Image_t =====================" << std::endl;
		File << Precursor << "this: " << "0x" << this << std::endl;
		File << Precursor << "WindowHandle: " << "0x" << WindowHandle;
		File << Precursor << "HScreen: " << "0x" << HScreen;
		File << Precursor << "Screen: " << "0x" << Screen;
		File << Precursor << "Event: " << "0x" << &Event;
		File << Precursor << "KeyState: " << "0x" << KeyState;

		File << Precursor << "Width: " << Width;
		File << Precursor << "Height: " << Height;
		File << Precursor << "BitsPerPixel: " << BitsPerPixel;
		File << Precursor << "Caption: " << *Caption;
		File << Precursor << "ID: " << ID;

		File << Precursor << "Shown: " << Shown;
		File << Precursor << "MouseFocus: " << MouseFocus;
		File << Precursor << "KeyboardFocus: " << KeyboardFocus;
		File << Precursor << "Minimized: " << Minimized;
		File << Precursor << "HardwareAccelerated: " << HardwareAccelerated;
		File << Precursor << "Running: " << Running;

		File << Precursor << "FRAME_SAMPLE_SIZE: " << TimerHandle.FRAME_SAMPLE_SIZE;
		File << Precursor << "WiCurrentFPSdth: " << TimerHandle.CurrentFPS;

		for (int i = 0; i < TimerHandle.FRAME_SAMPLE_SIZE; i++)
			File << Precursor << "LastFPS[" << i << "]: " << TimerHandle.LastFPS[i];

		File << Precursor << "LastFrame in ns: " << duration_cast <nanoseconds> (TimerHandle.LastFrame->time_since_epoch()).count();
		File << Precursor << "LastFPSDisplay in ns: " << duration_cast <nanoseconds> (TimerHandle.LastFPSDisplay->time_since_epoch()).count();
		File << Precursor << "LastBench in ns: " << duration_cast <nanoseconds> (TimerHandle.LastBench->time_since_epoch()).count();
		File << Precursor << "LastIdentifier: " << *(TimerHandle.LastIdentifier);

		File.close();
	}
	else
		return 1;

	return 0;
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
			Running = false;
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

	for (int i = 0; i < 284; i++)
		Keyboard.Key[i].WasChanged = false;

	for (int i = 0; i < 5; i++)
		Mouse.Button[i].WasChanged = false;

	Mouse.Wheel.WasChanged = false;

	while (SDL_PollEvent(&Event))
	{
		EventHandler();

		if (Event.type == SDL_KEYDOWN)
		{
			if (Event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE && Keyboard.CollectTextInput)
			{
				if (Keyboard.TextInput->length() > 0)
					Keyboard.TextInput->pop_back();
			}

			if (Keyboard.Key[Event.key.keysym.scancode].IsPressed)
				Keyboard.Key[Event.key.keysym.scancode].WasChanged = false;
			else
				Keyboard.Key[Event.key.keysym.scancode].WasChanged = true;

			Keyboard.Key[Event.key.keysym.scancode].IsPressed = true;
			Keyboard.Key[Event.key.keysym.scancode].TimesRepeated = Event.key.repeat;
			Keyboard.Key[Event.key.keysym.scancode].TimeAtLastPress = SDL_GetTicks();
			Keyboard.Key[Event.key.keysym.scancode].MousePosnWhenLastPressed.X = Mouse.GetX();
			Keyboard.Key[Event.key.keysym.scancode].MousePosnWhenLastPressed.Y = Mouse.GetY();
		}

		else if (Event.type == SDL_KEYUP)
		{
			if (Keyboard.Key[Event.key.keysym.scancode].IsPressed)
				Keyboard.Key[Event.key.keysym.scancode].WasChanged = true;
			else
				Keyboard.Key[Event.key.keysym.scancode].WasChanged = false;

			Keyboard.Key[Event.key.keysym.scancode].IsPressed = false;
			Keyboard.Key[Event.key.keysym.scancode].TimesRepeated = Event.key.repeat;
			Keyboard.Key[Event.key.keysym.scancode].TimeAtLastRelease = SDL_GetTicks();
			Keyboard.Key[Event.key.keysym.scancode].MousePosnWhenLastReleased.X = Mouse.GetX();
			Keyboard.Key[Event.key.keysym.scancode].MousePosnWhenLastReleased.Y = Mouse.GetY();
		}

		else if (Event.type == SDL_TEXTINPUT)
		{
			if (Keyboard.CollectTextInput)
				Keyboard.TextInput->append(Event.text.text);
		}

		else if (Event.type == SDL_MOUSEMOTION)
		{
			Mouse.X = Event.motion.x;
			Mouse.Y = Event.motion.y;
		}

		else if (Event.type == SDL_MOUSEBUTTONDOWN)
		{
			if (Mouse.Button[Event.button.button].IsPressed)
				Mouse.Button[Event.button.button].WasChanged = false;
			else
				Mouse.Button[Event.button.button].WasChanged = true;

			Mouse.Button[Event.button.button].IsPressed = true;
			Mouse.Button[Event.button.button].NumberOfClicks = Event.button.clicks;
			Mouse.Button[Event.button.button].TimeAtLastPress = SDL_GetTicks();
			Mouse.Button[Event.button.button].MousePosnWhenLastPressed.X = Mouse.GetX();
			Mouse.Button[Event.button.button].MousePosnWhenLastPressed.Y = Mouse.GetY();
		}

		else if (Event.type == SDL_MOUSEBUTTONUP)
		{
			if (Mouse.Button[Event.button.button].IsPressed)
				Mouse.Button[Event.button.button].WasChanged = true;
			else
				Mouse.Button[Event.button.button].WasChanged = false;

			Mouse.Button[Event.button.button].IsPressed = false;
			Mouse.Button[Event.button.button].NumberOfClicks = Event.button.clicks;
			Mouse.Button[Event.button.button].TimeAtLastRelease = SDL_GetTicks();
			Mouse.Button[Event.button.button].MousePosnWhenLastReleased.X = Mouse.GetX();
			Mouse.Button[Event.button.button].MousePosnWhenLastReleased.Y = Mouse.GetY();
		}

		else if (Event.type == SDL_MOUSEWHEEL)
		{
			if (Event.wheel.y != 0)// || Event.wheel.x != 0)
				Mouse.Wheel.WasChanged = true;

			Mouse.Wheel.AmountScrolled = Event.wheel.y;

			if (Event.wheel.x > 0)
			{
				Mouse.Wheel.TimeAtLastUpScroll = SDL_GetTicks();
				Mouse.Wheel.MousePosnWhenLastScrolledUp.X = Mouse.GetX();
				Mouse.Wheel.MousePosnWhenLastScrolledUp.Y = Mouse.GetY();
			}
			else if (Event.wheel.x < 0)
			{
				Mouse.Wheel.TimeAtLastDownScroll = SDL_GetTicks();
				Mouse.Wheel.MousePosnWhenLastScrolledDown.X = Mouse.GetX();
				Mouse.Wheel.MousePosnWhenLastScrolledDown.Y = Mouse.GetY();
			}
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
	FunctionReturn Ret;
	SDL_Surface *BlackScreen = SDL_CreateRGBSurface(0, 1920, 1080, BitsPerPixel, 0x00000000, 0x00000000, 0x00000000, 0x000000ff);

	if (!Running)
		return Exit;

	Ret = in_Spawn();

	if (Ret != Continue)
	{
		in_Despawn();
		return Ret;
	}

	while (IsRunning())
	{
		if (!HardwareAccelerated)
			SDL_BlitSurface(BlackScreen, { NULL }, Screen, { NULL });

		FetchEvents();
		Ret = in_Loop();
	
		if (Ret != Continue)
		{
			SDL_FreeSurface(BlackScreen);
			in_Despawn();
			return Ret;
		}

		Refresh();
	}

	SDL_FreeSurface(BlackScreen);
	return in_Despawn();
}

void DLL_API Window_t::Close()
{
	SDL_HideWindow(WindowHandle);
	Running = false;
	ClearData();
}

#undef DLL_API