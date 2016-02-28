#pragma once
#include <SLGE.h>
#include "User Interface.h"
using namespace SLGE;


//Base class for all Heads Up Displays
class HUD_t : public UI_t
{
protected:

	int SelectorPosition;

	virtual void ClearData()
	{
		SelectorPosition = 0;
		UI_t::ClearData();
	}

public:

	Object_t Selector;

	HUD_t() : UI_t()
	{
		ClearData();
	}

	HUD_t(Window_t *in_Window) : HUD_t()
	{
		Register(in_Window);
	}

	void Init()
	{
		Selector.Register(WindowHandle);
		Selector.AddLayer("HUD2.png", { 640, 0, 81, 0 }, { NULL });
		SelectorPosition = 0;
	}

	int Display()
	{
		int Ret = 0;

		if (Ret = UI_t::Display())
			return Ret;

		if (Ret = Selector.Display())
			return Ret;

		return Ret;
	}

	void HandleEvents()
	{
		int HUD_BOX_SIZE = (int) (W / 8);

		SetCoords(WindowHandle->GetWidth() / 2, WindowHandle->GetHeight() - 30);

		if (WindowHandle->GetKeyState(SDL_SCANCODE_1))
			SelectorPosition = 0;
		if (WindowHandle->GetKeyState(SDL_SCANCODE_2))
			SelectorPosition = 1;
		if (WindowHandle->GetKeyState(SDL_SCANCODE_3))
			SelectorPosition = 2;
		if (WindowHandle->GetKeyState(SDL_SCANCODE_4))
			SelectorPosition = 3;

		if (WindowHandle->Mouse[WHEEL::Main].WasChanged)
		{
			SelectorPosition += WindowHandle->Mouse[WHEEL::Main].AmountScrolled;

			while (SelectorPosition < 0)
				SelectorPosition += 4;
			while (SelectorPosition > 3)
				SelectorPosition -= 4;
		}

		Selector.SetCoords(X - W / 2 + Selector.GetW() / 2 + HUD_BOX_SIZE * SelectorPosition + 160, Y);
	}
};