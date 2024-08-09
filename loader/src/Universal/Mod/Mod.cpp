#include "Mod/Mod.hpp"

namespace BlueBrick {

	Mod::Mod() {
		Logger = std::unique_ptr<BlueBrick::Logger>(new BlueBrick::Logger(this));
	}

}
