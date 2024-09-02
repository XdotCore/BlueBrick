#pragma once

#include "Mod/Mod.hpp"
#include <vector>

namespace BlueBrick {

	class ModLoader final {
	private:
		std::vector<Mod*> loadedMods;
		std::string currentDll;

	public:
		static inline bool isImGuiSetUp = false;
		static inline bool isImGuiShowing = true;

		static ModLoader& instance();

		static void Start();
		static void LoadMods();
		static void AddMod(Mod* mod);

		static const std::string& GetDll();

		static void AttachImGui();
		static void StartImGui();
		static void StopImGui();
		static void DrawImGui();
	};

}
