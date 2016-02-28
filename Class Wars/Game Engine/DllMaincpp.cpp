#include "SLGE.h"

using namespace SLGE;

BOOL WINAPI DllMain(HINSTANCE ModuleHandle, DWORD ReasonForCall, LPVOID Reserved)
{
    switch(ReasonForCall) 
    { 
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		{
			std::ofstream File;
			File.open("Splatter Labs Game Engine Version.txt", std::ios::out);

			if (File.is_open())
			{
				File << "Author:" << "\n\n";

				File << "	Andrew Harsh" << "\n";
				File << "	Senior Framework Developer" << "\n";
				File << "	Splatter Labs" << "\n";
				File << "	" << __DATE__ << "\n\n";

				File << "Version:" << "\n\n";

				File << "	" << Version;
			}
			break;
		}

        case DLL_PROCESS_DETACH:
		case DLL_THREAD_DETACH:
			remove("Splatter Labs Game Engine Version.txt");
            break;
    }
    return TRUE;
}