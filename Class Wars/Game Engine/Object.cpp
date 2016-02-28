#include "SLGE.h"

#define DLL_API __declspec(dllexport) 
using namespace SLGE;


//Private Methods
DLL_API Object_t::Object_t()
{
	//ObjectID = nullptr;

	Image = nullptr;
	HImage = nullptr;
	Clip = nullptr;
	DisplayClip = nullptr;

	ClearData();
}

DLL_API Object_t::Object_t(Window_t *in_WindowHandle) : Object_t()
{
	Register(in_WindowHandle);
}

void DLL_API Object_t::SetDisplay()
{
	DisplayClip[ImageToDisplay].x = (int) (X - W / 2);
	DisplayClip[ImageToDisplay].y = (int) (Y - H / 2);
	DisplayClip[ImageToDisplay].w = (int) W;
	DisplayClip[ImageToDisplay].h = (int) H;

	//ImageCenter[ImageToDisplay].x = (int) X;
	//ImageCenter[ImageToDisplay].y = (int) Y;
}

int DLL_API Object_t::Display()
{
	if (WindowHandle == nullptr)
		return 1;

	SetDisplay();

	if (WindowHandle->HardwareAccelerated && WindowHandle->HScreen != NULL && HImage[ImageToDisplay] != NULL)
		return SDL_RenderCopyEx(WindowHandle->HScreen, HImage[ImageToDisplay], &Clip[ImageToDisplay], &DisplayClip[ImageToDisplay], ImageAngle[ImageToDisplay], &ImageCenter[ImageToDisplay], ImageFlip[ImageToDisplay]);
	else if (WindowHandle->Screen != NULL && Image[ImageToDisplay] != NULL)
		return SDL_BlitSurface(Image[ImageToDisplay], &Clip[ImageToDisplay], WindowHandle->Screen, &DisplayClip[ImageToDisplay]);
	else 
		return 1;
}


//Status Methods
int DLL_API Object_t::GetNumberOfImages()
{
	return NumberOfImages;
}

int DLL_API Object_t::GetCurrentImage()
{
	return ImageToDisplay;
}

double DLL_API Object_t::GetX()
{
	return X;
}

double DLL_API Object_t::GetY()
{
	return Y;
}

double DLL_API Object_t::GetW()
{
	return W;
}

double DLL_API Object_t::GetH()
{
	return H;
}

bool DLL_API Object_t::IsOverlapping(SDL_Rect in_Rect)
{
	if (X + W / 2 >= in_Rect.x - in_Rect.w / 2 && 
		X - W / 2 <= in_Rect.x + in_Rect.w / 2 && 
		Y + H / 2 >= in_Rect.y - in_Rect.h / 2 && 
		Y - H / 2 <= in_Rect.y + in_Rect.h / 2)
		return true;
	else
		return false;
}

bool DLL_API Object_t::IsOverlapping(Object_t *in_Object)
{
	if (X + W / 2 >= in_Object->GetX() - in_Object->GetW() / 2 && 
		X - W / 2 <= in_Object->GetX() + in_Object->GetW() / 2 && 
		Y + H / 2 >= in_Object->GetY() - in_Object->GetH() / 2 && 
		Y - H / 2 <= in_Object->GetY() + in_Object->GetH() / 2)
		return true;
	else
		return false;
}

bool DLL_API Object_t::IsWithin(SDL_Rect in_Rect)
{
	if (X - W / 2 >= in_Rect.x - in_Rect.w / 2 && 
		X + W / 2 <= in_Rect.x + in_Rect.w / 2 && 
		Y - H / 2 >= in_Rect.y - in_Rect.h / 2 && 
		Y + H / 2 <= in_Rect.y + in_Rect.h / 2)
		return true;
	else
		return false;
}

bool DLL_API Object_t::IsWithin(Object_t *in_Object)
{
	if (X - W / 2 >= in_Object->GetX() - in_Object->GetW() / 2 && 
		X + W / 2 <= in_Object->GetX() + in_Object->GetW() / 2 && 
		Y - H / 2 >= in_Object->GetY() - in_Object->GetH() / 2 && 
		Y + H / 2 <= in_Object->GetY() + in_Object->GetH() / 2)
		return true;
	else
		return false;
}


