#include "GUI/Flow/Screens/MainMenuScreen.hpp"
#include "Logger/Logger.hpp"
#include "Hooking/MemberFuncData.hpp"

using namespace Lego::Events;
using namespace Lego::GUI;
using namespace BlueBrick;

extern BlueBrick::Logger MainLogger;

namespace Lego::GUI {

	constexpr intptr_t vftable_for_FlowPageHandler2 = 0xDD5948;
	constexpr intptr_t vftable_for_IEventListener = 0xDD5938;

	void MainMenuScreen::GUI2PageHandler_dtor() {
		GUI2PageHandler_dtor_data().Call(this);
	}
	FuncData<decltype(&MainMenuScreen::GUI2PageHandler_dtor)>& MainMenuScreen::GUI2PageHandler_dtor_data() {
		static MemberFuncData<vftable_for_FlowPageHandler2, 0, rcmp::cconv::thiscall_, decltype(&GUI2PageHandler_dtor), GUI2PageHandler> funcData("MainMenuScreen::GUI2PageHandler_dtor");
		return funcData;
	}

	void MainMenuScreen::Update(GUI2Page* page, PageState state, void** managerProcessData) {
		Update_data().Call(this, page, state, managerProcessData);
	}
	FuncData<decltype(&MainMenuScreen::Update)>& MainMenuScreen::Update_data() {
		static MemberFuncData<vftable_for_FlowPageHandler2, 1, rcmp::cconv::thiscall_, decltype(&Update), MainMenuScreen> funcData("MainMenuScreen::Update");
		return funcData;
	}

	int MainMenuScreen::GetGUI2PageHandler_int_0xc() {
		return GetGUI2PageHandler_int_0xc_data().Call(this);
	}
	FuncData<decltype(&MainMenuScreen::GetGUI2PageHandler_int_0xc)>& MainMenuScreen::GetGUI2PageHandler_int_0xc_data() {
		static MemberFuncData<vftable_for_FlowPageHandler2, 2, rcmp::cconv::thiscall_, decltype(&GetGUI2PageHandler_int_0xc), MainMenuScreen> funcData("MainMenuScreen::GetGUI2PageHandler_int_0xc");
		return funcData;
	}

	int MainMenuScreen::Return2() {
		return Return2_data().Call(this);
	}
	FuncData<decltype(&MainMenuScreen::Return2)>& MainMenuScreen::Return2_data() {
		static MemberFuncData<vftable_for_FlowPageHandler2, 3, rcmp::cconv::stdcall_, decltype(&Return2), MainMenuScreen> funcData("MainMenuScreen::Return2");
		return funcData;
	}

	void MainMenuScreen::SetGUI2PageHandler_int_0xc(int val) {
		SetGUI2PageHandler_int_0xc_data().Call(this, val);
	}
	FuncData<decltype(&MainMenuScreen::SetGUI2PageHandler_int_0xc)>& MainMenuScreen::SetGUI2PageHandler_int_0xc_data() {
		static MemberFuncData<vftable_for_FlowPageHandler2, 4, rcmp::cconv::thiscall_, decltype(&SetGUI2PageHandler_int_0xc), MainMenuScreen> funcData("MainMenuScreen::SetGUI2PageHandler_int_0xc");
		return funcData;
	}

	void MainMenuScreen::IEventListener_dtor() {
		IEventListener_dtor_data().Call(this);
	}
	FuncData<decltype(&MainMenuScreen::IEventListener_dtor)>& MainMenuScreen::IEventListener_dtor_data() {
		static MemberFuncData<vftable_for_IEventListener, 0, rcmp::cconv::thiscall_, decltype(&IEventListener_dtor), IEventListener> funcData("MainMenuScreen::IEventListener_dtor");
		return funcData;
	}

	void MainMenuScreen::RecieveEvent(Event* event, NuEventData* data) {
		RecieveEvent_data().Call(this, event, data);
	}
	FuncData<decltype(&MainMenuScreen::RecieveEvent)>& MainMenuScreen::RecieveEvent_data() {
		static MemberFuncData<vftable_for_IEventListener, 1, rcmp::cconv::thiscall_, decltype(&RecieveEvent), IEventListener> funcData("MainMenuScreen::RecieveEvent");
		return funcData;
	}

}
