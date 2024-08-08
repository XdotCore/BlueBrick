#pragma once

#include "ColorBase.hpp"
#include "Types.hpp"

namespace BlueBrick {

	/// <summary>
	/// Represents the 256 xterm colors
	/// </summary>
	class BLUEBRICK_DLL Color256 final : public ColorBase {
	public:
		/// <summary>
		/// The index of the color
		/// </summary>
		const byte val;

		/// <summary>
		/// Assigns the color index
		/// </summary>
		/// <param name="val"> The index of the color </param>
		Color256(byte val);

		/// <inheritdoc/>
		std::string Start() const override;
	};

}