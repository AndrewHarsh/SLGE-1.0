#include <SLGE.h>

using namespace SLGE;
const int ARRAY_SIZE = 10;

Window Window1(640, 480, "Cool Program", false);

//Must initialize each object with a Window
Object Background(&Window1);

//Arrays cannot be initialized with a window in the constructor
//NPC Array[ARRAY_SIZE];


class Dummy_t : public Entity
{
	class HealthBar_t : public Object
	{
		friend class Dummy_t;
	} HealthBar[2];

	int PerFrameLoop()
	{
		HealthBar[0].SetCoords(X, Y - 40);
		HealthBar[1].SetCoords(X, Y - 40);

		HealthBar[0].Clip[0].w = CurrentHealth / MaxHealth * W * 2;
		HealthBar[0].DisplayClip[0].x = X - W;

		if (CurrentHealth <= 0)
		{
			WindowHandle->RemoveFromScreen(this);
			WindowHandle->RemoveFromScreen(&HealthBar[0]);
			WindowHandle->RemoveFromScreen(&HealthBar[1]);

			SetCoords(-30000, -30000);
		}

		Animate();
		SetImage();
		Display();

		/*
		int Temp = ImageToDisplay;

		ImageToDisplay = NumberOfImages - 1;
			DisplayClip[ImageToDisplay].x = static_cast <int> (round(X - Clip[ImageToDisplay].w / 2.0));
			DisplayClip[ImageToDisplay].y = static_cast <int> (round(Y + H / 2.0 - Clip[ImageToDisplay].h) + AttackDistance);
			DisplayClip[ImageToDisplay].w = Clip[ImageToDisplay].w;
			DisplayClip[ImageToDisplay].h = Clip[ImageToDisplay].h;
		Display();

		ImageToDisplay = NumberOfImages - 2;
			DisplayClip[ImageToDisplay].x = static_cast <int> (round(X - Clip[ImageToDisplay].w / 2.0));
			DisplayClip[ImageToDisplay].y = static_cast <int> (round(Y + H / 2.0 - Clip[ImageToDisplay].h));
			DisplayClip[ImageToDisplay].w = Clip[ImageToDisplay].w;
			DisplayClip[ImageToDisplay].h = Clip[ImageToDisplay].h;
		Display();

		ImageToDisplay = Temp;
		*/

		return 0;
	}

	int SetImage()
	{
		DisplayClip[ImageToDisplay].x = static_cast <int> (round(X - Clip[ImageToDisplay].w / 2.0));
		DisplayClip[ImageToDisplay].y = Y - 20;
		DisplayClip[ImageToDisplay].w = Clip[ImageToDisplay].w;
		DisplayClip[ImageToDisplay].h = Clip[ImageToDisplay].h;

		return 0;
	}

	int Animate()
	{
		if (++ImageToDisplay >= NumberOfImages - 2)
			ImageToDisplay = 0;

		return 0;
	}

public:

	Dummy_t()
	{
		ClearData();
	}

	Dummy_t(Window* Window)
	{
		ClearData();
		Register(Window);
		SetUp();
	}

	void SetUp()
	{
		HealthBar[1].Register(WindowHandle);
		HealthBar[0].Register(WindowHandle);

		for (int i = 0; i < 160; i += 32)
			OpenImage("1.png", { i, 0, 32, 32 }, { 255, 0, 153, 0 });
		for (int i = 0; i < 160; i += 32)
			OpenImage("1.png", { i, 32, 32, 32 }, { 255, 0, 153, 0 });

		SetTraits(100, 100, 3);
		SetAnimation(10);
		SetCoords((rand() % (Window1.GetWidth() - 32)) + 16, (rand() % (Window1.GetHeight() - 32)) + 16, 20, 5);

		//OpenImage("CollisionBox.png", { 0, 0, W, H }, { 255, 0, 153, 0 });
		//OpenImage("AttackBox.png", { 0, 0, W + AttackDistance * 2, H + AttackDistance * 2 }, { 255, 0, 153, 0 });

		HealthBar[0].OpenImage("Health Bar.png", { 0, 0, (int) W * 2, 6 }, { 255, 255, 255, 0 });	
		HealthBar[1].OpenImage("Health Bar.png", { 0, 6, (int) W * 2, 6 }, { 255, 255, 255, 0 });

		HealthBar[0].SetCoords(X, Y, W * 2, 40);
		HealthBar[1].SetCoords(X, Y, W * 2, 40);
	}

} Array[ARRAY_SIZE];



