#pragma once
#include <SLGE.h>
#include "User Interface.h"
using namespace SLGE;


//Base class for all buttons
class Button_t : public UI_t
{
protected:

	bool ButtonPressed;
	std::string Message;
	TTF_Font *Font;

	virtual void ClearData()
	{
		if (Font != nullptr)
		{
			TTF_CloseFont(Font);
			Font = nullptr;
		}

		Message.empty();
		ButtonPressed = false;

		UI_t::ClearData();
	}

public:

	Button_t() : UI_t()
	{
		Font = nullptr;
		ClearData();
	}

	Button_t(Window_t *in_Window) : Button_t()
	{
		Register(in_Window);
	}

	void Init(std::string in_Message)
	{
		Font = TTF_OpenFont("Cheeseburger.ttf", 29);

		if (Font == nullptr)
		{
			std::cout << "Could not load Cheeseburger.ttf. Error: " << TTF_GetError() << std::endl;
			return;
		}

		AddText(in_Message, Font, { 0x01, 0x01, 0x01 });
		(*Image)[Image->size() - 1].SetCoords(X, Y);
		AddLayer(Image->size() - 1);

		TTF_CloseFont(Font);
	}


	//Class specific methods
	bool IsPressed()
	{
		if (HoveringOver())
		{
			if (WindowHandle->GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT))
			{
				ButtonPressed = true;
				(*ImageToDisplay)[0] = 2;
			}
			else if (ButtonPressed)
				 return true;
		}
		else
		{
			(*ImageToDisplay)[0] = 0;
			ButtonPressed = false;
		}

		return false;
	}

	bool HoveringOver()
	{
		int MouseX = 0; 
		int MouseY = 0;

		WindowHandle->GetMouseState(&MouseX, &MouseY);

		if (IsOverlapping({ MouseX, MouseY, 1, 1 }))
		{
			(*ImageToDisplay)[0] = 1;
			return true;
		}
		else
		{
			return false;
		}
	}
};
