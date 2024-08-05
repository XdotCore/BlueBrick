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

		struct Info {
			const std::string& Name;
			const std::string& Version;
			const std::string& Author;
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