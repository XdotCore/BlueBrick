#include "GUI/Flow/Screens/MainMenuScreen.hpp"
#include "Logger/Logger.hpp"
#include "Hooking/MemberFuncData.hpp"

using namespace Lego::Events;
using namespace Lego::GUI;
using namespace BlueBrick;

extern BlueBrick::Logger MainLogger;

namespace Lego::GUI {

	static void** const vftable_for_FlowPageHandler2 = (void**)0xDD5948;
	static void** const vftable_for_IEventListener = (void**)0xDD5938;

	const char GUI2PageHandler_dtor_name[] = "MainMenuScreen::GUI2PageHandler_dtor_name";
	const char Update_name[] = "MainMenuScreen::Update";

	const char GetGUI2PageHandler_int_0xc_name[] = "MainMenuScreen::GetGUI2PageHandler_int_0xc";
	const char Return2_name[] = "MainMenuScreen::Return2";
	const char SetGUI2PageHandler_int_0xc_name[] = "MainMenuScreen::SetGUI2PageHandler_int_0xc";

	const char IEventListener_dtor_name[] = "MainMenuScreen::IEventListener_dtor";
	const char RecieveEvent_name[] = "MainMenuScreen::RecieveEvent";

	void MainMenuScreen::GUI2PageHandler_dtor() {
		GUI2PageHandler_dtor_data().Call(this);
	}
	FuncData<decltype(&MainMenuScreen::GUI2PageHandler_dtor)>& MainMenuScreen::GUI2PageHandler_dtor_data() {
		static MemberFuncData<GUI2PageHandler_dtor_name, rcmp::cconv::thiscall_, decltype(&MainMenuScreen::GUI2PageHandler_dtor), GUI2PageHandler> funcData(vftable_for_FlowPageHandler2, 0);
		return funcData;
	}

	void MainMenuScreen::Update(GUI2Page* page, PageState state, void** managerProcessData) {
		Update_data().Call(this, page, state, managerProcessData);
	}
	FuncData<decltype(&MainMenuScreen::Update)>& MainMenuScreen::Update_data() {
		static MemberFuncData<Update_name, rcmp::cconv::thiscall_, decltype(&MainMenuScreen::Update), MainMenuScreen> funcData(vftable_for_FlowPageHandler2, 1);
		return funcData;
	}

	int MainMenuScreen::GetGUI2PageHandler_int_0xc() {
		return GetGUI2PageHandler_int_0xc_data().Call(this);
	}
	FuncData<decltype(&MainMenuScreen::GetGUI2PageHandler_int_0xc)>& MainMenuScreen::GetGUI2PageHandler_int_0xc_data() {
		static MemberFuncData<GetGUI2PageHandler_int_0xc_name, rcmp::cconv::thiscall_, decltype(&MainMenuScreen::GetGUI2PageHandler_int_0xc), MainMenuScreen> funcData(vftable_for_FlowPageHandler2, 2);
		return funcData;
	}

	int MainMenuScreen::Return2() {
		return Return2_data().Call(this);
	}
	FuncData<decltype(&MainMenuScreen::Return2)>& MainMenuScreen::Return2_data() {
		static MemberFuncData<Return2_name, rcmp::cconv::stdcall_, decltype(&MainMenuScreen::Return2), MainMenuScreen> funcData(vftable_for_FlowPageHandler2, 3);
		return funcData;
	}

	void MainMenuScreen::SetGUI2PageHandler_int_0xc(int val) {
		SetGUI2PageHandler_int_0xc_data().Call(this, val);
	}
	FuncData<decltype(&MainMenuScreen::SetGUI2PageHandler_int_0xc)>& MainMenuScreen::SetGUI2PageHandler_int_0xc_data() {
		static MemberFuncData<SetGUI2PageHandler_int_0xc_name, rcmp::cconv::thiscall_, decltype(&MainMenuScreen::SetGUI2PageHandler_int_0xc), MainMenuScreen> funcData(vftable_for_FlowPageHandler2, 4);
		return funcData;
	}

	void MainMenuScreen::IEventListener_dtor() {
		IEventListener_dtor_data().Call(this);
	}
	FuncData<decltype(&MainMenuScreen::IEventListener_dtor)>& MainMenuScreen::IEventListener_dtor_data() {
		static MemberFuncData<IEventListener_dtor_name, rcmp::cconv::thiscall_, decltype(&MainMenuScreen::IEventListener_dtor), IEventListener> funcData(vftable_for_IEventListener, 0);
		return funcData;
	}

	void MainMenuScreen::RecieveEvent(Event* event, NuEventData* data) {
		RecieveEvent_data().Call(this, event, data);
	}
	FuncData<decltype(&MainMenuScreen::RecieveEvent)>& MainMenuScreen::RecieveEvent_data() {
		static MemberFuncData<RecieveEvent_name, rcmp::cconv::thiscall_, decltype(&MainMenuScreen::RecieveEvent), IEventListener> funcData(vftable_for_IEventListener, 1);
		return funcData;
	}

}
