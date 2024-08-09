#include "Mod/ModInfo.hpp"

namespace BlueBrick {

	ModInfo::ModInfo(std::string name, std::string version, std::string author) :
		Name(name),
		NameColor(std::nullopt),
		Version(version),
		Author(author),
		AuthorColor(std::nullopt) { }

	ModInfo::ModInfo(std::string name, const ColorBase& nameColor, std::string version, std::string author) :
		Name(name),
		NameColor(&nameColor),
		Version(version),
		Author(author),
		AuthorColor(std::nullopt) { }

	ModInfo::ModInfo(std::string name, std::string version, std::string author, const ColorBase& authorColor) :
		Name(name),
		NameColor(std::nullopt),
		Version(version),
		Author(author),
		AuthorColor(&authorColor) { }

	ModInfo::ModInfo(std::string name, const ColorBase& nameColor, std::string version, std::string author, const ColorBase& authorColor) :
		Name(name),
		NameColor(&nameColor),
		Version(version),
		Author(author),
		AuthorColor(&authorColor) { }

	static std::string StartOptionalColor(std::optional<const ColorBase*> optCol) {
		// second condition should never happen
		if (!optCol.has_value() || *optCol == nullptr)
			return ColorBase::End();

		return (*optCol)->Start();
	}

	std::string ModInfo::StartNameColor() const {
		return StartOptionalColor(NameColor);
	}

	std::string ModInfo::StartAuthorColor() const {
		return StartOptionalColor(AuthorColor);
	}

}
