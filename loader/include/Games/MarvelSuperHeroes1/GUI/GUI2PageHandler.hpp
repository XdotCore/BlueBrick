#pragma once

#include "CSListLink.hpp"
#include "GUI/GUI2Page.hpp"

namespace Lego::GUI {

	class BLUEBRICK_DLL GUI2PageHandler : public CSListLink<GUI2PageHandler> {
	public:
		bool bool_0x0;
		char* name;
		GUI2Page* page;
		int int_0xc;

		virtual void GUI2PageHandler_dtor();
		// TODO: create ManagerProcessData class
		virtual void Update(GUI2Page* page, PageState state, void** managerProcessData) = 0;
	};

}
