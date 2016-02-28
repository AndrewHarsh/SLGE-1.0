#include "SLGE.h"

#define DLL_API __declspec(dllexport) 
using namespace SLGE;


//Private Methods
DLL_API Object_t::Object_t()
{
	ID = new std::string;
	WindowHandle = nullptr;

	Image = new std::vector <Image_t>;
	ImageToDisplay = new std::vector <int>;
	Animation = new std::vector <Animation_t>;

	ClearData();
}

DLL_API Object_t::~Object_t()
{
	ClearData();

	//Animation = new std::vector <Animation_t>;

	WindowHandle = nullptr;

	Animation->clear();

	if (Image != nullptr)
		delete Image;
	if (ImageToDisplay != nullptr)
		delete ImageToDisplay;
	if (ID != nullptr)
		delete ID;
	if (Animation != nullptr)
		delete Animation;
	
	Animation = nullptr;
	Image = nullptr;
	ImageToDisplay = nullptr;
	ID = nullptr;
}

DLL_API Object_t::Object_t(Window_t *in_WindowHandle) : Object_t()
{
	Register(in_WindowHandle);
}

void DLL_API Object_t::ClearData()
{
	X = 0;
	Y = 0;
	W = 0;
	H = 0;
}

int DLL_API Object_t::Log(std::ofstream &File, std::string Precursor)
{
	if (!File.is_open())
		File.open("Log Data.txt", std::ios::app);

	if (File.is_open())
	{
		File << Precursor << "=================== Object_t =====================" << std::endl;
		File << Precursor << "this: " << "0x" << this << std::endl;
		File << Precursor << "ID: " << *ID << std::endl;
		File << Precursor << "WindowHandle: " << "0x" << WindowHandle;

		for (int i = 0; i < (int) Image->size(); i++)
			File << Precursor << "Image[" << i << "]: " << "0x" << &(*Image)[i];

		for (int i = 0; i < (int) ImageToDisplay->size(); i++)
			File << Precursor << "ImageToDisplay[" << i << "]: " << (*ImageToDisplay)[i];

		File << Precursor << "X: " << X << std::endl;
		File << Precursor << "Y: " << Y << std::endl;
		File << Precursor << "W: " << W << std::endl;
		File << Precursor << "H: " << H << std::endl;

		File.close();
	}
	else
		return 1;

	return 0;
}

void DLL_API Object_t::SetImage(int in_ImageIndex)
{
	if (in_ImageIndex >= 0 && in_ImageIndex < (int) Image->size())
		(*Image)[in_ImageIndex].SetCoords(X - (*Image)[in_ImageIndex].W() / 2, Y - (*Image)[in_ImageIndex].H() / 2);
}

void DLL_API Object_t::Animate(int in_Layer, int in_Direction)
{
	if (in_Layer >= 0 && in_Layer < (int) Animation->size() && 
		in_Direction >= 0 && in_Direction < 16 && 
		(*Animation)[in_Layer].Active)
	{
		(*Animation)[in_Layer].Direction[in_Direction].TimeDisplayed += 1 / WindowHandle->TimerHandle.GetFPS();

		while ((*Animation)[in_Layer].Direction[in_Direction].TimeDisplayed > (*Animation)[in_Layer].Direction[in_Direction].Duration &&
				(*Animation)[in_Layer].Direction[in_Direction].Duration > 0)
		{
			(*Animation)[in_Layer].Direction[in_Direction].TimeDisplayed -= (*Animation)[in_Layer].Direction[in_Direction].Duration;
			(*ImageToDisplay)[in_Layer]++;

			if ((*ImageToDisplay)[in_Layer] >= (*Animation)[in_Layer].Direction[in_Direction].StartPosition + (*Animation)[in_Layer].Direction[in_Direction].TotalNumber ||
				(*ImageToDisplay)[in_Layer] < (*Animation)[in_Layer].Direction[in_Direction].StartPosition)
			{
				(*ImageToDisplay)[in_Layer] = (*Animation)[in_Layer].Direction[in_Direction].StartPosition;
			}
		}
	}
}

int DLL_API Object_t::Display()
{
	for (int i = 0; i < (int) ImageToDisplay->size(); i++)
	{
		Animate(i, 0);
		SetImage((*ImageToDisplay)[i]);

		if ((*Image)[(*ImageToDisplay)[i]].Display() != EXIT_SUCCESS)
			return 1;
	}

	return 0;
}


