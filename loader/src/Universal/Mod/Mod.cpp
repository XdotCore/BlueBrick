#include "Mod/Mod.hpp"
#include "Mod/ModLoader.hpp"

namespace BlueBrick {

	Mod::Mod() :
		Logger(this),
		HookManager(this),
		Dll(ModLoader::instance().GetDll()) {
		ModLoader::instance().AddMod(this);
	}

}
