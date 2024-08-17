#include <windows.h>
#include <filesystem>
#include "Mod/ModLoader.hpp"
#include <format>

using namespace BlueBrick;

BOOL WINAPI DllMain(HINSTANCE dll, DWORD reason, LPVOID _) {
	if (reason != DLL_PROCESS_ATTACH)
		return TRUE;

	// we don't need to handle other calls since BlueBrick's job should be done after the attach
	DisableThreadLibraryCalls(dll);

	MessageBoxA(NULL, "for debugging", "a", MB_OK);
	ModLoader::Start();

	return TRUE;
}