class Player_t : public Entity
{
private:
	
	struct
	{
		Object Hat;
		Object Shirt;
		Object Pants;
		Object Shoes;
		Object Weapon[2];
	} Equipped;

	class HealthBar_t : public Object
	{
		friend class Player_t;
	} HealthBar[2];

public:
	struct Bow_t
	{
		static const int MAX_ARROWS = 100;
		class Arrow_t : public Entity
		{
		public:
			double TargetX;
			double TargetY;
			friend class Player_t;

			int Animate()
			{
				switch (Facing)
				{
					//default:
					//ImageToDisplay = 0;
				}

				return 0;
			}

			int PerFrameLoop()
			{
				MoveTo(TargetX, TargetY);

				Animate();
				SetImage();
				Display();

				if (!Moving)
				{	 
					if (TargetX < Window1.GetWidth() / 2)
						TargetX = -10 * abs(TargetX);
					else
						TargetX = 10 * abs(TargetX);

					if (TargetY < Window1.GetHeight() / 2)
						TargetY = -10 * abs(TargetY);
					else
						TargetY = 10 * abs(TargetY);
				}

				if (!IsOverlapping({WindowHandle->GetWidth() / 2, WindowHandle->GetHeight() / 2, WindowHandle->GetWidth(), WindowHandle->GetHeight()}))
				{
					SetCoords(-30000, -30000);
					TargetX = X;
					TargetY = Y;
				}

				return 0;
			}

		} Arrows[MAX_ARROWS];
		int LastArrowFired;
		bool ReadyToFire;
		double DrawTime;
		double Cooldown;
	} Bow;

private:


	bool MoveUp;
	bool MoveDown;
	bool MoveLeft;
	bool MoveRight;

	bool LeftMouseUp;
	bool RightMouseUp;

	int MouseX;
	int MouseY;


	int ResetLoopVariables()
	{
		LastX = X;
		LastY = Y;

		Moving = false;
		Attacking = false;

		Bow.Cooldown += 1 / WindowHandle->TimerHandle.GetFPS();

		if (Bow.Cooldown >= Bow.DrawTime)
		{
			Bow.Cooldown = 0;
			Bow.ReadyToFire = true;
		}

		return 0;
	}

	int PerFrameLoop()
	{
		HealthBar[0].SetCoords(X, Y - 60);
		HealthBar[1].SetCoords(X, Y - 60);

		HealthBar[0].Clip[0].w = CurrentHealth / MaxHealth * W * 2;
		HealthBar[0].DisplayClip[0].x = X - W;

		if (CurrentHealth <= 0)
		{
			WindowHandle->RemoveFromScreen(this);
			WindowHandle->RemoveFromScreen(&HealthBar[0]);
			WindowHandle->RemoveFromScreen(&HealthBar[1]);
		}

		/*
		int Temp = ImageToDisplay;

		ImageToDisplay = NumberOfImages - 1;
			DisplayClip[ImageToDisplay].x = static_cast <int> (round(X - Clip[ImageToDisplay].w / 2.0));
			DisplayClip[ImageToDisplay].y = static_cast <int> (round(Y + H / 2.0 - Clip[ImageToDisplay].h) + AttackDistance);
			DisplayClip[ImageToDisplay].w = Clip[ImageToDisplay].w;
			DisplayClip[ImageToDisplay].h = Clip[ImageToDisplay].h;
		Display();

		ImageToDisplay = NumberOfImages - 2;
			DisplayClip[ImageToDisplay].x = static_cast <int> (round(X - Clip[ImageToDisplay].w / 2.0));
			DisplayClip[ImageToDisplay].y = static_cast <int> (round(Y + H / 2.0 - Clip[ImageToDisplay].h));
			DisplayClip[ImageToDisplay].w = Clip[ImageToDisplay].w;
			DisplayClip[ImageToDisplay].h = Clip[ImageToDisplay].h;
		Display();

		ImageToDisplay = Temp; 
		*/

		Animate();
		SetImage();
		Display();

		ResetLoopVariables();

		return 0;
	}

