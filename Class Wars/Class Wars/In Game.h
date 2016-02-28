#pragma once
#include <SLGE.h>
#include "User Interface.h"
#include "Menu.h"
using namespace SLGE;


class menuInGame_t : public Menu_t
{
protected:

	bool Open;
	bool Moving;
	double Speed; //in pixels per second
	int UnitsKilled;

	TTF_Font *Font;

	virtual void ClearData()
	{
		if (Font != nullptr)
		{
			TTF_CloseFont(Font);
			Font = nullptr;
		}

		Open = false;
		Moving = false;
		Speed = 0;
	    UnitsKilled;

		Menu_t::ClearData();
	}

public:

	UI_t KillCounter;

	menuInGame_t() : Menu_t()
	{
		Font = nullptr;
		ClearData();
	}

	menuInGame_t(Window_t *in_Window) : menuInGame_t()
	{
		Register(in_Window);
	}

	int Display()
	{
		int Ret = 0;

		if (Ret = UI_t::Display())
			return Ret;
		
		if (Ret = KillCounter.Display())
			return Ret;

		return Ret;
	}

	void Init(double in_Speed)
	{
		Speed = in_Speed;
		(*Image)[0].SetTransparency(0.75);

		Font = TTF_OpenFont("Cheeseburger.ttf", 30);

		KillCounter.Register(WindowHandle);
		KillCounter.AddText("Units Killed: " + std::to_string(UnitsKilled), Font, { 0, 0, 0, 255 });
		KillCounter.SetCoords(X, Y + 100);

		TTF_CloseFont(Font);

		//this->Button.Spawn(1, in_Window);
		//OpenButton.Register(in_Window);
		//OpenButton.AddImage("Arrow button.png", { 0, 0, 30, 64 }, { 0xff, 0xff, 0xff});	
		//OpenButton.AddImage("Arrow button.png", { 0, 0, 30, 64 }, { 0xff, 0xff, 0xff});
		//OpenButton.AddImage("Arrow button.png", { 0, 0, 30, 64 }, { 0xff, 0xff, 0xff});
		//OpenButton.SetCoords(OpenButton.GetW() / 2, WindowHandle->GetHeight() / 2);
	}

	void AddToKilledUnits()
	{
		UnitsKilled++;
	}

	void HandleEvents()
	{
		if (WindowHandle->GetKeyState(SDL_SCANCODE_TAB) && !Moving)
		{
			Open = Open ? false : true;
			Moving = true;	 
		}

		if (Moving)
		{
			if (X > WindowHandle->GetWidth() - W / 2 && Open)
			{
				X -= Speed / WindowHandle->TimerHandle.GetFPS();

				if (X < WindowHandle->GetWidth() - W / 2 )
					X = WindowHandle->GetWidth() - W / 2 ;
			}
			else if (X < WindowHandle->GetWidth() + W / 2 && !Open)
			{
				X += Speed / WindowHandle->TimerHandle.GetFPS();
	
				if (X > WindowHandle->GetWidth() + W / 2 )
					X = WindowHandle->GetWidth() + W / 2 ;
			}
			else
				Moving = false;
		}
		else if (!Open)
			SetCoords(WindowHandle->GetWidth() + W / 2, Y);

		//Font = TTF_OpenFont("Cheeseburger.ttf", 30);

		SDL_Color TextColor = { 0, 0, 0, 0 };
		//KillCounter.GetImageAtIndex(0)->Load("Units Killed: " + std::to_string(UnitsKilled), *Font, TextColor);
		KillCounter.SetCoords(X, Y + 100);

		//TTF_CloseFont(Font);
	}
};


