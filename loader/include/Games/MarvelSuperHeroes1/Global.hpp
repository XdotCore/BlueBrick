#pragma once

#include "Export.hpp"
#include "Types.hpp"
#include "Hooking/HookManager.hpp"

namespace Lego {

	class BLUEBRICK_DLL Global final {
	public:
		static int RunGame(int cmdLineArgCount, char** cmdLineArgs);

		static void AddToCoins(uint64* coinsPtr, uint64 toAdd, int multEnabled, bool roundTo10s);

		static int CreateWindow(void* hInstance, int param_2, char* icon);

		// TODO: find the class this is a part of
		static void* CreateAdditionalWindow(void* _this, char* windowName, int x, int y, int width, int height);

	private:
		static BlueBrick::FuncData<decltype(RunGame)>& RunGame_data();
		static BlueBrick::FuncData<decltype(AddToCoins)>& AddToCoins_data();
		static BlueBrick::FuncData<decltype(CreateWindow)>& CreateWindow_data();
		static BlueBrick::FuncData<decltype(CreateAdditionalWindow)>& CreateAdditionalWindow_data();

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

template<>
BlueBrick::FuncDataBase& BlueBrick::HookManager::GetFuncData<Lego::Global>(void(*func)(uint64*, uint64, int, bool)) {
	using namespace Lego;

	if (func == Global::AddToCoins)
		return Global::AddToCoins_data();

	throw noFuncDataException;
}

template<>
BlueBrick::FuncDataBase& BlueBrick::HookManager::GetFuncData<Lego::Global>(int(*func)(void*, int, char*)) {
	using namespace Lego;

	if (func == Global::CreateWindow)
		return Global::CreateWindow_data();

	throw noFuncDataException;
}

template<>
BlueBrick::FuncDataBase& BlueBrick::HookManager::GetFuncData<Lego::Global>(void* (*func)(void* _this, char*, int, int, int, int)) {
	using namespace Lego;

	if (func == Global::CreateAdditionalWindow)
		return Global::CreateAdditionalWindow_data();

	throw noFuncDataException;
}
