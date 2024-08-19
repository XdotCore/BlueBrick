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

}
