#include <SLGE.h>
using namespace SLGE;

Window_t Window(1080, 720, "Sandbox!", false);


enum Direction_t
{
	Left,
	Right,
	Up,
	Down
};

class Entity_t : public Object_t
{
protected:

	int MoveSpeed;

public:

	//Calls all the constructors that need to be called
	Entity_t() : Object_t()
	{
		ClearData();
	}

	//Associates the object with a window for later use. MUST be done either in a constructor (below) or by directly calling the "Register()" method
	Entity_t(Window_t *in_Window) : Entity_t()
	{
		Register(in_Window);
	}
	
	virtual void Init()
	{
		MoveSpeed = 200;
	}

	void Move(Direction_t Direction)
	{
		switch (Direction)
		{
			case Left:
			X -= MoveSpeed / WindowHandle->TimerHandle.GetFPS();
			break;
			case Right:
			X += MoveSpeed / WindowHandle->TimerHandle.GetFPS();
			break;
			case Up:
			Y -= MoveSpeed / WindowHandle->TimerHandle.GetFPS();
			break;
			case Down:
			Y += MoveSpeed / WindowHandle->TimerHandle.GetFPS();
			break;
		}
	}

};



DynamicClass <Object_t> Square;
DynamicClass <Object_t> Enemy;
DynamicClass <Entity_t, Object_t> Player;

FunctionReturn Spawn()
{
	Square.Spawn(20, &Window);
	Enemy.Spawn(5, &Window);
	Player.Spawn(1, &Window);

	SDL_Rect Size = { 0, 0, 10, 10 };
	SDL_Color Null = { NULL };


	for (int i = 0; i < Square.NumberOfObjects(); i++)
	{
		Square[i].AddImage("Collision Box.png", { 0, 0, rand() % 90 + 10, rand() % 90 + 10 }, { NULL });
		Square[i].SetCoords(rand() % 1080, rand() % 720);
	}

	for (int i = 0; i < Enemy.NumberOfObjects(); i++)
	{
		for (int ii = 0; ii < 8; ii++)
		{
			for (int iii = 0; iii < 16; iii++)
				Enemy[i].AddImage("Sprite\\Red.png", { 1 + 100 * ii, 1 + 100 * iii, 98, 98 }, { 255, 255, 255 });
		}

		Enemy[i].SetCoords(rand() % 1080, rand() % 720);
	}

	Player[0].AddImage("Sprite\\Player.png", { 1, 1, 98, 98 }, { 255, 255, 255 });
	Player[0].SetCoords(0, 0);// 540, 360);
	Player[0].Init();

	//All<Object_t>(&Object_t::AddImage, "Collision Box.png", Size, Null);
	//All<Object_t>(&Object_t::SetCoords, rand() % 1080, rand() % 720);
	//All<Object_t>(&Object_t::SetSize, 10, 10);

	return Continue;
}

FunctionReturn Run()
{
	if (Window.Keyboard[KEY::ESCAPE].IsPressed)
		return Exit;

	double Speed = 100 / Window.TimerHandle.GetFPS();

	if (Window.Keyboard[KEY::D].IsPressed)
		Player[0].Move(Right);
	if (Window.Keyboard[KEY::A].IsPressed)
		Player[0].Move(Left);
	if (Window.Keyboard[KEY::S].IsPressed)
		Player[0].Move(Down);
	if (Window.Keyboard[KEY::W].IsPressed)
		Player[0].Move(Up);

	if (!Player[0].IsWithin({ 540, 360, 1080, 720 }))
	{ 
		if (Player[0].GetX() < 0)
			Player[0].SetCoords(0, Player[0].GetY());
		if (Player[0].GetX() > 1080)
			Player[0].SetCoords(1080, Player[0].GetY());
		if (Player[0].GetY() < 0)
			Player[0].SetCoords(Player[0].GetX(), 0);
		if (Player[0].GetY() > 720)
			Player[0].SetCoords(Player[0].GetX(), 720);
	}

	All <Object_t>(&Object_t::Display);
	return Continue;
}

FunctionReturn Despawn()
{
	Square.Despawn();
	Enemy.Despawn();
	Player.Despawn();

	return Continue;
}



//#undef main

extern "C" int main(int argc, char* argv[])
{
	Window.TimerHandle.GetFPS();

	srand(unsigned(time(NULL)));

	while (Window.IsRunning())
	{
		if (Window.Run(Spawn, Run, Despawn) == Error)
			return 1;
	}

	return 0;
}
