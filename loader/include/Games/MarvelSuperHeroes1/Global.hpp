#pragma once

#include "Export.hpp"
#include "Hooking/ClassManager.hpp"

namespace Lego {

	class BLUEBRICK_DLL Global final {
	public:
		static int RunGame(int cmdLineArgCount, char** cmdLineArgs);

	private:
		static BlueBrick::FuncData<decltype(RunGame)>& RunGame_data();

		friend BlueBrick::ClassManager<Global>;
	};

}

template<>
template<typename Ret, typename... Args>
BlueBrick::FuncDataBase& BlueBrick::ClassManager<Lego::Global>::GetFuncData(Ret(*func)(Args...)) {
	using namespace Lego;

	if (IsSameAndEqual(func, Global::RunGame))
		return Global::RunGame_data();

	throw noFuncDataException;
}
