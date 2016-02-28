#pragma once 


//
//  Declare the game init, loop, and despawn code (Can be in separate header or dll but must come after classes are initialized)
//

enum GameScreen_t
{
	MainMenu,
	Options,
	Audio,
	Video,
	Quit,
	PlayGame,
	GameMode
} CurrentScreen;  //Sets which screen to go to when the loop breaks

enum PlayerType_t
{
	Nudist,
	Fencer,
	Barbarian,
	Archer,
	Paladin,
	Mage
} PlayerType;

//===============//
//   Main Menu   //
//===============//

//Spawns in and initializes the Main Menu objects
FunctionReturn SpawnMainMenu()
{
	//Spawn
	Background.Spawn(1, &Window1);
	Button.Spawn(4, &Window1);

	//Setup backgrouns
	Background[0].AddImage("Background.png", { NULL }, { NULL });
	Background[0].AddLayer("Main Menu.png", { 0, 0, 640, 480}, { NULL });
	Background[0].SetCoords(WIDTH / 2, HEIGHT / 2);

	//Setup Buttons
	for (int i = 0; i < Button.NumberOfObjects(); i++)
	{
		Button[i].AddImage("Main Menu Button.png", { 0, 0, 300, 50 }, { 255, 255, 255 });
		Button[i].AddImage("Main Menu Button.png", { 300, 0, 300, 50 }, { 255, 255, 255 });
		Button[i].AddImage("Main Menu Button.png", { 600, 0, 300, 50 }, { 255, 255, 255 });
		Button[i].SetCoords(WIDTH / 2, HEIGHT / 2 - (Button.NumberOfObjects() / 2 * 70) + 70 * i + 70);
	}

	Button[0].Init("Play");
	Button[1].Init("Options");
	Button[3].Init("Exit Game");

	return Continue;
}

//Runs all code that must be executed each frame in the Main Menu
FunctionReturn RunMainMenu()
{
	Background[0].SetCoords(Window1.GetWidth() / 2, Window1.GetHeight() / 2);

	for (int i = 0; i < Button.NumberOfObjects(); i++)
		Button[i].SetCoords(Window1.GetWidth() / 2, Window1.GetHeight() / 2 - (Button.NumberOfObjects() / 2 * 70) + 70 * i + 70);

	//Check if a button was pressed
	if (Button[0].IsPressed())
	{
		CurrentScreen = GameMode;
		return Exit;
	}
	if (Button[1].IsPressed())
	{
		CurrentScreen = Options;
		return Exit;
	}
	if (Button[2].IsPressed())
	{
		PlayerType = Nudist;
		CurrentScreen = PlayGame;
		return Exit;
	}
	if (Button[3].IsPressed())
	{
		CurrentScreen = Quit;
		return Exit;
	}

	//Display all
	All <Object_t> (&Object_t::Display);
	All <UI_t> (&UI_t::DisplayAll);

	return Continue;
}

//Despawns all Main Menu objects that need to be despawned
FunctionReturn DespawnMainMenu()
{
	//Despawn all
	Background.Despawn();
	Button.Despawn();

	return Continue;
}



//===============//
//   Main Menu   //
//===============//

//Spawns in and initializes the Main Menu objects
FunctionReturn SpawnGameMenu()
{
	//Spawn
	Background.Spawn(1, &Window1);
	Button.Spawn(6, &Window1);

	//Setup backgrouns
	Background[0].AddImage("Background.png", { NULL }, { NULL });
	Background[0].SetCoords(WIDTH / 2, HEIGHT / 2);

	//Setup Buttons
	for (int i = 0; i < Button.NumberOfObjects(); i++)
	{
		Button[i].AddImage("Main Menu Button.png", { 0, 0, 300, 50 }, { 255, 255, 255 });
		Button[i].AddImage("Main Menu Button.png", { 300, 0, 300, 50 }, { 255, 255, 255 });
		Button[i].AddImage("Main Menu Button.png", { 600, 0, 300, 50 }, { 255, 255, 255 });
		Button[i].SetCoords(WIDTH / 2, HEIGHT / 2 - (Button.NumberOfObjects() / 2 * 70) + 70 * i + 40);
	}

	Button[0].Init("Fencer");
	Button[1].Init("Barbarian");
	Button[2].Init("Archer");
	Button[3].Init("Paladin");
	Button[4].Init("Mage");
	Button[5].Init("Back");

	return Continue;
}

