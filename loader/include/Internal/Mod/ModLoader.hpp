#pragma once

#include "Mod/Mod.hpp"
#include "Mod/Overlay.hpp"
#include <vector>

namespace BlueBrick {

	class ModLoader final {
	private:
		std::vector<Mod*> loadedMods;
		std::string currentDll;

	public:
		static ModLoader& instance();

		void Start();
		void LoadMods();
		void AddMod(Mod* mod);

		const std::vector<Mod*>& GetLoadedMods();
		const std::string& GetDll();
	};

}
