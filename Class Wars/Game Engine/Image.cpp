#include "SLGE.h"

#define DLL_API __declspec(dllexport) 
using namespace SLGE;


DLL_API Image_t::Image_t()
{
	ID = new std::string();
	ID->empty();

	WindowHandle = nullptr;
	Software = nullptr;
	Hardware = nullptr;

	ClearData();
}

DLL_API Image_t::Image_t(Window_t *in_Window) : Image_t()
{
	if (in_Window != nullptr)
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
	if (in_Window == nullptr || in_Window->WindowHandle == nullptr || !in_Window->IsRunning())
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
	return DisplayClip.w;
}

int DLL_API Image_t::H()
{
	return DisplayClip.h;
}

int DLL_API Image_t::ClipX()
{
	return LoadClip.x;
}

int DLL_API Image_t::ClipY()
{
	return LoadClip.y;
}

int DLL_API Image_t::ClipW()
{
	return LoadClip.w;
}

int DLL_API Image_t::ClipH()
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

const char DLL_API *Image_t::GetID() const
{
	return ID->c_str();
}


int DLL_API Image_t::Load(const std::string in_Filename, SDL_Rect in_Clip, const SDL_Color in_ColorKey)
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

		//Set blending function
		SDL_SetTextureBlendMode(Hardware, SDL_BLENDMODE_BLEND);

		//Modulate texture alpha
		SDL_SetTextureAlphaMod(Hardware, 255 - in_ColorKey.a);
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

		//Set blending function
		SDL_SetSurfaceBlendMode(Software, SDL_BLENDMODE_BLEND);

		//Modulate texture alpha
		SDL_SetSurfaceAlphaMod(Software, 255 - in_ColorKey.a);
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

int DLL_API Image_t::Load(std::string in_Message, TTF_Font *in_Font, SDL_Color in_TextColor)
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

int DLL_API Image_t::SetID(const char in_ID[])
{
	if (*ID == "")
		*ID = in_ID;
	else
		return 1;

	return 0;
}


int DLL_API Image_t::SetClip(int in_X, int in_Y, int in_W, int in_H)
{
	LoadClip.x = in_X;
	LoadClip.y = in_Y;

	if (in_W >= 0)
		LoadClip.w = in_W;
	if (in_H >= 0)
		LoadClip.h = in_H;

	if (!WindowHandle->HardwareAccelerated)
	{
		DisplayClip.w = LoadClip.w;
		DisplayClip.h = LoadClip.h;
	}

	return 0;
}
					 
int DLL_API Image_t::SetCoords(double in_X, double in_Y)
{
	DisplayClip.x = static_cast <int> (in_X);
	DisplayClip.y = static_cast <int> (in_Y);

	return 0;
}

int DLL_API Image_t::SetSize(int in_W, int in_H)
{
	if (in_W >= 0)
		DisplayClip.w = in_W;
	if (in_H >= 0)
		DisplayClip.h = in_H;

	if (!WindowHandle->HardwareAccelerated)
	{
		LoadClip.w = DisplayClip.w;
		LoadClip.h = DisplayClip.h;
	}

	return 0;
}

int DLL_API Image_t::SetTransparency(double in_Percentage)
{
	if (in_Percentage < 0 || in_Percentage > 1)
		return 1;

	if (WindowHandle->HardwareAccelerated)
	{
		//Set blending function
		SDL_SetTextureBlendMode(Hardware, SDL_BLENDMODE_BLEND);

		//Modulate texture alpha
		SDL_SetTextureAlphaMod(Hardware, (Uint8) (255 * in_Percentage));
	}
	else
	{
		//Set blending function
		SDL_SetSurfaceBlendMode(Software, SDL_BLENDMODE_BLEND);

		//Modulate texture alpha
		SDL_SetSurfaceAlphaMod(Software, (Uint8) (255 * in_Percentage));
	}

	return 0;
}

int DLL_API Image_t::Rotate(double in_Angle, SDL_Point in_Center)
{
	Angle = in_Angle;
	Center.x = in_Center.x;
	Center.y = in_Center.y;

	return 0;
}

int DLL_API Image_t::Flip(SDL_RendererFlip in_FlipType)
{
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