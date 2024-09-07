#include "Logger/Color/Color.hpp"

namespace BlueBrick {

	Color::Color(int rgb) : 
		r((byte)(rgb >> 16)),
		g((byte)(rgb >> 8)),
		b((byte)rgb),
		rgb(rgb),
		rf((float)r / 255),
		gf((float)g / 255),
		bf((float)b / 255) { }

	Color::Color(byte r, byte g, byte b) :
		r(r),
		g(g),
		b(b),
		rgb((r << 16) + (g << 8) + b),
		rf((float)r / 255),
		gf((float)g / 255),
		bf((float)b / 255) { }

	std::string Color::Start() const {
		return std::format("\x1b[38;2;{};{};{}m", (int)this->r, (int)this->g, (int)this->b);
	}

#pragma region web colors

	const Color& Color::AliceBlue() {
		static const Color color(0xF0F8FF);
		return color;
	}

	const Color& Color::AntiqueWhite() {
		static const Color color(0xFAEBD7);
		return color;
	}

	const Color& Color::Aqua() {
		static const Color color(0x00FFFF);
		return color;
	}

	const Color& Color::Aquamarine() {
		static const Color color(0x7FFFD4);
		return color;
	}

	const Color& Color::Azure() {
		static const Color color(0xF0FFFF);
		return color;
	}

	const Color& Color::Beige() {
		static const Color color(0xF5F5DC);
		return color;
	}

	const Color& Color::Bisque() {
		static const Color color(0xFFE4C4);
		return color;
	}

	const Color& Color::Black() {
		static const Color color(0x000000);
		return color;
	}

	const Color& Color::BlanchedAlmond() {
		static const Color color(0xFFEBCD);
		return color;
	}

	const Color& Color::Blue() {
		static const Color color(0x0000FF);
		return color;
	}

	const Color& Color::BlueViolet() {
		static const Color color(0x8A2BE2);
		return color;
	}

	const Color& Color::Brown() {
		static const Color color(0xA52A2A);
		return color;
	}

	const Color& Color::BurlyWood() {
		static const Color color(0xDEB887);
		return color;
	}

	const Color& Color::CadetBlue() {
		static const Color color(0x5F9EA0);
		return color;
	}

	const Color& Color::Chartreuse() {
		static const Color color(0x7FFF00);
		return color;
	}

	const Color& Color::Chocolate() {
		static const Color color(0xD2691E);
		return color;
	}

	const Color& Color::Coral() {
		static const Color color(0xFF7F50);
		return color;
	}

	const Color& Color::CornflowerBlue() {
		static const Color color(0x6495ED);
		return color;
	}

	const Color& Color::Cornsilk() {
		static const Color color(0xFFF8DC);
		return color;
	}

	const Color& Color::Crimson() {
		static const Color color(0xDC143C);
		return color;
	}

	const Color& Color::Cyan() {
		static const Color color(0x00FFFF);
		return color;
	}

	const Color& Color::DarkBlue() {
		static const Color color(0x00008B);
		return color;
	}

	const Color& Color::DarkCyan() {
		static const Color color(0x008B8B);
		return color;
	}

	const Color& Color::DarkGoldenrod() {
		static const Color color(0xB8860B);
		return color;
	}

	const Color& Color::DarkGray() {
		static const Color color(0xA9A9A9);
		return color;
	}

	const Color& Color::DarkGreen() {
		static const Color color(0x006400);
		return color;
	}

	const Color& Color::DarkKhaki() {
		static const Color color(0xBDB76B);
		return color;
	}

	const Color& Color::DarkMagenta() {
		static const Color color(0x8B008B);
		return color;
	}

	const Color& Color::DarkOliveGreen() {
		static const Color color(0x556B2F);
		return color;
	}

	const Color& Color::DarkOrange() {
		static const Color color(0xFF8C00);
		return color;
	}

	const Color& Color::DarkOrchid() {
		static const Color color(0x9932CC);
		return color;
	}

	const Color& Color::DarkRed() {
		static const Color color(0x8B0000);
		return color;
	}

	const Color& Color::DarkSalmon() {
		static const Color color(0xE9967A);
		return color;
	}

	const Color& Color::DarkSeaGreen() {
		static const Color color(0x8FBC8B);
		return color;
	}

	const Color& Color::DarkSlateBlue() {
		static const Color color(0x483D8B);
		return color;
	}

	const Color& Color::DarkSlateGray() {
		static const Color color(0x2F4F4F);
		return color;
	}

	const Color& Color::DarkTurquoise() {
		static const Color color(0x00CED1);
		return color;
	}

	const Color& Color::DarkViolet() {
		static const Color color(0x9400D3);
		return color;
	}

	const Color& Color::DeepPink() {
		static const Color color(0xFF1493);
		return color;
	}

	const Color& Color::DeepSkyBlue() {
		static const Color color(0x00BFFF);
		return color;
	}

	const Color& Color::DimGray() {
		static const Color color(0x696969);
		return color;
	}

