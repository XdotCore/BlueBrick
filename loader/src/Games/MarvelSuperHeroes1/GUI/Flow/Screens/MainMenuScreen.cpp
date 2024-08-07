#include "GUI/Flow/Screens/MainMenuScreen.hpp"
#include "Logger/Logger.hpp"
#include "HookHelpers.hpp"
#include "rcmp.hpp"

using namespace Lego::Events;
using namespace Lego::GUI;

using ClassManager = BlueBrick::ClassManager<MainMenuScreen>;
using FuncData = BlueBrick::FuncData;
using CallConv = BlueBrick::CallConv;

extern BlueBrick::Logger MainLogger;

void ClassManager::Init() {
	HookHelpers::HookFunction<class Tag1>(&MainMenuScreen::GUI2PageHandler_dtor,   "MainMenuScreen::GUI2PageHandler_dtor");
	HookHelpers::HookFunction<class Tag2>(&MainMenuScreen::Update,                 "MainMenuScreen::Update");
	HookHelpers::HookFunction<class Tag3>(&MainMenuScreen::GetGui2Manager_int_0xc, "MainMenuScreen::GetGui2Manager_int_0xc");
	HookHelpers::HookFunction<class Tag4>(&MainMenuScreen::Return2,                "MainMenuScreen::Return2");
	HookHelpers::HookFunction<class Tag5>(&MainMenuScreen::SetGUI2Manager_int_0xc, "MainMenuScreen::SetGUI2Manager_int_0xc");
	HookHelpers::HookFunction<class Tag6>(&MainMenuScreen::IEventListener_dtor,    "MainMenuScreen::IEventListener_dtor");
	HookHelpers::HookFunction<class Tag7>(&MainMenuScreen::RecieveEvent,           "MainMenuScreen::RecieveEvent");
}

namespace Lego::GUI {

	static void** const vftable_for_FlowPageHandler2 = (void**)0xDD5948;
	static void** const vftable_for_IEventListener = (void**)0xDD5938;

	void MainMenuScreen::GUI2PageHandler_dtor() {
		ClassManager::CallFunc(&MainMenuScreen::GUI2PageHandler_dtor, this);
	}
	FuncData* MainMenuScreen::GUI2PageHandler_dtor_data() {
		static FuncData funcData(vftable_for_FlowPageHandler2, 0, CallConv::Thiscall);
		return &funcData;
	}

	void MainMenuScreen::Update(GUI2Page* page, PageState state, void** managerProcessData) {
		ClassManager::CallFunc(&MainMenuScreen::Update, this, page, state, managerProcessData);
	}
	FuncData* MainMenuScreen::Update_data() {
		static FuncData funcData(vftable_for_FlowPageHandler2, 1, CallConv::Thiscall);
		return &funcData;
	}

	int MainMenuScreen::GetGui2Manager_int_0xc() {
		return ClassManager::CallFunc(&MainMenuScreen::GetGui2Manager_int_0xc, this);
	}
	FuncData* MainMenuScreen::GetGUI2Manager_int_0xc_data() {
		static FuncData funcData(vftable_for_FlowPageHandler2, 2, CallConv::Thiscall);
		return &funcData;
	}

	int MainMenuScreen::Return2() {
		return ClassManager::CallFunc(&MainMenuScreen::Return2, this);
	}
	FuncData* MainMenuScreen::Return2_data() {
		static FuncData funcData(vftable_for_FlowPageHandler2, 3, CallConv::Stdcall);
		return &funcData;
	}

	void MainMenuScreen::SetGUI2Manager_int_0xc(int val) {
		ClassManager::CallFunc(&MainMenuScreen::SetGUI2Manager_int_0xc, this, val);
	}
	FuncData* MainMenuScreen::SetGUI2Manager_int_0xc_data() {
		static FuncData funcData(vftable_for_FlowPageHandler2, 4, CallConv::Thiscall);
		return &funcData;
	}

	void MainMenuScreen::IEventListener_dtor() {
		ClassManager::CallFunc(&MainMenuScreen::IEventListener_dtor, this);
	}
	FuncData* MainMenuScreen::IEventListener_dtor_data() {
		static FuncData funcData(vftable_for_IEventListener, 0, CallConv::Thiscall);
		return &funcData;
	}

	void MainMenuScreen::RecieveEvent(Event* event, int a) {
		ClassManager::CallFunc(&MainMenuScreen::RecieveEvent, this, event, a);
	}
	FuncData* MainMenuScreen::RecieveEvent_data() {
		static FuncData funcData(vftable_for_IEventListener, 1, CallConv::Thiscall);
		return &funcData;
	}

}
