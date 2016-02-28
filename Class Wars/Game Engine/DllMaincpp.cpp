#include "SLGE.h"
#include <map>

#define DLL_API __declspec(dllexport) 
using namespace SLGE;

#undef SDL_DestroyWindow
#undef SDL_CreateWindow

#undef IMG_Load 
#undef SDL_ConvertSurface 
#undef SDL_CreateTextureFromSurface 
#undef SDL_FreeSurface 
#undef SDL_DestroyTexture 


struct Image
{
	SDL_Surface *Data;
	int RefCount;
};

static int OpenWindows = 0;
static std::map <std::string, Image> ImageStore;
static std::map <SDL_Surface*, int> Test;


BOOL WINAPI DllMain(HINSTANCE ModuleHandle, DWORD ReasonForCall, LPVOID Reserved)
{
    switch(ReasonForCall) 
    { 
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		{
			static int OpenWindows = 0;
			static std::map <std::string, SDL_Surface*> ImageList;
			static std::map <SDL_Surface*, int> Test;

			// Version info
			std::ofstream File;
			File.open("Splatter Labs Game Engine Version.txt", std::ios::out);

			if (File.is_open())
			{
				File << "Author:" << "\n\n";

				File << "	Andrew Harsh" << "\n";
				File << "	Senior Framework Developer" << "\n";
				File << "	Splatter Labs" << "\n";
				File << "	" << __DATE__ << "\n\n";

				File << "Version:" << "\n\n";

				File << "	" << Version;
			}
			break;
		}

        case DLL_PROCESS_DETACH:
		case DLL_THREAD_DETACH:
		{
			//Close SDL
			IMG_Quit();
			TTF_Quit();
			Mix_Quit();

			SDL_Quit();

			//Remove Version info
			remove("Splatter Labs Game Engine Version.txt");
			break;
		}
    }
    return TRUE;
}

SDL_Window DLL_API *SLGE::SDL_CreateWindowEx(const char* in_Title, int in_X, int in_Y, int in_W, int in_H, Uint32 in_Flags)
{
	//Initialize SDL
	if (!SDL_WasInit(NULL))
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		{
			printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
			return NULL;
		}

		if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
		{
			printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
			SDL_Quit();
			return NULL;
		}

		if (TTF_Init() < 0)
		{
			printf("SDL_Mixer could not initialize! SDL_Error: %s\n", SDL_GetError());
			IMG_Quit();
			SDL_Quit();
			return NULL;
		}

		/*
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) //4096
		{
			printf("Audio could not initialize! SDL_Error: %s\n", SDL_GetError());
			TTF_Quit();
			IMG_Quit();
			SDL_Quit();
			return NULL;
		}
		
		Mix_AllocateChannels(2);
		*/
	}

	SDL_Window* ExportWindow = SDL_CreateWindow(in_Title, in_X, in_Y, in_W, in_H, in_Flags);

	if (ExportWindow != NULL)
		OpenWindows++;

	return ExportWindow;
}					 
	
void DLL_API SLGE::SDL_DestroyWindowEx(SDL_Window* in_Window)
{
	SDL_DestroyWindow(in_Window);
	OpenWindows--;

	if (OpenWindows <= 0)
	{
		//Mix_Quit();
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();

		OpenWindows = 0;
	}
}


SDL_Surface DLL_API *SLGE::IMG_LoadEx(const char* in_File)
{	
	SDL_Surface* Temp = IMG_Load(in_File);

	if (Temp == NULL)
		return NULL;

	if (Test.find(Temp) != Test.end())
		Test[Temp] = 1;
	else
		Test[Temp]++;

	return Temp;

	/*
	if (ImageStore.find(std::string(in_File)) != ImageStore.end)
	{
		ImageStore[in_File].Data = IMG_Load(in_File);

		if (ImageStore[in_File].Data != NULL)
			ImageStore[in_File].RefCount = 1;
		else
		{
			ImageStore.erase(ImageStore.find(in_File));
			return NULL;
		}
	}
	else
		ImageStore[in_File].RefCount++;

	return ImageStore[in_File].Data;
	*/
}

SDL_Surface DLL_API *SLGE::SDL_ConvertSurfaceEx(SDL_Surface*, SDL_PixelFormat*, Uint32)
{
	return NULL;
}

SDL_Texture DLL_API *SLGE::SDL_CreateTextureFromSurfaceEx(SDL_Renderer*, SDL_Surface*)
{
	return NULL;
}

void DLL_API SLGE::SDL_FreeSurfaceEx(SDL_Surface* in_Surface)
{
	if (Test.find(in_Surface) != Test.end())
	{
		Test[in_Surface]--;

		if (Test[in_Surface] <= 0)
		{
			SDL_FreeSurface(in_Surface);
			Test.erase(in_Surface);
		}
	}
}

void DLL_API SLGE::SDL_DestroyTextureEx(SDL_Texture*)
{

}

#undef DLL_API