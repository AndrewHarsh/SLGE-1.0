#include "SLGE.h"

#define DLL_API __declspec(dllexport) 
using namespace SLGE;


DLL_API UI::UI()
{
	Text = new std::string;
	InputText = new std::string;
	TextImage = nullptr;
	Font = nullptr;

	ClearData();
}

DLL_API UI::UI(Window *in_WindowHandle) : UI()
{
	Register(in_WindowHandle);
}

void DLL_API UI::ClearData()
{
	if (TextImage != nullptr)
	{
		delete[] TextImage;
		TextImage = nullptr;
	}

	if (Font != nullptr)
	{
		TTF_CloseFont(Font);
		Font = nullptr;
	}

	Object::ClearData();

	FontColor = { NULL };
	MouseX = 0;
	MouseY = 0;
	TextX = 0;
	TextY = 0;
	LeftMouseUp = true;
	RightMouseUp = true;
}


int DLL_API UI::EventHandler(SDL_Event *in_Event)
{
	if (in_Event->type == SDL_MOUSEMOTION)
		SDL_GetMouseState(&MouseX, &MouseY);

	if (in_Event->type == SDL_MOUSEBUTTONDOWN)
	{
		if (in_Event->button.button == SDL_BUTTON_LEFT)
			LeftMouseUp = false;

		if (in_Event->button.button == SDL_BUTTON_RIGHT)
			RightMouseUp = false;
	}

	if (in_Event->type == SDL_MOUSEBUTTONUP)
	{
		if (in_Event->button.button == SDL_BUTTON_LEFT)
			LeftMouseUp = true;

		if (in_Event->button.button == SDL_BUTTON_RIGHT)
			RightMouseUp = true;
	}

	if (in_Event->type == SDL_TEXTINPUT)
		*InputText = in_Event->text.text;

	return 0;
}

bool DLL_API UI::HoveringOver()
{
	if (MouseX <= X + W / 2 &&
		MouseX >= X - W / 2 &&
		MouseY <= Y + H / 2 &&
		MouseY >= Y - H / 2)
	{
		SelectImage(1);
		return true;
	}

	SelectImage(0);
	return false;
}

bool DLL_API UI::IsClicked()
{
	if (HoveringOver())
	{
		if (!LeftMouseUp)
		{
			SelectImage(2);
			return true;
		}

		SelectImage(1);
		return false;
	}

	SelectImage(0);
	return false;
}



#undef DLL_API