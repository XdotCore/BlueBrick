#include "Mod/Mod.hpp"
#include "GUI/Flow/Screens/MainMenuScreen.hpp"
#include "Global.hpp"
#include "Logger/Color/Color.hpp"
#include "Logger/Color/ConsoleColor.hpp"
#include "Logger/Color/Color256.hpp"
#include <iostream>
#include <windows.h>
#include <functional>

using namespace BlueBrick;
using namespace Lego;
using namespace Lego::GUI;
using namespace Lego::Events;

class TestMod : public Mod {
public:
	static inline BlueBrick::Logger* Logger;

	static inline Hook* UpdatePostfix;

	ModInfo& GetInfo() override {
		static Color nameColor = Color(0x9932CC);
		static ModInfo info = ModInfo("TestMod", nameColor, "1.0.0", "X.Core", Color::DarkOrange());
		return info;
	}

	void OnInitialized() override {
		Logger = &(Mod::Logger);

		Logger->Message("Hello from test mod {1}😘💩{0} hello again {2} oog", Color::End(), Color::Aqua(), Color::DarkSalmon());
		Logger->Message(Severity::Debug, "Debug: debug");
		Logger->Message(Severity::Warning, "Warning: warning");
		Logger->Message(Severity::Error, "Error: error");

		HookManager.AttachPrefix(&MainMenuScreen::Update, a);
		UpdatePostfix = &HookManager.AttachPostfix(&MainMenuScreen::Update, b);
		HookManager.AttachPrefix(&MainMenuScreen::RecieveEvent, c);

		HookManager.AttachPrefix<Global>(Global::RunGame, d);
		HookManager.AttachPostfix<Global>(Global::RunGame, e);
	}

	static void a(MainMenuScreen* _this, GUI2Page*& page, PageState& state, void**& m) {
		Logger->Message("Hello world!");
	}

	static void b(MainMenuScreen* _this, GUI2Page* page, PageState state, void** m) {
		Logger->Message("Goodbye world!");
		UpdatePostfix->SetEnabled(false);
	}

	static void c(MainMenuScreen* _this, Event*& event, NuEventData*& data) {
		int base = (int)(GetModuleHandle(NULL)) - 0x400000;
		Logger->Message("{:x}, {:x}, {:x}", (int)*(void**)_this - base, (int)event - base, (int)*(void**)data - base);
	}

	static void d(int& cmdLineArgCount, char**& cmdLineArgs) {
		Logger->Message("{}", cmdLineArgCount);
		for (int i = 0; i < cmdLineArgCount; i++)
			Logger->Message("{}: {}", i, cmdLineArgs[i]);
	}

	static void e(int& result, int cmdLineArgCount, char** cmdLineArgs) {
		Logger->Message("Game End");
	}
} testMod;
