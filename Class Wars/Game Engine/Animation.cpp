#include "SLGE.h"

#define DLL_API __declspec(dllexport) 
using namespace SLGE;


//AnimateDirection_t
DLL_API AnimateDirection_t::AnimateDirection_t()
{
	ClearData();
}

void DLL_API AnimateDirection_t::ClearData()
{
	StartPosition = 0;
	TotalNumber = 0;
	Duration = 0;
	TimeDisplayed = 0;
	Active = false;
}

//=========================================================

//AnimateType_t
DLL_API AnimateType_t::AnimateType_t()
{
	direction = new std::vector <AnimateDirection_t>;
	Map = new std::vector <int>;

	ClearData();
}

DLL_API AnimateType_t::AnimateType_t(const AnimateType_t &in_Copy) : AnimateType_t()
{
	*direction = *(in_Copy.direction);
	*Map = *(in_Copy.Map);
}

AnimateType_t DLL_API &AnimateType_t::operator=(const AnimateType_t &in_Copy)
{
	*direction = *(in_Copy.direction);
	*Map = *(in_Copy.Map);

	return *this;
}

DLL_API AnimateType_t::~AnimateType_t()
{
	delete direction;
	delete Map;
}

void DLL_API AnimateType_t::ClearData()
{
	direction->clear();
	Map->clear();
	Active = false;
}


AnimateDirection_t DLL_API &AnimateType_t::Direction(int in_Direction)
{
	if (in_Direction >= 0 && in_Direction < (int) Map->size())
	{
		if ((*Map)[in_Direction] == in_Direction)
			return (*direction)[in_Direction];
	}

	for (int i = 0; i < (int) Map->size(); i++)
	{
		if ((*Map)[i] == in_Direction)
			return (*direction)[i];
	}

	AddDirection(in_Direction);
	return (*direction)[0];
}

int DLL_API AnimateType_t::AddDirection(int in_Direction)
{
	Map->push_back(in_Direction);
	direction->push_back(AnimateDirection_t());
	return 0;
}

int DLL_API AnimateType_t::RemoveDirection(int in_Direction)
{
	for (int i = 0; i < (int) Map->size(); i++)
	{
		if ((*Map)[i] == in_Direction)
		{
			direction->erase(direction->begin() + i);
			Map->erase(Map->begin() + i);
			return 0;
		}
	}
	return 1;
}


void DLL_API AnimateType_t::SetAll(int in_StartPosition, double in_Duration, int in_TotalFrames)
{
	SetTotalFramesAll(in_TotalFrames);
	SetDurationAll(in_Duration);
	SetStartPositionAll(in_StartPosition);
}

void DLL_API AnimateType_t::SetTotalFramesAll(int in_TotalNumber)
{
	for (int i = 0; i < (int) direction->size(); i++)
		(*direction)[i].TotalNumber = in_TotalNumber;
}

void DLL_API AnimateType_t::SetDurationAll(double in_Duration)
{
	for (int i = 0; i < (int) direction->size(); i++)
		(*direction)[i].Duration = in_Duration;
}

void DLL_API AnimateType_t::SetStartPositionAll(int in_StartPosition)
{
	int StartOffset = 0;

	for (int i = 0; i < (int) direction->size(); i++)
	{
		(*direction)[i].StartPosition = in_StartPosition + StartOffset;
		StartOffset += (*direction)[i].TotalNumber;
	}
}


//==========================================================

//Animation_t
DLL_API Animation_t::Animation_t()
{
	type = new std::vector <AnimateType_t>;
	Map = new std::vector <int>;

	ClearData();
}

DLL_API Animation_t::Animation_t(const Animation_t &in_Copy) : Animation_t()
{
	*type = *(in_Copy.type);
	*Map = *(in_Copy.Map);
}

Animation_t DLL_API &Animation_t::operator=(const Animation_t &in_Copy)
{
	*type = *(in_Copy.type);
	*Map = *(in_Copy.Map);

	return *this;
}

DLL_API Animation_t::~Animation_t()
{
	delete type;
	delete Map;
}

void DLL_API Animation_t::ClearData()
{
	type->clear();
	Map->clear();
}


AnimateType_t DLL_API &Animation_t::Type(int in_Type)
{
	if (in_Type >= 0 && in_Type < (int) Map->size())
	{
		if ((*Map)[in_Type] == in_Type)
			return (*type)[in_Type];
	}

	for (int i = 0; i < (int) Map->size(); i++)
	{
		if ((*Map)[i] == in_Type)
			return (*type)[i];
	}

	AddType(in_Type);
	return (*type)[0];
}

int DLL_API Animation_t::AddType(int in_Type)
{
	Map->push_back(in_Type);
	type->push_back(AnimateType_t());
	return 0;
}

int DLL_API Animation_t::RemoveType(int in_Type)
{
	for (int i = 0; i < (int) Map->size(); i++)
	{
		if ((*Map)[i] == in_Type)
		{
			type->erase(type->begin() + i);
			Map->erase(Map->begin() + i);
			return 0;
		}
	}
	return 1;
}


void DLL_API Animation_t::SetAll(int in_StartPosition, double in_Duration, int in_TotalFrames)
{
	SetTotalFramesAll(in_TotalFrames);
	SetDurationAll(in_Duration);
	SetStartPositionAll(in_StartPosition);
}

void DLL_API Animation_t::SetTotalFramesAll(int in_TotalNumber)
{
	for (int i = 0; i < (int) type->size(); i++)
		(*type)[i].SetTotalFramesAll(in_TotalNumber);
}

void DLL_API Animation_t::SetDurationAll(double in_Duration)
{
	for (int i = 0; i < (int) type->size(); i++)
		(*type)[i].SetDurationAll(in_Duration);
}

void DLL_API Animation_t::SetStartPositionAll(int in_StartPosition)
{
	for (int i = 0; i < (int) type->size(); i++)
		(*type)[i].SetStartPositionAll(in_StartPosition);
}


#undef DLL_API