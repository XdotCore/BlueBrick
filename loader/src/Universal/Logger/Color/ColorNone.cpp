#include "Logger/Color/ColorNone.hpp"

namespace BlueBrick {

	std::string ColorNone::Start() const {
		return "\x1b[0m";
	}

	const ColorNone& ColorNone::None() {
		static const ColorNone end;
		return end;
	}

}
