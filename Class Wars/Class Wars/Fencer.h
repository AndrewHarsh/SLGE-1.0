#pragma once
#include <SLGE.h>
#include "Entity.h"
#include "Player.h"
using namespace SLGE;



class Fencer_t : public Player_t
{
protected:

	//Used to clear ALL data from a class (use wisely)
	void ClearData()
	{
		//Todo: Add code to clear all attributes of the class
		Player_t::ClearData();
	}

public:

	//Calls all the constructors that need to be called
	Fencer_t() : Player_t()
	{
		ClearData();
	}

	//Associates the object with a window for later use. MUST be done either in a constructor (below) or by directly calling the "Register()" method
	Fencer_t(Window_t *in_Window) : Fencer_t()
	{
		Register(in_Window);
	}

	void Init()
	{		
		W = 16;
		H = 38;
		Facing = Down;
		MoveSpeed = 200;


		//Animations
		Moving.SetAll(1, 0.05, 8);
		Attacking.SetAll(33, 0.1, 8);
		Idle.SetDurationAll(0.1);
		Idle.SetTotalFramesAll(1);
		//Idle.Direction[Down].StartPosition = 1;
		//Idle.Direction[Right].StartPosition = 9;
		//Idle.Direction[Left].StartPosition = 17;
		//Idle.Direction[Up].StartPosition = 25;

		Animation->push_back(Moving);
		Animation->push_back(Attacking);
		Animation->push_back(Idle);


		//Images
		AddLayer("Collision Box.png", { 0, 0, (int)W, (int)H }, { 255, 0, 153, 0 });
		for (int i = 0; i < 20; i++)	
		{
			for (int ii = 0; ii < 8; ii++)
				AddImage("Player.png", { (ii * 100) + 1, (i * 100) + 1, 98, 98 }, { 255, 255, 255, 0 });
		}
		AddLayer("Archer Hat.png", { NULL }, { 0xFF, 0xFF, 0xFF, 0 });
		AnimationLayer = 1;

		//Weapon
		MainWeapon.Range = 35;
		MainWeapon.Damage = 200;

		Arc.AddImage("Sword Arc.png", { NULL }, { 0xFF, 0xFF, 0xFF, 0xA0 });


		//Healthbar
		HealthBar.AddImage("Health.png", { 0, 50, 200, 20 }, { NULL });
		HealthBar.AddLayer("Health.png", { 0, 100, 200, 20 }, { NULL });
		HealthBar.SetCoords(HealthBar.GetW() / 2 + 10, HealthBar.GetH() / 2 + 10);
		HealthBar.Init();

		CurrentHealth = 100;
		InitialHealth = 100;
	}
};