	const Color& Color::DodgerBlue() {
		static const Color color(0x1E90FF);
		return color;
	}

	const Color& Color::Firebrick() {
		static const Color color(0xB22222);
		return color;
	}

	const Color& Color::FloralWhite() {
		static const Color color(0xFFFAF0);
		return color;
	}

	const Color& Color::ForestGreen() {
		static const Color color(0x228B22);
		return color;
	}

	const Color& Color::Fuchsia() {
		static const Color color(0xFF00FF);
		return color;
	}

	const Color& Color::Gainsboro() {
		static const Color color(0xDCDCDC);
		return color;
	}

	const Color& Color::GhostWhite() {
		static const Color color(0xF8F8FF);
		return color;
	}

	const Color& Color::Gold() {
		static const Color color(0xFFD700);
		return color;
	}

	const Color& Color::Goldenrod() {
		static const Color color(0xDAA520);
		return color;
	}

	const Color& Color::Gray() {
		static const Color color(0x808080);
		return color;
	}

	const Color& Color::Green() {
		static const Color color(0x008000);
		return color;
	}

	const Color& Color::GreenYellow() {
		static const Color color(0xADFF2F);
		return color;
	}

	const Color& Color::Honeydew() {
		static const Color color(0xF0FFF0);
		return color;
	}

	const Color& Color::HotPink() {
		static const Color color(0xFF69B4);
		return color;
	}

	const Color& Color::IndianRed() {
		static const Color color(0xCD5C5C);
		return color;
	}

	const Color& Color::Indigo() {
		static const Color color(0x4B0082);
		return color;
	}

	const Color& Color::Ivory() {
		static const Color color(0xFFFFF0);
		return color;
	}

	const Color& Color::Khaki() {
		static const Color color(0xF0E68C);
		return color;
	}

	const Color& Color::Lavender() {
		static const Color color(0xE6E6FA);
		return color;
	}

	const Color& Color::LavenderBlush() {
		static const Color color(0xFFF0F5);
		return color;
	}

	const Color& Color::LawnGreen() {
		static const Color color(0x7CFC00);
		return color;
	}

	const Color& Color::LemonChiffon() {
		static const Color color(0xFFFACD);
		return color;
	}

	const Color& Color::LightBlue() {
		static const Color color(0xADD8E6);
		return color;
	}

	const Color& Color::LightCoral() {
		static const Color color(0xF08080);
		return color;
	}

	const Color& Color::LightCyan() {
		static const Color color(0xE0FFFF);
		return color;
	}

	const Color& Color::LightGoldenrodYellow() {
		static const Color color(0xFAFAD2);
		return color;
	}

	const Color& Color::LightGray() {
		static const Color color(0xD3D3D3);
		return color;
	}

	const Color& Color::LightGreen() {
		static const Color color(0x90EE90);
		return color;
	}

	const Color& Color::LightPink() {
		static const Color color(0xFFB6C1);
		return color;
	}

	const Color& Color::LightSalmon() {
		static const Color color(0xFFA07A);
		return color;
	}

	const Color& Color::LightSeaGreen() {
		static const Color color(0x20B2AA);
		return color;
	}

	const Color& Color::LightSkyBlue() {
		static const Color color(0x87CEFA);
		return color;
	}

	const Color& Color::LightSlateGray() {
		static const Color color(0x778899);
		return color;
	}

	const Color& Color::LightSteelBlue() {
		static const Color color(0xB0C4DE);
		return color;
	}

	const Color& Color::LightYellow() {
		static const Color color(0xFFFFE0);
		return color;
	}

	const Color& Color::Lime() {
		static const Color color(0x00FF00);
		return color;
	}

	const Color& Color::LimeGreen() {
		static const Color color(0x32CD32);
		return color;
	}

	const Color& Color::Linen() {
		static const Color color(0xFAF0E6);
		return color;
	}

	const Color& Color::Magenta() {
		static const Color color(0xFF00FF);
		return color;
	}

	const Color& Color::Maroon() {
		static const Color color(0x800000);
		return color;
	}

	const Color& Color::MediumAquamarine() {
		static const Color color(0x66CDAA);
		return color;
	}

	const Color& Color::MediumBlue() {
		static const Color color(0x0000CD);
		return color;
	}

	const Color& Color::MediumOrchid() {
		static const Color color(0xBA55D3);
		return color;
	}

	const Color& Color::MediumPurple() {
		static const Color color(0x9370DB);
		return color;
	}

	const Color& Color::MediumSeaGreen() {
		static const Color color(0x3CB371);
		return color;
	}

	const Color& Color::MediumSlateBlue() {
		static const Color color(0x7B68EE);
		return color;
	}

	const Color& Color::MediumSpringGreen() {
		static const Color color(0x00FA9A);
		return color;
	}

	const Color& Color::MediumTurquoise() {
		static const Color color(0x48D1CC);
		return color;
	}

	const Color& Color::MediumVioletRed() {
		static const Color color(0xC71585);
		return color;
	}

	const Color& Color::MidnightBlue() {
		static const Color color(0x191970);
		return color;
	}

