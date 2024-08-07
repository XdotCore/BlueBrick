#include "Mod/Mod.hpp"
#include "Mod/ModEntry.hpp"
#include "GUI/Flow/Screens/MainMenuScreen.hpp"

using namespace BlueBrick;
using namespace Lego::GUI;

$ModEntry(TestMod) {
public:
	Info GetInfo() override {
		return { "TestMod", "1.0.0", "X.Core" };
	}

	void OnInitialized() override {
		Logger->Message("Hello from test mod");

		FuncData data = ClassManager<MainMenuScreen>::GetFuncData(&MainMenuScreen::IEventListener_dtor);
		Logger->Message("{} {:x}[{}]", data.IsVirtual(), (intptr_t)data.Vftable(), data.Index());
	}
};