	int HandleEvents(SDL_Event *Event)
	{
		/*
		const Uint8 *Keystate = SDL_GetKeyboardState(NULL);

		if (Keystate[SDL_SCANCODE_W])
			Move(Up);
		if (Keystate[SDL_SCANCODE_S])
			Move(Down);
		if (Keystate[SDL_SCANCODE_A])
			Move(Left);
		if (Keystate[SDL_SCANCODE_D])
			Move(Right);

		*/

		if (Event->type == SDL_MOUSEMOTION)
			SDL_GetMouseState(&MouseX, &MouseY);

		if (Event->type == SDL_MOUSEBUTTONDOWN)
		{
			if (Event->button.button == SDL_BUTTON_LEFT)
				LeftMouseUp = false;
			if (Event->button.button == SDL_BUTTON_RIGHT)
				RightMouseUp = false;
		}

		if (Event->type == SDL_MOUSEBUTTONUP)
		{
			if (Event->button.button == SDL_BUTTON_LEFT)
				LeftMouseUp = true;
			if (Event->button.button == SDL_BUTTON_RIGHT)
				RightMouseUp = true;
		}

		//if (Event.type == SDL_Mo)

		if (Event->type == SDL_KEYDOWN)
		{
			if (Event->key.keysym.sym == SDLK_w)
				MoveUp = true;
			if (Event->key.keysym.sym == SDLK_s)
				MoveDown = true;
			if (Event->key.keysym.sym == SDLK_a)
				MoveLeft = true;
			if (Event->key.keysym.sym == SDLK_d)
				MoveRight = true;
		}

		if (Event->type == SDL_KEYUP)
		{
			if (Event->key.keysym.sym == SDLK_w)
				MoveUp = false;
			if (Event->key.keysym.sym == SDLK_s)
				MoveDown = false;
			if (Event->key.keysym.sym == SDLK_a)
				MoveLeft = false;
			if (Event->key.keysym.sym == SDLK_d)
				MoveRight = false;
		}

		return 0;
	}

	int Animate()
	{
		int FaceFrame;
		int TypeFrame;

		switch (Facing)
		{
			case Down:
			case DownRight:
			case DownLeft:
				FaceFrame = 0;
				break;
			case Right:
				FaceFrame = 1;
				break;
			case Left:
				FaceFrame = 2;
				break;
			case Up:
			case UpRight:
			case UpLeft:
				FaceFrame = 3;
				break;	
			default:
				FaceFrame = 0;
				break;
		}

		if (Attacking)
		{
			TypeFrame = 4;
		}
		else if (Moving)
		{
			TypeFrame = 0;
		}
		else
		{
			ImageToDisplay = FaceFrame * 8;

			return 0;
		}

		AnimateFrame += 1000 / WindowHandle->TimerHandle.GetFPS(); //milliseconds per frame

		if (AnimateSpeed && static_cast <int> (AnimateFrame) >= AnimateSpeed)
		{
			AnimateFrame = 0;

			if (++ImageToDisplay >= ((TypeFrame + FaceFrame) * 8) + 8 || ImageToDisplay < (TypeFrame + FaceFrame) * 8)
				ImageToDisplay = ((TypeFrame + FaceFrame) * 8) + 1;
		}

		return 0;
	}

