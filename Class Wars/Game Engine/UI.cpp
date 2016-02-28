#include "SLGE.h"

#define DLL_API __declspec(dllexport) 
using namespace SLGE;


DLL_API UI::UI()
{
	Text = nullptr;
	Font = nullptr;

	ClearData();
}

DLL_API UI::UI(Window *in_WindowHandle)
{
	WindowHandle = in_WindowHandle;

	UI::UI();
}

void DLL_API UI::ClearData()
{
	if (Text != nullptr)
	{
		delete[] Text;
		Text = nullptr;
	}

	if (Font != nullptr)
	{
		TTF_CloseFont(Font);
		Font = nullptr;
	}

	TextX = 0;
	TextY = 0;
	TextLength = 0;
	FontColor = { NULL };

	Object::ClearData();
}

#undef DLL_API