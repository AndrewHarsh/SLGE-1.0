#ifndef _SLGE_H_
#define _SLGE_H_

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <Windows.h>
#include <SDL.h>

#pragma comment(lib, "SDL2")
#pragma comment(lib, "SDL2main")
#pragma comment(lib, "SDL2_image")
#pragma comment(lib, "SDL2_mixer")
#pragma comment(lib, "SDL2_ttf")


#define SDL_IMAGE
#define SDL_TTF
#define SDL_MIXER

#define Colour Color
#define colour color


#ifdef SLGE_EXPORT
	#define DLL_API __declspec(dllexport) 
#else
	#define DLL_API	__declspec(dllimport)
#endif


#ifdef SDL_IMAGE
#include <SDL_image.h>
#endif

#ifdef SDL_TTF
#include <SDL_ttf.h>
#endif

#ifdef SDL_MIXER
#include <SDL_mixer.h>
#endif


namespace SLGE
{
	const char Version[] = "1.0.0";

	typedef std::chrono::high_resolution_clock HRC;
	using std::chrono::milliseconds;
	using std::chrono::microseconds;
	using std::chrono::duration_cast;

	enum Direction;
	struct Color;
	class Window;
	class Object;
	class Entity;
	class NPC;
	class UI;

	enum DLL_API Direction
	{
		Up,
		UpRight,
		Right,
		DownRight,
		Down,
		DownLeft,
		Left,
		UpLeft
	};

	struct DLL_API Color
	{
		unsigned int R;
		unsigned int G;
		unsigned int B;
		unsigned int A;
	};

	//Base Classes
	class DLL_API Window
	{
	friend class Object;
	protected:

		SDL_Window *WindowHandle;
		SDL_Surface *Screen;
		SDL_Event Event;

		int Width;
		int Height;
		const int BitsPerPixel = 32;
		char* Caption;

		Object **ScreenObjects;
		int NumberOfObjects;

		bool Running;

		void ClearData();

	public:

		class DLL_API Timer
		{
		friend class Window;
		protected:

			HRC::time_point *LastFrame;
			HRC::time_point *LastBench;
			HRC::time_point *LastFPSDisplay;
			HRC::time_point *LastBenchDisplay;
			char *LastIdentifier;

			int FPS;
			double CurrentFPS;

			void DisplayFPS();
			int CapFPS();

		public:

			Timer();

			int Benchmark(const char Identifier[]);

		} TimerHandle;

		Window();
		Window(const int Width, const int Height);
		template <int S>
		Window(const int Width, const int Height, const char(&Caption)[S])
		{
			Init(Width, Height, Caption);
		}
		~Window();

		int GetWidth();
		int GetHeight();
		int GetBPP();
		bool IsRunning();

		int Init();
		int Init(const int Width, const int Height);
		template <int S>
		int Init(const int in_Width, const int in_Height, const char(&in_Caption)[S])
		{
			Width = in_Width;
			Height = in_Height;
	
			if (Caption != nullptr)
			{
				delete[] Caption;
				Caption = nullptr;
			}

			Caption = new char[S];

			for (int i = 0; i < S; i++)
				Caption[i] = in_Caption[i];

			Caption[S - 1] = '\0';

			return Init();
		}

		int AddToScreen(Object *ScreenObject);
		int ChangeScreenPosition(Object *ScreenObject, const int NewPosition);
		int ChangeScreenPosition(const int OldPosition, const int NewPosition);
		int RemoveFromScreen(Object *ScreenObject);
		int RemoveFromScreen(const int Position);

		int Refresh();
	};

	class DLL_API Object
	{
	friend class Window;
	protected:

		Window *WindowHandle;
		SDL_Surface **Image;
		SDL_Rect *Clip;
		int NumberOfImages;
		int ImageToDisplay;

		double X;
		double Y;
		double W;
		double H;

		virtual void ClearData();
		virtual void PerFrameActions();
		virtual void EventHandler(SDL_Event* Event);
		virtual void CollisionDetection();
		virtual void Animate();
		virtual void Display();

	public:

		Object();
		Object(Window *WindowHandle);

		int GetNumberOfImages();
		int GetCurrentImage();
		double GetX();
		double GetY();
		double GetW();
		double GetH();

		int Register(Window *Window);
		void Deregister();
		void SetCoords(const double X, const double Y, const double W, const double H);

		int OpenImage(const std::string Filename, SDL_Rect Clip, const Color ColorKey);
		int SelectImage(const int Position);
		int MoveImage(const int Position, const int NewPosition);
		int DeleteImage(const int Position);
	};

	//Built-in Classes
	class DLL_API Entity : public Object
	{
	protected:

		double Speed;
		double Health;
		double AttackDamage;

		void ClearData();

	public:

		Entity();
		Entity(Window *WindowHandle);

		double GetSpeed();
		double GetHealth();
		double GetAttackDamage();

		int SetTraits(const double Speed, const double Health, const double AttackDamage);

		int Move(const Direction);
		int MoveTo(const double X, const double Y);

		int TakeDamage(const double AttackDamage);
		int DealDamage(Entity *Victim, const double AttackDamage);
	};

	class DLL_API NPC : public Entity
	{
	protected:

		void ClearData();

	public:

		NPC();
		NPC(Window *WindowHandle);

		int Wander(const SDL_Rect WanderArea, const double RestTime);
		int Follow(const Object *Leader, const double Distance);
	};

	class DLL_API UI : public Object
	{
	protected:

		std::string *Text;
		std::string *InputText;
		SDL_Surface *TextImage;
		TTF_Font *Font;
		SDL_Color FontColor;

		double TextX;
		double TextY;
		int MouseX;
		int MouseY;

		bool LeftMouseUp;
		bool RightMouseUp;

		void ClearData();
		void EventHandler(SDL_Event *Event);

	public:

		UI();
		UI(Window *WindowHandle);

		bool HoveringOver();
		bool IsClicked();
	};
}

#undef DLL_API
#endif
