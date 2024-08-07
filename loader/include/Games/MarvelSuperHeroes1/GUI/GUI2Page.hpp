#pragma once

#include "Export.hpp"

namespace Lego::GUI {

	// TODO: fill in class
	class BLUEBRICK_DLL GUI2Page;

	enum class BLUEBRICK_DLL PageState {
		Init = 0x1,
		Entered = 0x2,
		Input = 0xe,
		Exit = 0xf
	};

}