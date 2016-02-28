#pragma once
#include <SLGE.h>
using namespace SLGE;


//Entity
#include "Entity.h"
	#include "Monster.h"
		#include "Boss.h"
		#include "Inanimate.h"
		#include "Minion.h"
	#include "NPC.h"
	#include "Player.h"
		#include "Archer.h"
		#include "Barbarian.h"
		#include "Fencer.h"
		#include "Mage.h"
		#include "Paladin.h"

//Floor
#include "Floor.h"

//Walls
#include "Walls.h"
	#include "Archway.h"
	#include "Furniture.h"
	#include "Door.h"

//UI
#include "User Interface.h"
	#include "Bar.h"
		#include "Health Bar.h"
		#include "Slider Bar.h"
	#include "Button.h"
		#include "Grey Button.h"
	#include "HUD.h"
	#include "Menu.h"
		#include "Game Options.h"
		#include "In Game.h"
		#include "Main.h"
		#include "Options.h"



//TEMPLATE CLASS
//	Use to create new classes
//	Replace "TEMPLATE_t" with "<Class Name>_t"
//	Replace "Object_t" with "<Inherited Class Name>_t" but ALL classes MUST ultimately inherit from "Object_t"
class TEMPLATE_t : public Object_t
{
protected:

	//Used to clear ALL data from a class (use wisely)
	virtual void ClearData()
	{
		//Todo: Add code to clear all attributes of the class
		Object_t::ClearData();
	}

public:

	//Calls all the constructors that need to be called
	TEMPLATE_t() : Object_t()
	{
		ClearData();
	}

	//Associates the object with a window for later use. MUST be done either in a constructor (below) or by directly calling the "Register()" method
	TEMPLATE_t(Window_t *in_Window) : TEMPLATE_t()
	{
		Register(in_Window);
	}
};








