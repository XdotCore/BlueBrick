#include "GUI/Flow/Screens/MainMenuScreen.hpp"
#include "Logger/Logger.hpp"
#include <rcmp.hpp>
#include <windows.h>
#include <functional>

using namespace Lego::Events;
using namespace Lego::GUI;

using ClassManager = BlueBrick::ClassManager<MainMenuScreen>;

extern BlueBrick::Logger MainLogger;

namespace Lego::GUI {

	void MainMenuScreen::GUI2PageHandler_dtor() {
		ClassManager::CallFunc(&MainMenuScreen::GUI2PageHandler_dtor, this);
	}

	void MainMenuScreen::Update(GUI2Page* page, PageState state, void** managerProcessData) {
		ClassManager::CallFunc(&MainMenuScreen::Update, this, page, state, managerProcessData);
	}

	int MainMenuScreen::GetGui2Manager_int_0xc() {
		return ClassManager::CallFunc(&MainMenuScreen::GetGui2Manager_int_0xc, this);
	}

	int MainMenuScreen::Return2() {
		return ClassManager::CallFunc(&MainMenuScreen::Return2, this);
	}

	void MainMenuScreen::SetGUI2Manager_int_0xc(int val) {
		ClassManager::CallFunc(&MainMenuScreen::SetGUI2Manager_int_0xc, this, val);
	}

	void MainMenuScreen::IEventListener_dtor() {
		ClassManager::CallFunc(&MainMenuScreen::IEventListener_dtor, this);
	}

	void MainMenuScreen::RecieveEvent(Event* event) {
		ClassManager::CallFunc(&MainMenuScreen::RecieveEvent, this, event);
	}

}
