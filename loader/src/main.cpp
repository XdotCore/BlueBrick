#include <windows.h>
#include <filesystem>
#include <string>
#include <cstdint>
#include <rcmp.hpp>
#include "Mod/Mod.hpp"
#include "Logger/Logger.hpp"

using namespace BlueBrick;

// The logger instance used for main BlueBrick logs
Logger MainLogger(nullptr);

void AttachHooks() {
	intptr_t base = (intptr_t)GetModuleHandle(NULL);

	rcmp::hook_function<rcmp::cdecl_t<void(uint64_t*, uint64_t, bool, bool)>>(0x7E1070 + base, [](auto original, uint64_t* coinsPtr, uint64_t toAdd, bool multEnabled, bool param_4) {
		MessageBoxA(NULL, "Coin", "Coin", MB_OK);
		original(coinsPtr, toAdd, multEnabled, param_4);
	});

	MainLogger.Message("Hooked AddToCoins 0x{:x}", 0x7E1070 + base);
}

void LoadMods() {
	const std::filesystem::path modsDir = "BlueBrick/Mods";
	std::filesystem::create_directories(modsDir);

	for (const std::filesystem::directory_entry& modFile : std::filesystem::directory_iterator(modsDir)) {
		if (!std::filesystem::is_regular_file(modFile))
			continue;

		MainLogger.Message(modFile.path().string());

		HMODULE modHandle = LoadLibraryA(modFile.path().string().c_str());
		if (modHandle == nullptr || modHandle == INVALID_HANDLE_VALUE)
			continue;

		using ModEntry = void* (*)();
		ModEntry modEntry = reinterpret_cast<ModEntry>(GetProcAddress(modHandle, "modEntry"));
		if (modEntry == nullptr)
			continue;

		Mod* mod = reinterpret_cast<Mod*>(modEntry());
		Logger* logger = new Logger(mod);
		mod->Logger = logger;
		MainLogger.Message("Loaded {} v{}", mod->GetName(), mod->GetVersion());
		mod->OnInitialized();
	}
}

BOOL WINAPI DllMain(HINSTANCE dll, DWORD reason, LPVOID _) {
	switch (reason) {
		case DLL_PROCESS_ATTACH: {
			LoadMods();
		} break;
	}

	return TRUE;
}