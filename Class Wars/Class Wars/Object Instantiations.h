#pragma once


//
//	Declare your class objects and window (Can be in separate header or dll but must come after classes are defined)
//


Window_t Window1(WIDTH, HEIGHT, "Dungeon!", HARDWARE_ACCELERATED);	 //The window that the program runs in

DynamicClass <Object_t> Background;									 //The background of the game
DynamicClass <Monster_t, Entity_t> DummyTarget;						 //The enemies / monsters
DynamicClass <Player_t, Entity_t> Player;							 //The Player

DynamicClass <Button_t, UI_t> Button;								 //All stand-alone buttons (buttons not nested within a class)
DynamicClass <Menu_t, UI_t> Inventory;								 //The "tab" menu
DynamicClass <HUD_t, UI_t> HUD;										 //The UI along the bottom of the screen
DynamicClass <Bar_t, UI_t> Bar;										 //All stand alone bar (bars not nested within a class)