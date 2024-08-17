#pragma once

#include "Mod/Mod.hpp"
#include <vector>

namespace BlueBrick {

	class ModLoader {
	private:
		std::vector<Mod*> loadedMods;
		std::string currentDll;

		static ModLoader& instance();
	public:
		static void Start();
		static void LoadMods();
		static void AddMod(Mod* mod);

		static const std::string& GetDll();
	};

}
