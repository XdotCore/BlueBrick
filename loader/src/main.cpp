#include <windows.h>
#include <filesystem>
#include "Mod/Mod.hpp"
#include "Logger/Logger.hpp"

using namespace BlueBrick;

// The logger instance used for main BlueBrick logs
Logger MainLogger(nullptr);

void LoadMods() {
	const std::filesystem::path modsDir = "BlueBrick/Mods";
	std::filesystem::create_directories(modsDir);

	for (const std::filesystem::directory_entry& modFile : std::filesystem::directory_iterator(modsDir)) {
		if (!std::filesystem::is_regular_file(modFile) || !modFile.path().has_extension() || modFile.path().extension().string() != ".dll")
			continue;

		HMODULE modHandle = LoadLibraryA(modFile.path().string().c_str());
		if (modHandle == nullptr || modHandle == INVALID_HANDLE_VALUE)
			continue;

		using ModEntry = void* (*)();
		ModEntry modEntry = reinterpret_cast<ModEntry>(GetProcAddress(modHandle, "modEntry"));
		if (modEntry == nullptr)
			continue;

		Mod* mod = reinterpret_cast<Mod*>(modEntry());
		if (mod == nullptr)
			continue;

		Logger* logger = new Logger(mod);
		mod->Logger = logger;

		Mod::Info info = mod->GetInfo();
		MainLogger.Message("Loaded {} v{} by {}", info.Name, info.Version, info.Author);

		mod->OnInitialized();
	}
}

BOOL WINAPI DllMain(HINSTANCE dll, DWORD reason, LPVOID _) {
	switch (reason) {
		case DLL_PROCESS_ATTACH: {
			MessageBoxA(NULL, "for debugging", "a", MB_OK);

			LoadMods();
		} break;
	}

	return TRUE;
}
