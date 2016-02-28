#ifndef _SLGE_H_
#define _SLGE_H_

#include "Macro Definitions.h"
#include "Template Classes.h"

#undef OUT
#undef DELETE

namespace SLGE
{
	namespace WHEEL
	{
		enum Wheels
		{
			Main = 0
		};
	}

	namespace BUTTON
	{
		enum Buttons
		{
			LEFT = 1,
			MIDDLE = 2,
			RIGHT = 3,
			X1 = 4,
			X2 = 5
		};
	}

	namespace KEY
	{
		enum Keys
		{
			SDL_SCANCODE_UNKNOWN = 0,

			A = 4,
			B = 5,
			C = 6,
			D = 7,
			E = 8,
			F = 9,
			G = 10,
			H = 11,
			I = 12,
			J = 13,
			K = 14,
			L = 15,
			M = 16,
			N = 17,
			O = 18,
			P = 19,
			Q = 20,
			R = 21,
			S = 22,
			T = 23,
			U = 24,
			V = 25,
			W = 26,
			X = 27,
			Y = 28,
			Z = 29,

			ONE = 30,
			TWO = 31,
			THREE = 32,
			FOUR = 33,
			FIVE = 34,
			SIX = 35,
			SEVEN = 36,
			EIGHT = 37,
			NINE = 38,
			ZERO = 39,

			RETURN = 40,
			ESCAPE = 41,
			BACKSPACE = 42,
			TAB = 43,
			SPACE = 44,

			MINUS = 45,
			EQUALS = 46,
			LEFTBRACKET = 47,
			RIGHTBRACKET = 48,
			BACKSLASH = 49, 
			NONUSHASH = 50, 
			SEMICOLON = 51,
			APOSTROPHE = 52,
			GRAVE = 53, 
			COMMA = 54,
			PERIOD = 55,
			SLASH = 56,

			CAPSLOCK = 57,

			F1 = 58,
			F2 = 59,
			F3 = 60,
			F4 = 61,
			F5 = 62,
			F6 = 63,
			F7 = 64,
			F8 = 65,
			F9 = 66,
			F10 = 67,
			F11 = 68,
			F12 = 69,

			PRINTSCREEN = 70,
			SCROLLLOCK = 71,
			PAUSE = 72,
			INSERT = 73, 
			HOME = 74,
			PAGEUP = 75,
			DELETE = 76,
			END = 77,
			PAGEDOWN = 78,
			RIGHT = 79,
			LEFT = 80,
			DOWN = 81,
			UP = 82,

			NUMLOCKCLEAR = 83, 
			KP_DIVIDE = 84,
			KP_MULTIPLY = 85,
			KP_MINUS = 86,
			KP_PLUS = 87,
			KP_ENTER = 88,
			KP_1 = 89,
			KP_2 = 90,
			KP_3 = 91,
			KP_4 = 92,
			KP_5 = 93,
			KP_6 = 94,
			KP_7 = 95,
			KP_8 = 96,
			KP_9 = 97,
			KP_0 = 98,
			KP_PERIOD = 99,

			NONUSBACKSLASH = 100, 
			APPLICATION = 101, 
			POWER = 102,
			KP_EQUALS = 103,
			F13 = 104,
			F14 = 105,
			F15 = 106,
			F16 = 107,
			F17 = 108,
			F18 = 109,
			F19 = 110,
			F20 = 111,
			F21 = 112,
			F22 = 113,
			F23 = 114,
			F24 = 115,
			EXECUTE = 116,
			HELP = 117,
			MENU = 118,
			SELECT = 119,
			STOP = 120,
			AGAIN = 121,   
			UNDO = 122,
			CUT = 123,
			COPY = 124,
			PASTE = 125,
			FIND = 126,
			MUTE = 127,
			VOLUMEUP = 128,
			VOLUMEDOWN = 129,
			KP_COMMA = 133,
			KP_EQUALSAS400 = 134,

			INTERNATIONAL1 = 135, 
			INTERNATIONAL2 = 136,
			INTERNATIONAL3 = 137, 
			INTERNATIONAL4 = 138,
			INTERNATIONAL5 = 139,
			INTERNATIONAL6 = 140,
			INTERNATIONAL7 = 141,
			INTERNATIONAL8 = 142,
			INTERNATIONAL9 = 143,
			LANG1 = 144, 
			LANG2 = 145, 
			LANG3 = 146, 
			LANG4 = 147, 
			LANG5 = 148, 
			LANG6 = 149, 
			LANG7 = 150, 
			LANG8 = 151, 
			LANG9 = 152, 

