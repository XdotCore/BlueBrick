#include "Mod/Mod.hpp"
#include "Mod/ModLoader.hpp"

namespace BlueBrick {

	Mod::Mod() :
		Logger(this),
		HookManager(this),
		Dll(ModLoader::GetDll()) {
		ModLoader::AddMod(this);
	}

}
