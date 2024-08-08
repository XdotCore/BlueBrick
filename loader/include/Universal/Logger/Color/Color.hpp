#pragma once

#include "ColorBase.hpp"
#include "Types.hpp"
#include <iomanip>

namespace BlueBrick {

	/// <summary>
	/// Represents a rgb color
	/// </summary>
	class BLUEBRICK_DLL Color final : public ColorBase {
	public:// <summary>
			/// The combined rgb int 0xRRGGBB
			/// </summary>
		const int rgb;

		/// <summary>
		/// The red component
		/// </summary>
		const byte r;
		/// <summary>
		/// The green component
		/// </summary>
		const byte g;
		/// <summary>
		/// The blue component
		/// </summary>
		const byte b;

		/// <summary>
		/// Assigns the rgb values based on a combined rgb int
		/// </summary>
		/// <param name="rgb"> The combined rgb int 0xRRGGBB </param>
		Color(int rgb);
		/// <summary>
		/// Assigns the rgb values based on the individual components
		/// </summary>
		/// <param name="r"> The red component </param>
		/// <param name="g"> The green component </param>
		/// <param name="b"> The blue component </param>
		Color(byte r, byte g, byte b);

		/// <inheritdoc/>
		std::string Start() const override;

	#pragma region web colors

		/// <summary>
		/// Gets the web color with value #F0F8FF
		/// </summary>
		static const Color& AliceBlue();

		/// <summary>
		/// Gets the web color with value #FAEBD7
		/// </summary>
		static const Color& AntiqueWhite();

		/// <summary>
		/// Gets the web color with value #00FFFF
		/// </summary>
		static const Color& Aqua();

		/// <summary>
		/// Gets the web color with value #7FFFD4
		/// </summary>
		static const Color& Aquamarine();

		/// <summary>
		/// Gets the web color with value #F0FFFF
		/// </summary>
		static const Color& Azure();

		/// <summary>
		/// Gets the web color with value #F5F5DC
		/// </summary>
		static const Color& Beige();

		/// <summary>
		/// Gets the web color with value #FFE4C4
		/// </summary>
		static const Color& Bisque();

		/// <summary>
		/// Gets the web color with value #000000
		/// </summary>
		static const Color& Black();

		/// <summary>
		/// Gets the web color with value #FFEBCD
		/// </summary>
		static const Color& BlanchedAlmond();

		/// <summary>
		/// Gets the web color with value #0000FF
		/// </summary>
		static const Color& Blue();

		/// <summary>
		/// Gets the web color with value #8A2BE2
		/// </summary>
		static const Color& BlueViolet();

		/// <summary>
		/// Gets the web color with value #A52A2A
		/// </summary>
		static const Color& Brown();

		/// <summary>
		/// Gets the web color with value #DEB887
		/// </summary>
		static const Color& BurlyWood();

		/// <summary>
		/// Gets the web color with value #5F9EA0
		/// </summary>
		static const Color& CadetBlue();

		/// <summary>
		/// Gets the web color with value #7FFF00
		/// </summary>
		static const Color& Chartreuse();

		/// <summary>
		/// Gets the web color with value #D2691E
		/// </summary>
		static const Color& Chocolate();

		/// <summary>
		/// Gets the web color with value #FF7F50
		/// </summary>
		static const Color& Coral();

		/// <summary>
		/// Gets the web color with value #6495ED
		/// </summary>
		static const Color& CornflowerBlue();

		/// <summary>
		/// Gets the web color with value #FFF8DC
		/// </summary>
		static const Color& Cornsilk();

		/// <summary>
		/// Gets the web color with value #DC143C
		/// </summary>
		static const Color& Crimson();

		/// <summary>
		/// Gets the web color with value #00FFFF
		/// </summary>
		static const Color& Cyan();

		/// <summary>
		/// Gets the web color with value #00008B
		/// </summary>
		static const Color& DarkBlue();

		/// <summary>
		/// Gets the web color with value #008B8B
		/// </summary>
		static const Color& DarkCyan();

		/// <summary>
		/// Gets the web color with value #B8860B
		/// </summary>
		static const Color& DarkGoldenrod();

		/// <summary>
		/// Gets the web color with value #A9A9A9
		/// </summary>
		static const Color& DarkGray();

		/// <summary>
		/// Gets the web color with value #006400
		/// </summary>
		static const Color& DarkGreen();

		/// <summary>
		/// Gets the web color with value #BDB76B
		/// </summary>
		static const Color& DarkKhaki();

		/// <summary>
		/// Gets the web color with value #8B008B
		/// </summary>
		static const Color& DarkMagenta();