	void SetUp()
	{
		AttackDistance = 10;

		for (int i = 0; i < 20; i++)
		{
			for (int ii = 0; ii < 8; ii++)
				OpenImage("Player.png", { (ii * 100) + 1, (i * 100) + 1, 98, 98 }, { 255, 255, 255, 0 });
		}

		SetCoords(WindowHandle->GetWidth() / 2, WindowHandle->GetHeight() / 2, 20, 5);
		SetTraits(150, 200, 50);
		SetAnimation(100);

		//OpenImage("CollisionBox.png", { 0, 0, W, H }, { 255, 0, 153, 0 });
		//OpenImage("AttackBox.png", { 0, 0, W + AttackDistance * 2, H + AttackDistance * 2 }, { 255, 0, 153, 0 });

		HealthBar[0].OpenImage("Health Bar.png", { 0, 0, (int) W * 2, 6 }, { 255, 255, 255, 0 });	
		HealthBar[1].OpenImage("Health Bar.png", { 0, 6, (int) W * 2, 6 }, { 255, 255, 255, 0 });

		HealthBar[0].SetCoords(X, Y, W * 2, 40);
		HealthBar[1].SetCoords(X, Y, W * 2, 40);

		for (int i = 0; i < Bow.MAX_ARROWS; i++)
		{
			Bow.Arrows[i].OpenImage("Arrow.png", { 0, 0, 32, 7 }, { 255, 255, 255, 0 });
			Bow.Arrows[i].OpenImage("Arrow.png", { 32, 0, 32, 7 }, { 255, 255, 255, 0 });
			Bow.Arrows[i].OpenImage("Arrow.png", { 0, 7, 7, 32 }, { 255, 255, 255, 0 });
			Bow.Arrows[i].OpenImage("Arrow.png", { 7, 7, 7, 32 }, { 255, 255, 255, 0 });

			Bow.Arrows[i].OpenImage("Arrow.png", { 14, 7, 35, 32 }, { 255, 255, 255, 0 });
			Bow.Arrows[i].OpenImage("Arrow.png", { 49, 7, 32, 35 }, { 255, 255, 255, 0 });
			Bow.Arrows[i].OpenImage("Arrow.png", { 81, 7, 35, 32 }, { 255, 255, 255, 0 });
			Bow.Arrows[i].OpenImage("Arrow.png", { 116, 7, 32, 35 }, { 255, 255, 255, 0 });

			Bow.Arrows[i].SetCoords(-30000, -30000, 15, 15);
			Bow.Arrows[i].SetTraits(400, 1000, 100);
		}

		Bow.DrawTime = 0.1;
		Bow.ReadyToFire = true;
	}

	int SetImage()
	{
		DisplayClip[ImageToDisplay].x = static_cast <int> (round(X - Clip[ImageToDisplay].w / 2.0));
		DisplayClip[ImageToDisplay].y = Y - 65;//static_cast <int> (round(Y + H / 2.0 - Clip[ImageToDisplay].h));
		DisplayClip[ImageToDisplay].w = Clip[ImageToDisplay].w;
		DisplayClip[ImageToDisplay].h = Clip[ImageToDisplay].h;

		return 0;
	}

	void ClearData()
	{
		MoveUp = false;
		MoveDown = false;
		MoveLeft = false;
		MoveRight = false;

		LeftMouseUp = true;
		RightMouseUp = true;

		MouseX = 0;
		MouseY = 0;

		Entity::ClearData();
	}

public:
	Player_t(Window* WindowHandle)
	{
		ClearData();
		Register(WindowHandle);
		HealthBar[1].Register(WindowHandle);
		HealthBar[0].Register(WindowHandle);

		for (int i = 0; i < Bow.MAX_ARROWS; i++)
			Bow.Arrows[i].Register(WindowHandle);

		SetUp();
	}

