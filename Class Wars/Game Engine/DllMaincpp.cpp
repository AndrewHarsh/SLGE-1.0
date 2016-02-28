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
	SDL_Surface* Data;
	SDL_Texture* HData;
	std::string Name;
	int SurfaceCount;
	int TextureCount;
};

static class ImageTable
{
private:
	std::map <std::string, int> NameMap;
	std::map <SDL_Surface*, int> SurfaceMap;
	std::map <SDL_Texture*, int> TextureMap;
	std::vector <Image> Data;

public:

	int Add(Image in_Data)
	{
		//Make sure data is valid
		if (in_Data.Data == nullptr && in_Data.HData == nullptr)
			return 1;


		bool NoName, NoSurface, NoTexture;

		//Verify and check data
		if (in_Data.Name == "")
			NoName = true;
		else
		{
			if (NameMap.find(in_Data.Name) == NameMap.end())
				NoName = true;
			else
				NoName = false;
		}

		if (in_Data.Data == nullptr)
			NoSurface = true;
		else
		{
			if (SurfaceMap.find(in_Data.Data) == SurfaceMap.end())
				NoSurface = true;
			else
				NoSurface = false;
		}

		if (in_Data.HData == nullptr)
			NoTexture = true;
		else
		{
			if (TextureMap.find(in_Data.HData) == TextureMap.end())
				NoTexture = true;
			else
				NoTexture = false;
		}


		//Create new data if none exists
		if (NoName &&
			NoSurface&&
			NoTexture)
		{
			//Allocate space
			Data.push_back({in_Data.Data, in_Data.HData, in_Data.Name, 0, 0});

			//Add link between maps and data if possible
			if (in_Data.Name != "")
				NameMap[in_Data.Name] = Data.size() - 1;

			if (in_Data.Data != nullptr)
				SurfaceMap[in_Data.Data] = Data.size() - 1;

			if (in_Data.HData != nullptr)
				TextureMap[in_Data.HData] = Data.size() - 1;
		}
		//Add refence to current table if already exists
		else  
		{
			if (in_Data.Data != nullptr)
				Data[SurfaceMap[in_Data.Data]].SurfaceCount++;
			if (in_Data.HData != nullptr)
				Data[TextureMap[in_Data.HData]].TextureCount++;
		}

		return 0;
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
		
		int Position = NameMap[in_Key];
		Data.erase(Data.begin() + Position);

		//Shift all of the indexes back one to make up for the erased key
		for (auto Iter = NameMap.begin(); Iter != NameMap.end() && NameMap.size() > 0; Iter++)
		{
			if (Iter->second > Position)
				Iter->second--;
			if (Iter->second == Position)
				NameMap.erase(Iter);
		}

		for (auto Iter = SurfaceMap.begin(); Iter != SurfaceMap.end() && SurfaceMap.size() > 0; Iter++)
		{
			if (Iter->second > Position)
				Iter->second--;
			if (Iter->second == Position)
				SurfaceMap.erase(Iter);
		}

		for (auto Iter = TextureMap.begin(); Iter != TextureMap.end() && TextureMap.size() > 0; Iter++)
		{
			if (Iter->second > Position)
				Iter->second--;
			if (Iter->second == Position)
				TextureMap.erase(Iter);
		}

		return 0;
	}

