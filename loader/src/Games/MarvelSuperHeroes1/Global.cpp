#include "Global.hpp"
#include "Hooking/StaticFuncData.hpp"

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

}
