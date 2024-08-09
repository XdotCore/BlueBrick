#pragma once

#include "Export.hpp"
#include "ModInfo.hpp"
#include "Logger/Logger.hpp"
#include "Logger/Color/ColorBase.hpp"
#include <string>
#include <optional>

namespace BlueBrick {

	class Mod {
	private:
	public:
		std::shared_ptr<BlueBrick::Logger> Logger;

		virtual ModInfo& GetInfo() = 0;

		virtual void OnInitialized() { }

		BLUEBRICK_DLL Mod();

		template<class Derived>
		static Derived& Get() {
			static Derived mod = Derived();
			return mod;
		}
	};

}

/**
* Auto-generates the entry function for loading the mod
*
* e.g.
* $ModEntry(TestMod) {
*	...
* }
*
* expands to
*
* class TestMod;
* BLUEBRICK_API void* modEntry() {
*	return BlueBrick::Mod::Get<TestMod>();
* }
* class TestMod final : public BlueBrick::Mod {
*	...
* }
*/

#define $ModEntry(MOD_NAME) class MOD_NAME;\
	BLUEBRICK_API MOD_NAME& modEntry() { return BlueBrick::Mod::Get<MOD_NAME>(); } \
	class MOD_NAME final : public BlueBrick::Mod
