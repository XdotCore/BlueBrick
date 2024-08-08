#pragma once

#include "ColorBase.hpp"
#include "Types.hpp"

namespace BlueBrick {

	/// <summary>
	/// Represents the 16 basic console colors
	/// </summary>
	class BLUEBRICK_DLL ConsoleColor final : public ColorBase {
	private:
		ConsoleColor(byte code);
	public:
		/// <summary>
		/// The console code index of the color
		/// </summary>
		const byte code;

		/// <inheritdoc/>
		std::string Start() const override;

		static const ConsoleColor& Black();
		static const ConsoleColor& Red();
		static const ConsoleColor& Green();
		static const ConsoleColor& Yellow();
		static const ConsoleColor& Blue();
		static const ConsoleColor& Magenta();
		static const ConsoleColor& Cyan();
		static const ConsoleColor& White();
		static const ConsoleColor& BrightBlack();
		static const ConsoleColor& BrightRed();
		static const ConsoleColor& BrickGreen();
		static const ConsoleColor& BrightYellow();
		static const ConsoleColor& BrickBlue();
		static const ConsoleColor& BrightMagenta();
		static const ConsoleColor& BrightCyan();
		static const ConsoleColor& BrightWhite();
	};

}