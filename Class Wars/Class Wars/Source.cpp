#include <SLGE.h>	 
#include "Class Definitions.h"
using namespace SLGE;	


//==================//
// GLOBAL VARIABLES //
//==================//

//   Constants
bool HARDWARE_ACCELERATED = true;									//Runs the program in the GPU instead of CPU
int WIDTH = 640;													//Initial width of window
int HEIGHT = 480;													//Initial height of window

//   Enumerations
enum GameScreen_t
{
	MainMenu,
	Options,
	Audio,
	Video,
	Quit,
	PlayGame,
	GameMode,
	MikeGame
} CurrentScreen;							//Sets which screen to go to when the loop breaks
enum PlayerType_t
{
	Nudist,
	Fencer,
	Barbarian,
	Archer,
	Paladin,
	Mage
} PlayerType;

//   Classes
Window_t Window1(WIDTH, HEIGHT, "Dungeon!", HARDWARE_ACCELERATED);	 //The window that the program runs in

std::vector <int> Array;

DynamicClass <Object_t> Background;									 //The background of the game
DynamicClass <Monster_t, Entity_t> DummyTarget;						 //The enemies / monsters
DynamicClass <Player_t, Entity_t> Player;							 //The Player

DynamicClass <menuMain_t, Menu_t, UI_t> menuMain;					 //The menu for the main screen
DynamicClass <menuGameOptions_t, Menu_t, UI_t> menuGameOptions;		 //The menu for the Gamemode screen
DynamicClass <menuOption_t, Menu_t, UI_t> menuOption;				 //The menu for the Options screen
DynamicClass <menuInGame_t, Menu_t, UI_t> menuInGame;				 //The menu for the in game screen

DynamicClass <Button_t, UI_t> Button;								 //All stand-alone buttons (buttons not nested within a class)
DynamicClass <menuInGame_t, UI_t> Inventory;						 //The "tab" menu
DynamicClass <HUD_t, UI_t> HUD;										 //The UI along the bottom of the screen
DynamicClass <Bar_t, UI_t> Bar;										 //All stand alone bar (bars not nested within a class)


//======================//
// FUNCTION DEFINITIONS //
//======================//


//   Mike's Sandbox
FunctionReturn SpawnMikeGame();
FunctionReturn RunMikeGame();
FunctionReturn DespawnMikeGame();


//   Main Menu   
FunctionReturn SpawnMainMenu()
{
	Background.Spawn(1, &Window1);
	menuMain.Spawn(1, &Window1);

	Background[0].AddImage("Background.png", { NULL }, { NULL });
	Background[0].SetCoords(WIDTH / 2, HEIGHT / 2);

	menuMain[0].SetCoords(WIDTH / 2, HEIGHT / 2);
	menuMain[0].Init();

	return Continue;
}

FunctionReturn RunMainMenu()
{
	Background[0].SetCoords(Window1.GetWidth() / 2, Window1.GetHeight() / 2);
	menuMain[0].SetCoords(Window1.GetWidth() / 2, Window1.GetHeight() / 2);

	for (int i = 0; i < 4; i++)
		menuMain[0].Button[i].SetCoords(Window1.GetWidth() / 2, (Window1.GetHeight() / 2) + (70 * i) - 70);

	//if (Window1.GetKeyState(SDL_SCANCODE_ESCAPE))
	//{
	//	Window1.Close();
	//	return Exit;
	//}

	//Check if a button was pressed
	if (menuMain[0].Button[0].IsPressed())
	{
		CurrentScreen = GameMode;
		return Exit;
	}
	if (menuMain[0].Button[1].IsPressed())
	{
		PlayerType = Nudist;
		CurrentScreen = Options;
		return Exit;
	}
	if (menuMain[0].Button[2].IsPressed())
	{
		CurrentScreen = MikeGame;
		return Exit;
	}
	if (menuMain[0].Button[3].IsPressed())
	{
		CurrentScreen = Quit;
		return Exit;
	}

	//Display all
	All <Object_t> (&Object_t::Display);
	All <Entity_t> (&Entity_t::Display);
	All <UI_t> (&UI_t::Display);

	return Continue;
}

FunctionReturn DespawnMainMenu()
{
	//Despawn all
	Background.Despawn();
	menuMain.Despawn();

	return Continue;
}