	int Movement()
	{
		if (MoveUp)
		{
			if (MoveLeft)
				Move(UpLeft);
			else if (MoveRight)
				Move(UpRight);
			else if (!MoveDown)
				Move(Up);
		}

		else if (MoveDown)
		{
			if (MoveLeft)
				Move(DownLeft);
			else if (MoveRight)
				Move(DownRight);
			else if (!MoveUp)
				Move(Down);
		}

		else if (MoveLeft && !MoveRight)
			Move(Left);

		else if (MoveRight && !MoveLeft)
			Move(Right);

		return 0;
	}

	int Attack(Entity* Victim)
	{
		if (!LeftMouseUp)
		{
			Attacking = true;

			if (IsWithinAttackRange(Victim) && IsFacing(Victim))
				DealDamage(Victim, AttackDamage);
		}
		else
			Attacking = false;

		return 0;
	}

	int FireArrows()
	{
		if (!RightMouseUp && Bow.ReadyToFire)
		{
			if (++Bow.LastArrowFired >= Bow.MAX_ARROWS)
				Bow.LastArrowFired = 0;

			//Bow.Arrows[Bow.LastArrowFired].Register(WindowHandle);

			Bow.Arrows[Bow.LastArrowFired].X = X;
			Bow.Arrows[Bow.LastArrowFired].Y = Y;

			Bow.Arrows[Bow.LastArrowFired].TargetX = (MouseX - (WindowHandle->GetWidth() / 2)) * 10 + (WindowHandle->GetWidth() / 2);
			Bow.Arrows[Bow.LastArrowFired].TargetY = (MouseY - (WindowHandle->GetHeight() / 2)) * 10 + (WindowHandle->GetHeight() / 2);

			Bow.ReadyToFire = false;
		}

		return 0;
	}

	int MoveArrows(Direction in_Direction)
	{
		for (int i = 0; i < Bow.MAX_ARROWS; i++)
		{
			switch (in_Direction)
			{
				case Left:
					Bow.Arrows[i].SetCoords(Bow.Arrows[i].X + 200.0 / Window1.TimerHandle.GetFPS(), Bow.Arrows[i].Y);
					Bow.Arrows[i].TargetX += 200.0 / Window1.TimerHandle.GetFPS();
					break;
				case Right:
					Bow.Arrows[i].SetCoords(Bow.Arrows[i].X - 200.0 / Window1.TimerHandle.GetFPS(), Bow.Arrows[i].Y);
					Bow.Arrows[i].TargetX -= 200.0 / Window1.TimerHandle.GetFPS();
					break;
				case Down:
					Bow.Arrows[i].SetCoords(Bow.Arrows[i].X, Bow.Arrows[i].Y + 200.0 / Window1.TimerHandle.GetFPS());
					Bow.Arrows[i].TargetY += 200.0 / Window1.TimerHandle.GetFPS();
					break;
				case Up:
					Bow.Arrows[i].SetCoords(Bow.Arrows[i].X, Bow.Arrows[i].Y - 200.0 / Window1.TimerHandle.GetFPS());
					Bow.Arrows[i].TargetY -= 200.0 / Window1.TimerHandle.GetFPS();
					break;
			}
		}

		return 0;
	}

} Player(&Window1);


void SetUp()
{
	srand(unsigned(time(NULL)));

	//Load the images 
	Background.OpenImage("Image.png", { NULL }, { NULL });

	//Set the Coordinates for X, Y, W, H (Width and Height are not the width and height of the image)
	Background.SetCoords(540, 360, 1080, 720);

	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		Array[i].Register(&Window1);
		Array[i].SetUp();
	}

	/*
	for (int ii = 0; ii < ARRAY_SIZE; ii++)
	{
		//Must register arrays / anything that was not registered when it was declared
		Array[ii].Register(&Window1);

		for (int i = 0; i < 160; i += 32)
			Array[ii].OpenImage("1.png", { i, 0, 32, 32 }, { 255, 0, 153, 0 });
		for (int i = 0; i < 160; i += 32)
			Array[ii].OpenImage("1.png", { i, 32, 32, 32 }, { 255, 0, 153, 0 });

		Array[ii].SetTraits(100, 100, 3);
		Array[ii].SetAnimation(10);
		Array[ii].SetCoords((rand() % (Window1.GetWidth() - 32)) + 16, (rand() % (Window1.GetHeight() - 32)) + 16, 20, 5);
	}
	*/
}

