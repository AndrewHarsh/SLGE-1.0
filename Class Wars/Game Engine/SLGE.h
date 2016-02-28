#ifndef _SLGE_H_
#define _SLGE_H_

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <chrono>
#include <thread>
#include <Windows.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#pragma comment(lib, "SDL2")
#pragma comment(lib, "SDL2main")
#pragma comment(lib, "SDL2_image")
#pragma comment(lib, "SDL2_mixer")
#pragma comment(lib, "SDL2_ttf")


#define Colour Color
#define colour color

#define SDL_CreateWindow SDL_CreateWindowEx
#define SDL_DestroyWindow SDL_DestroyWindowEx

#define IMG_Load IMG_LoadEx
#define SDL_CreateRGBSurface SDL_CreateRGBSurfaceEx
#define SDL_ConvertSurface SDL_ConvertSurfaceEx
#define SDL_CreateTextureFromSurface SDL_CreateTextureFromSurfaceEx
#define SDL_FreeSurface SDL_FreeSurfaceEx
#define SDL_DestroyTexture SDL_DestroyTextureEx


#ifdef SLGE_EXPORT
	#define DLL_API __declspec(dllexport) 
#else
	#define DLL_API	__declspec(dllimport)
#endif


namespace SLGE
{
	const char Version[] = "1.0.0";
	const int HARDWARE_ACCELERATION = 0x01; //0000 0001


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


	SDL_Window DLL_API *SDL_CreateWindowEx(const char* Title, int X, int Y, int W, int H, Uint32 Flags);
	void DLL_API SDL_DestroyWindowEx(SDL_Window* Window);

	SDL_Surface DLL_API *IMG_LoadEx(const char* File);
	SDL_Surface DLL_API *SDL_CreateRGBSurfaceEx(Uint32 Flags, int Width, int Height, int Depth, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);
	SDL_Surface DLL_API *SDL_ConvertSurfaceEx(SDL_Surface*, SDL_PixelFormat*, Uint32);
	SDL_Texture DLL_API *SDL_CreateTextureFromSurfaceEx(SDL_Renderer*, SDL_Surface*);
	void DLL_API SDL_FreeSurfaceEx(SDL_Surface*);
	void DLL_API SDL_DestroyTextureEx(SDL_Texture*);


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
	friend class Timer;
	friend class Object;
	protected:
		SDL_Window *WindowHandle;
		SDL_Renderer *HScreen;
		SDL_Surface *Screen;
		SDL_Event Event;

		Object **ScreenObjects;
		int NumberOfObjects;

		int Width;
		int Height;
		const int BitsPerPixel = 32;
		std::string *Caption;
		int WindowID;

		bool Shown;
		bool MouseFocus;
		bool KeyboardFocus;
		bool Minimized;
		bool HardwareAccelerated;
		bool Running;

		void ClearData();
		int EventHandler();

	public:

		class DLL_API Timer
		{
		friend class Window;
		protected:

			HRC::time_point *LastFrame;
			HRC::time_point *LastBench;
			HRC::time_point *LastFPSDisplay;
			HRC::time_point *LastBenchDisplay;
			Uint32 StartTime;
			double CPUFreq;
			LARGE_INTEGER Start;
			LARGE_INTEGER End;
			long double Duration;
			char *LastIdentifier;

			int FPS;
			double CurrentFPS;

			void DisplayFPS();
			int CapFPS();

		public:

			Timer();

			double GetFPS();

			int Benchmark(const char Identifier[]);

		} TimerHandle;

		Window();
		Window(const int Width, const int Height);
		Window(const int Width, const int Height, const std::string Caption);
		Window(const int Width, const int Height, const std::string Caption, const bool Flags);

		// Status
		int GetOpenWindows();
		int GetWidth();
		int GetHeight();
		int GetBPP();
		Object* GetScreenObject(const int Index);
		int GetNumberOfObjects();

		bool IsShown();
		bool IsMouseFocused();
		bool IsKeyboardFocused();
		bool IsMinimized();
		bool IsHardwareAccelerated();
		bool IsRunning();

