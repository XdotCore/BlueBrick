#include "Logger/Color/ColorBase.hpp"

namespace BlueBrick {

	const std::string& ColorBase::End() {
		static std::string end = "\x1b[0m";
		return end;
	}

}
