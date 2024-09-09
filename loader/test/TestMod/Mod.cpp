#include "Mod/Mod.hpp"
#include "GUI/Flow/Screens/MainMenuScreen.hpp"
#include "Global.hpp"
#include "Logger/Color/Color.hpp"
#include "Logger/Color/ConsoleColor.hpp"
#include "Logger/Color/Color256.hpp"
#include "imgui.h"
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

	static inline Hook* UpdatePrefix;
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

		// normal hook tests
		UpdatePrefix = &HookManager.AttachPrefix(&MainMenuScreen::Update, UpdatePrefixTest);
		UpdatePostfix = &HookManager.AttachPostfix(&MainMenuScreen::Update, UpdatePostfixTest);
		HookManager.AttachPrefix(&MainMenuScreen::RecieveEvent, RecieveEventTest);

		HookManager.AttachPrefix<Global>(Global::RunGame, RunGamePrefixTest);
		HookManager.AttachPostfix<Global>(Global::RunGame, RunGamePostfixTest);

		// modify value hook tests
		HookManager.AttachPrefix<Global>(Global::AddToCoins, AddToCoinsTest);

		// test createWindow
#undef CreateWindow
		HookManager.AttachPostfix<Global>(Global::CreateWindow, [](int& result, void* hInstance, int param_2, char* icon) {
			Logger->Message("Made window");
		});
		HookManager.AttachPostfix<Global>(Global::CreateAdditionalWindow, [](void*& result, void* _this, char* windowName, int x, int y, int width, int height) {
			Logger->Message("Made additional window: {}, {}, {}, {}, {}", windowName, x, y, width, height);
		});
	}

	static void UpdatePrefixTest(MainMenuScreen* _this, GUI2Page*& page, PageState& state, void**& m) {
		Logger->Message("Hello world!");
		UpdatePrefix->SetEnabled(false);
	}

	static void UpdatePostfixTest(MainMenuScreen* _this, GUI2Page* page, PageState state, void** m) {
		Logger->Message("Goodbye world!");
		UpdatePostfix->SetEnabled(false);
	}

	static void RecieveEventTest(MainMenuScreen* _this, Event*& event, NuEventData*& data) {
		int base = (int)(GetModuleHandle(NULL)) - 0x400000;
		Logger->Message("{:x}, {:x}, {:x}", (int)*(void**)_this - base, (int)event - base, (int)*(void**)data - base);
	}

	static void RunGamePrefixTest(int& cmdLineArgCount, char**& cmdLineArgs) {
		Logger->Message("{}", cmdLineArgCount);
		for (int i = 0; i < cmdLineArgCount; i++)
			Logger->Message("{}: {}", i, cmdLineArgs[i]);
	}

	static void RunGamePostfixTest(int& result, int cmdLineArgCount, char** cmdLineArgs) {
		Logger->Message("Game End");
	}

	static void AddToCoinsTest(uint64*& coinsPtr, uint64& toAdd, int& multEnabled, bool& roundTo10s) {
		toAdd = 42069;
		roundTo10s = false;
	}

	void OnDraw() override {
		ImGui::Begin("Test Mod Window");

		ImGui::Text("Hello World!");

		ImGui::End();
	}

} testMod;
