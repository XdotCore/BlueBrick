#pragma once

#include "Export.hpp"
#include <format>
#include <string>
#include <sstream>
#include <type_traits>

namespace BlueBrick {

	/// <summary>
	/// An abstract base class for color definitions
	/// </summary>
	class BLUEBRICK_DLL ColorBase {
	public:
		/// <summary>
		/// Tells the console to start the color
		/// </summary>
		/// <returns> The string console code </returns>
		virtual std::string Start() const = 0;
		/// <summary>
		/// Tells the console to reset the color
		/// </summary>
		/// <returns> The string console code </returns>
		static const std::string& End();
	};

}

/// <summary>
/// A default string formatter for colors. Tells the console to start the color
/// </summary>
template<class ColorType> requires std::is_base_of_v<BlueBrick::ColorBase, ColorType>
struct std::formatter<ColorType> {
	template<typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& ctx) {
		return ctx.end();
	}

	template<typename FormatContext>
	FormatContext::iterator format(ColorType color, FormatContext& ctx) const {
		std::ostringstream out;
		out << color.Start();
		return std::ranges::copy(std::move(out).str(), ctx.out()).out;
	}
};
