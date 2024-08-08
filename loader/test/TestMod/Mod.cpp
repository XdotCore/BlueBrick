#include "Mod/Mod.hpp"
#include "GUI/Flow/Screens/MainMenuScreen.hpp"
#include "Logger/Color.hpp"

using namespace BlueBrick;
using namespace Lego::GUI;

$ModEntry(TestMod) {
public:
	static inline BlueBrick::Logger* TestModLogger;

	Info GetInfo() override {
		return { "TestMod", "1.0.0", "X.Core" };
	}

	void OnInitialized() override {
		Logger->Message("Hello from test mod {1}😘💩{0} hello again {2} oog", Color::End(), Color(0xaf70f5), Color(0xfcba03));
		TestModLogger = Logger;

		ClassManager<MainMenuScreen>::AttachPrefix(&MainMenuScreen::Update, a);
		ClassManager<MainMenuScreen>::AttachPostfix(&MainMenuScreen::Update, b);
	}

	static void a(MainMenuScreen* _this, GUI2Page* page, PageState state, void** m) {
		TestModLogger->Message("Hello world! {}", (int)state);
	}

	static void b(MainMenuScreen* _this, GUI2Page* page, PageState state, void** m) {
		TestModLogger->Message("Goodbye world! {}", (int)state);
	}
};
