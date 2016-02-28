#pragma once

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


#ifdef SLGE_EXPORT
	#define DLL_API __declspec(dllexport) 
#else
	#define DLL_API	__declspec(dllimport)
#endif

#define Colour Color
#define colour color

//For an overloaded class method
#define HandleCollisionWith HandleCollisionWithO

#define SDL_CreateWindow SDL_CreateWindowEx
#define SDL_DestroyWindow SDL_DestroyWindowEx

#define IMG_Load IMG_LoadEx
#define SDL_CreateRGBSurface SDL_CreateRGBSurfaceEx
#define SDL_ConvertSurface SDL_ConvertSurfaceEx
#define SDL_CreateTextureFromSurface SDL_CreateTextureFromSurfaceEx
#define SDL_FreeSurface SDL_FreeSurfaceEx
#define SDL_DestroyTexture SDL_DestroyTextureEx


namespace SLGE
{
	const char Version[] = "1.0.0";
	const int HARDWARE_ACCELERATION = 0x01; //0000 0001

	//REAL high resolution clock structure stolen from Stack Overflow user Dave
	//http://stackoverflow.com/questions/16299029/resolution-of-stdchronohigh-resolution-clock-doesnt-correspond-to-measureme
	struct HighResClock
	{
		typedef long long                               rep;
		typedef std::nano                               period;
		typedef std::chrono::duration<rep, period>      duration;
		typedef std::chrono::time_point<HighResClock>   time_point;
		static const bool is_steady = true;

		static time_point now();
	};

	typedef HighResClock HRC;
	using std::chrono::milliseconds;
	using std::chrono::microseconds;
	using std::chrono::nanoseconds;
	using std::chrono::duration_cast;

	//Injected functions
	SDL_Window DLL_API *SDL_CreateWindowEx(const char* Title, int X, int Y, int W, int H, Uint32 Flags);
	void DLL_API SDL_DestroyWindowEx(SDL_Window* Window);

	SDL_Surface DLL_API *IMG_LoadEx(const char* File);
	SDL_Surface DLL_API *SDL_CreateRGBSurfaceEx(Uint32 Flags, int Width, int Height, int Depth, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);
	SDL_Surface DLL_API *SDL_ConvertSurfaceEx(SDL_Surface*, SDL_PixelFormat*, Uint32);
	SDL_Texture DLL_API *SDL_CreateTextureFromSurfaceEx(SDL_Renderer*, SDL_Surface*);
	void DLL_API SDL_FreeSurfaceEx(SDL_Surface*);
	void DLL_API SDL_DestroyTextureEx(SDL_Texture*);

	//Declarations
	enum DLL_API FunctionReturn
	{
		Error = -1,
		Continue = 0,
		Exit = 1
	};

	template <typename C, typename ...D>
	class DynamicClass;

	class DLL_API Window_t;
	class DLL_API Image_t;
	class DLL_API Object_t;
}