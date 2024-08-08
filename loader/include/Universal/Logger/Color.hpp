#pragma once

#include "Export.hpp"
#include "Types.hpp"
#include <format>
#include <string>
#include <sstream>
#include <iomanip>

namespace BlueBrick {

	class BLUEBRICK_DLL Color {
	public:
		const byte r, g, b;

		Color(int rgb);
		Color(byte r, byte g, byte b);

		std::string Start() const;
		static std::string End();

		// TODO: add default colors
	};

}

template<>
struct std::formatter<BlueBrick::Color, char> {
	bool choseFormat = false;
	bool code = false;
	bool hex = false;
	bool list = false;

	bool choseChannels = false;
	bool r = false;
	bool g = false;
	bool b = false;

	template<typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& ctx) {
		constexpr const char* invalidFormat = "Invalid format args for Color. Only one format can be chosen.";

		auto it = ctx.begin();
		for (; it != ctx.end(); it++) {
			switch (*it) {
				case '#': {
					if (hex || list)
						throw std::format_error(invalidFormat);
					else {
						choseFormat = true;
						code = true;
					}
				} break;
				case 'x':
				case 'X': {
					if (code || list)
						throw std::format_error(invalidFormat);
					else {
						choseFormat = true;
						hex = true;
					}
				} break;
				case 'l':
				case 'L': {
					if (code || hex)
						throw std::format_error(invalidFormat);
					else {
						choseFormat = true;
						list = true;
					}
				} break;

				case 'r':
				case 'R': {
					choseChannels = true;
					r = true;
				} break;
				case 'g':
				case 'G': {
					choseChannels = true;
					g = true;
				} break;
				case 'b':
				case 'B': {
					choseChannels = true;
					b = true;
				} break;

				case '}':
					return it;

				default:
					throw std::format_error(std::format("Invalid format args for Color. Option '{}' is not defined", *it));
			}
		}

		return it;
	}

	template<typename FormatContext>
	FormatContext::iterator format(BlueBrick::Color color, FormatContext& ctx) const {
		std::ostringstream out;

		if (!choseFormat)
			out << color.Start();
		else {
			if (code)
				out << '#' << std::hex;
			else if (hex)
				out << "0x" << std::hex;
			else if (list)
				out << "[ ";

			bool r = this->r, g = this->g, b = this->b;
			if (!choseChannels)
				r = g = b = true;

			if (r) {
				out << (int)color.r;
				if (list && (g || b))
					out << ", ";
			}
			if (g) {
				out << (int)color.g;
				if (list && b)
					out << ", ";
			}
			if (b)
				out << (int)color.b;

			if (list)
				out << " ]";
		}

		return std::ranges::copy(std::move(out).str(), ctx.out()).out;
	}
};
