#include "Mod/ModInfo.hpp"

namespace BlueBrick {

	ModInfo::ModInfo(const std::string& name, const std::string& version, const std::string& author) :
		Name(name),
		NameColor(Color::None()),
		Version(version),
		Author(author),
		AuthorColor(Color::None()) { }

	ModInfo::ModInfo(const std::string& name, const Color& nameColor, const std::string& version, const std::string& author) :
		Name(name),
		NameColor(nameColor),
		Version(version),
		Author(author),
		AuthorColor(Color::None()) { }

	ModInfo::ModInfo(const std::string& name, const std::string& version, const std::string& author, const Color& authorColor) :
		Name(name),
		NameColor(Color::None()),
		Version(version),
		Author(author),
		AuthorColor(authorColor) { }

	ModInfo::ModInfo(const std::string& name, const Color& nameColor, const std::string& version, const std::string& author, const Color& authorColor) :
		Name(name),
		NameColor(nameColor),
		Version(version),
		Author(author),
		AuthorColor(authorColor) { }

}