//   Game Menu   
FunctionReturn SpawnGameMenu()
{
	//Spawn
	Background.Spawn(1, &Window1);
	menuGameOptions.Spawn(1, &Window1);

	//Setup backgrouns
	Background[0].AddImage("Background.png", { NULL }, { NULL });
	Background[0].SetCoords(WIDTH / 2, HEIGHT / 2);

	menuGameOptions[0].SetCoords(WIDTH / 2, HEIGHT / 2);
	menuGameOptions[0].Init();

	return Continue;
}

FunctionReturn RunGameMenu()
{
	Background[0].SetCoords(Window1.GetWidth() / 2, Window1.GetHeight() / 2);

	for (int i = 0; i < 6; i++)
		menuGameOptions[0].Button[i].SetCoords(Window1.GetWidth() / 2, Window1.GetHeight() / 2 + 70 * i - 170);


	//Check if a button was pressed
	if (menuGameOptions[0].Button[0].IsPressed())
	{
		PlayerType = Fencer;
		CurrentScreen = PlayGame;
		return Exit;
	}
	if (menuGameOptions[0].Button[1].IsPressed())
	{
		PlayerType = Barbarian;
		CurrentScreen = PlayGame;
		return Exit;
	}
	if (menuGameOptions[0].Button[2].IsPressed())
	{
		PlayerType = Archer;
		CurrentScreen = PlayGame;
		return Exit;
	}
	if (menuGameOptions[0].Button[3].IsPressed())
	{
		PlayerType = Paladin;
		CurrentScreen = PlayGame;
		return Exit;
	}
	if (menuGameOptions[0].Button[4].IsPressed())
	{
		PlayerType = Mage;
		CurrentScreen = PlayGame;
		return Exit;
	}
	if (menuGameOptions[0].Button[5].IsPressed())
	{
		CurrentScreen = MainMenu;
		return Exit;
	}

	//Display all
	All <Object_t> (&Object_t::Display);
	All <Entity_t> (&Entity_t::Display);
	All <UI_t> (&UI_t::Display);

	return Continue;
}

FunctionReturn DespawnGameMenu()
{
	//Despawn all
	Background.Despawn();
	menuGameOptions.Despawn();

	return Continue;
}


//  Options Screen  
FunctionReturn SpawnOptions()
{
   	//Spawn
	Background.Spawn(1, &Window1);
	menuOption.Spawn(1, &Window1);

	//Setup backgrouns
	Background[0].AddImage("Background.png", { NULL }, { NULL });
	Background[0].SetCoords(WIDTH / 2, HEIGHT / 2);

	menuOption[0].SetCoords(WIDTH / 2, HEIGHT / 2);
	menuOption[0].Init();

	return Continue;
}

FunctionReturn RunOptions()
{
	Background[0].SetCoords(Window1.GetWidth() / 2, Window1.GetHeight() / 2);

	for (int i = 0; i < 4; i++)
		menuOption[0].Button[i].SetCoords(Window1.GetWidth() / 2, Window1.GetHeight() / 2 + 70 * i - 170);


	//Check if a button was pressed
	if (menuOption[0].Button[0].IsPressed())
	{
		CurrentScreen = Options;
		return Exit;
	}
	if (menuOption[0].Button[1].IsPressed())
	{
		CurrentScreen = Options;
		return Exit;
	}
	if (menuOption[0].Button[2].IsPressed())
	{
		CurrentScreen = Options;
		return Exit;
	}
	if (menuOption[0].Button[3].IsPressed())
	{
		CurrentScreen = MainMenu;
		return Exit;
	}

	//Display all
	All <Object_t> (&Object_t::Display);
	All <Entity_t> (&Entity_t::Display);
	All <UI_t> (&UI_t::Display);

	return Continue;
}

FunctionReturn DespawnOptions()
{
   //Despawn all
	Background.Despawn();
	menuOption.Despawn();

	return Continue;
}


