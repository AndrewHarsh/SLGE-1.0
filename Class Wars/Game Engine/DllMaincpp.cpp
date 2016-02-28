#include "SLGE.h"
#include <vector>
#include <map>

#define DLL_API __declspec(dllexport) 
using namespace SLGE;

#undef SDL_DestroyWindow
#undef SDL_CreateWindow

#undef IMG_Load 
#undef SDL_CreateRGBSurface
#undef SDL_ConvertSurface 
#undef SDL_CreateTextureFromSurface 
#undef SDL_FreeSurface 
#undef SDL_DestroyTexture 


struct Image
{
	SDL_Surface* LoadData;
	SDL_Surface* Data;
	SDL_Texture* HData;
	std::string Name;
	SDL_PixelFormat* Format;
	SDL_Renderer* Renderer;
};


//==============================================================//
//  THIS CLASS WORKS BUT SHOULD BE REDONE TO BE MORE EFFICIENT  //
//==============================================================//
static class ImageTable
{
private:
	//Table 1
	std::map <std::string, int> NameMap;
	std::map <SDL_Surface*, int> SurfaceMap;
	std::map <SDL_Texture*, int> TextureMap;
	//Table 2
	std::map <SDL_PixelFormat*, int> FormatMap;
	std::map <SDL_Renderer*, int> RenderMap;
	//Table 3
	std::vector <Image> Data;

	void ShiftBack(int Position)
	{
		auto NameIter = --NameMap.begin();
		auto SurfaceIter = --SurfaceMap.begin();
		auto TextureIter = --TextureMap.begin();
		Data.erase(Data.begin() + Position);

		//Shift all of the indexes back one to make up for the erased key
		for (auto Iter = NameMap.begin(); Iter != NameMap.end() && NameMap.size() > 0; Iter++)
		{
			if (Iter->second > Position)
				Iter->second--;
			if (Iter->second == Position)
				NameIter = Iter;//NameMap.erase(Iter);
		}

		for (auto Iter = SurfaceMap.begin(); Iter != SurfaceMap.end() && SurfaceMap.size() > 0; Iter++)
		{
			if (Iter->second > Position)
				Iter->second--;
			if (Iter->second == Position)
				SurfaceIter = Iter;//SurfaceMap.erase(Iter);
		}

		for (auto Iter = TextureMap.begin(); Iter != TextureMap.end() && TextureMap.size() > 0; Iter++)
		{
			if (Iter->second > Position)
				Iter->second--;
			if (Iter->second == Position)
				TextureIter = Iter; TextureMap.erase(Iter);
		}

		if (NameIter != --NameMap.begin())
			NameMap.erase(NameIter);
		if (SurfaceIter != --SurfaceMap.begin())
			SurfaceMap.erase(SurfaceIter);
		if (TextureIter != --TextureMap.begin())
			TextureMap.erase(TextureIter);
	}

public:

