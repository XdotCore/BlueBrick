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
		std::shared_ptr<BlueBrick::Logger> Logger;

		virtual ModInfo& GetInfo() = 0;

		virtual void OnInitialized() { }

		BLUEBRICK_DLL Mod();
	};

}
