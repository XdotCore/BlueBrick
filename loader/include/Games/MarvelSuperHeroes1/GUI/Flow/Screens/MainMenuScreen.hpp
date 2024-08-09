#pragma once

#include "Export.hpp"
#include "Events/IEventListener.hpp"
#include "GUI/Flow/FlowPageHandler2.hpp"
#include "GUI/GUI2Page.hpp"
#include "GUI/GUI2MenuEntry.hpp"
#include "Hooking/ClassManager.hpp"
#include "Types.hpp"

namespace Lego::GUI {

	class BLUEBRICK_DLL MainMenuScreen : public FlowPageHandler2, public Events::IEventListener {
	private:
		static BlueBrick::FuncData& GUI2PageHandler_dtor_data();
		static BlueBrick::FuncData& Update_data();

		static BlueBrick::FuncData& GetGUI2Manager_int_0xc_data();
		static BlueBrick::FuncData& Return2_data();
		static BlueBrick::FuncData& SetGUI2Manager_int_0xc_data();

		static BlueBrick::FuncData& IEventListener_dtor_data();
		static BlueBrick::FuncData& RecieveEvent_data();

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
		uint enabledMenuEntries;
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
		void RecieveEvent(Events::Event* event, int a) override;

		friend BlueBrick::ClassManager<MainMenuScreen>;
	};

}

template<>
BLUEBRICK_DLL void BlueBrick::ClassManager<Lego::GUI::MainMenuScreen>::Init();

template<>
template<typename Ret, typename... Args>
BlueBrick::FuncData& BlueBrick::ClassManager<Lego::GUI::MainMenuScreen>::GetFuncData(Ret(Lego::GUI::MainMenuScreen::* func)(Args...)) {
	using namespace Lego::GUI;

	if (IsSameAndEqual(func, &MainMenuScreen::GUI2PageHandler_dtor))
		return MainMenuScreen::GUI2PageHandler_dtor_data();
	if (IsSameAndEqual(func, &MainMenuScreen::Update))
		return MainMenuScreen::Update_data();
	if (IsSameAndEqual(func, &MainMenuScreen::GetGui2Manager_int_0xc))
		return MainMenuScreen::GetGUI2Manager_int_0xc_data();
	if (IsSameAndEqual(func, &MainMenuScreen::Return2))
		return MainMenuScreen::Return2_data();
	if (IsSameAndEqual(func, &MainMenuScreen::SetGUI2Manager_int_0xc))
		return MainMenuScreen::SetGUI2Manager_int_0xc_data();
	if (IsSameAndEqual(func, &MainMenuScreen::IEventListener_dtor))
		return MainMenuScreen::IEventListener_dtor_data();
	if (IsSameAndEqual(func, &MainMenuScreen::RecieveEvent))
		return MainMenuScreen::RecieveEvent_data();

	throw noFuncDataException;
}
