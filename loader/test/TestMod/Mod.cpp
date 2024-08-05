#include "Mod/Mod.hpp"
#include "Mod/ModEntry.hpp"

$ModEntry(TestMod) {
public:
	std::string GetName() override {
		return "TestMod";
	}

	std::string GetVersion() override {
		return "1.0.0";
	}

	void OnInitialized() override {
		Logger->Message("Hello from test mod");
	}
};