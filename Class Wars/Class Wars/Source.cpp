#include <SLGE.h>
using namespace SLGE;

#include "Constant Instantiations.h"
#include "Class Definitions.h"
#include "Object Instantiations.h"
#include "Function Definitions.h"


//
//  Main program loop will look simple
//

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

			case PlayGame:
			if (Window1.Run(SpawnGame, RunGame, DespawnGame) == Error)
				return 1;
			break;

			default:
			CurrentScreen = PlayGame;
			break;
		}
	}

	return 0;
}
