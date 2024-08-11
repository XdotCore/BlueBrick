#pragma once

#include "Export.hpp"
#include "../FlowPageHandler2.hpp"
#include "../../GUI2Page.hpp"
#include "../../GUI2MenuEntry.hpp"
#include "Events/IEventListener.hpp"
#include "Hooking/ClassManager.hpp"
#include "Types.hpp"

namespace Lego::GUI {

	class BLUEBRICK_DLL MainMenuScreen : public FlowPageHandler2, public Events::IEventListener {
	public:
		GUI2Page* page;
		byte bitfield_0x4;
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
		GUI2MenuEntry* selectedMenuEntry;
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

		// vftable0 inherited from FlowPageHandler2
		void GUI2PageHandler_dtor() override;
		void Update(GUI2Page* page, PageState state, void** managerProcessData) override;

		// vftable0 new functions
		virtual int GetGUI2PageHandler_int_0xc();
		virtual int Return2();
		virtual void SetGUI2PageHandler_int_0xc(int val);

		// vftable1 inherited from IEventListener
		void IEventListener_dtor() override;
		void RecieveEvent(Events::Event* event, Events::NuEventData* data) override;

	private:
		static BlueBrick::FuncData<decltype(&MainMenuScreen::GUI2PageHandler_dtor)>& GUI2PageHandler_dtor_data();
		static BlueBrick::FuncData<decltype(&MainMenuScreen::Update)>& Update_data();

		static BlueBrick::FuncData<decltype(&MainMenuScreen::GetGUI2PageHandler_int_0xc)>& GetGUI2PageHandler_int_0xc_data();
		static BlueBrick::FuncData<decltype(&MainMenuScreen::Return2)>& Return2_data();
		static BlueBrick::FuncData<decltype(&MainMenuScreen::SetGUI2PageHandler_int_0xc)>& SetGUI2PageHandler_int_0xc_data();

		static BlueBrick::FuncData<decltype(&MainMenuScreen::IEventListener_dtor)>& IEventListener_dtor_data();
		static BlueBrick::FuncData<decltype(&MainMenuScreen::RecieveEvent)>& RecieveEvent_data();

		friend BlueBrick::ClassManager<MainMenuScreen>;
	};

}

template<>
template<typename Ret, typename... Args>
BlueBrick::FuncDataBase& BlueBrick::ClassManager<Lego::GUI::MainMenuScreen>::GetFuncData(Ret(Lego::GUI::MainMenuScreen::* func)(Args...)) {
	using namespace Lego::GUI;

	if (IsSameAndEqual(func, &MainMenuScreen::GUI2PageHandler_dtor))
		return MainMenuScreen::GUI2PageHandler_dtor_data();
	if (IsSameAndEqual(func, &MainMenuScreen::Update))
		return MainMenuScreen::Update_data();
	if (IsSameAndEqual(func, &MainMenuScreen::GetGUI2PageHandler_int_0xc))
		return MainMenuScreen::GetGUI2PageHandler_int_0xc_data();
	if (IsSameAndEqual(func, &MainMenuScreen::Return2))
		return MainMenuScreen::Return2_data();
	if (IsSameAndEqual(func, &MainMenuScreen::SetGUI2PageHandler_int_0xc))
		return MainMenuScreen::SetGUI2PageHandler_int_0xc_data();
	if (IsSameAndEqual(func, &MainMenuScreen::IEventListener_dtor))
		return MainMenuScreen::IEventListener_dtor_data();
	if (IsSameAndEqual(func, &MainMenuScreen::RecieveEvent))
		return MainMenuScreen::RecieveEvent_data();

	throw noFuncDataException;
}
