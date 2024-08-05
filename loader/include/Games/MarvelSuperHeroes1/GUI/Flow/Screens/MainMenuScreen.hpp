#pragma once

#include "Events/IEventListener.hpp"
#include "GUI/Flow/FlowPageHandler2.hpp"
#include "GUI/GUI2Page.hpp"
#include "GUI/GUI2MenuEntry.hpp"
#include <cstdint>

namespace Lego::GUI {

	class MainMenuScreen : public FlowPageHandler2 : public IEventListener {
	public:
		GUI2Page* page;
		uint8_t bitfield_0x4;
		GUI2MenuEntry*[12] menuEntries;
		GUI2MenuEntry* newGameMenuEntry;
		GUI2MenuEntry* freeplayMenuEntry;
		GUI2MenuEntry* selectLevelMenuEntry;
		GUI2MenuEntry* showControlsMenuEntry;
		GUI2MenuEntry* xboxLiveMarketplaceMenuEntry;
		GUI2MenuEntry* comingSoonMenuEntry;
		GUI2MenuEntry* loadGameMenuEntry;
		GUI2MenuEntry* networkGameMenuEntry;
		GUI2MenuEntry* optionsMenuEntry;
		GUI2MenuEntry* quitToWindowsMenuEntry;
		GUI2MenuEntry* languageMenuEntry;
		GUI2MenuEntry* helpMenuEntry;
		uint32_t enabledMenuEntries;
		short short_0x6c;
		GUI2MenuEntryTextOverride[12];
		bool bool_0xd0;
		int int_0xd4;
		int int_0xd8;
		int int_0xdc;
		int int_0xe0;
		int int_0xe4;
		bool bool_0xe8;

		void IEventListener_dtor() override;
		void RecievedEvent(Event* event) override;

		void GUI2PageHandler_dtor() override;
		void Update(GUI2Page* page, PageState state, void** managerProcessData) override;

		virtual int GetGui2Manager_int_0xc();
		virtual void Return2();
		virtual void SetGUI2Manager_int_0xc(int val);
	}

}