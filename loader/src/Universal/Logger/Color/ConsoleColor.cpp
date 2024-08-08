#include "Logger/Color/ConsoleColor.hpp"

namespace BlueBrick {

	ConsoleColor::ConsoleColor(byte code) : code(code) {}

	std::string ConsoleColor::Start() const {
		return std::format("\x1b[{}m", (int)code);
	}

	const ConsoleColor& ConsoleColor::Black() {
		static ConsoleColor color(30);
		return color;
	}

	const ConsoleColor& ConsoleColor::Red() {
		static ConsoleColor color(31);
		return color;
	}

	const ConsoleColor& ConsoleColor::Green() {
		static ConsoleColor color(32);
		return color;
	}

	const ConsoleColor& ConsoleColor::Yellow() {
		static ConsoleColor color(33);
		return color;
	}

	const ConsoleColor& ConsoleColor::Blue() {
		static ConsoleColor color(34);
		return color;
	}

	const ConsoleColor& ConsoleColor::Magenta() {
		static ConsoleColor color(35);
		return color;
	}

	const ConsoleColor& ConsoleColor::Cyan() {
		static ConsoleColor color(36);
		return color;
	}

	const ConsoleColor& ConsoleColor::White() {
		static ConsoleColor color(37);
		return color;
	}

	const ConsoleColor& ConsoleColor::BrightBlack() {
		static ConsoleColor color(90);
		return color;
	}

	const ConsoleColor& ConsoleColor::BrightRed() {
		static ConsoleColor color(91);
		return color;
	}

	const ConsoleColor& ConsoleColor::BrickGreen() {
		static ConsoleColor color(92);
		return color;
	}

	const ConsoleColor& ConsoleColor::BrightYellow() {
		static ConsoleColor color(93);
		return color;
	}

	const ConsoleColor& ConsoleColor::BrickBlue() {
		static ConsoleColor color(94);
		return color;
	}

	const ConsoleColor& ConsoleColor::BrightMagenta() {
		static ConsoleColor color(95);
		return color;
	}

	const ConsoleColor& ConsoleColor::BrightCyan() {
		static ConsoleColor color(96);
		return color;
	}

	const ConsoleColor& ConsoleColor::BrightWhite() {
		static ConsoleColor color(97);
		return color;
	}

}
