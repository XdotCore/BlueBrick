#include "Logger/Color.hpp"

namespace BlueBrick {

	Color::Color(int rgb) : b((byte)rgb), g((byte)(rgb >> 8)), r((byte)(rgb >> 16)) { }

	Color::Color(byte r, byte g, byte b) : r(r), g(g), b(b) { }

	std::string Color::Start() const {
		return std::format("\x1b[38;2;{};{};{}m", (int)this->r, (int)this->g, (int)this->b);
	}

	std::string Color::End() {
		return "\x1b[0m";
	}
}
