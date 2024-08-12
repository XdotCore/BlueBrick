#include "Mod/ModLoader.hpp"
#include "Logger/Logger.hpp"
#include <filesystem>
#include <windows.h>

extern BlueBrick::Logger MainLogger;

namespace BlueBrick {

	ModLoader& ModLoader::instance() {
		static ModLoader instance;
		return instance;
	}

	void ModLoader::LoadMods() {
		const std::filesystem::path modsDir = "BlueBrick/Mods";
		std::filesystem::create_directories(modsDir);

		// first pass: load dlls
		for (const std::filesystem::directory_entry& modFile : std::filesystem::directory_iterator(modsDir)) {
			if (!std::filesystem::is_regular_file(modFile) || !modFile.path().has_extension() || modFile.path().extension().string() != ".dll")
				continue;

			LoadLibraryA(modFile.path().string().c_str());
		}

		// second pass: process mods that added themselves
		for (Mod* mod : instance().loadedMods) {
			ModInfo& info = mod->GetInfo();
			MainLogger.Message("Loaded {1}{0}{5} v{2} by {4}{3}{5}",
			/*0*/	info.Name,
			/*1*/	info.StartNameColor(),
			/*2*/	info.Version,
			/*3*/	info.Author,
			/*4*/	info.StartAuthorColor(),
			/*5*/	ColorBase::End());

			mod->OnInitialized();
		}
	}

	void ModLoader::AddMod(Mod* mod) {
		instance().loadedMods.push_back(mod);
	}

}