		// Action
		int Init();
		int Init(const int Width, const int Height);
		int Init(const int in_Width, const int in_Height, const std::string Caption, const bool Flags);

		int AddToScreen(Object *ScreenObject);
		int ChangeScreenPosition(Object *ScreenObject, const int NewPosition);
		int ChangeScreenPosition(const int OldPosition, const int NewPosition);
		int RemoveFromScreen(Object *ScreenObject);
		int RemoveFromScreen(const int Position);

		int Refresh();
		int RunLoop(void (&Loop)(void));
	};

	class DLL_API Object
	{
	friend class Window;
	typedef void (*PassiveFunction)();
	protected:

		Window *WindowHandle;

		//Image
		SDL_Surface **Image;
		SDL_Texture **HImage;
		SDL_Rect *Clip;
		SDL_Rect *DisplayClip;
		int ImageToDisplay;
		int NumberOfImages;
			
		//Location
		double X;
		double Y;
		double W;
		double H;
		double LastX;
		double LastY;
		double LastW;
		double LastH;

		//Attributes
		bool DoResetLoopVariables;		//Resets variables that are changed every loop
		bool DoHandleEvents;			//Handles events for the object
		bool DoDetectCollisions;		//Object can collide with other objects 
		bool DoAnimate;					//Object is animated
		bool DoSetImage;				//Object's display X and Y are changed from the location X and Y
		bool DoHandleSound;				//Handles sound for the object
		bool DoDisplay;					//Object is displayed
		bool DoDynamicDepth;			//Object's z-layer can be dynamically changed 

		//Replacable functions
		virtual void ClearData();
		virtual int ResetLoopVariables();
		virtual int HandleEvents(SDL_Event* Event);
		virtual int DetectCollisions(Object* Object);
		virtual int Animate();
		virtual int SetImage();
		virtual int HandleSound();

		virtual int PerFrameLoop();

		int Display();

	public:

		Object();
		Object(Window *WindowHandle);

		//Status
		int GetNumberOfImages();
		int GetCurrentImage();
		double GetX();
		double GetY();
		double GetW();
		double GetH();

		bool DoesDisplay();
		bool DoesDetectCollisions();
		bool DoesDynamicDepth();

		bool IsOverlapping(SDL_Rect Area);
		bool IsOverlapping(Object* Object);
		bool IsWithin(SDL_Rect Area);
		bool IsWithin(Object* Object);

		//Actions
		int Register(Window *Window);
		void Deregister();
		void SetCoords(const double X, const double Y, const double W = NULL, const double H = NULL);

		int OpenImage(const std::string Filename, SDL_Rect Clip, const Color ColorKey);
		int SelectImage(const int Position);
		int MoveImage(const int Position, const int NewPosition);
		int DeleteImage(const int Position);
	};


	//Built-in Classes
	class DLL_API Entity : public Object
	{
	protected:

		double LastX;
		double LastY;
		double Speed;
		Direction Facing;

		double CurrentHealth;
		double MaxHealth;

		double AnimateFrame;
		double AnimateSpeed;

		double AttackDamage;
		double AttackDistance;

		//Status
		bool Moving;
		bool Attacking;
		bool BeingHit;

		//Internal Functions
		void ClearData();		
		int ResetLoopVariables();
		int HandleEvents(SDL_Event *Event);
		int DetectCollisions(Object *Object);
		int Animate();

	public:

		Entity();
		Entity(Window *WindowHandle);

		double GetSpeed();
		double GetHealth();
		double GetAttackDamage();
		bool IsMoving();
		bool IsAttacking();
		bool IsBeingHit();
		bool IsOverlapping(Object *Object);
		bool IsWithin(Object *Object);
		bool IsWithinAttackRange(Object *Object);
		bool IsFacing(Object *Object);
		bool IsCollidingWith(Object *Object);

		int SetTraits(const double Speed, const double Health, const double AttackDamage);
		int SetAnimation(const double Speed);

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
		int Follow(Object *Leader, const double Distance);
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
		int EventHandler(SDL_Event *Event);

	public:

		UI();
		UI(Window *WindowHandle);

		bool HoveringOver();
		bool IsClicked();
	};
}

#undef DLL_API
#endif