	int Add(Image in_Data)
	{
		//Make sure data is valid
		if (in_Data.LoadData == nullptr && in_Data.Data == nullptr && in_Data.HData == nullptr)
			return 1;


		int Position = -1;
		bool NoName = false;
		bool NoLoad = false;
		bool NoSurface = false;
		bool NoTexture = false;
		bool NoFormat = false;
		bool NoRenderer = false;

		//Verify and check data
		if (in_Data.Name == "")
			NoName = true;
		else
		{
			if (NameMap.find(in_Data.Name) == NameMap.end())
				NoName = true;
			else
				Position = NameMap[in_Data.Name];
		}

		if (in_Data.LoadData == nullptr)
			NoLoad = true;
		else
		{
			if (SurfaceMap.find(in_Data.LoadData) == SurfaceMap.end())
				NoLoad = true;
			else
				Position = SurfaceMap[in_Data.LoadData];
		}

		if (in_Data.Data == nullptr)
			NoSurface = true;
		else
		{
			if (SurfaceMap.find(in_Data.Data) == SurfaceMap.end())
				NoSurface = true;
			else
				Position = SurfaceMap[in_Data.Data];
		}

		if (in_Data.HData == nullptr)
			NoTexture = true;
		else
		{
			if (TextureMap.find(in_Data.HData) == TextureMap.end())
				NoTexture = true;
			else
				Position = TextureMap[in_Data.HData];
		}

		if (in_Data.Format == nullptr)
			NoFormat = true;
		else
		{
			if (FormatMap.find(in_Data.Format) == FormatMap.end())
				NoFormat = true;
			else
				Position = FormatMap[in_Data.Format];
		}

		if (in_Data.Renderer == nullptr)
			NoRenderer = true;
		else
		{
			if (RenderMap.find(in_Data.Renderer) == RenderMap.end())
				NoRenderer = true;
			else
				Position = RenderMap[in_Data.Renderer];
		}


		//Create new data if none exists
		if (NoName &&
			NoLoad &&
			NoSurface &&
			NoTexture)
		{
			//Allocate space
			Data.push_back(in_Data);

			//Add link between maps and data if possible
			if (in_Data.Name != "")
				NameMap[in_Data.Name] = Data.size() - 1;

			if (in_Data.LoadData != nullptr)
				SurfaceMap[in_Data.LoadData] = Data.size() - 1;

			if (in_Data.Data != nullptr)
				SurfaceMap[in_Data.Data] = Data.size() - 1;

			if (in_Data.HData != nullptr)
				TextureMap[in_Data.HData] = Data.size() - 1;
		}

		//Add
		//else if (NoFormat && NoRenderer)//((Data[Position].Format != in_Data.Format && in_Data.Format != nullptr) && 
				//(Data[Position].Renderer != in_Data.Renderer && in_Data.Renderer != nullptr))
		//{		 
			//Data.push_back(in_Data);

			//if (in)
		//}

		//Add refence to current table if already exists
		else  
		{
			//Update Table
			if (in_Data.Data != nullptr)
			{		 
				SurfaceMap[in_Data.Data] = Position;
				Data[Position].Data = in_Data.Data;
			}
			if (in_Data.HData != nullptr)
			{
				TextureMap[in_Data.HData] = Position;
				Data[Position].HData = in_Data.HData;
			}
			if (in_Data.LoadData != nullptr)
			{
				SurfaceMap[in_Data.LoadData] = Position;
				Data[Position].LoadData = in_Data.LoadData;
			}
			if (in_Data.Name != "")
			{
				NameMap[in_Data.Name] = Position;
				Data[Position].Name = in_Data.Name;
			}
		}

		return 0;
	}

	int Size()
	{
		return Data.size();
	}

	Image* Pull(unsigned in_Key)
	{
		if (in_Key < 0 || in_Key >= Data.size())
			return nullptr;

		return &Data[in_Key];
	}

	Image* Pull(std::string in_Key)
	{
		if (NameMap.find(in_Key) == NameMap.end())
			return nullptr;

		return &Data[NameMap[in_Key]];
	}

	Image* Pull(SDL_Surface* in_Key)
	{
		if (SurfaceMap.find(in_Key) == SurfaceMap.end())
			return nullptr;

		return &Data[SurfaceMap[in_Key]];
	}

	Image* Pull(SDL_Texture* in_Key)
	{
		if (TextureMap.find(in_Key) == TextureMap.end())
			return nullptr;

		return &Data[TextureMap[in_Key]];
	}

	int Return(Image* in_Handle)
	{
		return 0;
	}

	int Remove(std::string in_Key)
	{
		if (in_Key == "")
			return 1;

		if (NameMap.find(in_Key) == NameMap.end())
			return 1;
		
		ShiftBack(NameMap[in_Key]);

		return 0;
	}

	int Remove(SDL_Surface* in_Key)
	{
		if (in_Key == nullptr)
			return 1;

		if (SurfaceMap.find(in_Key) == SurfaceMap.end())
			return 1;

		//if (--Data[SurfaceMap[in_Key]].SurfaceCount <= 0)
			ShiftBack(SurfaceMap[in_Key]);

		return 0;
	}

