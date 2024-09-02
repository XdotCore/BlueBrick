#include "Global.hpp"
#include "Hooking/StaticFuncData.hpp"

#undef CreateWindow

using namespace BlueBrick;

namespace Lego {

	int Global::RunGame(int cmdLineArgCount, char** cmdLineArgs) {
		return RunGame_data().Call(cmdLineArgCount, cmdLineArgs);
	}
	FuncData<decltype(Global::RunGame)>& Global::RunGame_data() {
		static StaticFuncData<0x185410, rcmp::cconv::cdecl_, decltype(RunGame)> funcData("Global::RunGame");
		return funcData;
	}

	void Global::AddToCoins(uint64* coinsPtr, uint64 toAdd, int multEnabled, bool roundTo10s) {
		return AddToCoins_data().Call(coinsPtr, toAdd, multEnabled, roundTo10s);
	}
	FuncData<decltype(Global::AddToCoins)>& Global::AddToCoins_data() {
		static StaticFuncData<0x7E1070, rcmp::cconv::cdecl_, decltype(AddToCoins)> funcData("Global::AddToCoins");
		return funcData;
	}

	int Global::CreateWindow(void* hInstance, int param_2, char* icon) {
		return CreateWindow_data().Call(hInstance, param_2, icon);
	}
	FuncData<decltype(Global::CreateWindow)>& Global::CreateWindow_data() {
		static StaticFuncData<0x2e2210, rcmp::cconv::cdecl_, decltype(CreateWindow)> funcData("Global::CreateWindow");
		return funcData;
	}

	void* Global::CreateAdditionalWindow(void* _this, char* windowName, int x, int y, int width, int height) {
		return CreateAdditionalWindow_data().Call(_this, windowName, x, y, width, height);
	}
	FuncData<decltype(Global::CreateAdditionalWindow)>& Global::CreateAdditionalWindow_data() {
		static StaticFuncData<0x2e0760, rcmp::cconv::thiscall_, decltype(CreateAdditionalWindow)> funcData("Global::CreateAdditionalWindow");
		return funcData;
	}

}
