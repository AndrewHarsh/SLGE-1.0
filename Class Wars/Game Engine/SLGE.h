#ifndef _SLGE_H_
#define _SLGE_H_

#include "Macro Definitions.h"
#include "Template Classes.h"

namespace SLGE
{
	//Window_t
	//	The base class for any window like object	
	class DLL_API Window_t
	{
		typedef FunctionReturn(*Function)();
		friend class Timer_t;
		friend class Object_t;
		friend class Image_t;

	protected:

		SDL_Window *WindowHandle; //Handle to the SDL window
		SDL_Renderer *HScreen; //Handle to the GPU renderer
		SDL_Surface *Screen; //Handle to the CPU renderer
		SDL_Event Event; //Object that holds all of the events
		const Uint8 *KeyState; //Object that holds all of the keyboard states

		int Width; //Window width
		int Height; //Window height
		const int BitsPerPixel = 32;
		std::string *Caption; //Title of window
		int WindowID; //Window identifier

		bool Shown;	//True if window is shown
		bool MouseFocus; //True if mouse events are handled by window
		bool KeyboardFocus;	//True if keyboard events are handled by window
		bool Minimized;	//True if window is minimized
		bool HardwareAccelerated; //True if window is using GPU
		bool Running; //True if window is running


		//Clears all data
		void ClearData();

		//Fills Event with new events
		int FetchEvents();

		//Processes the events in Event
		int EventHandler();

		//Blits the screen
		int Refresh();
		
	public:

		//Used for any time based operations in a window
		class DLL_API Timer_t
		{
		friend class Window_t;
		protected:

			HRC::time_point *LastFrame; 
			HRC::time_point *LastBench;
			HRC::time_point *LastFPSDisplay;
			HRC::time_point *LastBenchDisplay;
			Uint32 StartTime;
			double CPUFreq;
			long double Duration;
			char *LastIdentifier;

			int FPS;
			double CurrentFPS;

			void DisplayFPS();
			int CapFPS();

		public:

			Timer_t();

			//Returns the FPS of the window it is nested in
			double GetFPS(); 

			//Outputs the time passed since the last call to "Benchmark" to the console window 
			//	@Identifier is a string identifier placed before the benchmark time in order to keep track of multiple benchmarks
			int Benchmark(const char Identifier[]);	

		} TimerHandle;
		
		//Sets up the window and clears internal variables
		Window_t();

		//Sets up the window and clears internal variables
		//	@Width is the with of the window
		//	@Height is the height of the window
		Window_t(const int Width, const int Height);

		//Sets up the window and clears internal variables
		//	@Width is the with of the window
		//	@Height is the height of the window
		//	@Caption is the window title
		Window_t(const int Width, const int Height, const std::string Caption);

		//Sets up the window and clears internal variables
		//	@Width is the with of the window
		//	@Height is the height of the window
		//	@Caption is the window title
		//	@Flags determines whether it will be hardware accelerated or not
		Window_t(const int Width, const int Height, const std::string Caption, const bool Flags);


		//==== Status ====//

		//Returns the width of the window
		int GetWidth();

		//Returns the height of the window
		int GetHeight();

		//Returns the Bits Per Pixel of the window
		int GetBPP();

		//Returns the internal event object
		const SDL_Event* GetEvent() const;

		//Returns the state of a specific key
		//	@Key is the scancode of the desired key
		const Uint8 GetKeyState(int Key) const;

		//Returns a bit set identifying which buttons were pressed
		//	To interpret the bitset, check if "GetMouseState & SDL_BUTTON(<button to check>)" is true
		//	@X sets the passed in reference to the X position of the mouse
		//	@Y sets the passed in reference to the Y position of the mouse
		const Uint32 GetMouseState(int *X, int *Y) const;

		//Returns true if window is visible
		bool IsShown();

		//Returns true if mouse events are handled by the window
		bool IsMouseFocused();

		//Returns true if keyboard events are handled by the window
		bool IsKeyboardFocused();

		//Returns true if the window is minimized
		bool IsMinimized();

		//Returns true if the window is running on the GPU
		bool IsHardwareAccelerated();

		//Returns true if the window is running
		bool IsRunning();


		//==== Action ====//
		
		//Initializes the window
		int Init();

		//Initializes the window
		//	@Width is the with of the window
		//	@Height is the height of the window
		int Init(const int Width, const int Height);

		//Initializes the window
		//	@Width is the with of the window
		//	@Height is the height of the window
		//	@Caption is the window title
		//	@Flags determines whether it will be hardware accelerated or not
		int Init(const int Width, const int Height, const std::string Caption, const bool Flags);

		//Runs the window
		//	@Spawn is a pointer to a function that spawns all objects
		//	@Loop is a pointer to a function that executes code each frame
		//	@Despawn is a pointer to a function that cleans up all resources
		int Run(Function Spawn, Function Loop, Function Despawn);
	};



	//Image_t 
	//	The base class for an image object
	class DLL_API Image_t
	{	
		friend class Object_t;

	protected:

		Window_t *WindowHandle;
		SDL_Surface *Software;
		SDL_Texture *Hardware;

		SDL_Rect LoadClip;
		SDL_Rect DisplayClip;

		double Angle;
		SDL_Point Center;
		SDL_RendererFlip FlipType;
									  
		void ClearData();
		int Display();

	public:

		Image_t();
		Image_t(Window_t *in_Window);
		~Image_t();

		int X();
		int Y();
		int W();
		int H();
		double GetAngle();
		SDL_Point GetCenter();
		SDL_RendererFlip GetFlipType();							  

		int Register(Window_t *Window);

		int OpenImage(std::string Filename, SDL_Rect Clip, SDL_Color ColorKey);
		int LoadText(std::string Message, TTF_Font *Font, SDL_Color TextColor);

		int SetCoords(double X, double Y, double W, double H);
		int SetImageProp(double Angle, SDL_Point Center, SDL_RendererFlip FlipType);
	};



	//Object_t 
	//	The base class for any object that will be
	//	displayed on the screen	
	class DLL_API Object_t
	{
	protected:

		Window_t *WindowHandle;

		//Image
		std::vector <Image_t> *Image;
		std::vector <int> *ImageToDisplay;
			
		//Location
		double X;
		double Y;
		double W;
		double H;

		//Replacable functions
		virtual void ClearData();
		virtual void SetDisplay(int ImageIndex);

	public:

		Object_t();
		Object_t(Window_t *WindowHandle);
		~Object_t();
		int Register(Window_t *Window);

		//Status
		int GetNumberOfImages();
		int GetLayeredImage(int Layer);
		double GetX();
		double GetY();
		double GetW();
		double GetH();

		bool IsOverlapping(SDL_Rect Area);
		bool IsOverlapping(Object_t* Object);		   
		bool IsWithin(SDL_Rect Area);
		bool IsWithin(Object_t* Object);

		//Actions
		void SetCoords(double X, double Y, double W = NULL, double H = NULL);

		int AddImage(std::string Filename, SDL_Rect Clip, SDL_Color ColorKey);
		int AddText(std::string Message, TTF_Font *Font, SDL_Color TextColor);
		int MoveImage(int Position, const int NewPosition);
		int DeleteImage(int Position);

		int AddLayer(std::string Filename, SDL_Rect Clip, SDL_Color ColorKey);
		int AddLayer(int ImagePosition);
		int MoveLayer(int Position, const int NewPosition);
		int DeleteLayer(int Position);

		int Display();
	};
}

#undef DLL_API
#endif
													