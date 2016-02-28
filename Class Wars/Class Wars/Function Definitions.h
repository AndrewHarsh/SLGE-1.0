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
	PlayGame
} CurrentScreen;  //Sets which screen to go to when the loop breaks


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
	Background[0].AddImage("Main Menu.png", { 0, 0, 640, 480}, { NULL });
	Background[0].SetCoords(WIDTH / 2, HEIGHT / 2, 0, 0);

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
	//Check if a button was pressed
	if (Button[0].IsPressed())
	{
		CurrentScreen = PlayGame;
		return Exit;
	}
	if (Button[1].IsPressed())
	{
		CurrentScreen = Options;
		return Exit;
	}
	if (Button[2].IsPressed())
	{
		CurrentScreen = MainMenu;
		return Exit;
	}
	if (Button[3].IsPressed())
	{
		CurrentScreen = Quit;
		return Exit;
	}

	//Display all
	DynamicClass <Object_t>::All(&Object_t::Display);
	DynamicClass <UI_t>::All(&UI_t::DisplayAll);

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
//  Game Screen  //
//===============//

//Spawns in and initializes the Game objects
FunctionReturn SpawnGame()
{
	//Spawn
	Background.Spawn(1, &Window1);
	DummyTarget.Spawn(10, &Window1);
	Player.Spawn(1, &Window1);
	Inventory.Spawn(1, &Window1);
	HUD.Spawn(1, &Window1);
	Bar.Spawn(5, &Window1);


	//Setup the background
	Background[0].AddImage("Image.png", { NULL }, { NULL });
	Background[0].SetCoords(WIDTH / 2, HEIGHT / 2);

	//Setup the monsters
	for (int i = 0; i < DummyTarget.NumberOfObjects(); i++)
	{
		DummyTarget[i].AddImage("1.png", { 0, 0, 32, 32 }, { 255, 0, 153, 0 });
		DummyTarget[i].Init(rand() % WIDTH, rand() % HEIGHT, 50);
	}

	//Setup the player
	for (int i = 0; i < 20; i++)	
	{
		for (int ii = 0; ii < 8; ii++)
			Player[0].AddImage("Player.png", { (ii * 100) + 1, (i * 100) + 1, 98, 98 }, { 255, 255, 255, 0 });
	}
	Player[0].Init();
	Player[0].SetCoords(WIDTH / 2, HEIGHT / 2, 0, 0);

	//Setup the "tab" menu
	Inventory[0].AddImage("Players.png", { NULL }, { 0xFF, 0xFF, 0xFF, 0x44 });
	Inventory[0].SetCoords(WIDTH + 212, HEIGHT / 2 - 30);

	//Setup the HUD
	HUD[0].AddImage("HUD2.png", { 0, 0, 640, 0 }, { NULL });
	HUD[0].SetCoords(WIDTH / 2, HEIGHT - 30);
	HUD[0].Selector.SetCoords(WIDTH / 2 - 120, HEIGHT - 30);

	//Setup the player health bar
	Bar[0].AddImage("Health.png", { 0, 50, 200, 20 }, { NULL });
	Bar[0].AddLayer("Health.png", { 0, 100, 120, 20 }, { NULL });
	Bar[0].SetCoords(Bar[0].GetW() / 2 + 10, Bar[0].GetH() / 2 + 10);

	//Setup the player magic bar
	Bar[1].AddImage("Health.png", { 0, 150, 200, 20 }, { NULL });
	Bar[1].AddLayer("Health.png", { 0, 0, 30, 20 }, { NULL });
	Bar[1].SetCoords(Bar[1].GetW() / 2 + 10, Bar[0].GetY() + Bar[0].GetH() + 10);

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
	//Window1.TimerHandle.Benchmark("Run Game");

	//Check events for all
	DynamicClass <Player_t>::All(&Player_t::EventHandler);
	DynamicClass <Menu_t>::All(&Menu_t::HandleEvents);
	DynamicClass <HUD_t>::All(&HUD_t::HandleEvents);
		
	//Move enemies
	SDL_Rect Temp = { WIDTH / 2, HEIGHT / 2, WIDTH, HEIGHT }; //Cant pass an initialization list into a variadic template function
	DynamicClass <Monster_t>::All(&Monster_t::Wander, Temp, NULL);


	//Spawn/despawn enemies 
	if (Window1.GetKeyState(SDL_SCANCODE_Q))
	{
		DummyTarget.Despawn(0, 1);
	}
	if (Window1.GetKeyState(SDL_SCANCODE_E))
	{
		DummyTarget.Spawn(1, &Window1);
		DummyTarget[DummyTarget.NumberOfObjects() - 1].AddImage("1.png", { 0, 0, 32, 32 }, { NULL });
		DummyTarget[DummyTarget.NumberOfObjects() - 1].Init(rand() % Window1.GetWidth(), rand() % Window1.GetHeight(), 50);
	}
	if (Window1.GetKeyState(SDL_SCANCODE_ESCAPE))
	{
		CurrentScreen = MainMenu;
		return Exit;
	}

	
	//Attack
	if (Window1.GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		for (int i = 0; i < DummyTarget.NumberOfObjects(); i++)
		{
			if (Player[0].CanAttack(&DummyTarget[i]))
				Player[0].Attack(&DummyTarget[i], Player[0].MainWeapon);
		}
	}


	//Scroll screen
	int ScrollSpeed = (int) (150 / Window1.TimerHandle.GetFPS()); //Speed at which the screen scrolls
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


	//Window1.TimerHandle.Benchmark("Display"); 

	//Display
	Player[0].Animate();

	DynamicClass <Object_t>::All(&Object_t::Display);
	DynamicClass <Entity_t>::All(&Entity_t::DisplayAll);
	DynamicClass <UI_t>::All(&UI_t::DisplayAll);

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