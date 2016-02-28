#include "SLGE.h"

#define DLL_API __declspec(dllexport) 
using namespace SLGE;


DLL_API Animation_t::AnimateDirection_t::AnimateDirection_t()
{
	ClearData();
}

void DLL_API Animation_t::AnimateDirection_t::ClearData()
{
	StartPosition = 0;
	TotalNumber = 0;
	Duration = 0;
	TimeDisplayed = 0;
}


DLL_API Animation_t::Animation_t()
{
	ClearData();
}

void DLL_API Animation_t::ClearData()
{
	for (int i = 0; i < 8; i++)
		Direction[i].ClearData();

	Active = false;
}

void DLL_API Animation_t::SetAll(int in_StartPosition, double in_Duration, int in_TotalFrames)
{
	SetTotalFramesAll(in_TotalFrames);
	SetDurationAll(in_Duration);
	SetStartPositionAll(in_StartPosition);
}

void DLL_API Animation_t::SetTotalFramesAll(int in_TotalNumber)
{
	for (int i = 0; i < 16; i++)
		Direction[i].TotalNumber = in_TotalNumber;
}

void DLL_API Animation_t::SetDurationAll(double in_Duration)
{
	for (int i = 0; i < 16; i++)
		Direction[i].Duration = in_Duration;
}

void DLL_API Animation_t::SetStartPositionAll(int in_StartPosition)
{
	int StartOffset = 0;

	for (int i = 0; i < 16; i++)
	{
		Direction[i].StartPosition = in_StartPosition + StartOffset;
		StartOffset += Direction[i].TotalNumber;
	}
}


#undef DLL_API