//Runs all code that must be executed each frame in the Main Menu
FunctionReturn RunGameMenu()
{
	Background[0].SetCoords(Window1.GetWidth() / 2, Window1.GetHeight() / 2);

	for (int i = 0; i < Button.NumberOfObjects(); i++)
		Button[i].SetCoords(Window1.GetWidth() / 2, Window1.GetHeight() / 2 - (Button.NumberOfObjects() / 2 * 70) + 70 * i + 40);


	//Check if a button was pressed
	if (Button[0].IsPressed())
	{
		PlayerType = Fencer;
		CurrentScreen = PlayGame;
		return Exit;
	}
	if (Button[1].IsPressed())
	{
		PlayerType = Barbarian;
		CurrentScreen = PlayGame;
		return Exit;
	}
	if (Button[2].IsPressed())
	{
		PlayerType = Archer;
		CurrentScreen = PlayGame;
		return Exit;
	}
	if (Button[3].IsPressed())
	{
		PlayerType = Paladin;
		CurrentScreen = PlayGame;
		return Exit;
	}
	if (Button[4].IsPressed())
	{
		PlayerType = Mage;
		CurrentScreen = PlayGame;
		return Exit;
	}
	if (Button[5].IsPressed())
	{
		CurrentScreen = MainMenu;
		return Exit;
	}

	//Display all
	All <Object_t> (&Object_t::Display);
	All <UI_t> (&UI_t::DisplayAll);

	return Continue;
}

//Despawns all Main Menu objects that need to be despawned
FunctionReturn DespawnGameMenu()
{
	//Despawn all
	Background.Despawn();
	Button.Despawn();

	return Continue;
}

//===============//
//  Game Screen  //
//===============//

//Spawns in and initializes the Game objects
FunctionReturn SpawnGame()
{
	//Spawn
	Background.Spawn(1, &Window1);
	DummyTarget.Spawn(10, &Window1);

	switch (PlayerType)
	{
		case Fencer:
			Player.Spawn<Fencer_t>(1, &Window1);
			break;
		case Barbarian:
			Player.Spawn<Barbarian_t>(1, &Window1);
			break;
		case Archer:
			Player.Spawn<Archer_t>(1, &Window1);
			break;
		case Paladin:
			Player.Spawn<Paladin_t>(1, &Window1);
			break;
		case Mage:
			Player.Spawn<Mage_t>(1, &Window1);
			break;
		default:
			Player.Spawn(1, &Window1);

			break;
	}

	Inventory.Spawn(1, &Window1);
	HUD.Spawn(1, &Window1);
	Bar.Spawn(5, &Window1);


	//Setup the background
	Background[0].AddImage("Image.png", { NULL }, { NULL });
	Background[0].SetCoords(WIDTH / 2, HEIGHT / 2);
	Background[0].SetID("Background");

	//Setup the monsters
	for (int i = 0; i < DummyTarget.NumberOfObjects(); i++)
	{
		for (int ii = 0; ii < 5; ii++)
			DummyTarget[i].AddImage("1.png", { ii * 32, 0, 32, 32 }, { 255, 0, 153, 0 });
		for (int ii = 0; ii < 5; ii++)
			DummyTarget[i].AddImage("1.png", { ii * 32, 32, 32, 32 }, { 255, 0, 153, 0 });

		DummyTarget[i].Init(rand() % WIDTH, rand() % HEIGHT, 50);
	}

	//Setup the player
	Player[0].Init();
	Player[0].SetCoords(WIDTH / 2, HEIGHT / 2);

	//Setup the "tab" menu
	Inventory[0].AddImage("Players.png", { NULL }, { NULL });
	Inventory[0].Init(1000);
	Inventory[0].SetCoords(WIDTH + 212, HEIGHT / 2 - 30);

	//Setup the HUD
	HUD[0].Init();
	HUD[0].AddImage("HUD2.png", { 0, 0, 640, 0 }, { NULL });
	HUD[0].SetCoords(WIDTH / 2, HEIGHT - 30);
	HUD[0].Selector.SetCoords(WIDTH / 2 - 120, HEIGHT - 30);

	//Setup the player health bar
	//Bar[0].AddImage("Health.png", { 0, 50, 200, 20 }, { NULL });
	//Bar[0].AddLayer("Health.png", { 0, 100, 200, 20 }, { NULL });
	//Bar[0].SetCoords(Bar[0].GetW() / 2 + 10, Bar[0].GetH() / 2 + 10);

	//Setup the player magic bar
	Bar[1].AddImage("Health.png", { 0, 150, 200, 20 }, { NULL });
	Bar[1].AddLayer("Health.png", { 0, 0, 30, 20 }, { NULL });
	Bar[1].SetCoords(Bar[1].GetW() / 2 + 10, 50);//Bar[0].GetY() + Bar[0].GetH() + 10);

	//Setup the player experience bar
	Bar[2].AddImage("Health.png", { 0, 50, 20, 5 }, { NULL });
	Bar[2].AddLayer("Health.png", { 0, 0, 15, 5 }, { NULL });
	Bar[2].SetCoords(WIDTH - 40, HEIGHT - Bar[2].GetH() / 2 - 10);

	//Setup the HUD bomb bar
	Bar[3].AddImage("Health.png", { 0, 50, 20, 5 }, { NULL });
	Bar[3].AddLayer("Health.png", { 0, 0, 10, 5 }, { NULL });
	Bar[3].SetCoords(WIDTH - 120, HEIGHT - Bar[3].GetH() / 2 - 10);

	//Setup the HUD arrow bar
	Bar[4].AddImage("Health.png", { 0, 150, 150, 15 }, { NULL });
	Bar[4].AddLayer("Health.png", { 0, 0, 130, 15 }, { NULL });
	Bar[4].SetCoords(Bar[4].GetW() / 2 + 10, HEIGHT - HUD[0].GetH() - 20);


	return Continue;
}