		/// <summary>
		/// Gets the web color with value #556B2F
		/// </summary>
		static const Color& DarkOliveGreen();

		/// <summary>
		/// Gets the web color with value #FF8C00
		/// </summary>
		static const Color& DarkOrange();

		/// <summary>
		/// Gets the web color with value #9932CC
		/// </summary>
		static const Color& DarkOrchid();

		/// <summary>
		/// Gets the web color with value #8B0000
		/// </summary>
		static const Color& DarkRed();

		/// <summary>
		/// Gets the web color with value #E9967A
		/// </summary>
		static const Color& DarkSalmon();

		/// <summary>
		/// Gets the web color with value #8FBC8B
		/// </summary>
		static const Color& DarkSeaGreen();

		/// <summary>
		/// Gets the web color with value #483D8B
		/// </summary>
		static const Color& DarkSlateBlue();

		/// <summary>
		/// Gets the web color with value #2F4F4F
		/// </summary>
		static const Color& DarkSlateGray();

		/// <summary>
		/// Gets the web color with value #00CED1
		/// </summary>
		static const Color& DarkTurquoise();

		/// <summary>
		/// Gets the web color with value #9400D3
		/// </summary>
		static const Color& DarkViolet();

		/// <summary>
		/// Gets the web color with value #FF1493
		/// </summary>
		static const Color& DeepPink();

		/// <summary>
		/// Gets the web color with value #00BFFF
		/// </summary>
		static const Color& DeepSkyBlue();

		/// <summary>
		/// Gets the web color with value #696969
		/// </summary>
		static const Color& DimGray();

		/// <summary>
		/// Gets the web color with value #1E90FF
		/// </summary>
		static const Color& DodgerBlue();

		/// <summary>
		/// Gets the web color with value #B22222
		/// </summary>
		static const Color& Firebrick();

		/// <summary>
		/// Gets the web color with value #FFFAF0
		/// </summary>
		static const Color& FloralWhite();

		/// <summary>
		/// Gets the web color with value #228B22
		/// </summary>
		static const Color& ForestGreen();

		/// <summary>
		/// Gets the web color with value #FF00FF
		/// </summary>
		static const Color& Fuchsia();

		/// <summary>
		/// Gets the web color with value #DCDCDC
		/// </summary>
		static const Color& Gainsboro();

		/// <summary>
		/// Gets the web color with value #F8F8FF
		/// </summary>
		static const Color& GhostWhite();

		/// <summary>
		/// Gets the web color with value #FFD700
		/// </summary>
		static const Color& Gold();

		/// <summary>
		/// Gets the web color with value #DAA520
		/// </summary>
		static const Color& Goldenrod();

		/// <summary>
		/// Gets the web color with value #808080
		/// </summary>
		static const Color& Gray();

		/// <summary>
		/// Gets the web color with value #008000
		/// </summary>
		static const Color& Green();

		/// <summary>
		/// Gets the web color with value #ADFF2F
		/// </summary>
		static const Color& GreenYellow();

		/// <summary>
		/// Gets the web color with value #F0FFF0
		/// </summary>
		static const Color& Honeydew();

		/// <summary>
		/// Gets the web color with value #FF69B4
		/// </summary>
		static const Color& HotPink();

		/// <summary>
		/// Gets the web color with value #CD5C5C
		/// </summary>
		static const Color& IndianRed();

		/// <summary>
		/// Gets the web color with value #4B0082
		/// </summary>
		static const Color& Indigo();

		/// <summary>
		/// Gets the web color with value #FFFFF0
		/// </summary>
		static const Color& Ivory();

		/// <summary>
		/// Gets the web color with value #F0E68C
		/// </summary>
		static const Color& Khaki();

		/// <summary>
		/// Gets the web color with value #E6E6FA
		/// </summary>
		static const Color& Lavender();

		/// <summary>
		/// Gets the web color with value #FFF0F5
		/// </summary>
		static const Color& LavenderBlush();

		/// <summary>
		/// Gets the web color with value #7CFC00
		/// </summary>
		static const Color& LawnGreen();

		/// <summary>
		/// Gets the web color with value #FFFACD
		/// </summary>
		static const Color& LemonChiffon();

		/// <summary>
		/// Gets the web color with value #ADD8E6
		/// </summary>
		static const Color& LightBlue();

		/// <summary>
		/// Gets the web color with value #F08080
		/// </summary>
		static const Color& LightCoral();

		/// <summary>
		/// Gets the web color with value #E0FFFF
		/// </summary>
		static const Color& LightCyan();

		/// <summary>
		/// Gets the web color with value #FAFAD2
		/// </summary>
		static const Color& LightGoldenrodYellow();

