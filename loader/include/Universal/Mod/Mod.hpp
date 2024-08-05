#pragma once

#include <string>
#include "Export.hpp"
#include "ModEntry.hpp"
#include "Logger/Logger.hpp"

namespace BlueBrick {

	class BLUEBRICK_DLL Mod {
	private:
	public:
		Logger* Logger;

		virtual std::string GetName() = 0;
		virtual std::string GetVersion() = 0;

		virtual void OnInitialized() { }

		template<class Derived>
		static Derived* Get() {
			static Derived* mod = new Derived();
			return mod;
		}
	};

}