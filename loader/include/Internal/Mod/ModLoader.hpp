#pragma once

#include "Mod/Mod.hpp"
#include <vector>

namespace BlueBrick {

	class ModLoader {
	private:
		std::vector<Mod*> loadedMods;

		static ModLoader& instance();
	public:
		static void LoadMods();
		static void AddMod(Mod* mod);
	};

}
