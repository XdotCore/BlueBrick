#include "Logger/Color/Color256.hpp"

namespace BlueBrick {

	Color256::Color256(byte val) : val(val) { }

	std::string Color256::Start() const {
		return std::format("\x1b[38;5;{}m", val);
	}

}
