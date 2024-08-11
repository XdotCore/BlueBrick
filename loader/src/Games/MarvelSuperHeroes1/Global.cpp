#include "Global.hpp"
#include "Hooking/StaticFuncData.hpp"

using namespace BlueBrick;

namespace Lego {

	const char RunGame_name[] = "Global::RunGame";

	int Global::RunGame(int cmdLineArgCount, char** cmdLineArgs) {
		return RunGame_data().Call(cmdLineArgCount, cmdLineArgs);
	}
	FuncData<decltype(Global::RunGame)>& Global::RunGame_data() {
		static StaticFuncData<RunGame_name, rcmp::cconv::cdecl_, decltype(RunGame)> funcData((void*)0x5076A0);
		return funcData;
	}

}