			ALTERASE = 153,
			SYSREQ = 154,
			CANCEL = 155,
			CLEAR = 156,
			PRIOR = 157,
			RETURN2 = 158,
			SEPARATOR = 159,
			OUT = 160,
			OPER = 161,
			CLEARAGAIN = 162,
			CRSEL = 163,
			EXSEL = 164,

			KP_00 = 176,
			KP_000 = 177,
			THOUSANDSSEPARATOR = 178,
			DECIMALSEPARATOR = 179,
			CURRENCYUNIT = 180,
			CURRENCYSUBUNIT = 181,
			KP_LEFTPAREN = 182,
			KP_RIGHTPAREN = 183,
			KP_LEFTBRACE = 184,
			KP_RIGHTBRACE = 185,
			KP_TAB = 186,
			KP_BACKSPACE = 187,
			KP_A = 188,
			KP_B = 189,
			KP_C = 190,
			KP_D = 191,
			KP_E = 192,
			KP_F = 193,
			KP_XOR = 194,
			KP_POWER = 195,
			KP_PERCENT = 196,
			KP_LESS = 197,
			KP_GREATER = 198,
			KP_AMPERSAND = 199,
			KP_DBLAMPERSAND = 200,
			KP_VERTICALBAR = 201,
			KP_DBLVERTICALBAR = 202,
			KP_COLON = 203,
			KP_HASH = 204,
			KP_SPACE = 205,
			KP_AT = 206,
			KP_EXCLAM = 207,
			KP_MEMSTORE = 208,
			KP_MEMRECALL = 209,
			KP_MEMCLEAR = 210,
			KP_MEMADD = 211,
			KP_MEMSUBTRACT = 212,
			KP_MEMMULTIPLY = 213,
			KP_MEMDIVIDE = 214,
			KP_PLUSMINUS = 215,
			KP_CLEAR = 216,
			KP_CLEARENTRY = 217,
			KP_BINARY = 218,
			KP_OCTAL = 219,
			KP_DECIMAL = 220,
			KP_HEXADECIMAL = 221,

			LCTRL = 224,
			LSHIFT = 225,
			LALT = 226, 
			LGUI = 227, 
			RCTRL = 228,
			RSHIFT = 229,
			RALT = 230, 
			RGUI = 231, 

			MODE = 257,   

			AUDIONEXT = 258,
			AUDIOPREV = 259,
			AUDIOSTOP = 260,
			AUDIOPLAY = 261,
			AUDIOMUTE = 262,
			MEDIASELECT = 263,
			WWW = 264,
			MAIL = 265,
			CALCULATOR = 266,
			COMPUTER = 267,
			AC_SEARCH = 268,
			AC_HOME = 269,
			AC_BACK = 270,
			AC_FORWARD = 271,
			AC_STOP = 272,
			AC_REFRESH = 273,
			AC_BOOKMARKS = 274,

			BRIGHTNESSDOWN = 275,
			BRIGHTNESSUP = 276,
			DISPLAYSWITCH = 277, 
			KBDILLUMTOGGLE = 278,
			KBDILLUMDOWN = 279,
			KBDILLUMUP = 280,
			EJECT = 281,
			SLEEP = 282,

			APP1 = 283,
			APP2 = 284,
		};
	}

	struct Coord_t
	{
		double X;
		double Y;
	};

	struct Size_t
	{
		double W;
		double H;
	};

	struct Rect_t
	{
		Coord_t Coord;
		Size_t Size;
	};

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
		int ID; //Window identifier

		bool Shown;	//True if window is shown
		bool MouseFocus; //True if mouse events are handled by window
		bool KeyboardFocus;	//True if keyboard events are handled by window
		bool Minimized;	//True if window is minimized
		bool HardwareAccelerated; //True if window is using GPU
		bool Running; //True if window is running


		//Clears all data
		virtual void ClearData();

		//Saves all current data to a file
		virtual int Log(std::ofstream &File, std::string Precursor);

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
			static const int FRAME_SAMPLE_SIZE = 100;
			double CurrentFPS; //Actual FPS
			double LastFPS[FRAME_SAMPLE_SIZE];
			HRC::time_point *LastFrame;
			HRC::time_point *LastFPSDisplay; //Point in time that the FPS was last displayed

