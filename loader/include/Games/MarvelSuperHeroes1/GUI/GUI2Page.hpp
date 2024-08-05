#pragma once

namespace Lego::GUI {

	// TODO: fill in class
	class GUI2Page;

	enum PageState {
		PageState_Init = 0x1,
		PageState_Entered = 0x2,
		PageState_Input = 0xe,
		PageState_Exit = 0xf
	};

}