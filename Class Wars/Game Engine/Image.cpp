#include "SLGE.h"

#define DLL_API __declspec(dllexport) 
using namespace SLGE;


DLL_API Image_t::Image_t()
{
	WindowHandle = nullptr;
	Software = nullptr;
	Hardware = nullptr;

	ClearData();
}

DLL_API Image_t::Image_t(Window_t *in_Window) : Image_t()
{
	Register(in_Window);
}

DLL_API Image_t::~Image_t()
{
	ClearData();
}

void DLL_API Image_t::ClearData()
{
	if (WindowHandle != nullptr)
	{
		if (WindowHandle->HardwareAccelerated)
		{
			if (Hardware != nullptr)
			{
				SDL_DestroyTexture(Hardware);
				Hardware = nullptr;
			}
		}
		else
		{
			if (Software != nullptr)
			{
				SDL_FreeSurface(Software);
				Software = nullptr;
			}
		}
	}

	LoadClip = { 0, 0, 0, 0 };
	DisplayClip = { 0, 0, 0, 0 };
	Angle = 0;
	Center = { 0, 0 };
	FlipType = SDL_FLIP_NONE;

	WindowHandle = nullptr;
}

int DLL_API Image_t::Register(Window_t *in_Window)
{
	if (in_Window->WindowHandle == nullptr || !in_Window->IsRunning())
		return 1;

	if (in_Window->HardwareAccelerated  && in_Window->HScreen != nullptr)
		WindowHandle = in_Window;
	else if (in_Window->Screen != nullptr)
		WindowHandle = in_Window;
	else
		return 1;

	return 0;
}


int DLL_API Image_t::X()
{
	return DisplayClip.x;
}

int DLL_API Image_t::Y()
{
	return DisplayClip.y;
}

int DLL_API Image_t::W()
{
	return LoadClip.w;
}

int DLL_API Image_t::H()
{
	return LoadClip.h;
}

double DLL_API Image_t::GetAngle()
{
	return Angle;
}

SDL_Point DLL_API Image_t::GetCenter()
{
	return Center;
}

SDL_RendererFlip DLL_API Image_t::GetFlipType()
{
	return FlipType;
}

int DLL_API Image_t::OpenImage(const std::string in_Filename, SDL_Rect in_Clip, const SDL_Color in_ColorKey)
{
	if (WindowHandle == nullptr || 
		!WindowHandle->IsRunning() || 
		(WindowHandle->HScreen == nullptr && WindowHandle->Screen == nullptr) || 
		!WindowHandle->IsRunning())
		return 1;

	if (WindowHandle->HardwareAccelerated)
	{
		if (Hardware != nullptr)
			SDL_DestroyTexture(Hardware);
	}
	else
	{
		if (Software != nullptr)
			SDL_FreeSurface(Software);
	}

	SDL_Surface* LoadedSurface = IMG_Load(in_Filename.c_str());

	if (LoadedSurface == nullptr)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", in_Filename.c_str(), IMG_GetError());

		if (in_Clip.w <= 0 || in_Clip.h <= 0)
			LoadedSurface = SDL_CreateRGBSurface(0, WindowHandle->GetWidth(), WindowHandle->GetHeight(), WindowHandle->GetBPP(), 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
		else
			LoadedSurface = SDL_CreateRGBSurface(0, in_Clip.w, in_Clip.h, WindowHandle->GetBPP(), 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);

		if (LoadedSurface == nullptr)
		{
			printf("Unable to load default image! SDL_image Error: %s\n", IMG_GetError());

			if (WindowHandle->HardwareAccelerated)
				Hardware = nullptr;
			else
				Software = nullptr;

			return EXIT_FAILURE;
		}
	}

	if (in_ColorKey.r != NULL || in_ColorKey.g != NULL || in_ColorKey.b != NULL || in_ColorKey.a != NULL)
	{
		//Map the color key
		Uint32 ColorKey = SDL_MapRGB(LoadedSurface->format, in_ColorKey.r, in_ColorKey.g, in_ColorKey.b);

		//Set all pixels of in_Color to be transparent
		SDL_SetColorKey(LoadedSurface, SDL_TRUE, ColorKey);
	}

	if (WindowHandle->HardwareAccelerated)
	{
		//Convert surface to screen format
		Hardware = SDL_CreateTextureFromSurface(WindowHandle->HScreen, LoadedSurface);

		if (Hardware == nullptr)
		{
			printf("Unable to convert texture %s! SDL Error: %s\n", in_Filename.c_str(), SDL_GetError());
			SDL_FreeSurface(LoadedSurface);
			return EXIT_FAILURE;
		}

	}
	else
	{
		//Convert surface to screen format
		Software = SDL_ConvertSurface(LoadedSurface, WindowHandle->Screen->format, NULL);

		if (Software == nullptr)
		{
			printf("Unable to optimize image %s! SDL Error: %s\n", in_Filename.c_str(), SDL_GetError());
			SDL_FreeSurface(LoadedSurface);
			return EXIT_FAILURE;
		}
	}


	LoadClip.x = in_Clip.x;
	LoadClip.y = in_Clip.y;

	if (in_Clip.w > 0 && in_Clip.x + in_Clip.w <= LoadedSurface->w)
		LoadClip.w = in_Clip.w;
	else
		LoadClip.w = LoadedSurface->w;

	if (in_Clip.h > 0 && in_Clip.y + in_Clip.h <= LoadedSurface->h)
		LoadClip.h = in_Clip.h;
	else
		LoadClip.h = LoadedSurface->h;

	//Position defaults
	DisplayClip.x = 0;
	DisplayClip.y = 0;
	DisplayClip.w = LoadClip.w;
	DisplayClip.h = LoadClip.h;

	SDL_FreeSurface(LoadedSurface);

	return 0;
}

