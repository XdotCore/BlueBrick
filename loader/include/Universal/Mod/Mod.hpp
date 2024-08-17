#pragma once

#include "Export.hpp"
#include "ModInfo.hpp"
#include "Logger/Logger.hpp"
#include "Logger/Color/ColorBase.hpp"
#include <string>
#include <optional>

namespace BlueBrick {

	class Mod {
	public:
		const std::shared_ptr<BlueBrick::Logger> Logger;

		const std::string Dll;

		virtual ModInfo& GetInfo() = 0;

		virtual void OnInitialized() { }

		BLUEBRICK_DLL Mod();
	};

}
