#pragma once

#include "Export.hpp"
#include "Logger/Logger.hpp"
#include <string>

namespace BlueBrick {

	class Mod {
	private:
	public:
		Logger* Logger = nullptr;

		struct Info {
			const std::string Name;
			const std::string Version;
			const std::string Author;
		};

		virtual Info GetInfo() = 0;

		virtual void OnInitialized() { }

		template<class Derived>
		static Derived* Get() {
			static Derived* mod = new Derived();
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
	BLUEBRICK_API void* modEntry() { return BlueBrick::Mod::Get<MOD_NAME>(); } \
	class MOD_NAME final : public BlueBrick::Mod