			//Benchmarking
			HRC::time_point *LastBench; //Point in time that the last benchmark set this variable
			std::string *LastIdentifier; //String of the last identifier string

		public:

			//Sets up the timer and clears internal variables
			Timer_t();

			//Returns the FPS of the window it is nested in
			double GetFPS(); 

			//Returns the current tick
			Uint32 GetCurrentTick();

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

		//Holds keyboard events
		class DLL_API Keyboard_t
		{
		friend class Window_t;
		protected:

			struct Key_t
			{
				bool IsPressed;
				bool WasChanged;
				int TimesRepeated;

				Uint32 TimeAtLastPress;
				Uint32 TimeAtLastRelease;

				Coord_t MousePosnWhenLastPressed;
				Coord_t MousePosnWhenLastReleased;
			} Key[284];

			bool CollectTextInput;
			std::string* TextInput;

		public:

			Keyboard_t();
			~Keyboard_t();

			void StartCollectInputText(bool CollectInput);
			std::string GetInputText(bool ClearData = true);

			const Key_t operator[](KEY::Keys in_Key) const;

		} Keyboard;

		//Holds mouse events
		class DLL_API Mouse_t
		{
		friend class Window_t;
		protected:

			struct Button_t
			{
				bool IsPressed;
				bool WasChanged;
				int NumberOfClicks;

				Uint32 TimeAtLastPress;
				Uint32 TimeAtLastRelease;

				Coord_t MousePosnWhenLastPressed;
				Coord_t MousePosnWhenLastReleased;
			} Button[5];

			struct Wheel_t
			{
				bool WasChanged;
				int AmountScrolled;

				Uint32 TimeAtLastUpScroll;
				Uint32 TimeAtLastDownScroll;
						
				Coord_t MousePosnWhenLastScrolledUp;
				Coord_t MousePosnWhenLastScrolledDown;
			} Wheel;

			int X;
			int Y;
			int LastX;
			int LastY;

		public:

			int GetX();
			int GetY();
			int GetLastX();
			int GetLastY();
			int GetRelativeX();
			int GetRelativeY();

			bool HasMovedHorizontally();
			bool HasMovedVertically();
			bool HasMoved();
			bool IsWithin(SDL_Rect Area);
			bool IsWithin(Object_t* Object);

			const Button_t operator[](BUTTON::Buttons Button) const;

			const Wheel_t operator[](WHEEL::Wheels Wheel) const;

		} Mouse;
		

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