//Status Methods
const char DLL_API* Object_t::GetID() const
{
	return ID->c_str(); 
}

Image_t DLL_API &Object_t::GetImageAtIndex(int in_Index)
{
	if (in_Index >= 0 && in_Index < (int) Image->size())
		return (*Image)[in_Index];

	return (*Image)[0];
}

int DLL_API Object_t::GetIndexOfImage(const Image_t* in_Image)
{
	for (int i = 0; i < (int) Image->size(); i++)
	{
		if (in_Image == &((*Image)[i]))
			return i;
	}

	return -1;
}

int DLL_API Object_t::GetIndexOfImage(const char in_ID[])
{
	for (int i = 0; i < (int) Image->size(); i++)
	{
		if (!strcmp(in_ID, (*Image)[i].GetID()))
			return i;
	}

	return -1;
}

int DLL_API Object_t::GetImageAtLayer(int in_Layer)
{
	if (in_Layer >= 0 && in_Layer < (int) ImageToDisplay->size())
		return (*ImageToDisplay)[in_Layer];
	else
		return -1;
}

int DLL_API Object_t::GetNumberOfImages()
{
	return Image->size();
}

int DLL_API Object_t::GetNumberOfLayers()
{
	return ImageToDisplay->size();
}

double DLL_API Object_t::GetX()
{
	return X;
}

double DLL_API Object_t::GetY()
{
	return Y;
}
																
double DLL_API Object_t::GetW()
{
	return W;
}

double DLL_API Object_t::GetH()
{
	return H;
}

bool DLL_API Object_t::IsOverlapping(SDL_Rect in_Rect)
{
	if (X + W / 2 >= in_Rect.x - in_Rect.w / 2 && 
		X - W / 2 <= in_Rect.x + in_Rect.w / 2 && 
		Y + H / 2 >= in_Rect.y - in_Rect.h / 2 && 
		Y - H / 2 <= in_Rect.y + in_Rect.h / 2)
		return true;
	else
		return false;
}

bool DLL_API Object_t::IsOverlapping(Object_t *in_Object)
{
	if (X + W / 2 >= in_Object->GetX() - in_Object->GetW() / 2 && 
		X - W / 2 <= in_Object->GetX() + in_Object->GetW() / 2 && 
		Y + H / 2 >= in_Object->GetY() - in_Object->GetH() / 2 && 
		Y - H / 2 <= in_Object->GetY() + in_Object->GetH() / 2)
		return true;
	else
		return false;
}

bool DLL_API Object_t::IsWithin(SDL_Rect in_Rect)
{
	if (X - W / 2 >= in_Rect.x - in_Rect.w / 2 && 
		X + W / 2 <= in_Rect.x + in_Rect.w / 2 && 
		Y - H / 2 >= in_Rect.y - in_Rect.h / 2 && 
		Y + H / 2 <= in_Rect.y + in_Rect.h / 2)
		return true;
	else
		return false;
}

bool DLL_API Object_t::IsWithin(Object_t *in_Object)
{
	if (X - W / 2 >= in_Object->GetX() - in_Object->GetW() / 2 && 
		X + W / 2 <= in_Object->GetX() + in_Object->GetW() / 2 && 
		Y - H / 2 >= in_Object->GetY() - in_Object->GetH() / 2 && 
		Y + H / 2 <= in_Object->GetY() + in_Object->GetH() / 2)
		return true;
	else
		return false;
}


//Setting Methods
int DLL_API Object_t::Register(Window_t *in_Window)
{
	if (in_Window == nullptr || in_Window->WindowHandle == nullptr || !in_Window->IsRunning() || (in_Window->HScreen == nullptr && in_Window->Screen == nullptr))
		return 1;

	WindowHandle = in_Window;

	return 0;
}

int DLL_API Object_t::SetID(const char in_ID[])
{
	if (*ID == "")
		*ID = in_ID;
	else
		return 1;

	return 0;
}

int DLL_API Object_t::SetImageID(const char in_ID[], int in_Index)
{
	if (in_Index >= 0 && in_Index < (int) Image->size())
		return (*Image)[in_Index].SetID(in_ID);
	else
		return 1;
}


