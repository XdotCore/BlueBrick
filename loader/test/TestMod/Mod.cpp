#include "Mod/Mod.hpp"
#include "Mod/ModEntry.hpp"

$ModEntry(TestMod) {
public:
	Info GetInfo() override {
		return { "TestMod", "1.0.0", "X.Core" };
	}

	void OnInitialized() override {
		Logger->Message("Hello from test mod");
	}
};
