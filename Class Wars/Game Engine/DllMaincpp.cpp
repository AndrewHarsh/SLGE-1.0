#include "SLGE.h"

BOOL WINAPI DllMain(HINSTANCE ModuleHandle, DWORD ReasonForCall, LPVOID Reserved)
{
	if (ReasonForCall == DLL_PROCESS_ATTACH) // Self-explanatory
	{
		DisableThreadLibraryCalls(ModuleHandle); // Disable DllMain calls for DLL_THREAD_*

		if (Reserved == NULL) // Dynamic load
		{
			// Return FALSE if you don't want your module to be dynamically loaded
			//SDLInit();
		}
		else // Static load
		{
			// Return FALSE if you don't want your module to be statically loaded
			//return FALSE;
		}
	}

	if (ReasonForCall == DLL_PROCESS_DETACH) // Self-explanatory
	{
		if (Reserved == NULL) // Either loading the DLL has failed or FreeLibrary was called
		{
			// Cleanup
		}
		else // Process is terminating
		{
			// Cleanup
		}
	}
	return TRUE;
}