void ScrollScreen()
{
	if (!Player.IsWithin({ (Window1.GetWidth() / 2), (Window1.GetHeight() / 2), 200, 200 }))
	{
		if (Player.GetX() > (Window1.GetWidth() / 2) + 100)
		{
			Player.SetCoords((Window1.GetWidth() / 2) + 100, Player.GetY());
			Player.MoveArrows(Right);

			for (int i = 0; i < ARRAY_SIZE; i++)
				Array[i].SetCoords(Array[i].GetX() - 200.0 / Window1.TimerHandle.GetFPS(), Array[i].GetY());
			Background.SetCoords(Background.GetX() - 200.0 / Window1.TimerHandle.GetFPS(), Background.GetY());
		}

		else if (Player.GetX() < (Window1.GetWidth() / 2) - 100)
		{
			Player.SetCoords((Window1.GetWidth() / 2) - 100, Player.GetY());
			Player.MoveArrows(Left);

			for (int i = 0; i < ARRAY_SIZE; i++)
				Array[i].SetCoords(Array[i].GetX() + 200.0 / Window1.TimerHandle.GetFPS(), Array[i].GetY());
			Background.SetCoords(Background.GetX() + 200.0 / Window1.TimerHandle.GetFPS(), Background.GetY());
		}

		if (Player.GetY() > (Window1.GetHeight() / 2) + 100)
		{
			Player.SetCoords(Player.GetX(), (Window1.GetHeight() / 2) + 100);
			Player.MoveArrows(Up);

			for (int i = 0; i < ARRAY_SIZE; i++)
				Array[i].SetCoords(Array[i].GetX(), Array[i].GetY() - 200.0 / Window1.TimerHandle.GetFPS());
			Background.SetCoords(Background.GetX(), Background.GetY() - 200.0 / Window1.TimerHandle.GetFPS());
		}

		else if (Player.GetY() < (Window1.GetHeight() / 2) - 100)
		{
			Player.SetCoords(Player.GetX(), (Window1.GetHeight() / 2) - 100);
			Player.MoveArrows(Down);

			for (int i = 0; i < ARRAY_SIZE; i++)
				Array[i].SetCoords(Array[i].GetX(), Array[i].GetY() + 200.0 / Window1.TimerHandle.GetFPS());
			Background.SetCoords(Background.GetX(), Background.GetY() + 200.0 / Window1.TimerHandle.GetFPS());
		}
	}
}

void GameLoop()
{
	Player.Movement();

	ScrollScreen();

	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		Player.DetectCollisions(&Array[i]);
		
		for (int ii = 0; ii < ARRAY_SIZE; ii++)
		{
			if (i == ii)
				continue;
			Array[ii].DetectCollisions(&Array[i]);
			Array[ii].DetectCollisions(&Player);
		}
	}

	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		Player.Attack(&Array[i]);

		for (int ii = 0; ii < Player.Bow.MAX_ARROWS; ii++)
		{
			if (Player.Bow.Arrows[ii].IsOverlapping(&Array[i]))
			{
				Player.Bow.Arrows[ii].DealDamage(&Array[i], Player.Bow.Arrows[ii].GetAttackDamage());
				//Window1.RemoveFromScreen(&Player.Bow.Arrows[ii]);
				Player.Bow.Arrows[ii].SetCoords(-30000, -30000);
				Player.Bow.Arrows[ii].TargetX = -30000;
				Player.Bow.Arrows[ii].TargetY = -30000;
			}
		}
	}

	Player.FireArrows();
}

extern "C" int SDL_main(int argc, char* argv[])
{
	SetUp();						 

	while (Window1.IsRunning())
		Window1.Run(GameLoop);


	return 0;
}
