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

			//FPS
			Uint32 StartTime; //Point in time that the last frame set this variable 
			double CurrentFPS; //Actual FPS
			HRC::time_point *LastFPSDisplay; //Point in time that the FPS was last displayed

			//Benchmarking
			HRC::time_point *LastBench; //Point in time that the last benchmark set this variable
			std::string *LastIdentifier; //String of the last identifier string

		public:

			//Sets up the timer and clears internal variables
			Timer_t();

			//Returns the FPS of the window it is nested in
			double GetFPS(); 

			//Displays the FPS in the console window
			//	@Delay is the number of milliseconds between displays
			void DisplayFPS(int Delay);
			
			//Caps the FPS
			//	@DisplayFPS is the constant FPS that the game will try to stay at
			int CapFPS(double DesiredFPS);

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

		Window_t *WindowHandle;	//Handle to the window
		SDL_Surface *Software; //Handle to the software pixels
		SDL_Texture *Hardware; //Handle to the hardware pixels

		SDL_Rect LoadClip; //Clip used to trim the loaded image
		SDL_Rect DisplayClip; //Clip used to set the images's display coordinates and size. This is the ACTUAL X, Y, W, H of the image on the screen 

		double Angle; //Degrees rotated counter clockwise from loaded position
		SDL_Point Center; //Center of the rotation
		SDL_RendererFlip FlipType; //Determines whether to flip horizontally, verticlaly, or not at all
					

		//Clears all data
		void ClearData();

	public:

		//Clears all internal variables
		Image_t();
		
		//Clears all internal variables and registers with a window
		//	@Window is the window to register to
		Image_t(Window_t *Window);

		//Cleans up the dynamically allocated images
		~Image_t();


		//Returns the X coordinate
		int X();

		//Returns the Y coordinate
		int Y();

		//Returns the image width
		int W();

		//Returns the image height
		int H();

		//Returns the rotation of the image
		double GetAngle();

		//Returns the center of rotation of the image
		SDL_Point GetCenter();

		//Returns what type of flip has been applied to the image
		SDL_RendererFlip GetFlipType();							  


		//Registers an image with a window
		//	MUST BE DONE in order to display any image
		int Register(Window_t *Window);


		//Opens an image from a file 
		//	@Filename is the file to load
		//	@Clip is rectangle specifying how much of the image to load
		//		NULL loads the entire image
		//	@ColorKey is an RGB value that determines what color will become transparent
		//		NULL specifies no colorkey
		int OpenImage(std::string Filename, SDL_Rect Clip, SDL_Color ColorKey);

		//Creates a text image
		//	@Message is the text to load
		//	@Font is a handle to the desired font
		//	@TextColor is an RGB value that determines the text color
		int LoadText(std::string Message, TTF_Font *Font, SDL_Color TextColor);


		//Sets the coordinates of the image
		//	@X is the X coordinate
		// 	@Y is the Y coordinate
		//	@W is the Width
		//	@H is the Height
		int SetCoords(double X, double Y, double W, double H);

		//Sets some properties of the Image
		// @Angle sets the rotation of the image
		// @Center sets the center of rotation of the image
		// @FlipType sets the type of flipping to apply to the image (horizontal, vertical, none)
		int SetImageProp(double Angle, SDL_Point Center, SDL_RendererFlip FlipType);


		//Displays the image
		int Display();
	};



	//Object_t 
	//	The base class for any object that will be
	//	displayed on the screen	
	class DLL_API Object_t
	{
	protected:

		Window_t *WindowHandle;

	//Image
		std::vector <Image_t> *Image; //All images that could be displayed by a single object
		std::vector <int> *ImageToDisplay; //An array of indexes for pointing to which image is to be displayed and in what order
			
	//Location
		double X; //X coordinate of the object but NOT the image
		double Y; //Y coordinate of the object but NOT the image
		double W; //Width of the object	but NOT the image
		double H; //Height of the object but NOT the image

	//Replacable functions
		virtual void ClearData(); //Clears all internal variables
		virtual void SetDisplay(int ImageIndex); //Sets the Image X, Y, W, and H based on the object's X, Y, W, and H

	public:

	//Init
		//Clears the object
		Object_t();

		//Clears the object and registers it with a window
		//	@WindowHandle is the window to register to
		Object_t(Window_t *WindowHandle);

		//Cleans up the dynamically allocated variables
		~Object_t();

		//Manual register function for static arrays
		int Register(Window_t *Window);


	//Status
		//Returns the total number of images inside the object 
		int GetNumberOfImages();

		//Returns the index of a layered image
		//	@Layer is the layer that the image will be displayed at
		int GetLayeredImage(int Layer);

		//Returns the object's X coordinate
		double GetX();
		
		//Returns the object's Y coordinate
		double GetY();
		
		//Returns the object's width
		double GetW();
		
		//Returns the object's height
		double GetH();


		//True if the object overlaps a rectangle
		//	@Area is the rectangle to check overlapping with
		bool IsOverlapping(SDL_Rect Area);

		//True if the object overlaps an object
		//	@Object is the Object to check overlapping with
		bool IsOverlapping(Object_t* Object);	

		//True if the object is completely inside a rectangle
		//	@Area is the rectangle to check being inside of
		bool IsWithin(SDL_Rect Area);

		//True if the object is completely inside an object
		//	@Object is the object to check being inside of
		bool IsWithin(Object_t* Object);


	//Actions
		//Sets the X, Y, Width and Height
		//	@X sets the X coordinate
		//	@Y sets the Y coordinate
		//	@W sets the width
		//		If NULL the width remains unchanged
		//	@H sets the height
		//		If NULL the height remains unchanged
		void SetCoords(double X, double Y, double W = NULL, double H = NULL);


		//Opens an image from a file 
		//	@Filename is the file to load
		//	@Clip is rectangle specifying how much of the image to load
		//		NULL loads the entire image
		//	@ColorKey is an RGB value that determines what color will become transparent
		//		NULL specifies no colorkey
		int AddImage(std::string Filename, SDL_Rect Clip, SDL_Color ColorKey);

		//Creates a text image
		//	@Message is the text to load
		//	@Font is a handle to the desired font
		//	@TextColor is an RGB value that determines the text color
		int AddText(std::string Message, TTF_Font *Font, SDL_Color TextColor);

		//Moves an image from one position in the array to another position
		//	@Position is the position of the image to move
		//	@NewPosition is the position to move the image to
		int MoveImage(int Position, const int NewPosition);

		//Deletes an image from the image array
		//	@Position is the location of the image to delete
		int DeleteImage(int Position);


		//Opens an image from a file to layer on top of the current images
		//	@Filename is the file to load
		//	@Clip is rectangle specifying how much of the image to load
		//		NULL loads the entire image
		//	@ColorKey is an RGB value that determines what color will become transparent
		//		NULL specifies no colorkey
		int AddLayer(std::string Filename, SDL_Rect Clip, SDL_Color ColorKey);

		//Creates a text image to layer on top of the current images
		//	@Message is the text to load
		//	@Font is a handle to the desired font
		//	@TextColor is an RGB value that determines the text color
		int AddLayer(int ImagePosition);

		//Moves a layered image from one layer to another
		//	@Position is the layer of the image to move
		//	@NewPosition is the layer to move the image to
		int MoveLayer(int Position, const int NewPosition);

		//Deletes an layer from the layers displayed
		//	@Position is the location of the layer to delete
		int DeleteLayer(int Position);


		//Displays all layered images on the screen
		int Display();
	};
}

#undef DLL_API
#endif
													