	const Color& Color::MintCream() {
		static const Color color(0xF5FFFA);
		return color;
	}

	const Color& Color::MistyRose() {
		static const Color color(0xFFE4E1);
		return color;
	}

	const Color& Color::Moccasin() {
		static const Color color(0xFFE4B5);
		return color;
	}

	const Color& Color::NavajoWhite() {
		static const Color color(0xFFDEAD);
		return color;
	}

	const Color& Color::Navy() {
		static const Color color(0x000080);
		return color;
	}

	const Color& Color::OldLace() {
		static const Color color(0xFDF5E6);
		return color;
	}

	const Color& Color::Olive() {
		static const Color color(0x808000);
		return color;
	}

	const Color& Color::OliveDrab() {
		static const Color color(0x6B8E23);
		return color;
	}

	const Color& Color::Orange() {
		static const Color color(0xFFA500);
		return color;
	}

	const Color& Color::OrangeRed() {
		static const Color color(0xFF4500);
		return color;
	}

	const Color& Color::Orchid() {
		static const Color color(0xDA70D6);
		return color;
	}

	const Color& Color::PaleGoldenrod() {
		static const Color color(0xEEE8AA);
		return color;
	}

	const Color& Color::PaleGreen() {
		static const Color color(0x98FB98);
		return color;
	}

	const Color& Color::PaleTurquoise() {
		static const Color color(0xAFEEEE);
		return color;
	}

	const Color& Color::PaleVioletRed() {
		static const Color color(0xDB7093);
		return color;
	}

	const Color& Color::PapayaWhip() {
		static const Color color(0xFFEFD5);
		return color;
	}

	const Color& Color::PeachPuff() {
		static const Color color(0xFFDAB9);
		return color;
	}

	const Color& Color::Peru() {
		static const Color color(0xCD853F);
		return color;
	}

	const Color& Color::Pink() {
		static const Color color(0xFFC0CB);
		return color;
	}

	const Color& Color::Plum() {
		static const Color color(0xDDA0DD);
		return color;
	}

	const Color& Color::PowderBlue() {
		static const Color color(0xB0E0E6);
		return color;
	}

	const Color& Color::Purple() {
		static const Color color(0x800080);
		return color;
	}

	const Color& Color::RebeccaPurple() {
		static const Color color(0x663399);
		return color;
	}

	const Color& Color::Red() {
		static const Color color(0xFF0000);
		return color;
	}

	const Color& Color::RosyBrown() {
		static const Color color(0xBC8F8F);
		return color;
	}

	const Color& Color::RoyalBlue() {
		static const Color color(0x4169E1);
		return color;
	}

	const Color& Color::SaddleBrown() {
		static const Color color(0x8B4513);
		return color;
	}

	const Color& Color::Salmon() {
		static const Color color(0xFA8072);
		return color;
	}

	const Color& Color::SandyBrown() {
		static const Color color(0xF4A460);
		return color;
	}

	const Color& Color::SeaGreen() {
		static const Color color(0x2E8B57);
		return color;
	}

	const Color& Color::SeaShell() {
		static const Color color(0xFFF5EE);
		return color;
	}

	const Color& Color::Sienna() {
		static const Color color(0xA0522D);
		return color;
	}

	const Color& Color::Silver() {
		static const Color color(0xC0C0C0);
		return color;
	}

	const Color& Color::SkyBlue() {
		static const Color color(0x87CEEB);
		return color;
	}

	const Color& Color::SlateBlue() {
		static const Color color(0x6A5ACD);
		return color;
	}

	const Color& Color::SlateGray() {
		static const Color color(0x708090);
		return color;
	}

	const Color& Color::Snow() {
		static const Color color(0xFFFAFA);
		return color;
	}

	const Color& Color::SpringGreen() {
		static const Color color(0x00FF7F);
		return color;
	}

	const Color& Color::SteelBlue() {
		static const Color color(0x4682B4);
		return color;
	}

	const Color& Color::Tan() {
		static const Color color(0xD2B48C);
		return color;
	}

	const Color& Color::Teal() {
		static const Color color(0x008080);
		return color;
	}

	const Color& Color::Thistle() {
		static const Color color(0xD8BFD8);
		return color;
	}

	const Color& Color::Tomato() {
		static const Color color(0xFF6347);
		return color;
	}

	const Color& Color::Turquoise() {
		static const Color color(0x40E0D0);
		return color;
	}

	const Color& Color::Violet() {
		static const Color color(0xEE82EE);
		return color;
	}

	const Color& Color::Wheat() {
		static const Color color(0xF5DEB3);
		return color;
	}

	const Color& Color::White() {
		static const Color color(0xFFFFFF);
		return color;
	}

	const Color& Color::WhiteSmoke() {
		static const Color color(0xF5F5F5);
		return color;
	}

	const Color& Color::Yellow() {
		static const Color color(0xFFFF00);
		return color;
	}

	const Color& Color::YellowGreen() {
		static const Color color(0x9ACD32);
		return color;
	}

#pragma endregion

}
