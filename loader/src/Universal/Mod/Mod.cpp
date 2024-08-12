#include "Mod/Mod.hpp"
#include "Mod/ModLoader.hpp"

namespace BlueBrick {

	Mod::Mod() {
		Logger = std::unique_ptr<BlueBrick::Logger>(new BlueBrick::Logger(this));
		ModLoader::AddMod(this);
	}

}
