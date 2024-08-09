#pragma once

#include "Export.hpp"
#include "Logger/Color/ColorBase.hpp"
#include <string>
#include <optional>

namespace BlueBrick {

	// TODO: optionally read this from a json file
	struct ModInfo {
	public:
		const std::string Name;
		const std::optional<const ColorBase*> NameColor;
		const std::string Version;
		const std::string Author;
		const std::optional<const ColorBase*> AuthorColor;

		BLUEBRICK_DLL ModInfo(std::string name, std::string version, std::string author);
		BLUEBRICK_DLL ModInfo(std::string name, const ColorBase& nameColor, std::string version, std::string author);
		BLUEBRICK_DLL ModInfo(std::string name, std::string version, std::string author, const ColorBase& authorColor);
		BLUEBRICK_DLL ModInfo(std::string name, const ColorBase& nameColor, std::string version, std::string author, const ColorBase& authorColor);

		BLUEBRICK_DLL std::string StartNameColor() const;
		BLUEBRICK_DLL std::string StartAuthorColor() const;
	};

}
