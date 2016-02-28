#include "SLGE.h"
#include <fstream>		    

#define DLL_API __declspec(dllexport) 
using namespace SLGE;


//Clock functions from the internet
const long long g_Frequency = []() -> long long 
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    return frequency.QuadPart;
}();

HighResClock::time_point HighResClock::now()
{
    LARGE_INTEGER count;
    QueryPerformanceCounter(&count);
    return time_point(duration(count.QuadPart * static_cast<rep>(period::den) / g_Frequency));
}
//End of non-SplatterLabs code


DLL_API Window_t::Timer_t::Timer_t()
{
	LastFrame = new HRC::time_point;
	LastFPSDisplay = new HRC::time_point;
	LastBench = new HRC::time_point;
	LastIdentifier = new std::string;

	*LastFrame = HRC::now();
	*LastFPSDisplay = HRC::now();
	*LastBench = HRC::now();

	CurrentFPS = 0;

	for (int i = 0; i < FRAME_SAMPLE_SIZE; i++)
		LastFPS[i] = 0;

	system("IF EXIST Benchmark_Results.csv DEL Benchmark_Results.csv");
}

void DLL_API Window_t::Timer_t::DisplayFPS(int in_Delay)
{
	HRC::time_point Now = HRC::now();

	if (duration_cast<milliseconds>(Now - *LastFPSDisplay).count() > in_Delay)
	{
		*LastFPSDisplay = HRC::now();
		
		std::cout << CurrentFPS <<  std::endl;
	}
}

int DLL_API Window_t::Timer_t::CapFPS(double in_DesiredFPS)
{
	auto Elapsed = duration_cast <nanoseconds> (HRC::now() - *LastFrame).count();
	 
	if (in_DesiredFPS > 0)
	{
		if ((double) Elapsed < 1000.0 / in_DesiredFPS)
			SDL_Delay((Uint32) (1000.0 / (in_DesiredFPS - (double) Elapsed)));
	}

	Elapsed = duration_cast <nanoseconds> (HRC::now() - *LastFrame).count();

	if (Elapsed <= 0)
		Elapsed = 1;

	for (int i = 0; i < FRAME_SAMPLE_SIZE; i++)
		CurrentFPS += LastFPS[i];

	CurrentFPS /= FRAME_SAMPLE_SIZE + 1;

	CurrentFPS = 1000000000.0 / (double) Elapsed;

	if (CurrentFPS <= 0)
		CurrentFPS = 1;

	for (int i = 1; i < FRAME_SAMPLE_SIZE; i++)
		LastFPS[i] = LastFPS[i - 1];

	LastFPS[0] = CurrentFPS;
	*LastFrame = HRC::now();

	return 0;
}


double DLL_API Window_t::Timer_t::GetFPS()
{
	return CurrentFPS;
}

int DLL_API Window_t::Timer_t::Benchmark(const char in_Identifier[])
{
	HRC::time_point ThisBench = HRC::now();
	long double ElapsedTime = duration_cast <microseconds> (ThisBench - *LastBench).count() / 1000.0; //In milliseconds
	auto ThisTime = SDL_GetTicks(); //In seconds
	auto LastTime = SDL_GetTicks() - (Uint32) (ElapsedTime / 1000); //In seconds
	std::fstream Benchmark;

	if (/*LastTime % 10 <= 3 && ThisTime % 10 >= 3 &&*/SDL_GetTicks() < 600000 && (*LastIdentifier) != "")
	{
		Benchmark.open("Benchmark_Results.csv", std::ios::app);

		if (Benchmark.is_open())
		{
			Benchmark << SDL_GetTicks() << ",";
			Benchmark <<  (*LastIdentifier) << " to " << in_Identifier << ",";
			Benchmark << ElapsedTime << ",";
			Benchmark << ElapsedTime / (1000.0 / CurrentFPS) * 100.0 << "%," << std::endl;

			Benchmark.close();
		}
	}

	*LastIdentifier = in_Identifier;
	*LastBench = HRC::now();

	return 0;
}


#undef DLL_API