		//Closes the window
		void Close();
	};



	//Image_t 
	//	The base class for an image object
	class DLL_API Image_t
	{	
		friend class Object_t;

	protected:

		std::string *ID; //A character string with the name / ID of the image

		Window_t *WindowHandle;	//Handle to the window
		SDL_Surface *Software; //Handle to the software pixels
		SDL_Texture *Hardware; //Handle to the hardware pixels

		SDL_Rect LoadClip; //Clip used to trim the loaded image
		SDL_Rect DisplayClip; //Clip used to set the images's display coordinates and size. This is the ACTUAL X, Y, W, H of the image on the screen 

		double Angle; //Degrees rotated counter clockwise from loaded position
		SDL_Point Center; //Center of the rotation
		SDL_RendererFlip FlipType; //Determines whether to flip horizontally, verticlaly, or not at all

		//Clears all data
		virtual void ClearData();

		//Saves all current data to a file
		virtual int Log(std::ofstream &File, std::string Precursor);

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

		//Returns the original image X coordinate
		int ClipX();

		//Returns the original image Y coordinate
		int ClipY();

		//Returns the original image width
		int ClipW();

		//Returns the original image height
		int ClipH();

		//Returns the rotation of the image
		double GetAngle();

		//Returns the center of rotation of the image
		SDL_Point GetCenter();

		//Returns what type of flip has been applied to the image
		SDL_RendererFlip GetFlipType();	

		//Returns the ID of the Image
		const char* GetID() const;


		//Registers an image with a window
		//	MUST BE DONE in order to display any image
		virtual int Register(Window_t *Window);


		//Opens an image from a file 
		//	@Filename is the file to load
		//	@Clip is rectangle specifying how much of the image to load
		//		NULL loads the entire image
		//	@ColorKey is an RGB value that determines what color will become transparent
		//		NULL specifies no colorkey
		int Load(std::string Filename, SDL_Rect Clip, SDL_Color ColorKey);

		//Creates a text image
		//	@Message is the text to load
		//	@Font is a handle to the desired font
		//	@TextColor is an RGB value that determines the text color
		int Load(std::string Message, TTF_Font &Font, SDL_Color TextColor);

		//Assigns a character string ID to the image once
		//	@ID is a character string ID
		//returns 1 if ID is already set
		int SetID(const char ID[]);

		//Sets the clipping rect of the loaded image
		//	@X is the X position of the upper left corner
		// 	@Y is the Y of the upper left corner
		//	@W is the Width
		//	@H is the Height
		int SetClip(int X, int Y, int W, int H);

		//Sets the coordinates of the image
		//	@X is the X coordinate
		// 	@Y is the Y coordinate
		int SetCoords(double X, double Y);
		
		//Sets the size of the image
		//	@W is the Width
		//	@H is the Height
		int SetSize(int W, int H);

		//Sets the transparency
		//	@Percentage is a fractional number from 0 to 1 representing the percentage of opacity of the image
		int SetTransparency(double Percentage);

		//Rotates the image
		// @Angle sets the rotation of the image
		// @Center sets the center of rotation of the image
		int Rotate(double Angle, SDL_Point Center);

		//Flips the image
		// @FlipType sets the type of flipping to apply to the image (horizontal, vertical, none)
		int Flip(SDL_RendererFlip FlipType);


		//Displays the image
		virtual int Display();
	};



	//Object_t 
	//	The base class for any object that will be
	//	displayed on the screen	
	class DLL_API Object_t
	{
	protected:

		std::string *ID;

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
		//Clears all internal variables
		virtual void ClearData(); 

		//Saves all current data to a file
		virtual int Log(std::ofstream &File, std::string Precursor);

		//Sets the Image X, Y, W, and H based on the object's X, Y, W, and H
		//	@ImageIndex is the position of the image in the image array to set
		virtual void SetImage(int ImageIndex); 

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
		//	@Window is the window to register the object with
		virtual int Register(Window_t *Window);

		//Sets the ID for the object
		//	@ID is the character string ID
		int SetID(const char ID[]);

		//Sets the ID for an image
		//	@ID is the character string ID
		//	@Index is the location of the image
		int SetImageID(const char ID[], int Index);


	//Status
		//Returns the object's X coordinate
		double GetX();
		
		//Returns the object's Y coordinate
		double GetY();
		
		//Returns the object's width
		double GetW();
		
		//Returns the object's height
		double GetH();

		//Returns the ID of the object
		const char* GetID() const;


		//Returns the Image at an index
		//	@Index is the index of the image
		Image_t* GetImageAtIndex(int Index);

		//Returns the index of a layered image
		//	@Layer is the layer that the image will be displayed at
		int GetImageAtLayer(int Layer);

		//Returns the index of an image
		//	@Image is a pointer to an image stored in the internal image array
		int GetIndexOfImage(const Image_t* Image);

		//Returns the index of an image
		//	@ID is a character string ID of an image stored in the internal image array
		int GetIndexOfImage(const char ID[]);

		//Returns the total number of images inside the object 
		int GetNumberOfImages();

		//Returns the number of layers
		int GetNumberOfLayers();



		//True if the object overlaps a rectangle
		//	@Area is the rectangle to check overlapping with
		virtual bool IsOverlapping(SDL_Rect Area);

		//True if the object overlaps an object
		//	@Object is the Object to check overlapping with
		virtual bool IsOverlapping(Object_t* Object);	

		//True if the object is completely inside a rectangle
		//	@Area is the rectangle to check being inside of
		virtual bool IsWithin(SDL_Rect Area);

		//True if the object is completely inside an object
		//	@Object is the object to check being inside of
		virtual bool IsWithin(Object_t* Object);


	//Actions
		//Sets the X and Y coordinates
		//	@X sets the X coordinate
		//	@Y sets the Y coordinate
		void SetCoords(double X, double Y);
		
		//Sets the size of the object
		//	@W sets the width
		//		If less than 0 the width remains unchanged
		//	@H sets the height
		//		If less than 0 the height remains unchanged
		void SetSize(double W, double H);


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
		virtual int Display();
	};
}

#undef DLL_API
#endif
													