void DLL_API Object_t::SetCoords(double in_X, double in_Y)
{
	X = in_X;
	Y = in_Y;
}

void DLL_API Object_t::SetSize(double in_W, double in_H)
{
	if (in_W > 0)
		W = in_W;
	if (in_H > 0)
		H = in_H;
}

										 
int DLL_API Object_t::AddImage(std::string in_Filename, SDL_Rect in_Clip, SDL_Color in_ColorKey)
{
	Image->push_back(Image_t(WindowHandle));

	if ((*Image)[Image->size() - 1].Load(in_Filename, in_Clip, in_ColorKey))
		return 1;

	if (W <= 0)
		W = (*Image)[Image->size() - 1].W();
	if (H <= 0)
		H = (*Image)[Image->size() - 1].H();

	if (ImageToDisplay->size() == 0)
		return AddLayer(Image->size() - 1);

	return 0;
}

int DLL_API Object_t::AddText(std::string in_Message, TTF_Font *in_Font, SDL_Color in_TextColor)
{
	Image->push_back(Image_t(WindowHandle));

	if ((*Image)[Image->size() - 1].Load(in_Message, *in_Font, in_TextColor))
		return 1;

	if (W <= 0)
		W = (*Image)[Image->size() - 1].W();
	if (H <= 0)
		H = (*Image)[Image->size() - 1].H();

	if (ImageToDisplay->size() == 0)
		return AddLayer(Image->size() - 1);

	return 0;
}

int DLL_API Object_t::MoveImage(int in_Position, int in_NewPosition)
{
	if (in_Position == in_NewPosition || 
		in_Position < 0 || in_Position >= (int) Image->size() || 
		in_NewPosition < 0 || in_NewPosition >= (int) Image->size())
		return 1;

	Image->insert(Image->begin() + in_NewPosition, (*Image)[in_Position]);

	if (in_NewPosition > in_Position)
		Image->erase(Image->begin() + in_Position);
	else
		Image->erase(Image->begin() + in_Position + 1);

	return 0;
}

int DLL_API Object_t::DeleteImage(int in_Position)
{
	if (in_Position >= 0 && in_Position < (int) Image->size())
		Image->erase(Image->begin() + in_Position);
	else
		return 1;

	///*
	for (int i = 0; i < (int) ImageToDisplay->size(); i++)
	{
		if (in_Position < (*ImageToDisplay)[i])
			(*ImageToDisplay)[i]--;

		if (in_Position == (*ImageToDisplay)[i])
			ImageToDisplay->erase(ImageToDisplay->begin() + i);
	}
	//*/

	return 0;
}


int DLL_API Object_t::AddLayer(std::string in_Filename, SDL_Rect in_Clip, SDL_Color in_ColorKey)
{
	if (AddImage(in_Filename, in_Clip, in_ColorKey))
		return 1;

	ImageToDisplay->push_back(Image->size() - 1);
	Animation->push_back(Animation_t());

	return 0;
}

int DLL_API Object_t::AddLayer(int in_ImagePosition)
{
	if (in_ImagePosition >= 0 && in_ImagePosition < (int) Image->size())
	{
		ImageToDisplay->push_back(in_ImagePosition);
		Animation->push_back(Animation_t());
	}
	else
		return 1;

	return 0;
}

int DLL_API Object_t::MoveLayer(int in_Position, const int in_NewPosition)
{
	if (in_Position == in_NewPosition || 
		in_Position < 0 || in_Position >= (int) ImageToDisplay->size() || 
		in_NewPosition < 0 || in_NewPosition >= (int) ImageToDisplay->size())
		return 1;

	ImageToDisplay->insert(ImageToDisplay->begin() + in_NewPosition, (*ImageToDisplay)[in_Position]);

	if (in_NewPosition > in_Position)
		ImageToDisplay->erase(ImageToDisplay->begin() + in_Position);
	else
		ImageToDisplay->erase(ImageToDisplay->begin() + in_Position + 1);

	return 0;
}

int DLL_API Object_t::DeleteLayer(int in_Position)
{
	if (in_Position >= 0 && in_Position < (int) ImageToDisplay->size())
	{
		ImageToDisplay->erase(ImageToDisplay->begin() + in_Position);
		Animation->erase(Animation->begin() + in_Position);
	}
	else
		return 1;

	return 0;
}

#undef DLL_API