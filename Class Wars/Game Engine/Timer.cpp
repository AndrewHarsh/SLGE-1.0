#include "SLGE.h"
#include <fstream>

#define DLL_API __declspec(dllexport) 
using namespace SLGE;


DLL_API Window_t::Timer_t::Timer_t()
{
	LastFrame = new HRC::time_point;
	LastBench = new HRC::time_point;
	LastFPSDisplay = new HRC::time_point;
	LastBenchDisplay = new HRC::time_point;

	*LastFrame = HRC::now();
	*LastBench = HRC::now();
	*LastFPSDisplay = HRC::now();
	*LastBenchDisplay = HRC::now();

	FPS = 60;	

	LARGE_INTEGER Li;
	QueryPerformanceFrequency(&Li);

	CPUFreq = double(Li.QuadPart)/1000.0;

	LastIdentifier = nullptr;
}

void DLL_API Window_t::Timer_t::DisplayFPS()
{
	HRC::time_point Now = HRC::now();

	if (duration_cast<milliseconds>(Now - *LastFPSDisplay).count() > 1000)
	{
		*LastFPSDisplay = HRC::now();
		
		//SDL_SetWindowTitle(Handle, std::to_string(CurrentFPS).c_str());
		std::cout << Duration << std::endl;
	}
}

int DLL_API Window_t::Timer_t::CapFPS()
{
	std::fstream File;

	Uint32 EndTime = SDL_GetTicks();

	Uint32 Elapsed = EndTime - StartTime;
	Duration = (double) Elapsed;
	 
	if (Duration < 1000 / FPS)
		SDL_Delay((Uint32)(1000 / FPS - Duration));

	EndTime = SDL_GetTicks();
	Duration = EndTime - StartTime;

	if (Duration <= 0)
		Duration = 1;

	CurrentFPS = 1000 / (Duration);

	StartTime = SDL_GetTicks();

	if (CurrentFPS <= 0)
		CurrentFPS = 1;

	return 0;
}


double DLL_API Window_t::Timer_t::GetFPS()
{
	return CurrentFPS;
}

int DLL_API Window_t::Timer_t::Benchmark(const char in_Identifier[])
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