		/// <summary>
		/// Gets the web color with value #D3D3D3
		/// </summary>
		static const Color& LightGray();

		/// <summary>
		/// Gets the web color with value #90EE90
		/// </summary>
		static const Color& LightGreen();

		/// <summary>
		/// Gets the web color with value #FFB6C1
		/// </summary>
		static const Color& LightPink();

		/// <summary>
		/// Gets the web color with value #FFA07A
		/// </summary>
		static const Color& LightSalmon();

		/// <summary>
		/// Gets the web color with value #20B2AA
		/// </summary>
		static const Color& LightSeaGreen();

		/// <summary>
		/// Gets the web color with value #87CEFA
		/// </summary>
		static const Color& LightSkyBlue();

		/// <summary>
		/// Gets the web color with value #778899
		/// </summary>
		static const Color& LightSlateGray();

		/// <summary>
		/// Gets the web color with value #B0C4DE
		/// </summary>
		static const Color& LightSteelBlue();

		/// <summary>
		/// Gets the web color with value #FFFFE0
		/// </summary>
		static const Color& LightYellow();

		/// <summary>
		/// Gets the web color with value #00FF00
		/// </summary>
		static const Color& Lime();

		/// <summary>
		/// Gets the web color with value #32CD32
		/// </summary>
		static const Color& LimeGreen();

		/// <summary>
		/// Gets the web color with value #FAF0E6
		/// </summary>
		static const Color& Linen();

		/// <summary>
		/// Gets the web color with value #FF00FF
		/// </summary>
		static const Color& Magenta();

		/// <summary>
		/// Gets the web color with value #800000
		/// </summary>
		static const Color& Maroon();

		/// <summary>
		/// Gets the web color with value #66CDAA
		/// </summary>
		static const Color& MediumAquamarine();

		/// <summary>
		/// Gets the web color with value #0000CD
		/// </summary>
		static const Color& MediumBlue();

		/// <summary>
		/// Gets the web color with value #BA55D3
		/// </summary>
		static const Color& MediumOrchid();

		/// <summary>
		/// Gets the web color with value #9370DB
		/// </summary>
		static const Color& MediumPurple();

		/// <summary>
		/// Gets the web color with value #3CB371
		/// </summary>
		static const Color& MediumSeaGreen();

		/// <summary>
		/// Gets the web color with value #7B68EE
		/// </summary>
		static const Color& MediumSlateBlue();

		/// <summary>
		/// Gets the web color with value #00FA9A
		/// </summary>
		static const Color& MediumSpringGreen();

		/// <summary>
		/// Gets the web color with value #48D1CC
		/// </summary>
		static const Color& MediumTurquoise();

		/// <summary>
		/// Gets the web color with value #C71585
		/// </summary>
		static const Color& MediumVioletRed();

		/// <summary>
		/// Gets the web color with value #191970
		/// </summary>
		static const Color& MidnightBlue();

		/// <summary>
		/// Gets the web color with value #F5FFFA
		/// </summary>
		static const Color& MintCream();

		/// <summary>
		/// Gets the web color with value #FFE4E1
		/// </summary>
		static const Color& MistyRose();

		/// <summary>
		/// Gets the web color with value #FFE4B5
		/// </summary>
		static const Color& Moccasin();

		/// <summary>
		/// Gets the web color with value #FFDEAD
		/// </summary>
		static const Color& NavajoWhite();

		/// <summary>
		/// Gets the web color with value #000080
		/// </summary>
		static const Color& Navy();

		/// <summary>
		/// Gets the web color with value #FDF5E6
		/// </summary>
		static const Color& OldLace();

		/// <summary>
		/// Gets the web color with value #808000
		/// </summary>
		static const Color& Olive();

		/// <summary>
		/// Gets the web color with value #6B8E23
		/// </summary>
		static const Color& OliveDrab();

		/// <summary>
		/// Gets the web color with value #FFA500
		/// </summary>
		static const Color& Orange();

		/// <summary>
		/// Gets the web color with value #FF4500
		/// </summary>
		static const Color& OrangeRed();

		/// <summary>
		/// Gets the web color with value #DA70D6
		/// </summary>
		static const Color& Orchid();

		/// <summary>
		/// Gets the web color with value #EEE8AA
		/// </summary>
		static const Color& PaleGoldenrod();

		/// <summary>
		/// Gets the web color with value #98FB98
		/// </summary>
		static const Color& PaleGreen();

		/// <summary>
		/// Gets the web color with value #AFEEEE
		/// </summary>
		static const Color& PaleTurquoise();

		/// <summary>
		/// Gets the web color with value #DB7093
		/// </summary>
		static const Color& PaleVioletRed();

