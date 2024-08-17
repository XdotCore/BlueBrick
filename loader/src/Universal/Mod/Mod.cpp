#include "Mod/Mod.hpp"
#include "Mod/ModLoader.hpp"

namespace BlueBrick {

	Mod::Mod() :
		Logger(std::shared_ptr<BlueBrick::Logger>(new BlueBrick::Logger(this))),
		Dll(ModLoader::GetDll()) {
		ModLoader::AddMod(this);
	}

}
