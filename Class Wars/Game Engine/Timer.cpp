#include "SLGE.h"

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
	FPS = 30000;

	LastIdentifier = nullptr;
}

void DLL_API Window::Timer::DisplayFPS()
{
	HRC::time_point Now = HRC::now();

	if (duration_cast<milliseconds>(Now - *LastFPSDisplay).count() > 5000)
	{
		*LastFPSDisplay = HRC::now();
		std::cout << CurrentFPS << std::endl;
	}
}

int DLL_API Window::Timer::CapFPS()
{
	HRC::time_point ThisFrame = HRC::now();
	long long ElapsedTime = duration_cast <microseconds> (ThisFrame - *LastFrame).count();

	if (ElapsedTime < 1000000 / FPS)
		std::this_thread::sleep_for(microseconds((1000000 / FPS) - ElapsedTime));

	ThisFrame = HRC::now();
	ElapsedTime = duration_cast <microseconds> (ThisFrame - *LastFrame).count();
	CurrentFPS = 1000000 / static_cast <double> (ElapsedTime);
	*LastFrame = HRC::now();

	return 0;
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