	int Remove(SDL_Surface* in_Key)
	{
		if (in_Key == nullptr)
			return 1;

		if (SurfaceMap.find(in_Key) == SurfaceMap.end())
			return 1;

		if (--Data[SurfaceMap[in_Key]].SurfaceCount <= 0)
		{
			int Position = SurfaceMap[in_Key];
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

		return 0;
	}

	int Remove(SDL_Texture* in_Key)
	{
		if (in_Key == nullptr)
			return 1;

		if (TextureMap.find(in_Key) == TextureMap.end())
			return 1;

		if (--Data[TextureMap[in_Key]].TextureCount <= 0)
		{
			int Position = TextureMap[in_Key];
			Data.erase(Data.begin() + Position);

			//Shift all of the indexes back one to make up for the erased key
			for (auto Iter = NameMap.begin(); Iter != NameMap.end() && NameMap.size() > 0; Iter++)
			{
				if (Iter->second > Position)
					Iter->second--;
				if (Iter->second == Position)
					NameMap.erase(Iter);
			}

			for (auto Iter = SurfaceMap.begin(); Iter != SurfaceMap.end() && SurfaceMap.size() > 0; Iter++)
			{
				if (Iter->second > Position)
					Iter->second--;
				if (Iter->second == Position)
					SurfaceMap.erase(Iter);
			}

			for (auto Iter = TextureMap.begin(); Iter != TextureMap.end() && TextureMap.size() > 0; Iter++)
			{
				if (Iter->second > Position)
					Iter->second--;
				if (Iter->second == Position)
					TextureMap.erase(Iter);
			}
		}

		return 0;
	}
} AllImages;




/*
class ImageMap
{
private:
	SDL_Surface **Data;
	std::string *Name;
	int *RefCount;
	int Size;

public:
	ImageMap()
	{
		Data = nullptr;
		RefCount = nullptr;
		Size = 0;
	}

	void Add(SDL_Surface* in_Data, std::string in_Name)
	{
		for (int i = 0; i < Size; i++)
		{
			if (in_Name == Name[i])
			{
				RefCount[i]++;
				return;
			}
		}

		SDL_Surface **TempData = new SDL_Surface*[Size + 1];
		std::string *TempName = new std::string[Size + 1];
		int *TempCount = new int[Size + 1];

		for (int i = 0; i < Size; i++)
		{
			TempData[i] = Data[i];
			TempName[i] = Name[i];
			TempCount[i] = RefCount[i];
		}

		TempData[Size] = in_Data;
		TempName[Size] = in_Name;
		TempCount[Size] = 1;

		delete[] Data;
		delete[] Name;
		delete[] RefCount;

		Data = TempData;
		Name = TempName;
		RefCount = TempCount;

		Size++;
	}

	void Remove(SDL_Surface* in_Data)
	{
		for (int i = 0; i < Size; i++)
		{
			if (Data[i] == in_Data)
			{
				RefCount[i]--;

				if (RefCount[i] <= 0)
				{
					SDL_Surface **TempData = new SDL_Surface*[Size];
					std::string *TempName = new std::string[Size];
					int *TempCount = new int[Size];

					for (int ii = 0; ii < Size + 1; ii++)
					{
						if (ii < i)
						{
							TempData[ii] = Data[ii];
							TempName[ii] = Name[ii];
							TempCount[ii] = RefCount[ii];
						}
						else
						{
							TempData[ii] = Data[ii + 1];
							TempName[ii] = Name[ii + 1];
							TempCount[ii] = RefCount[ii + 1];
						}
					}

					delete[] Data;
					delete[] Name;
					delete[] RefCount;

					Data = TempData;
					Name = TempName;
					RefCount = TempCount;
				}
			}
		}
	}

	int Find(SDL_Surface* in_Data)
	{
		for (int i = 0; i < Size; i++)
		{
			if (Data[i] == in_Data)
				return i;
		}

		return -1;
	}

	int Find(std::string in_Name)
	{
		for (int i = 0; i < Size; i++)
		{
			if (Name[i] == in_Name)
				return i;
		}

		return -1;
	}
};

ImageMap Store;
int ImageMapSize;
*/

static int OpenWindows = 0;

static std::map <std::string, Image> Book1;
static std::map <SDL_Surface*, Image> Book2;
static std::map <SDL_Texture*, Image> Book3;


BOOL WINAPI DllMain(HINSTANCE ModuleHandle, DWORD ReasonForCall, LPVOID Reserved)
{
    switch(ReasonForCall) 
    { 
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		{
			static int OpenWindows = 0;
			//static std::map <std::string, SDL_Surface*> ImageList;
			//static std::map <SDL_Surface*, int> Test;
			static ImageTable AllImages;
			static std::map <std::string, Image> Book1;
			static std::map <SDL_Surface*, Image> Book2;
			static std::map <SDL_Surface*, Image> Book3;


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
	Image *Check = AllImages.Pull(in_File);

	if (Check == nullptr || Check->Data == nullptr)
	{
		SDL_Surface *NewSurface = IMG_Load(in_File);

		if (NewSurface == nullptr)
			return nullptr;

		AllImages.Add({ NewSurface, nullptr, in_File, 0, 0 });

		return NewSurface;
	}

	Check->SurfaceCount++;
	return Check->Data;






	/*
	SDL_Surface *Temp;

	if (Book1.find(in_File) != Book1.end())
	{
		Temp = Book1[in_File].Data;

		Book1[in_File].SurfaceCount++;
		Book2[Temp].SurfaceCount++;
		return Temp;
	}
	else
	{
		Temp = IMG_Load(in_File);

		if (Temp == NULL)
			return NULL;

		Image Temp1 = { Temp, NULL, in_File, 1, 0 };

		Book1[in_File] = Temp1;
		Book2[Temp] = Temp1;

		return Temp;
	}
	*/


	//SDL_Surface* Temp = IMG_Load(in_File);

	//return Temp;
	//if (Temp == NULL)
	//	return NULL;

	//if (Test.find(Temp) != Test.end())
	//	Test[Temp] = 1;
	//else
	//{
	//	Test[Temp]++;
	//	SDL_FreeSurface(Temp);
	//}

	//return Test.find(Temp)->first;

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

SDL_Surface DLL_API *SLGE::SDL_CreateRGBSurfaceEx(Uint32 in_Flags, int in_Width, int in_Height, int in_Depth, Uint32 in_Rmask, Uint32 in_Gmask, Uint32 in_Bmask, Uint32 in_Amask)
{
	return SDL_CreateRGBSurface(in_Flags, in_Width, in_Height, in_Depth, in_Rmask, in_Gmask, in_Bmask, in_Amask);
}

SDL_Surface DLL_API *SLGE::SDL_ConvertSurfaceEx(SDL_Surface* in_Surface, SDL_PixelFormat* in_Format, Uint32 in_Flags)
{
	SDL_Surface *NewSurface = SDL_ConvertSurface(in_Surface, in_Format, in_Flags);

	if (NewSurface == nullptr)
		return nullptr;

	Image *Check = AllImages.Pull(NewSurface);

	if (Check == nullptr || Check->Data == nullptr)
	{
		AllImages.Add({ NewSurface, nullptr, "", 0, 0 });

		return NewSurface;
	}

	Check->SurfaceCount++;
	return Check->Data;

	
	//if (Book2.find(in_Surface) != Book2.end())
	//{
	//	Book2[in_Surface].SurfaceCount++;
	//	Book1[Book2[in_Surface].Name].SurfaceCount++;

		//return SDL_ConvertSurface(in_Surface, in_Format, in_Flags);
	//}
	
	//return NULL;
}

SDL_Texture DLL_API *SLGE::SDL_CreateTextureFromSurfaceEx(SDL_Renderer* in_Renderer, SDL_Surface* in_Surface)
{
	SDL_Texture *NewTexture = SDL_CreateTextureFromSurface(in_Renderer, in_Surface);
		
	if (NewTexture == nullptr)
		return nullptr;

	Image *Check = AllImages.Pull(NewTexture);

	if (Check == nullptr || Check->Data == nullptr)
	{
		AllImages.Add({ nullptr, NewTexture, "", 0, 0 });

		return NewTexture;
	}

	Check->TextureCount++;
	return Check->HData;

	//if (Book2.find(in_Surface) != Book2.end())
	//{
	//	SDL_Surface *Temp;

		//Book2[in_Surface].RefCount++;
		//Book1[Book2[in_Surface].Name].RefCount++;

		//Book2[in_Surface].HData = 
	//}

	//return NULL;
}

void DLL_API SLGE::SDL_FreeSurfaceEx(SDL_Surface* in_Surface)
{
	Image *Check = AllImages.Pull(in_Surface);

	if (Check == nullptr || Check->Data == nullptr)
		return;

	if (--(Check->SurfaceCount) <= 0)
	{
		SDL_FreeSurface(in_Surface);
		AllImages.Remove(in_Surface);
	}

	/*
	if (Book2.find(in_Surface) != Book2.end())
	{
		if (Book2[in_Surface].SurfaceCount > 1)
			int hy = 9;

		Book2[in_Surface].SurfaceCount--;
		Book1[Book2[in_Surface].Name].SurfaceCount--;

		if (Book2[in_Surface].SurfaceCount <= 0)
		{
			SDL_FreeSurface(Book2[in_Surface].Data);
			Book1.erase(Book2[in_Surface].Name);
			Book2.erase(in_Surface);
		}
	}
	*/

	/*
	if (Test.find(in_Surface) != Test.end())
	{
		Test[in_Surface]--;

		if (Test[in_Surface] <= 0)
		{
			SDL_FreeSurface(in_Surface);
			Test.erase(in_Surface);
		}
	}
	*/
}

void DLL_API SLGE::SDL_DestroyTextureEx(SDL_Texture* in_Texture)
{
	Image *Check = AllImages.Pull(in_Texture);

	if (Check == nullptr || Check->Data == nullptr)
		return;

	if (--(Check->TextureCount) <= 0)
	{
		SDL_DestroyTexture(in_Texture);
		AllImages.Remove(in_Texture);
	}
}

#undef DLL_API