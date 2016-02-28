#include "SLGE.h"

#define DLL_API __declspec(dllexport) 
using namespace SLGE;


//Private Methods
DLL_API Object_t::Object_t()
{
	WindowHandle = nullptr;

	Image = new std::vector <Image_t>;
	ImageToDisplay = new std::vector <int>;

	ClearData();
}

DLL_API Object_t::~Object_t()
{
	ClearData();

	WindowHandle = nullptr;
	delete Image;
	delete ImageToDisplay;
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

void DLL_API Object_t::SetDisplay(int in_ImageIndex)
{
	if (in_ImageIndex >= 0 && in_ImageIndex < (int) Image->size())
		(*Image)[in_ImageIndex].SetCoords(X - (*Image)[in_ImageIndex].W() / 2, Y - (*Image)[in_ImageIndex].H() / 2, (*Image)[in_ImageIndex].W(), (*Image)[in_ImageIndex].H());
}

int DLL_API Object_t::Display()
{
	for (int i = 0; i < (int) ImageToDisplay->size(); i++)
	{
		SetDisplay((*ImageToDisplay)[i]);

		if ((*Image)[(*ImageToDisplay)[i]].Display() != 0)
			return 1;
	}

	return 0;
}


//Status Methods
int DLL_API Object_t::GetNumberOfImages()
{
	return Image->size();
}

int DLL_API Object_t::GetLayeredImage(int in_Layer)
{
	if (in_Layer >= 0 && in_Layer < (int) ImageToDisplay->size())
		return (*ImageToDisplay)[in_Layer];
	else
		return -1;
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
	if (in_Window->WindowHandle == nullptr || !in_Window->IsRunning() || (in_Window->HScreen == nullptr && in_Window->Screen == nullptr))
		return 1;

	WindowHandle = in_Window;

	return 0;
}

void DLL_API Object_t::SetCoords(double in_X, double in_Y, double in_W, double in_H)
{
	X = in_X;
	Y = in_Y;

	if (in_W > 0)
		W = in_W;
	if (in_H > 0)
		H = in_H;
}

										 
int DLL_API Object_t::AddImage(std::string in_Filename, SDL_Rect in_Clip, SDL_Color in_ColorKey)
{
	Image->push_back(Image_t(WindowHandle));

	if ((*Image)[Image->size() - 1].OpenImage(in_Filename, in_Clip, in_ColorKey))
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

	if ((*Image)[Image->size() - 1].LoadText(in_Message, in_Font, in_TextColor))
		return 1;

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

	for (int i = 0; i < (int) ImageToDisplay->size(); i++)
	{
		if (in_Position < (*ImageToDisplay)[i])
			(*ImageToDisplay)[i]--;

		if (in_Position == (*ImageToDisplay)[i])
			ImageToDisplay->erase(ImageToDisplay->begin() + i);
	}

	return 0;
}


int DLL_API Object_t::AddLayer(std::string in_Filename, SDL_Rect in_Clip, SDL_Color in_ColorKey)
{
	if (AddImage(in_Filename, in_Clip, in_ColorKey))
		return 1;

	ImageToDisplay->push_back(Image->size() - 1);

	return 0;
}

int DLL_API Object_t::AddLayer(int in_ImagePosition)
{
	if (in_ImagePosition >= 0 && in_ImagePosition < (int) Image->size())
		ImageToDisplay->push_back(in_ImagePosition);
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
		ImageToDisplay->erase(ImageToDisplay->begin() + in_Position);
	else
		return 1;

	return 0;
}

#undef DLL_API