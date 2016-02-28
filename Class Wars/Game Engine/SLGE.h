#include <iostream>
#include <Windows.h>
#include <SDL.h>

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

		Object **ScreenObjects;
		int NumberOfObjects;

		void ClearData();
		int Display(const Object *ObjectToDisplay);
		int EventHandler(const Object *ObjectToDisplay);

	public:

		Window();

		int GetWidth();
		int GetHeight();
		int GetBPP();

		int Init();
		int Init(const int Width, const int Height);

		int AddToScreen(Object *ScreenObject);
		int RemoveFromScreen(Object *ScreenObject);

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

		void ClearData();
		virtual int Display();
		virtual int EventHandler();

	public:

		Object();
		Object(Window *WindowHandle);

		int GetNumberOfImages();
		int GetCurrentImage();
		double GetX();
		double GetY();
		double GetW();
		double GetH();

		int Init(Window *Window);
		void SetCoords(const double X, const double Y, const double W, const double H);

		int OpenImage(const std::string Filename, SDL_Rect Clip, const Color ColorKey);
		int SelectImage(const int Position);
		int DeleteImage(const int Position);
	};

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

		char *Text;
		int TextLength;
		TTF_Font *Font;
		SDL_Color FontColor;

		float TextX;
		float TextY;

		void ClearData();

	public:

		UI();
		UI(Window *WindowHandle);

	};
}

#undef DLL_API
