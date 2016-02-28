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
#include <type_traits>

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

	struct Color;

	class Window_t;
	//class Image_t;
	class Object_t;


	SDL_Window DLL_API *SDL_CreateWindowEx(const char* Title, int X, int Y, int W, int H, Uint32 Flags);
	void DLL_API SDL_DestroyWindowEx(SDL_Window* Window);

	SDL_Surface DLL_API *IMG_LoadEx(const char* File);
	SDL_Surface DLL_API *SDL_CreateRGBSurfaceEx(Uint32 Flags, int Width, int Height, int Depth, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);
	SDL_Surface DLL_API *SDL_ConvertSurfaceEx(SDL_Surface*, SDL_PixelFormat*, Uint32);
	SDL_Texture DLL_API *SDL_CreateTextureFromSurfaceEx(SDL_Renderer*, SDL_Surface*);
	void DLL_API SDL_FreeSurfaceEx(SDL_Surface*);
	void DLL_API SDL_DestroyTextureEx(SDL_Texture*);


	enum DLL_API FunctionReturn
	{
		Error = -1,
		Continue = 0,
		Exit = 1
	};

	struct DLL_API Color
	{
		unsigned int R;
		unsigned int G;
		unsigned int B;
		unsigned int A;
	};



	///vvvvvvvvvvvvvvv///
	//	     WIP	   //
	///vvvvvvvvvvvvvvv///


	struct Local
	{
		int Data;
		int Index;
	};

	struct Global
	{
		std::vector <Local> *Pointer;
		int Index;
	};

	//Used to consolidate the class lists between derived and base classes
	template <class C>
	struct Data_t
	{
		static std::vector <C*> Data;
		static std::vector <Global> Link;
	};

	template <class C> 
	std::vector <C*> Data_t <C>::Data;

	template <class C> 
	std::vector <Global> Data_t <C>::Link;


	template <typename C, typename ...D>
	class DynamicClass
	{
		static const int NUMBER_OF_TYPE_ARGS = sizeof...(D) + 1;

		//Temp for debug only
		const std::vector <C*> *DATA = &(Data_t <C>::Data);
		const std::vector <Global> *LINK = &(Data_t <C>::Link);


		std::vector <C*> Data;
		std::vector <Local> Link[sizeof...(D) + 1];

		template <typename T1>
		void AddPointer()
		{
			const int TYPE_NUM = 0;
			Local TempL;
			Global TempG;

			// Point to the data
			Data_t <T1>::Data.push_back(Data[Data.size() - 1]);

			// Local
			TempL.Data = Data_t <T1>::Data.size() - 1;
			TempL.Index = 0;

			Link[TYPE_NUM].push_back(TempL);

			// Global
			TempG.Pointer = &Link[TYPE_NUM];
			TempG.Index = Link[TYPE_NUM].size() - 1;

			Data_t <T1>::Link.push_back(TempG);

			// Update Local
			Link[TYPE_NUM][Link[TYPE_NUM].size() - 1].Index = Data_t <T1>::Link.size() - 1;
		}

		template <typename T1, typename T2, typename ...Args>
		void AddPointer()
		{
			const int TYPE_NUM = sizeof...(Args) + 1;
			Local TempL;
			Global TempG;

			// Point to the data
			Data_t <T1>::Data.push_back(Data[Data.size() - 1]);

			// Local
			TempL.Data = Data_t <T1>::Data.size() - 1;
			TempL.Index = 0;

			Link[TYPE_NUM].push_back(TempL);

			// Global
			TempG.Pointer = &Link[TYPE_NUM];
			TempG.Index = Link[TYPE_NUM].size() - 1;

			Data_t <T1>::Link.push_back(TempG);

			// Update Local
			Link[TYPE_NUM][Link[TYPE_NUM].size() - 1].Index = Data_t <T1>::Link.size() - 1;

			AddPointer <T2, Args...>();
		}

		template <typename T1>
		void DeletePointer(int in_Index)
		{
			const int TYPE_NUM = 0;

		 	//std::vector	 Data_t <T1>::Link[Link[TYPE_NUM][i].Index].Index

			//Shift all local indexes behind the deleted data back one
			for (int i = 0; i < (int) Data_t <T1>::Link.size(); i++)
			{
				// (*PointerToVec)[IndexOfVec].data >= IndexOfDataDeleted
				if ((*Data_t <T1>::Link[i].Pointer)[Data_t <T1>::Link[i].Index].Data > Link[TYPE_NUM][in_Index].Data)
					(*Data_t <T1>::Link[i].Pointer)[Data_t <T1>::Link[i].Index].Data--;

				// (*PointerToVec)[IndexOfVec].index >= IndexOfIndexDeleted
				if ((*Data_t <T1>::Link[i].Pointer)[Data_t <T1>::Link[i].Index].Index > Link[TYPE_NUM][in_Index].Index)
					(*Data_t <T1>::Link[i].Pointer)[Data_t <T1>::Link[i].Index].Index--;
			}

			//Shift the global indexes behind the deleted data back one
			for (int i = 0; i < (int) Link[TYPE_NUM].size(); i++)
			{
				// Skip deleted index
				if (i == in_Index)
					continue;

				// IndexToObjectIndex[IndexOfGlobal + 1] > IndexDeleted
				if (Data_t <T1>::Link[Link[TYPE_NUM][i].Index + 1].Index > in_Index)
					Data_t <T1>::Link[Link[TYPE_NUM][i].Index + 1].Index--;
			}

			//Delete Data						  
			Data_t <T1>::Data.erase(Data_t <T1>::Data.begin() + Link[TYPE_NUM][in_Index].Data);
			Data_t <T1>::Link.erase(Data_t <T1>::Link.begin() + Link[TYPE_NUM][in_Index].Index);
			Link[TYPE_NUM].erase(Link[TYPE_NUM].begin());
		}

		template <typename T1, typename T2, typename ...Args>
		void DeletePointer(int in_Index)
		{
			const int TYPE_NUM = sizeof...(Args) + 1;

		 	//std::vector	 Data_t <T1>::Link[Link[TYPE_NUM][i].Index].Index

			//Shift all local indexes behind the deleted data back one
			for (int i = 0; i < (int) Data_t <T1>::Link.size(); i++)
			{
				// (*PointerToVec)[IndexOfVec].data >= IndexOfDataDeleted
				if ((*Data_t <T1>::Link[i].Pointer)[Data_t <T1>::Link[i].Index].Data > Link[TYPE_NUM][in_Index].Data)
					(*Data_t <T1>::Link[i].Pointer)[Data_t <T1>::Link[i].Index].Data--;

				// (*PointerToVec)[IndexOfVec].index >= IndexOfIndexDeleted
				if ((*Data_t <T1>::Link[i].Pointer)[Data_t <T1>::Link[i].Index].Index > Link[TYPE_NUM][in_Index].Index)
					(*Data_t <T1>::Link[i].Pointer)[Data_t <T1>::Link[i].Index].Index--;
			}

			//Shift the global indexes behind the deleted data back one
			for (int i = 0; i < (int) Link[TYPE_NUM].size(); i++)
			{
				// Skip deleted index
				if (i == in_Index)
					continue;

				// IndexToObjectIndex[IndexOfGlobal + 1] > IndexDeleted
				if (Data_t <T1>::Link[Link[TYPE_NUM][i].Index + 1].Index > in_Index)
					Data_t <T1>::Link[Link[TYPE_NUM][i].Index + 1].Index--;
			}

			//Delete Data						  
			Data_t <T1>::Data.erase(Data_t <T1>::Data.begin() + Link[TYPE_NUM][in_Index].Data);
			Data_t <T1>::Link.erase(Data_t <T1>::Link.begin() + Link[TYPE_NUM][in_Index].Index);
			Link[TYPE_NUM].erase(Link[TYPE_NUM].begin());

			DeletePointer <T2, Args...>(in_Index);
		}

		template <typename Type, typename Comparer>
		int GetTypeIndex(int Count)
		{
			if (std::is_same <Type, Comparer>::value)
				return Count;
			else
				return 0;
		}

		template <typename Type, typename Comparer, typename Comparer2, typename ...Args>
		int GetTypeIndex(int Count)
		{
			if (std::is_same <Type, Comparer>::value)
				return Count;
			else
				return GetTypeIndex<Type, Comparer2, Args...>(Count - 1);
		}



	public:

		//Local
		DynamicClass()
		{
		}

		void Spawn(int Amount, Window_t *WindowHandle = nullptr)
		{
			for (int i = 0; i < Amount; i++)
			{
				if (WindowHandle == nullptr)
					Data.push_back(new C());
				else
					Data.push_back(new C(WindowHandle));

				//Loop through the remaining class types
				AddPointer <C, D...>();
			}
		}

		void Despawn()
		{
			Despawn(0, Data.size());
		}

		void Despawn(int in_Index, int in_Amount)
		{
			if (in_Amount > 0 && Data.size() > 0)
			{
				for (int i = in_Index; i < in_Index + in_Amount; i++)
				{
					DeletePointer <C, D...>(in_Index);

					delete Data[in_Index];
					Data.erase(Data.begin() + in_Index);
				}
			}
		}

		template <typename T1 = C>
		int GetPosition(int in_Index = 0)
		{
			if (in_Index >= 0 && in_Index < Link[0].size())
			{
				int Type = GetTypeIndex <T1, C, D...>(NUMBER_OF_TYPE_ARGS - 1);

				return Link[Type][in_Index].Data;
			}
			else
				return -1;
		}

		int NumberOfObjects()
		{
			return Data.size();
		}

		C &operator[](unsigned in_Index)
		{
			if (in_Index >= 0 && in_Index < Data.size())
				return *(Data[in_Index]);
		
			return *(Data[0]);
		}

		const C &operator[](unsigned in_Index) const
		{
			if (in_Index >= 0 && in_Index < Data.size())
				return *(Data[in_Index]);
		
			return *(Data[0]);
		}

		//Global
		static void Swap(int Position, int NewPosition)
		{
			if (Position >= 0 && NewPosition >= 0 && Position < Data_t <C>::Data.size() && NewPosition < Data_t <C>::Data.size())
			{
				Global Temp2 = Data_t <C>::Link[Position];
				C* Temp3 = Data_t <C>::Data[Position]; 

				Data_t <C>::Link[Position] = Data_t <C>::Link[NewPosition];
				Data_t <C>::Link[NewPosition] = Temp2;

				Data_t <C>::Data[Position] = Data_t <C>::Data[NewPosition];
				Data_t <C>::Data[NewPosition] = Temp3;
			}
		}

		static void Move(int Position, int NewPosition)
		{
			if (Position >= 0 && NewPosition >= 0 && Position < Data_t <C>::Data.size() && NewPosition < Data_t <C>::Data.size())
			{
				if (Position > NewPosition)
				{
					for (int i = NewPosition; i < Position - 1; i++)
						Swap(i, i + 1);
				}

				else if (Position < NewPosition)
				{
					for (int i = Position; i < NewPosition - 1; i++)
						Swap(i, i + 1);
				}
			}
		}

		template <typename F, typename ...A>
		static void All(F Function, A... Args)
		{
			for (int i = 0; i < (int) Data_t <C>::Data.size(); i++)
				(Data_t <C>::Data[i]->*Function)(Args...);
		}

		static int NumberOfAllObjects()
		{
			return Data_t <C>::Data.size();
		}
	};

	///^^^^^^^^^^^^^^^///
	//	     WIP	   //
	///^^^^^^^^^^^^^^^///



	class DLL_API Window_t
	{
		typedef FunctionReturn(*Function)();
		friend class Timer_t;
		friend class Object_t;
		friend class Image_t;

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

	class DLL_API Image_t
	{	
		friend class Object_t;

	protected:

		Window_t *WindowHandle;

		union
		{
			SDL_Surface *Software;
			SDL_Texture *Hardware;
		};

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

	class DLL_API Object_t
	{
		friend class Window_t;

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

	public:	

		//Replacable functions
		virtual void ClearData();
		virtual void SetDisplay(int ImageIndex);

		int Display();

	//public:

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
	};
}

#undef DLL_API
#endif
													