//  Game Screen  
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

	Player.Spawn(5, &Window1);

	for (int i = 1; i < 6; i++)
	{
		Player[i].Init();
		Player[i].SetCoords(rand() % WIDTH, rand() % HEIGHT);
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

FunctionReturn RunGame()
{
	WIDTH = Window1.GetWidth();
	HEIGHT = Window1.GetHeight();

	//Check events for all
	//Window1.TimerHandle.Benchmark("Check Events");
	All <Player_t> (&Player_t::HandleEvents);
	for (int i = 0; i < DummyTarget.NumberOfObjects(); i++)
		Player[0].HandleCollisionWithE(&DummyTarget[i]);

	All <menuInGame_t> (&menuInGame_t::HandleEvents);
	All <HUD_t> (&HUD_t::HandleEvents);
		
	//Move enemies
	//Window1.TimerHandle.Benchmark("Move Enemies");
	SDL_Rect Temp = { WIDTH / 2, HEIGHT / 2, WIDTH, HEIGHT }; //Cant pass an initialization list into a variadic template function
	//All <Monster_t> (&Monster_t::Wander, Temp, NULL);
	All<Monster_t>(&Monster_t::MoveTo, (int)Player[0].GetX(), (int)Player[0].GetY());

	for (int i = 0; i < DummyTarget.NumberOfObjects(); i++)
	{
		for (int ii = i; ii < DummyTarget.NumberOfObjects(); ii++)
			DummyTarget[i].HandleCollisionWithE(&DummyTarget[ii]);
	}
	All<Monster_t>(&Monster_t::HandleCollisionWithE, &Player[0]);

	if (Player[0].IsDead())
	{
		CurrentScreen = MainMenu;
		return Exit;
	}
	//*/


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
	//*/

	//Handle killed Monsters
	int TotalEnemies = DummyTarget.NumberOfObjects();
	for (int i = 0; i < DummyTarget.NumberOfObjects(); i++)
	{
		if (DummyTarget[i].IsDead())
		{
			DummyTarget.Despawn(i, 1);
			i--;
			//Inventory[0].AddToKilledUnits();
		}
	}
	//*/

	//HAIL HYDRA
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
	//*/

	
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
	//*/


	//Scroll screen
	//Window1.TimerHandle.Benchmark("Scroll Screen");
	double ScrollSpeed = (double) (Player[0].GetSpeed() / Window1.TimerHandle.GetFPS()); //Speed at which the screen scrolls
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
	//*/

	
	//Make sure the UI is properly positioned
	Bar[2].SetCoords(Window1.GetWidth() / 2 + 200, Window1.GetHeight() - Bar[2].GetH() / 2 - 10);
	Bar[3].SetCoords(Window1.GetWidth() / 2 + 280, Window1.GetHeight() - Bar[3].GetH() / 2 - 10);
	Bar[4].SetCoords((Window1.GetWidth() - HUD[0].GetW() + Bar[4].GetW()) / 2 + 10, Window1.GetHeight() - HUD[0].GetH() - 20);


	
	/*
	//Check Collision
	for (int i = 0; i < (int) NumberOfObjects<Entity_t>(); i++)
	{		  
		for (int ii = 0; ii < (int) NumberOfObjects<Entity_t>(); ii++)
			Get<Entity_t>(ii).HandleCollisionWithE(&Get<Entity_t>(i));
	}
	//*/


	//Display
	//Window1.TimerHandle.Benchmark("Display All"); 
	//All <Entity_t> (&Entity_t::Animate);

	All <Object_t> (&Object_t::Display);
	All <Entity_t> (&Entity_t::Display);
	All <UI_t> (&UI_t::Display);

	return Continue;
}
	 
FunctionReturn DespawnGame()
{
	//std::cout << "Number of Objects on Screen: " << DummyTarget.NumberOfObjects() << std::endl;

	//Despawn all
	Background.Despawn();
	Player.Despawn();
	DummyTarget.Despawn();
	Inventory.Despawn();
	HUD.Despawn();
	Bar.Despawn();

	return Continue;
}



//=============//
// SOURCE MAIN //
//=============//

extern "C" int SDL_main(int argc, char* argv[])
{
	Window1.TimerHandle.GetFPS();

	srand(unsigned(time(NULL)));

	while (CurrentScreen != Quit && Window1.IsRunning())
	{
		switch (CurrentScreen)
		{
			case MainMenu:
			if (Window1.Run(SpawnMainMenu, RunMainMenu, DespawnMainMenu) == Error)
				return 1;
			break;

			case GameMode:
			if (Window1.Run(SpawnGameMenu, RunGameMenu, DespawnGameMenu) == Error)
				return 1;
			break;

			case Options:
			if (Window1.Run(SpawnOptions, RunOptions, DespawnOptions) == Error)
				return 1;
			break;

			case PlayGame:
			if (Window1.Run(SpawnGame, RunGame, DespawnGame) == Error)
				return 1;
			break;

			case MikeGame:
			if (Window1.Run(SpawnMikeGame, RunMikeGame, DespawnMikeGame) == Error)
				return 1;
			break;

			default:
			CurrentScreen = PlayGame;
			break;
		}
	}

	return 0;
}







//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//================================================= Mike's Sandbox =========================================================================================
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


DynamicClass <Floor_t, Object_t> mikeWorld;			//The level
DynamicClass <Player_t, Entity_t> mikePlayer;		//A dynamic array of players
DynamicClass <Monster_t, Entity_t> mikeMonster;		//A dynamic array of monsters

//Spawns in and initializes the Main Menu objects
FunctionReturn SpawnMikeGame()
{				
	//MUST spawn in ANY DynamicClass object in order for it to be used
	//You can specify the amount to spawn, as well as which window they will appear on
	mikeWorld.Spawn(1, &Window1);
	mikePlayer.Spawn(1, &Window1);
	mikeMonster.Spawn(5, &Window1);

	
	

	//Reference the first item in the mikeWorld object and add an image
	//No clipping or colorkeying has been set
	mikeWorld[0].AddImage("Mike Background.png", { NULL }, { NULL });
	
	//Add a player image, cropping out a single frame and removing all white
	mikePlayer[0].AddImage("Player.png", { 1, 1, 98, 98 }, { 255, 255, 255 });
	mikePlayer[0].SetCoords(200, 300);
	mikePlayer[0].Init();

	//mikeMonster.NumberOfObjects() returns all mikeMonster objects that have been spawned
	//This includes mikeMonster objects spawned in other loops
	for (int i = 0; i < mikeMonster.NumberOfObjects(); i++)
	{
		mikeMonster[i].AddImage("1.png", { 0, 0, 32, 32 }, { 255, 0, 153, 0 });
		mikeMonster[i].SetCoords(100 + 100 * i, 100);
	}

	



	return Continue;
}

bool CollectText = false;

//Runs all code that must be executed each frame in the Main Menu
FunctionReturn RunMikeGame()
{
	if (Window1.Keyboard[KEY::RETURN].WasChanged && Window1.Keyboard[KEY::RETURN].IsPressed)
	{
		if (CollectText)
		{
			CollectText = false;
			std::cout << "		Collecting text" << std::endl;
			Window1.Keyboard.StartCollectInputText(true);
		}
		else
		{
			CollectText = true;
			std::cout << "		Done collecting text\n" << std::endl;
			Window1.Keyboard.StartCollectInputText(false);
			std::cout << "		" << Window1.Keyboard.GetInputText(true) << std::endl;
		}
	}

		
	Room_t newroom;

	if (Window1.GetKeyState(SDL_SCANCODE_SLASH))
	{

	}

	if (Window1.GetKeyState(SDL_SCANCODE_Q))
	{
		newroom.Select_Shape(0);
		
	}

	if (Window1.GetKeyState(SDL_SCANCODE_W))
	{
		newroom.Select_Shape(1);

	}

	if (Window1.GetKeyState(SDL_SCANCODE_E))
	{
		newroom.Select_Shape(2);

	}

	if (Window1.GetKeyState(SDL_SCANCODE_R))
	{
		newroom.Select_Shape(3);

	}

	if (Window1.GetKeyState(SDL_SCANCODE_R))
	{
		newroom.Select_Shape(4);

	}

	if (Window1.GetKeyState(SDL_SCANCODE_T))
	{
		newroom.Select_Shape(5);

	}

	if (Window1.GetKeyState(SDL_SCANCODE_Y))
	{
		newroom.Select_Shape(6);

	}

	//Returns to the main menu when ESC is pressed
	if (Window1.GetKeyState(SDL_SCANCODE_ESCAPE))
	{
		CurrentScreen = MainMenu;
		return Exit;
	}

	//Displays to the screen
	All<Object_t>(&Object_t::Display);
	All<Entity_t>(&Entity_t::Display);

	//"All<class>(method)" works by calling "method" on "All" "class" objects. 
	//Methods with parameters can be inputted by following the method name with the arguments:
	//	All<Object_t>(&Object::Move, X, Y);

	return Continue;
}

//Despawns all Main Menu objects that need to be despawned
FunctionReturn DespawnMikeGame()
{
	mikeWorld.Despawn();
	mikePlayer.Despawn();
	mikeMonster.Despawn();

	return Continue;
}

//=====================================================================================================================================================

