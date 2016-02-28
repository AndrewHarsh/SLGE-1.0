#include "SLGE.h"
#include <fstream>

#define DLL_API __declspec(dllexport) 
using namespace SLGE;


DLL_API Window_t::Timer_t::Timer_t()
{
	LastFPSDisplay = new HRC::time_point;
	LastBench = new HRC::time_point;
	LastIdentifier = new std::string;

	*LastFPSDisplay = HRC::now();
	*LastBench = HRC::now();

	StartTime = SDL_GetTicks();

	system("IF EXIST Benchmark_Results.csv DEL Benchmark_Results.csv");
}

void DLL_API Window_t::Timer_t::DisplayFPS(int in_Delay)
{
	HRC::time_point Now = HRC::now();

	if (duration_cast<milliseconds>(Now - *LastFPSDisplay).count() > in_Delay)
	{
		*LastFPSDisplay = HRC::now();
		
		std::cout << CurrentFPS << std::endl;
	}
}

int DLL_API Window_t::Timer_t::CapFPS(double in_DesiredFPS)
{
	Uint32 EndTime = SDL_GetTicks();
	Uint32 Elapsed = EndTime - StartTime;
	 
	if ((double)Elapsed < 1000.0 / in_DesiredFPS)
		SDL_Delay((Uint32)(1000.0 / (in_DesiredFPS - (double) Elapsed)));

	Elapsed = SDL_GetTicks() - StartTime;

	if (Elapsed <= 0)
		Elapsed = 1;

	CurrentFPS = 1000.0 / (double) Elapsed;

	if (CurrentFPS <= 0)
		CurrentFPS = 1;

	StartTime = SDL_GetTicks();

	return 0;
}


double DLL_API Window_t::Timer_t::GetFPS()
{
	return CurrentFPS;
}

int DLL_API Window_t::Timer_t::Benchmark(const char in_Identifier[])
{
	HRC::time_point ThisBench = HRC::now();
	long double ElapsedTime = duration_cast <microseconds> (ThisBench - *LastBench).count() / 1000.0;
	std::fstream Benchmark;

	Benchmark.open("Benchmark_Results.csv", std::ios::app);

	if (Benchmark.is_open())
	{
		Benchmark << (*LastIdentifier) << " to " << in_Identifier << ",";
		Benchmark << ElapsedTime << ",";
		Benchmark << ElapsedTime / (1000.0 / CurrentFPS) * 100.0 << "%," << std::endl;

		Benchmark.close();
	}

	*LastIdentifier = in_Identifier;
	*LastBench = HRC::now();

	return 0;
}


#undef DLL_API