//Runs all code that must be executed each frame in the Game
FunctionReturn RunGame()
{
	///*

	WIDTH = Window1.GetWidth();
	HEIGHT = Window1.GetHeight();

	//Check events for all
	//Window1.TimerHandle.Benchmark("Check Events");
	All <Player_t> (&Player_t::EventHandler);
	All <Menu_t> (&Menu_t::HandleEvents);
	All <HUD_t> (&HUD_t::HandleEvents);
		
	//Move enemies
	//Window1.TimerHandle.Benchmark("Move Enemies");
	//SDL_Rect Temp = { WIDTH / 2, HEIGHT / 2, WIDTH, HEIGHT }; //Cant pass an initialization list into a variadic template function
	//All <Monster_t> (&Monster_t::Wander, Temp, NULL);
	All<Monster_t>(&Monster_t::MoveTo, Player[0].GetX(), Player[0].GetY());

	if (Player[0].IsDead())
	{
		CurrentScreen = MainMenu;
		return Exit;
	}


	//Spawn/despawn enemies 
	//Window1.TimerHandle.Benchmark("Spawn / Despawn Enemies");
	if (Window1.GetKeyState(SDL_SCANCODE_Q))
	{
		DummyTarget.Despawn(0, 1);
	}
	if (Window1.GetKeyState(SDL_SCANCODE_E))
	{
		DummyTarget.Spawn(1, &Window1);
		
		for (int ii = 0; ii < 5; ii++)
			DummyTarget[DummyTarget.NumberOfObjects() - 1].AddImage("1.png", { ii * 32, 0, 32, 32 }, { 255, 0, 153, 0 });
		for (int ii = 0; ii < 5; ii++)
			DummyTarget[DummyTarget.NumberOfObjects() - 1].AddImage("1.png", { ii * 32, 32, 32, 32 }, { 255, 0, 153, 0 });

		DummyTarget[DummyTarget.NumberOfObjects() - 1].Init(rand() % WIDTH, rand() % HEIGHT, 50);
	}
	if (Window1.GetKeyState(SDL_SCANCODE_ESCAPE))
	{
		CurrentScreen = MainMenu;
		return Exit;
	}

	int TotalEnemies = DummyTarget.NumberOfObjects();
	for (int i = 0; i < DummyTarget.NumberOfObjects(); i++)
	{
		if (DummyTarget[i].IsDead())
		{
			DummyTarget.Despawn(i, 1);
			i--;
			Inventory[0].AddToKilledUnits();
		}
	}
	int EnemiesKilled = TotalEnemies - DummyTarget.NumberOfObjects();

	for (int i = 0; i < EnemiesKilled * 2; i++)
	{
	   DummyTarget.Spawn(1, &Window1);
		
		for (int ii = 0; ii < 5; ii++)
			DummyTarget[DummyTarget.NumberOfObjects() - 1].AddImage("1.png", { ii * 32, 0, 32, 32 }, { 255, 0, 153, 0 });
		for (int ii = 0; ii < 5; ii++)
			DummyTarget[DummyTarget.NumberOfObjects() - 1].AddImage("1.png", { ii * 32, 32, 32, 32 }, { 255, 0, 153, 0 });

		DummyTarget[DummyTarget.NumberOfObjects() - 1].Init(rand() % WIDTH, rand() % HEIGHT, 50);
	}

	
	//Attack
	//Window1.TimerHandle.Benchmark("Attack Enemies");
	if (Window1.GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		for (int i = 0; i < DummyTarget.NumberOfObjects(); i++)
		{
			if (Player[0].CanAttack(&DummyTarget[i]))
				Player[0].Attack(&DummyTarget[i], Player[0].MainWeapon);
		}
	}

	for (int i = 0; i < NumberOfObjects<Monster_t>(); i++)
	{
		if (Get<Monster_t>(i).CanAttack(&Player[0]))
			Get<Monster_t>(i).Attack(&Player[0], Get<Monster_t>(i).MainWeapon);
	}


	//Scroll screen
	//Window1.TimerHandle.Benchmark("Scroll Screen");
	double ScrollSpeed = (double) (100 / Window1.TimerHandle.GetFPS()); //Speed at which the screen scrolls
	int BoxSize = 50; //Size from center to edge of box that player can freely walk in without moving the screen

	if (Player[0].GetX() > WIDTH / 2 + BoxSize)
	{
		Background[0].SetCoords(Background[0].GetX() - ScrollSpeed, Background[0].GetY());
		Player[0].SetCoords(WIDTH / 2 + BoxSize, Player[0].GetY());

		for (int i = 0; i < DummyTarget.NumberOfObjects(); i++)
			DummyTarget[i].SetCoords(DummyTarget[i].GetX() - ScrollSpeed, DummyTarget[i].GetY());
	}

	if (Player[0].GetX() < WIDTH / 2 - BoxSize)
	{
		Background[0].SetCoords(Background[0].GetX() + ScrollSpeed, Background[0].GetY());
		Player[0].SetCoords(WIDTH / 2 - BoxSize, Player[0].GetY());

		for (int i = 0; i < DummyTarget.NumberOfObjects(); i++)
			DummyTarget[i].SetCoords(DummyTarget[i].GetX() + ScrollSpeed, DummyTarget[i].GetY());
	}

	if (Player[0].GetY() > HEIGHT / 2 + BoxSize)
	{
		Background[0].SetCoords(Background[0].GetX(), Background[0].GetY() - ScrollSpeed);
		Player[0].SetCoords(Player[0].GetX(), HEIGHT / 2 + BoxSize);

		for (int i = 0; i < DummyTarget.NumberOfObjects(); i++)
			DummyTarget[i].SetCoords(DummyTarget[i].GetX(), DummyTarget[i].GetY() - ScrollSpeed);
	}

	if (Player[0].GetY() < HEIGHT / 2 - BoxSize)
	{
		Background[0].SetCoords(Background[0].GetX(), Background[0].GetY() + ScrollSpeed);
		Player[0].SetCoords(Player[0].GetX(), HEIGHT / 2 - BoxSize);

		for (int i = 0; i < DummyTarget.NumberOfObjects(); i++)
			DummyTarget[i].SetCoords(DummyTarget[i].GetX(), DummyTarget[i].GetY() + ScrollSpeed);
	}

	
	Bar[2].SetCoords(Window1.GetWidth() / 2 + 200, Window1.GetHeight() - Bar[2].GetH() / 2 - 10);
	Bar[3].SetCoords(Window1.GetWidth() / 2 + 280, Window1.GetHeight() - Bar[3].GetH() / 2 - 10);
	Bar[4].SetCoords((Window1.GetWidth() - HUD[0].GetW() + Bar[4].GetW()) / 2 + 10, Window1.GetHeight() - HUD[0].GetH() - 20);


	

	//Check Collision
	for (int i = 0; i < (int) NumberOfObjects<Entity_t>(); i++)
	{		  
		for (int ii = 0; ii < (int) NumberOfObjects<Entity_t>(); ii++)
			Get<Entity_t>(ii).HandleCollisionWithE(&Get<Entity_t>(i));
	}

	//Window1.TimerHandle.Benchmark("Display All"); 

	//Display
	All <Entity_t> (&Entity_t::Animate);

	//*/


	///*
	All <Object_t> (&Object_t::Display);
	All <Entity_t> (&Entity_t::DisplayAll);
	All <UI_t> (&UI_t::DisplayAll);
	//*/

	//Window1.TimerHandle.Benchmark("Refresh loop");

	return Continue;
}
	 
//Despawns all Game objects that need to be despawned
FunctionReturn DespawnGame()
{
	//std::cout << "Number of Objects on Screen: " << DummyTarget.NumberOfObjects() << std::endl;

	//Despawn all
	Background.Despawn();
	DummyTarget.Despawn();
	Player.Despawn();
	Inventory.Despawn();
	HUD.Despawn();
	Bar.Despawn();

	return Continue;
}