int DLL_API Image_t::LoadText(std::string in_Message, TTF_Font *in_Font, SDL_Color in_TextColor)
{
	if (WindowHandle == nullptr || 
		!WindowHandle->IsRunning() || 
		(WindowHandle->HScreen == nullptr && WindowHandle->Screen == nullptr) || 
		!WindowHandle->IsRunning() ||
		in_Font == nullptr)
		return 1;

	SDL_Surface *TempSoftware = Software;
	SDL_Texture *TempHardware = Hardware;

	if (WindowHandle->HardwareAccelerated)
	{
		if (Hardware != nullptr)
			SDL_DestroyTexture(Hardware);
	}
	else
	{
		if (Software != nullptr)
			SDL_FreeSurface(Software);
	}

	SDL_Surface* LoadedSurface = TTF_RenderText_Solid(in_Font, in_Message.c_str(), in_TextColor);

	if (LoadedSurface == nullptr)
		return 1;

	if (WindowHandle->HardwareAccelerated)
	{
		Hardware = SDL_CreateTextureFromSurface(WindowHandle->HScreen, LoadedSurface);

		if (Hardware == nullptr)
		{
			Hardware = TempHardware;
			SDL_FreeSurface(LoadedSurface);
			return 1;
		}
		else
			SDL_DestroyTexture(TempHardware);
	}
	else
	{
		Software = SDL_ConvertSurface(LoadedSurface, WindowHandle->Screen->format, NULL);

		if (Software == nullptr)
		{
			Software = TempSoftware;
			SDL_FreeSurface(LoadedSurface);
			return 1;
		}
		else
			SDL_FreeSurface(TempSoftware);
	}

	LoadClip.x = 0;
	LoadClip.y = 0;
	LoadClip.w = LoadedSurface->w;
	LoadClip.h = LoadedSurface->h;

	//Position defaults
	DisplayClip.x = 0;
	DisplayClip.y = 0;
	DisplayClip.w = LoadedSurface->w;
	DisplayClip.h = LoadedSurface->h;

	SDL_FreeSurface(LoadedSurface);

	return 0;
}


int DLL_API Image_t::SetCoords(double in_X, double in_Y, double in_W, double in_H)
{
	DisplayClip.x = static_cast <int> (in_X);
	DisplayClip.y = static_cast <int> (in_Y);

	if (in_W > 0)
		DisplayClip.w = static_cast <int> (in_W);
	if (in_H > 0)
		DisplayClip.h = static_cast <int> (in_H);

	return 0;
}

int DLL_API Image_t::SetImageProp(double in_Angle, SDL_Point in_Center, SDL_RendererFlip in_FlipType)
{
	Angle = in_Angle;
	Center.x = in_Center.x;
	Center.y = in_Center.y;
	FlipType = in_FlipType;

	return 0;
}

int DLL_API Image_t::Display()
{
	if (WindowHandle == nullptr || !WindowHandle->IsRunning())
		return 1;

	if (WindowHandle->HardwareAccelerated && WindowHandle->HScreen != nullptr && Hardware != nullptr)
		return SDL_RenderCopyEx(WindowHandle->HScreen, Hardware, &LoadClip, &DisplayClip, Angle, &Center, FlipType);
	else if (WindowHandle->Screen != nullptr && Software != nullptr)
		return SDL_BlitSurface(Software, &LoadClip, WindowHandle->Screen, &DisplayClip);
	else
		return 1;
}

#undef DLL_API