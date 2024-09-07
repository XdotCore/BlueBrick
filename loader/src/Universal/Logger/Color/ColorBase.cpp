#include "Logger/Color/ColorBase.hpp"
#include "Logger/Color/ColorNone.hpp"

namespace BlueBrick {

	const std::string& ColorBase::End() {
		static const std::string& end = ColorNone::None().Start();
		return end;
	}

}
