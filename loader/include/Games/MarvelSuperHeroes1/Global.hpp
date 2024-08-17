#pragma once

#include "Export.hpp"
#include "Hooking/HookManager.hpp"

namespace Lego {

	class BLUEBRICK_DLL Global final {
	public:
		static int RunGame(int cmdLineArgCount, char** cmdLineArgs);

	private:
		static BlueBrick::FuncData<decltype(RunGame)>& RunGame_data();

		friend BlueBrick::HookManager;
	};

}

template<>
BlueBrick::FuncDataBase& BlueBrick::HookManager::GetFuncData<Lego::Global>(int(*func)(int, char**)) {
	using namespace Lego;

	if (func == Global::RunGame)
		return Global::RunGame_data();

	throw noFuncDataException;
}