//Setting Methods
int DLL_API Object_t::Register(Window_t *in_Window)
{
	if (in_Window->WindowHandle == nullptr || (in_Window->HScreen == nullptr && in_Window->Screen == nullptr))
		return 1;

	WindowHandle = in_Window;

	return 0;
}

void DLL_API Object_t::Deregister()
{
	WindowHandle = nullptr;
}

void DLL_API Object_t::SetCoords(const double in_X, const double in_Y, const double in_W, const double in_H)
{
	X = in_X;
	Y = in_Y;

	if (in_W > 0)
		W = in_W;
	if (in_H > 0)
		H = in_H;

	DisplayClip[ImageToDisplay].x = (int) X;
	DisplayClip[ImageToDisplay].y = (int) Y;
	DisplayClip[ImageToDisplay].w = Clip[ImageToDisplay].w;
	DisplayClip[ImageToDisplay].h = Clip[ImageToDisplay].h;
}

										 
int DLL_API Object_t::OpenImage(const std::string in_Filename, const SDL_Rect in_Clip, const Color in_ColorKey)
{
	if (WindowHandle == nullptr)
		return 1;

	if (WindowHandle->HardwareAccelerated)
	{
		SDL_Texture **TempImageArray = new SDL_Texture*[NumberOfImages + 1];
		SDL_Rect *TempClipArray = new SDL_Rect[NumberOfImages + 1];
		SDL_Rect *TempDisplayClipArray = new SDL_Rect[NumberOfImages + 1];

		for (int i = 0; i < NumberOfImages; i++)
		{
			TempImageArray[i] = HImage[i];
			TempClipArray[i] = Clip[i];
			TempDisplayClipArray[i] = DisplayClip[i];
		}

		delete[] HImage;
		delete[] Clip;
		delete[] DisplayClip;

		HImage = TempImageArray;
		Clip = TempClipArray;
		DisplayClip = TempDisplayClipArray;

		ImageAngle.push_back(0);
		ImageCenter.push_back({ X, Y });
		ImageFlip.push_back(SDL_FLIP_NONE);
	}
	else
	{
		SDL_Surface **TempImageArray = new SDL_Surface*[NumberOfImages + 1];
		SDL_Rect *TempClipArray = new SDL_Rect[NumberOfImages + 1];
		SDL_Rect *TempDisplayClipArray = new SDL_Rect[NumberOfImages + 1];

		for (int i = 0; i < NumberOfImages; i++)
		{
			TempImageArray[i] = Image[i];
			TempClipArray[i] = Clip[i];
			TempDisplayClipArray[i] = DisplayClip[i];
		}

		delete[] Image;
		delete[] Clip;
		delete[] DisplayClip;

		Image = TempImageArray;
		Clip = TempClipArray;
		DisplayClip = TempDisplayClipArray;
	}

	SDL_Surface* LoadedSurface = IMG_Load(in_Filename.c_str());

	if (LoadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", in_Filename.c_str(), IMG_GetError());

		if (in_Clip.w <= 0 || in_Clip.h <= 0)
			LoadedSurface = SDL_CreateRGBSurface(0, WindowHandle->GetWidth(), WindowHandle->GetHeight(), WindowHandle->GetBPP(), 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
		else
			LoadedSurface = SDL_CreateRGBSurface(0, in_Clip.w, in_Clip.h, WindowHandle->GetBPP(), 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);

		if (LoadedSurface == NULL)
		{
			printf("Unable to load default image! SDL_image Error: %s\n", IMG_GetError());

			if (WindowHandle->HardwareAccelerated)
				HImage[NumberOfImages] = NULL;
			else
				Image[NumberOfImages] = NULL;

			return EXIT_FAILURE;
		}
	}

	if (in_ColorKey.R != NULL || in_ColorKey.G != NULL || in_ColorKey.B != NULL || in_ColorKey.A != NULL)
	{
		//Map the color key
		Uint32 ColorKey = SDL_MapRGB(LoadedSurface->format, in_ColorKey.R, in_ColorKey.G, in_ColorKey.B);

		//Set all pixels of in_Color to be transparent
		SDL_SetColorKey(LoadedSurface, SDL_TRUE, ColorKey);
	}

	if (WindowHandle->HardwareAccelerated)
	{
		//Convert surface to screen format
		HImage[NumberOfImages] = SDL_CreateTextureFromSurface(WindowHandle->HScreen, LoadedSurface);

		if (HImage[NumberOfImages] == NULL)
		{
			printf("Unable to convert texture %s! SDL Error: %s\n", in_Filename.c_str(), SDL_GetError());
			SDL_FreeSurface(LoadedSurface);
			return EXIT_FAILURE;
		}

	}
	else
	{
		//Convert surface to screen format
		Image[NumberOfImages] = SDL_ConvertSurface(LoadedSurface, WindowHandle->Screen->format, NULL);

		if (Image[NumberOfImages] == NULL)
		{
			printf("Unable to optimize image %s! SDL Error: %s\n", in_Filename.c_str(), SDL_GetError());
			SDL_FreeSurface(LoadedSurface);
			return EXIT_FAILURE;
		}
	}


	Clip[NumberOfImages].x = in_Clip.x;
	Clip[NumberOfImages].y = in_Clip.y;

	if (in_Clip.w > 0 && in_Clip.x + in_Clip.w <= LoadedSurface->w)
		Clip[NumberOfImages].w = in_Clip.w;
	else
		Clip[NumberOfImages].w = LoadedSurface->w;

	if (in_Clip.h > 0 && in_Clip.y + in_Clip.h <= LoadedSurface->h)
		Clip[NumberOfImages].h = in_Clip.h;
	else
		Clip[NumberOfImages].h = LoadedSurface->h;

	DisplayClip[NumberOfImages].x = static_cast <int> (round(X - Clip[NumberOfImages].w / 2.0));
	DisplayClip[NumberOfImages].y = static_cast <int> (round(Y + H / 2.0 - Clip[NumberOfImages].h));
	DisplayClip[NumberOfImages].w = Clip[NumberOfImages].w;
	DisplayClip[NumberOfImages].h = Clip[NumberOfImages].h;

	ImageToDisplay = NumberOfImages++;
	SDL_FreeSurface(LoadedSurface);

	return 0;
}

int DLL_API Object_t::SelectImage(const int in_Position)
{
	if (in_Position >= 0 && in_Position < NumberOfImages)
		ImageToDisplay = in_Position;
	else
		return 1;

	return 0;
}

int DLL_API Object_t::MoveImage(const int in_Position, const int in_NewPosition)
{
	if (in_Position == in_NewPosition || 
		in_Position < 0 || in_Position > NumberOfImages || 
		in_NewPosition < 0 || in_NewPosition > NumberOfImages)
		return 0;

	if (WindowHandle->HardwareAccelerated)
	{
		SDL_Texture *TempTexture = HImage[in_Position];
		SDL_Rect TempClip = Clip[in_Position];

		if (in_Position < in_NewPosition)
		{
			for (int i = in_Position; i < in_NewPosition; i++)
				HImage[i] = HImage[i + 1];
		}
		else
		{
			for (int i = in_Position; i > in_NewPosition; i--)
				HImage[i] = HImage[i - 1];
		}

		HImage[in_NewPosition] = TempTexture;
	}
	else
	{
		SDL_Surface *TempSurface = Image[in_Position];
		SDL_Rect TempClip = Clip[in_Position];

		if (in_Position < in_NewPosition)
		{
			for (int i = in_Position; i < in_NewPosition; i++)
				Image[i] = Image[i + 1];
		}
		else
		{
			for (int i = in_Position; i > in_NewPosition; i--)
				Image[i] = Image[i - 1];
		}

		Image[in_NewPosition] = TempSurface;
	}

	return 0;
}

int DLL_API Object_t::DeleteImage(const int in_Position)
{
	if (in_Position >= 0 && in_Position < NumberOfImages)
	{
		SDL_Surface **TempImageArray = new SDL_Surface*[NumberOfImages - 1];
		SDL_Rect *TempClipArray = new SDL_Rect[NumberOfImages - 1];

		for (int i = 0; i < NumberOfImages; i++)
		{
			if (i < in_Position)
			{
				TempImageArray[i] = Image[i];
				TempClipArray[i] = Clip[i];
			}
			else
			{
				TempImageArray[i] = Image[i + 1];
				TempClipArray[i] = Clip[i + 1];
			}
		}

		SDL_FreeSurface(Image[in_Position]);
		delete[] Clip;

		Image = TempImageArray;
		Clip = TempClipArray;

		NumberOfImages--;

		if (ImageToDisplay >= in_Position)
			ImageToDisplay--;
	}
	else
		return 1;

	return 0;
}

#undef DLL_API