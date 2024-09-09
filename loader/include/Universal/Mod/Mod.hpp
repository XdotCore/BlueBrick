#pragma once

#include "ModInfo.hpp"
#include "Logger/Logger.hpp"
#include "Logger/Color/ColorBase.hpp"
#include "Hooking/HookManager.hpp"
#include <string>
#include <optional>

namespace BlueBrick {

	class Mod {
	public:
		BlueBrick::Logger Logger;
		BlueBrick::HookManager HookManager;

		const std::string Dll;

		virtual ModInfo& GetInfo() = 0;

		virtual void OnInitialized() { }
		virtual void OnDraw() { }

		BLUEBRICK_DLL Mod();
	};

}
