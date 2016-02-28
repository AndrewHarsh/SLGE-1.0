#ifndef _SLGE_H_
#define _SLGE_H_

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <chrono>
#include <thread>
#include <Windows.h>
#include <vector>
#include <stdexcept>

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
	class Window_t;
	class Object_t;
	class Entity;
	class NPC;
	class UI;
	class Window_t;


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



	template <class C>
	struct Tuple
	{
		std::vector <C*> Data;
		std::vector <std::vector <int>*> Offset;
		std::vector <int> Index;

		Tuple()
		{
			Data.empty();
			Offset.empty();
			Index.empty();
		}
	};

	//Used to consolidate the class lists between derived and base classes
	template <class C>
	class Data_t
	{
	public:
		static Tuple <C> DynamicClass_ClassList;
	};

	template <class C> 
	Tuple <C> Data_t <C>::DynamicClass_ClassList;

	template <typename C, typename ...D>
	class DynamicClass
	{
		typedef bool (C::*bMethod)();
		typedef void (C::*vMethod)();
		typedef int (C::*iMethod)();

		std::vector <int> Index[sizeof...(D) +1];

		template <typename T1>
		void HiddenSpawn(int in_Index)
		{
			const Tuple <T1> *DATA = &(Data_t<T1>::DynamicClass_ClassList);

			//Index.push_back(Data_t <C>::DynamicClass_ClassList.Data.size());
			//Data_t <C>::DynamicClass_ClassList.Offset.push_back(&Index);
			//Data_t <C>::DynamicClass_ClassList.Index.push_back(Index.size() - 1);

			Data_t <T1>::DynamicClass_ClassList.Data.push_back(Data_t<C>::DynamicClass_ClassList.Data[in_Index]);
		}

		template <typename T1, typename T2, typename ...Args>
		void HiddenSpawn(int in_Index)
		{
			const Tuple <T1> *DATA = &(Data_t<T1>::DynamicClass_ClassList);

			//Index.push_back(Data_t <T1>::DynamicClass_ClassList.Data.size());
			//Data_t <T1>::DynamicClass_ClassList.Offset.push_back(&Index);
			//Data_t <T1>::DynamicClass_ClassList.Index.push_back(Index.size() - 1);
			
			Data_t <T1>::DynamicClass_ClassList.Data.push_back(Data_t<C>::DynamicClass_ClassList.Data[in_Index]);

			HiddenSpawn <T2, Args...>(in_Index);
		}

	public:

		DynamicClass()
		{
			for (int i = 0; i < Index[0].size(); i++)
				Index[i].empty();
		}

		/*
		void Spawn(int Amount, Window_t &WindowHandle = NULL)
		{
			for (int i = 0; i < Amount; i++)
			{
				//Align the internal Index with the external, static data
				Index.push_back(Data_t <C>::DynamicClass_ClassList.Data.size());
				
				//Add a reference to the internal Index for later manipulation
				Data_t <C>::DynamicClass_ClassList.Offset.push_back(&Index);
				Data_t <C>::DynamicClass_ClassList.Index.push_back(Index.size() - 1);

				//Add the data
				if (&WindowHandle == NULL)
					Data_t <C>::DynamicClass_ClassList.Data.push_back(new C());
				else
					Data_t <C>::DynamicClass_ClassList.Data.push_back(new C(&WindowHandle));
			}
		}
		*/

		void Spawn(int Amount, Window_t &WindowHandle = NULL)
		{
			for (int i = 0; i < Amount; i++)
			{
				const Tuple <C> *DATA = &(Data_t<C>::DynamicClass_ClassList);

				//Hold the positions of the first pointer
				//The first pointer is redundant since the actual data would be held in the same array
				int Temp1 = Data_t <C>::DynamicClass_ClassList.Data.size();
				int Temp2 = Data_t <C>::DynamicClass_ClassList.Offset.size();
				int Temp3 = Data_t <C>::DynamicClass_ClassList.Index.size();

				//Align the internal Index with the external, static data
				Index[0].push_back(Data_t <C>::DynamicClass_ClassList.Data.size());

				//Add a reference to the internal Index for later manipulation
				Data_t <C>::DynamicClass_ClassList.Offset.push_back(&Index[0]);
				Data_t <C>::DynamicClass_ClassList.Index.push_back(Index[0].size() - 1);

				//Add the data
				if (&WindowHandle == NULL)
					Data_t <C>::DynamicClass_ClassList.Data.push_back(new C());
				else
					Data_t <C>::DynamicClass_ClassList.Data.push_back(new C(&WindowHandle));

				//Loop through the remaining class types
				HiddenSpawn <C, D...>(Temp1);

				//Remove redundant data and Index location information
				Data_t <C>::DynamicClass_ClassList.Data.erase(Data_t <C>::DynamicClass_ClassList.Data.begin() + Temp1);
				Data_t <C>::DynamicClass_ClassList.Offset.erase(Data_t <C>::DynamicClass_ClassList.Offset.begin() + Temp2);
				Data_t <C>::DynamicClass_ClassList.Index.erase(Data_t <C>::DynamicClass_ClassList.Index.begin() + Temp3);
			}
		}

		void Despawn()
		{
			while (Index[0].size() > 0)
				Despawn(0);
		}

		void Despawn(int in_Index, int in_Amount = 1)
		{
			if (in_Index >= 0 && in_Amount > 0 && in_Index + in_Amount <= (int) Index[0].size())
			{
				for (int ii = 0; ii < in_Amount; ii++)
				{
					for (int iii = 0; iii < (sizeof...(D) + 1); iii++)
					{
						//Free data
						delete Data_t <C>::DynamicClass_ClassList.Data[Index[iii][in_Index]];
						Data_t <C>::DynamicClass_ClassList.Data[Index[iii][in_Index]] = nullptr;

						//Shift table data
						for (int i = 0; i < (int) Data_t <C>::DynamicClass_ClassList.Offset.size(); i++)
						{
							if (Data_t <C>::DynamicClass_ClassList.Offset[i]->at(Data_t <C>::DynamicClass_ClassList.Index[i]) > Index[iii][in_Index])
								(Data_t <C>::DynamicClass_ClassList.Offset[i]->at(Data_t <C>::DynamicClass_ClassList.Index[i]))--;
						}


						//Delete table data
						Data_t <C>::DynamicClass_ClassList.Data.erase(Data_t <C>::DynamicClass_ClassList.Data.begin() + Index[iii][in_Index]);
						Data_t <C>::DynamicClass_ClassList.Offset.erase(Data_t <C>::DynamicClass_ClassList.Offset.begin() + Index[iii][in_Index]);
						Data_t <C>::DynamicClass_ClassList.Index.erase(Data_t <C>::DynamicClass_ClassList.Index.begin() + Index[iii][in_Index]);
						Index[iii].erase(Index[iii].begin() + in_Index);

						//Update table Index
						for (int i = 0; i < (int) Index[i].size(); i++)
							Data_t <C>::DynamicClass_ClassList.Index[Index[iii][i]] = i;
					}

					in_Index++;
				}
			}
		}
		
		/*
		void Despawn(int in_Index, int in_Amount = 1)
		{
			if (in_Index >= 0 && in_Amount > 0 && in_Index + in_Amount <= (int) Index[0].size())
			{
				for (int ii = 0; ii < in_Amount; ii++)
				{
					//Free data
					delete Data_t <C>::DynamicClass_ClassList.Data[Index[0][in_Index]];
					Data_t <C>::DynamicClass_ClassList.Data[Index[0][in_Index]] = nullptr;

					//Shift table data
					for (int i = 0; i < (int) Data_t <C>::DynamicClass_ClassList.Offset.size(); i++)
					{
						if (Data_t <C>::DynamicClass_ClassList.Offset[i]->at(Data_t <C>::DynamicClass_ClassList.Index[i]) > Index[0][in_Index])
							(Data_t <C>::DynamicClass_ClassList.Offset[i]->at(Data_t <C>::DynamicClass_ClassList.Index[i]))--;
					}


					//Delete table data
					Data_t <C>::DynamicClass_ClassList.Data.erase(Data_t <C>::DynamicClass_ClassList.Data.begin() + Index[0][in_Index]);
					Data_t <C>::DynamicClass_ClassList.Offset.erase(Data_t <C>::DynamicClass_ClassList.Offset.begin() + Index[0][in_Index]);
					Data_t <C>::DynamicClass_ClassList.Index.erase(Data_t <C>::DynamicClass_ClassList.Index.begin() + Index[0][in_Index]);
					Index[0].erase(Index[0].begin() + in_Index);

					//Update table Index
					for (int i = 0; i < (int) Index[i].size(); i++)
						Data_t <C>::DynamicClass_ClassList.Index[Index[0][i]] = i;

					in_Index++;
				}
			}
		}
		//*/

		static void All(bMethod Run)
		{
			for (int i = 0; i < (int) Data_t <C>::DynamicClass_ClassList.Data.size(); i++)
				(Data_t <C>::DynamicClass_ClassList.Data[i]->*Run)();
		}

		static void All(vMethod Run)
		{
			for (int i = 0; i < (int) Data_t <C>::DynamicClass_ClassList.Data.size(); i++)
				(Data_t <C>::DynamicClass_ClassList.Data[i]->*Run)();
		}

		static void All(iMethod Run)
		{
			const Tuple <C> *DATA = &(Data_t<C>::DynamicClass_ClassList);

			for (int i = 0; i < (int) Data_t <C>::DynamicClass_ClassList.Data.size(); i++)
				(Data_t <C>::DynamicClass_ClassList.Data[i]->*Run)();
		}

		int NumberOfObjects()
		{
			return Index[0].size();
		}

		C &operator[](unsigned in_Index)
		{
			if (in_Index >= 0 && in_Index < Index[0].size())
				return *(Data_t <C>::DynamicClass_ClassList.Data[Index[0][in_Index]]);
		
			return *(Data_t <C>::DynamicClass_ClassList.Data[Index[0][in_Index]]);
		}

		const C &operator[](unsigned in_Index) const
		{
			if (in_Index >= 0 && in_Index < Index[0].size())
				return *(Data_t <C>::DynamicClass_ClassList.Data[Index[0][in_Index]]);
		
			return *(Data_t <C>::DynamicClass_ClassList.Data[Index[0][in_Index]]);
		}
	};

	/*
	template <class C, class D>
	class DynamicClass
	{
		typedef bool (D::*bMethod)();
		typedef void (D::*vMethod)();
		typedef int (D::*iMethod)();

		DynamicClass
		static Data_t <D> Data;
		std::vector <int> Index;

	public:

		DynamicClass()
		{
			Index.empty();
		}

		void Spawn(int Amount)
		{
			for (int i = 0; i < Amount; i++)
			{
				Index.push_back(Data.DynamicClass_ClassList.Data.size());
				Data.DynamicClass_ClassList.Offset.push_back(&Index);
				Data.DynamicClass_ClassList.Index.push_back(Index.size() - 1);
				Data.DynamicClass_ClassList.Data.push_back(new D());
				//DynamicClass_ClassList.Data.push_back(new C());
			}
		}

		void Spawn(int Amount, Window_t &WindowHandle)
		{
			for (int i = 0; i < Amount; i++)
			{
				Index.push_back(Data.DynamicClass_ClassList.Data.size());
				Data.DynamicClass_ClassList.Offset.push_back(&Index);
				Data.DynamicClass_ClassList.Index.push_back(Index.size() - 1);
				Data.DynamicClass_ClassList.Data.push_back(new D(&WindowHandle));
				//DynamicClass_ClassList.Data.push_back(new C(&WindowHandle));
			}
		}

		void Despawn()
		{
			while (Index.size() > 0)
				Despawn(0);
		}

		void Despawn(int in_Index, int in_Amount = 1)
		{
			if (in_Index >= 0 && in_Amount > 0 && in_Index + in_Amount <= (int) Index.size())
			{
				for (int ii = 0; ii < in_Amount; ii++)
				{
					//Free data
					delete Data.DynamicClass_ClassList.Data[Index[in_Index]];
					Data.DynamicClass_ClassList.Data[Index[in_Index]] = nullptr;

					//Shift table data
					for (int i = 0; i < (int) Data.DynamicClass_ClassList.Offset.size(); i++)
					{
						if (Data.DynamicClass_ClassList.Offset[i]->at(Data.DynamicClass_ClassList.Index[i]) > Index[in_Index])
							(Data.DynamicClass_ClassList.Offset[i]->at(Data.DynamicClass_ClassList.Index[i]))--;
					}


					//Delete table data
					Data.DynamicClass_ClassList.Data.erase(Data.DynamicClass_ClassList.Data.begin() + Index[in_Index]);
					Data.DynamicClass_ClassList.Offset.erase(Data.DynamicClass_ClassList.Offset.begin() + Index[in_Index]);
					Data.DynamicClass_ClassList.Index.erase(Data.DynamicClass_ClassList.Index.begin() + Index[in_Index]);
					Index.erase(Index.begin() + in_Index);

					//Update table Index
					for (int i = 0; i < (int) Index.size(); i++)
						Data.DynamicClass_ClassList.Index[Index[i]] = i;

					in_Index++;
				}
			}
		}

		static void All(bMethod Run)
		{
			for (int i = 0; i < (int) Data.DynamicClass_ClassList.Data.size(); i++)
				(Data.DynamicClass_ClassList.Data[i]->*Run)();
		}

		static void All(vMethod Run)
		{
			for (int i = 0; i < (int) Data.DynamicClass_ClassList.Data.size(); i++)
				(Data.DynamicClass_ClassList.Data[i]->*Run)();
		}

		static void All(iMethod Run)
		{
			for (int i = 0; i < (int) Data.DynamicClass_ClassList.Data.size(); i++)
				(Data.DynamicClass_ClassList.Data[i]->*Run)();
		}

		int NumberOfObjects()
		{
			return Index.size();
		}

		//template <class D = C>
		D &operator[](unsigned in_Index)
		{
			if (in_Index >= 0 && in_Index < Index.size())
				return *(static_cast <D*> (Data.DynamicClass_ClassList.Data[Index[in_Index]]));
				//return *(DynamicClass_ClassList.Data[Index[in_Index]]);
		
			return *(static_cast <D*> (Data.DynamicClass_ClassList.Data[Index[0]]));
			//return *(DynamicClass_ClassList.Data[Index[in_Index]]);
		}

		//template <class D = C>
		const D &operator[](unsigned in_Index) const
		{
			if (in_Index >= 0 && in_Index < Index.size())
				return *(static_cast <D*> (Data.DynamicClass_ClassList.Data[Index[in_Index]]));
				//return *(DynamicClass_ClassList.Data[Index[in_Index]]);
		
			return *(static_cast <D*> (Data.DynamicClass_ClassList.Data[Index[0]]));
			//return *(DynamicClass_ClassList.Data[Index[in_Index]]);
		}
	};

	template <class C, class D, class E>
	class DynamicClass
	{
		typedef bool (C::*bMethod)();
		typedef void (C::*vMethod)();
		typedef int (C::*iMethod)();

		static Data_t <C> Data;
		std::vector <int> Index;

	public:

		DynamicClass()
		{
			Index.empty();
		}

		void Spawn(int Amount)
		{
			for (int i = 0; i < Amount; i++)
			{
				Index.push_back(Data.DynamicClass_ClassList.Data.size());
				Data.DynamicClass_ClassList.Offset.push_back(&Index);
				Data.DynamicClass_ClassList.Index.push_back(Index.size() - 1);
				Data.DynamicClass_ClassList.Data.push_back(new D());
				//DynamicClass_ClassList.Data.push_back(new C());
			}
		}

		void Spawn(int Amount, Window_t &WindowHandle)
		{
			for (int i = 0; i < Amount; i++)
			{
				Index.push_back(Data.DynamicClass_ClassList.Data.size());
				Data.DynamicClass_ClassList.Offset.push_back(&Index);
				Data.DynamicClass_ClassList.Index.push_back(Index.size() - 1);
				Data.DynamicClass_ClassList.Data.push_back(new D(&WindowHandle));
				//DynamicClass_ClassList.Data.push_back(new C(&WindowHandle));
			}
		}

		void Despawn()
		{
			while (Index.size() > 0)
				Despawn(0);
		}

		void Despawn(int in_Index, int in_Amount = 1)
		{
			if (in_Index >= 0 && in_Amount > 0 && in_Index + in_Amount <= (int) Index.size())
			{
				for (int ii = 0; ii < in_Amount; ii++)
				{
					//Free data
					delete Data.DynamicClass_ClassList.Data[Index[in_Index]];
					Data.DynamicClass_ClassList.Data[Index[in_Index]] = nullptr;

					//Shift table data
					for (int i = 0; i < (int) Data.DynamicClass_ClassList.Offset.size(); i++)
					{
						if (Data.DynamicClass_ClassList.Offset[i]->at(Data.DynamicClass_ClassList.Index[i]) > Index[in_Index])
							(Data.DynamicClass_ClassList.Offset[i]->at(Data.DynamicClass_ClassList.Index[i]))--;
					}


					//Delete table data
					Data.DynamicClass_ClassList.Data.erase(Data.DynamicClass_ClassList.Data.begin() + Index[in_Index]);
					Data.DynamicClass_ClassList.Offset.erase(Data.DynamicClass_ClassList.Offset.begin() + Index[in_Index]);
					Data.DynamicClass_ClassList.Index.erase(Data.DynamicClass_ClassList.Index.begin() + Index[in_Index]);
					Index.erase(Index.begin() + in_Index);

					//Update table Index
					for (int i = 0; i < (int) Index.size(); i++)
						Data.DynamicClass_ClassList.Index[Index[i]] = i;

					in_Index++;
				}
			}
		}

		static void All(bMethod Run)
		{
			for (int i = 0; i < (int) Data.DynamicClass_ClassList.Data.size(); i++)
				(Data.DynamicClass_ClassList.Data[i]->*Run)();
		}

		static void All(vMethod Run)
		{
			for (int i = 0; i < (int) Data.DynamicClass_ClassList.Data.size(); i++)
				(Data.DynamicClass_ClassList.Data[i]->*Run)();
		}

		static void All(iMethod Run)
		{
			for (int i = 0; i < (int) Data.DynamicClass_ClassList.Data.size(); i++)
				(Data.DynamicClass_ClassList.Data[i]->*Run)();
		}

		int NumberOfObjects()
		{
			return Index.size();
		}

		//template <class D = C>
		D &operator[](unsigned in_Index)
		{
			if (in_Index >= 0 && in_Index < Index.size())
				return *(static_cast <D*> (Data.DynamicClass_ClassList.Data[Index[in_Index]]));
				//return *(DynamicClass_ClassList.Data[Index[in_Index]]);
		
			return *(static_cast <D*> (Data.DynamicClass_ClassList.Data[Index[0]]));
			//return *(DynamicClass_ClassList.Data[Index[in_Index]]);
		}

		//template <class D = C>
		const D &operator[](unsigned in_Index) const
		{
			if (in_Index >= 0 && in_Index < Index.size())
				return *(static_cast <D*> (Data.DynamicClass_ClassList.Data[Index[in_Index]]));
				//return *(DynamicClass_ClassList.Data[Index[in_Index]]);
		
			return *(static_cast <D*> (Data.DynamicClass_ClassList.Data[Index[0]]));
			//return *(DynamicClass_ClassList.Data[Index[in_Index]]);
		}
	};
	*/

	/*
	template <class C>
	Data_t <C> DynamicClass <C>::Data;

	template <class C, class D>
	Data_t <C> DynamicClass <C, D>::Data;

	template <class C, class D, class E>
	Data_t <C> DynamicClass <C, D, E>::Data;
	*/




	class DLL_API Window_t
	{
		typedef void(*Function)();
		friend class Timer_t;
		friend class Object_t;
	protected:
		SDL_Window *WindowHandle;
		SDL_Renderer *HScreen;
		SDL_Surface *Screen;
		SDL_Event Event;
		const Uint8 *KeyState;

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
		int FetchEvents();
		int EventHandler();
		int Refresh();
		
	public:

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

			double GetFPS();

			int Benchmark(const char Identifier[]);

		} TimerHandle;
		
		Window_t();
		Window_t(const int Width, const int Height);
		Window_t(const int Width, const int Height, const std::string Caption);
		Window_t(const int Width, const int Height, const std::string Caption, const bool Flags);

		int GetWidth();
		int GetHeight();
		int GetBPP();
		const SDL_Event* GetEvent() const;
		const Uint8 GetKeyState(int Key) const;
		const Uint32 GetMouseState(int &X, int &Y) const;

		bool IsShown();
		bool IsMouseFocused();
		bool IsKeyboardFocused();
		bool IsMinimized();
		bool IsHardwareAccelerated();
		bool IsRunning();

		// Action
		int Init();
		int Init(const int Width, const int Height);
		int Init(const int Width, const int Height, const std::string Caption, const bool Flags);

		int Run(Function Spawn, Function Loop, Function Despawn);
	};

	class DLL_API Object_t
	{
		friend class Window_t;
	protected:

		Window_t *WindowHandle;

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

	public:	

		//Replacable functions
		virtual void ClearData()
		{
			if (Image != nullptr)
			{
				for (int i = 0; i < NumberOfImages; i++)
					SDL_FreeSurface(Image[i]);

				Image = nullptr;
			}

			if (HImage != nullptr)
			{
				for (int i = 0; i < NumberOfImages; i++)
					SDL_DestroyTexture(HImage[i]);

				HImage = nullptr;
			}

			if (Clip != nullptr)
			{
				delete[] Clip;
				Clip = nullptr;
			}

			if (DisplayClip != nullptr)
			{
				delete[] DisplayClip;
				DisplayClip = nullptr;
			}

			NumberOfImages = 0;
			ImageToDisplay = 0;

			X = 0;
			Y = 0;
			W = 0;
			H = 0;
		}

		int Display();

	//public:

		Object_t();
		Object_t(Window_t *WindowHandle);
		int Register(Window_t *Window);
		void Deregister();

		//Status
		int GetNumberOfImages();
		int GetCurrentImage();
		double GetX();
		double GetY();
		double GetW();
		double GetH();

		bool IsOverlapping(SDL_Rect Area);
		bool IsOverlapping(Object_t* Object);		   
		bool IsWithin(SDL_Rect Area);
		bool IsWithin(Object_t* Object);

		//Actions
		void SetCoords(const double X, const double Y, const double W = NULL, const double H = NULL);

		int OpenImage(const std::string Filename, SDL_Rect Clip, const Color ColorKey);
		int SelectImage(const int Position);
		int MoveImage(const int Position, const int NewPosition);
		int DeleteImage(const int Position);
	};
}

#undef DLL_API
#endif
													