	int Remove(SDL_Texture* in_Key)
	{
		if (in_Key == nullptr)
			return 1;

		if (TextureMap.find(in_Key) == TextureMap.end())
			return 1;

		//if (--Data[TextureMap[in_Key]].TextureCount <= 0)
			ShiftBack(TextureMap[in_Key]);

		return 0;
	}
} AllImages;

static int OpenWindows = 0;


BOOL WINAPI DllMain(HINSTANCE ModuleHandle, DWORD ReasonForCall, LPVOID Reserved)
{
    switch(ReasonForCall) 
    { 
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		{
			static int OpenWindows = 0;
			static ImageTable AllImages;

			//remove("Data.txt");

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
			for (int i = 0; i < AllImages.Size(); i++)
			{
				Image* DeleteData = AllImages.Pull(i);

				if (DeleteData->Data != nullptr)
				{
					SDL_FreeSurface(DeleteData->Data);
					DeleteData->Data = nullptr;
				}
				if (DeleteData->LoadData != nullptr)
				{
					SDL_FreeSurface(DeleteData->LoadData);
					DeleteData->LoadData = nullptr;
				}
				if (DeleteData->HData != nullptr)
				{
					//SDL_DestroyTexture(DeleteData->HData);
					DeleteData->HData = nullptr;
				}
			}

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
	Image *Check = AllImages.Pull(in_File);

	if (Check == nullptr || Check->LoadData == nullptr)
	{
		SDL_Surface *NewSurface = IMG_Load(in_File);

		if (NewSurface == nullptr)
			return nullptr;

		// Must use TEMP in order to prevent heap corruption (not sure why)
		Image TEMP = { NewSurface, nullptr, nullptr, in_File, nullptr, nullptr };
		AllImages.Add(TEMP);//{ NewSurface, nullptr, nullptr, std::string(in_File), nullptr, nullptr });

		return NewSurface;
	}

	return Check->LoadData;
}

SDL_Surface DLL_API *SLGE::SDL_CreateRGBSurfaceEx(Uint32 in_Flags, int in_Width, int in_Height, int in_Depth, Uint32 in_Rmask, Uint32 in_Gmask, Uint32 in_Bmask, Uint32 in_Amask)
{
	return SDL_CreateRGBSurface(in_Flags, in_Width, in_Height, in_Depth, in_Rmask, in_Gmask, in_Bmask, in_Amask);
}

SDL_Surface DLL_API *SLGE::SDL_ConvertSurfaceEx(SDL_Surface* in_Surface, SDL_PixelFormat* in_Format, Uint32 in_Flags)
{
	Image *Check = AllImages.Pull(in_Surface);

	if (Check == nullptr || Check->Data == nullptr)
	{
		SDL_Surface *NewSurface = SDL_ConvertSurface(in_Surface, in_Format, in_Flags);

		if (NewSurface == nullptr)
			return nullptr;

		AllImages.Add({ in_Surface, NewSurface, nullptr, "", in_Format, nullptr });

		return NewSurface;
	}

	return Check->Data;
}

SDL_Texture DLL_API *SLGE::SDL_CreateTextureFromSurfaceEx(SDL_Renderer* in_Renderer, SDL_Surface* in_Surface)
{
	Image *Check = AllImages.Pull(in_Surface);

	if (Check == nullptr || Check->HData == nullptr)
	{
		SDL_Texture *NewTexture = SDL_CreateTextureFromSurface(in_Renderer, in_Surface);

		if (NewTexture == nullptr)
			return nullptr;

		AllImages.Add({ in_Surface, nullptr, NewTexture, "", nullptr, in_Renderer });

		return NewTexture;
	}

	return Check->HData;
}

void DLL_API SLGE::SDL_FreeSurfaceEx(SDL_Surface* in_Surface)
{
	Image *Check = AllImages.Pull(in_Surface);

	if (Check == nullptr || Check->Data == nullptr)
		return;
}

void DLL_API SLGE::SDL_DestroyTextureEx(SDL_Texture* in_Texture)
{
	Image *Check = AllImages.Pull(in_Texture);

	if (Check == nullptr || Check->Data == nullptr)
		return;
}

#undef DLL_API