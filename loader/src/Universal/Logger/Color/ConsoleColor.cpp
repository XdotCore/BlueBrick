#include "Logger/Color/ConsoleColor.hpp"

namespace BlueBrick {

	ConsoleColor::ConsoleColor(byte code) : code(code) {}

	std::string ConsoleColor::Start() const {
		return std::format("\x1b[{}m", (int)code);
	}

	const ConsoleColor& ConsoleColor::Black() {
		static const ConsoleColor color(30);
		return color;
	}

	const ConsoleColor& ConsoleColor::Red() {
		static const ConsoleColor color(31);
		return color;
	}

	const ConsoleColor& ConsoleColor::Green() {
		static const ConsoleColor color(32);
		return color;
	}

	const ConsoleColor& ConsoleColor::Yellow() {
		static const ConsoleColor color(33);
		return color;
	}

	const ConsoleColor& ConsoleColor::Blue() {
		static const ConsoleColor color(34);
		return color;
	}

	const ConsoleColor& ConsoleColor::Magenta() {
		static const ConsoleColor color(35);
		return color;
	}

	const ConsoleColor& ConsoleColor::Cyan() {
		static const ConsoleColor color(36);
		return color;
	}

	const ConsoleColor& ConsoleColor::White() {
		static const ConsoleColor color(37);
		return color;
	}

	const ConsoleColor& ConsoleColor::BrightBlack() {
		static const ConsoleColor color(90);
		return color;
	}

	const ConsoleColor& ConsoleColor::BrightRed() {
		static const ConsoleColor color(91);
		return color;
	}

	const ConsoleColor& ConsoleColor::BrickGreen() {
		static const ConsoleColor color(92);
		return color;
	}

	const ConsoleColor& ConsoleColor::BrightYellow() {
		static const ConsoleColor color(93);
		return color;
	}

	const ConsoleColor& ConsoleColor::BrickBlue() {
		static const ConsoleColor color(94);
		return color;
	}

	const ConsoleColor& ConsoleColor::BrightMagenta() {
		static const ConsoleColor color(95);
		return color;
	}

	const ConsoleColor& ConsoleColor::BrightCyan() {
		static const ConsoleColor color(96);
		return color;
	}

	const ConsoleColor& ConsoleColor::BrightWhite() {
		static const ConsoleColor color(97);
		return color;
	}

}
