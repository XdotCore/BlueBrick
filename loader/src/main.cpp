#include <windows.h>
#include <filesystem>
#include "Mod/ModLoader.hpp"
#include "Logger/Logger.hpp"

using namespace BlueBrick;

// The logger instance used for main BlueBrick logs
Logger MainLogger = Logger(nullptr);

static void PrintInfo() {
	MainLogger.Message("Running BlueBrick v{} in {}", BLUEBRICK_VERSION, GAME_NAME);
}

BOOL WINAPI DllMain(HINSTANCE dll, DWORD reason, LPVOID _) {
	switch (reason) {
		case DLL_PROCESS_ATTACH: {
			MessageBoxA(NULL, "for debugging", "a", MB_OK);

			PrintInfo();
			ModLoader::LoadMods();
		} break;
	}

	return TRUE;
}