		/// <summary>
		/// Gets the web color with value #FFEFD5
		/// </summary>
		static const Color& PapayaWhip();

		/// <summary>
		/// Gets the web color with value #FFDAB9
		/// </summary>
		static const Color& PeachPuff();

		/// <summary>
		/// Gets the web color with value #CD853F
		/// </summary>
		static const Color& Peru();

		/// <summary>
		/// Gets the web color with value #FFC0CB
		/// </summary>
		static const Color& Pink();

		/// <summary>
		/// Gets the web color with value #DDA0DD
		/// </summary>
		static const Color& Plum();

		/// <summary>
		/// Gets the web color with value #B0E0E6
		/// </summary>
		static const Color& PowderBlue();

		/// <summary>
		/// Gets the web color with value #800080
		/// </summary>
		static const Color& Purple();

		/// <summary>
		/// Gets the web color with value #663399
		/// </summary>
		static const Color& RebeccaPurple();

		/// <summary>
		/// Gets the web color with value #FF0000
		/// </summary>
		static const Color& Red();

		/// <summary>
		/// Gets the web color with value #BC8F8F
		/// </summary>
		static const Color& RosyBrown();

		/// <summary>
		/// Gets the web color with value #4169E1
		/// </summary>
		static const Color& RoyalBlue();

		/// <summary>
		/// Gets the web color with value #8B4513
		/// </summary>
		static const Color& SaddleBrown();

		/// <summary>
		/// Gets the web color with value #FA8072
		/// </summary>
		static const Color& Salmon();

		/// <summary>
		/// Gets the web color with value #F4A460
		/// </summary>
		static const Color& SandyBrown();

		/// <summary>
		/// Gets the web color with value #2E8B57
		/// </summary>
		static const Color& SeaGreen();

		/// <summary>
		/// Gets the web color with value #FFF5EE
		/// </summary>
		static const Color& SeaShell();

		/// <summary>
		/// Gets the web color with value #A0522D
		/// </summary>
		static const Color& Sienna();

		/// <summary>
		/// Gets the web color with value #C0C0C0
		/// </summary>
		static const Color& Silver();

		/// <summary>
		/// Gets the web color with value #87CEEB
		/// </summary>
		static const Color& SkyBlue();

		/// <summary>
		/// Gets the web color with value #6A5ACD
		/// </summary>
		static const Color& SlateBlue();

		/// <summary>
		/// Gets the web color with value #708090
		/// </summary>
		static const Color& SlateGray();

		/// <summary>
		/// Gets the web color with value #FFFAFA
		/// </summary>
		static const Color& Snow();

		/// <summary>
		/// Gets the web color with value #00FF7F
		/// </summary>
		static const Color& SpringGreen();

		/// <summary>
		/// Gets the web color with value #4682B4
		/// </summary>
		static const Color& SteelBlue();

		/// <summary>
		/// Gets the web color with value #D2B48C
		/// </summary>
		static const Color& Tan();

		/// <summary>
		/// Gets the web color with value #008080
		/// </summary>
		static const Color& Teal();

		/// <summary>
		/// Gets the web color with value #D8BFD8
		/// </summary>
		static const Color& Thistle();

		/// <summary>
		/// Gets the web color with value #FF6347
		/// </summary>
		static const Color& Tomato();

		/// <summary>
		/// Gets the web color with value #40E0D0
		/// </summary>
		static const Color& Turquoise();

		/// <summary>
		/// Gets the web color with value #EE82EE
		/// </summary>
		static const Color& Violet();

		/// <summary>
		/// Gets the web color with value #F5DEB3
		/// </summary>
		static const Color& Wheat();

		/// <summary>
		/// Gets the web color with value #FFFFFF
		/// </summary>
		static const Color& White();

		/// <summary>
		/// Gets the web color with value #F5F5F5
		/// </summary>
		static const Color& WhiteSmoke();

		/// <summary>
		/// Gets the web color with value #FFFF00
		/// </summary>
		static const Color& Yellow();

		/// <summary>
		/// Gets the web color with value #9ACD32
		/// </summary>
		static const Color& YellowGreen();

	#pragma endregion

	};
}

/// <summary>
/// Formats the rgb color
/// <para>
/// Supported formats are x/X:hex = 0xRRGGBB, #:code = #RRGGBB, l/L:list = [ r, g, b ]
/// </para>
/// <para>
/// If a format is chosen, specific channels can be chosen as well with r/g/b, default all three
/// </para>
/// <para>
/// If no format is chosen, the console is told to start the color instead
/// </para>
/// </summary>
template<>
struct std::formatter<BlueBrick::Color> {
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
