#include "SLGE.h"
#include <fstream>

#define DLL_API __declspec(dllexport) 
using namespace SLGE;


DLL_API Window::Timer::Timer()
{
	LastFrame = new HRC::time_point;
	LastBench = new HRC::time_point;
	LastFPSDisplay = new HRC::time_point;
	LastBenchDisplay = new HRC::time_point;

	*LastFrame = HRC::now();
	*LastBench = HRC::now();
	*LastFPSDisplay = HRC::now();
	*LastBenchDisplay = HRC::now();

	FPS = 10;	

	LARGE_INTEGER Li;
	QueryPerformanceFrequency(&Li);

	CPUFreq = double(Li.QuadPart)/1000.0;

	LastIdentifier = nullptr;
}

void DLL_API Window::Timer::DisplayFPS()
{
	HRC::time_point Now = HRC::now();

	if (duration_cast<milliseconds>(Now - *LastFPSDisplay).count() > 1000)
	{
		*LastFPSDisplay = HRC::now();
		
		//SDL_SetWindowTitle(Handle, std::to_string(CurrentFPS).c_str());
		std::cout << Duration << std::endl;
	}
}

int DLL_API Window::Timer::CapFPS()
{
	std::fstream File;

	Uint32 EndTime = SDL_GetTicks();

	Uint32 Elapsed = EndTime - StartTime;
	Duration = (double) Elapsed;

	File.open("Data.txt", std::ios::app);

	if (File.is_open())
	{
		File << EndTime << " - " << StartTime << " = " << Duration << std::endl;
		File << "				1000 / FPS - Duration:     " << 1000 / FPS - Duration << "   (Delay)" << std::endl;
		File << "				1000 / Duration:           " << 1000 / Duration << "   (FPS)" << std::endl;
		File.close();
	}

	if (Duration < 1000 / FPS)
		SDL_Delay(1000 / FPS - Duration);

	EndTime = SDL_GetTicks();
	Duration = EndTime - StartTime;

	if (Duration <= 0)
		Duration = 1;

	CurrentFPS = 1000 / (Duration);

	StartTime = SDL_GetTicks();

	if (CurrentFPS <= 0)
		CurrentFPS = 1;

	

	//QueryPerformanceCounter(&End);

	//LARGE_INTEGER Li;
	//QueryPerformanceFrequency(&Li);

	//CPUFreq = double(Li.QuadPart)/1000.0;

	//Duration = static_cast <long double> (End.QuadPart - Start.QuadPart) / CPUFreq;

	//QueryPerformanceCounter(&Start);

	//if (Duration < 1000 / FPS)
	//	SDL_Delay(1000 / FPS - Duration);

	//if (Duration < 1)
	//	Duration = 1;

	//if (Duration > 10)
	//	int heefdfs = 8;

	//CurrentFPS = 1000 / Duration;

	//if (CurrentFPS < 1)
	//	CurrentFPS = 1;

	return 0;
}


double DLL_API Window::Timer::GetFPS()
{
	return CurrentFPS;
}

int DLL_API Window::Timer::Benchmark(const char in_Identifier[])
{
	HRC::time_point ThisBench = HRC::now();
	long long ElapsedTime = duration_cast <microseconds> (ThisBench - *LastBench).count();


	if (duration_cast<milliseconds>(ThisBench - *LastBenchDisplay).count() > 5000)
	{
		if (LastIdentifier == nullptr)
		{
			int temp = strlen(in_Identifier);

			LastIdentifier = new char[strlen(in_Identifier) + 1];
			strcpy_s(LastIdentifier, strlen(in_Identifier) + 1, in_Identifier);

			std::cout << "	" << in_Identifier << ":	" << static_cast <double> (ElapsedTime) / 1000.0 << "ms";
			std::cout << "			" << (static_cast <double> (ElapsedTime) / 1000.0) / (1000.0 / CurrentFPS) * 100.0 << "%" << std::endl;
		}
		else if (!strcmp(LastIdentifier, in_Identifier))
		{
			delete[] LastIdentifier;
			LastIdentifier = nullptr;
			*LastBenchDisplay = HRC::now();
		}
		else
		{
			std::cout << "	" << in_Identifier << ":	" << static_cast <double> (ElapsedTime) / 1000.0 << "ms";
			std::cout << "			" << (static_cast <double> (ElapsedTime) / 1000.0) / (1000.0 / CurrentFPS) * 100.0 << "%" << std::endl;
		}
	}

	*LastBench = HRC::now();

	return 0;
}

#undef DLL_API