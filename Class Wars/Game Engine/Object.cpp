#include "SLGE.h"

#define DLL_API __declspec(dllexport) 
using namespace SLGE;


DLL_API Object::Object()
{
	Image = nullptr;
	HImage = nullptr;
	Clip = nullptr;

	ClearData();
}

DLL_API Object::Object(Window *in_WindowHandle) : Object()
{
	Register(in_WindowHandle);
}

void DLL_API Object::ClearData()
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

	NumberOfImages = 0;
	ImageToDisplay = 0;

	X = 0;
	Y = 0;
	W = 0;
	H = 0;
}

void DLL_API Object::PerFrameActions()
{
}

void DLL_API Object::EventHandler(SDL_Event* in_Event)
{
}

void DLL_API Object::CollisionDetection()
{
}

void DLL_API Object::Animate()
{
}

void DLL_API Object::Display()
{
	if (WindowHandle == nullptr || ((Image == nullptr || Image[ImageToDisplay] == NULL) && (HImage == nullptr || HImage[ImageToDisplay] == NULL)))
		return;

	SDL_Rect Offset;

	Offset.x = static_cast <int> (round(X - Clip[ImageToDisplay].w / 2.0));
	Offset.y = static_cast <int> (round(Y - Clip[ImageToDisplay].h / 2.0));
	Offset.w = Clip[ImageToDisplay].w;
	Offset.h = Clip[ImageToDisplay].h;

	if (WindowHandle->HardwareAccelerated)
		SDL_RenderCopy(WindowHandle->HScreen, HImage[ImageToDisplay], &Clip[ImageToDisplay], &Offset);
	else
		SDL_BlitSurface(Image[ImageToDisplay], &Clip[ImageToDisplay], WindowHandle->Screen, &Offset);
}


int DLL_API Object::GetNumberOfImages()
{
	return NumberOfImages;
}

int DLL_API Object::GetCurrentImage()
{
	return NumberOfImages;
}

double DLL_API Object::GetX()
{
	return X;
}

double DLL_API Object::GetY()
{
	return Y;
}

double DLL_API Object::GetW()
{
	return W;
}

double DLL_API Object::GetH()
{
	return H;
}


int DLL_API Object::Register(Window *in_Window)
{
	if (in_Window->WindowHandle == nullptr || (in_Window->HScreen == nullptr && in_Window->Screen == nullptr))
		return 1;

	WindowHandle = in_Window;
	WindowHandle->AddToScreen(this);

	return 0;
}

void DLL_API Object::Deregister()
{
	WindowHandle = nullptr;
}

void DLL_API Object::SetCoords(const double in_X, const double in_Y, const double in_W, const double in_H)
{
	X = in_X;
	Y = in_Y;

	if (in_W > 0)
		W = in_W;
	if (in_H > 0)
		H = in_H;
}

										 
int DLL_API Object::OpenImage(const std::string in_Filename, const SDL_Rect in_Clip, const Color in_ColorKey)
{
	if (WindowHandle == nullptr)
		return 1;

	if (WindowHandle->HardwareAccelerated)
	{
		SDL_Texture **TempImageArray = new SDL_Texture*[NumberOfImages + 1];
		SDL_Rect *TempClipArray = new SDL_Rect[NumberOfImages + 1];

		for (int i = 0; i < NumberOfImages; i++)
		{
			TempImageArray[i] = HImage[i];
			TempClipArray[i] = Clip[i];
		}

		delete[] HImage;
		delete[] Clip;

		HImage = TempImageArray;
		Clip = TempClipArray;
	}
	else
	{
		SDL_Surface **TempImageArray = new SDL_Surface*[NumberOfImages + 1];
		SDL_Rect *TempClipArray = new SDL_Rect[NumberOfImages + 1];

		for (int i = 0; i < NumberOfImages; i++)
		{
			TempImageArray[i] = Image[i];
			TempClipArray[i] = Clip[i];
		}

		delete[] Image;
		delete[] Clip;

		Image = TempImageArray;
		Clip = TempClipArray;
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

	//Map the color key
	Uint32 ColorKey = SDL_MapRGB(LoadedSurface->format, in_ColorKey.R, in_ColorKey.G, in_ColorKey.B);

	//Set all pixels of in_Color to be transparent
	SDL_SetColorKey(LoadedSurface, SDL_TRUE, ColorKey);

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

	ImageToDisplay = NumberOfImages;
	NumberOfImages++;
	SDL_FreeSurface(LoadedSurface);

	return 0;
}

int DLL_API Object::SelectImage(const int in_Position)
{
	if (in_Position >= 0 && in_Position < NumberOfImages)
		ImageToDisplay = in_Position;
	else
		return 1;

	return 0;
}

int DLL_API Object::DeleteImage(const int in_Position)
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