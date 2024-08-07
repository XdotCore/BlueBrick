#pragma once

#include "Export.hpp"
#include "Events/IEventListener.hpp"
#include "GUI/Flow/FlowPageHandler2.hpp"
#include "GUI/GUI2Page.hpp"
#include "GUI/GUI2MenuEntry.hpp"
#include "Hooking/ClassManager.hpp"
#include <cstdint>

namespace Lego::GUI {

	class BLUEBRICK_DLL MainMenuScreen : public FlowPageHandler2, public Events::IEventListener {
	public:
		GUI2Page* page;
		uint8_t bitfield_0x4;
		GUI2MenuEntry* menuEntries[12];
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
		GUI2MenuEntryTextOverride menuEntryTextOverrides[12];
		bool bool_0xd0;
		int int_0xd4;
		int int_0xd8;
		int int_0xdc;
		int int_0xe0;
		int int_0xe4;
		bool bool_0xe8;
		int int_0xec;
		int int_0xf0;
		int int_0xf4;

		void GUI2PageHandler_dtor() override;
		void Update(GUI2Page* page, PageState state, void** managerProcessData) override;

		virtual int GetGui2Manager_int_0xc();
		virtual int Return2();
		virtual void SetGUI2Manager_int_0xc(int val);

		void IEventListener_dtor() override;
		void RecieveEvent(Events::Event* event) override;
	};

}

template<>
template<typename Ret, typename... Args>
BlueBrick::FuncData BlueBrick::ClassManager<Lego::GUI::MainMenuScreen>::GetFuncData(Ret(Lego::GUI::MainMenuScreen::* func)(Args...)) {
	using namespace Lego::GUI;
	static void** vftable_for_FlowPageHandler2 = (void**)0xDD5948;
	static void** vftable_for_IEventListener = (void**)0xDD5938;

	if (IsSameAndEqual(func, &MainMenuScreen::GUI2PageHandler_dtor))
		return FuncData(vftable_for_FlowPageHandler2, 0, CallConv::Thiscall);
	if (IsSameAndEqual(func, &MainMenuScreen::Update))
		return FuncData(vftable_for_FlowPageHandler2, 1, CallConv::Thiscall);
	if (IsSameAndEqual(func, &MainMenuScreen::GetGui2Manager_int_0xc))
		return FuncData(vftable_for_FlowPageHandler2, 2, CallConv::Thiscall);
	if (IsSameAndEqual(func, &MainMenuScreen::Return2))
		return FuncData(vftable_for_FlowPageHandler2, 3, CallConv::Stdcall);
	if (IsSameAndEqual(func, &MainMenuScreen::SetGUI2Manager_int_0xc))
		return FuncData(vftable_for_FlowPageHandler2, 4, CallConv::Thiscall);
	if (IsSameAndEqual(func, &MainMenuScreen::IEventListener_dtor))
		return FuncData(vftable_for_IEventListener, 0, CallConv::Thiscall);
	if (IsSameAndEqual(func, &MainMenuScreen::RecieveEvent))
		return FuncData(vftable_for_IEventListener, 1, CallConv::Thiscall);

	return FuncData(nullptr);
}
