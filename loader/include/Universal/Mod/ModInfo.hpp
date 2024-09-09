#pragma once

#include "Logger/Color.hpp"
#include <string>
#include <optional>

namespace BlueBrick {

	// TODO: optionally read this from a json file
	struct ModInfo final {
	public:
		const std::string Name;
		const Color NameColor;
		const std::string Version;
		const std::string Author;
		const Color AuthorColor;

		BLUEBRICK_DLL ModInfo(const std::string& name, const std::string& version, const std::string& author);
		BLUEBRICK_DLL ModInfo(const std::string& name, const Color& nameColor, const std::string& version, const std::string& author);
		BLUEBRICK_DLL ModInfo(const std::string& name, const std::string& version, const std::string& author, const Color& authorColor);
		BLUEBRICK_DLL ModInfo(const std::string& name, const Color& nameColor, const std::string& version, const std::string& author, const Color& authorColor);
	};

}
