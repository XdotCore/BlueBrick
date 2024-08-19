#pragma once

#include "Export.hpp"
#include "FuncData.hpp"
#include <stdexcept>

namespace BlueBrick {
	class Mod;

	/// <summary>
	/// Manages mod hooking
	/// </summary>
	class BLUEBRICK_DLL HookManager final {
	private:
		static inline std::invalid_argument noFuncDataException = std::invalid_argument("No data for argument func in HookManager::GetFuncData");

		template<class Class, typename Ret, typename... Args>
		static FuncDataBase& GetFuncData(Ret(Class::* func)(Args...)) {
			throw noFuncDataException;
		}

		template<class Class, typename Ret, typename... Args>
		static FuncDataBase& GetFuncData(Ret(*func)(Args...)) {
			throw noFuncDataException;
		}

		Mod* mod;

	public:
		HookManager(Mod* mod);

		/// <summary>
		/// Adds a function to call before the given member function
		/// </summary>
		/// <typeparam name="...Args"> The return type of the function </typeparam>
		/// <typeparam name="Ret"> The argument types of the function </typeparam>
		/// <param name="func"> The mirrored function within the BlueBrick library </param>
		/// <param name="prefix"> The function to be called </param>
		template<class Class, typename Ret, typename... Args>
		Hook& AttachPrefix(Ret(Class::* func)(Args...), FuncData<decltype(func)>::PrefixType prefix) {
			using DataType = FuncData<decltype(func)>&;
			DataType data = dynamic_cast<DataType>(GetFuncData(func));

			auto hook = new HookPatch<decltype(prefix)>(mod, prefix);
			data.AddPrefix(hook);
			data.ApplyHook();

			return *hook;
		}

		/// <summary>
		/// Adds a function to call before the given static function
		/// </summary>
		/// <typeparam name="Ret"> The return type of the function </typeparam>
		/// <typeparam name="...Args"> The argument types of the function </typeparam>
		/// <param name="func"> The mirrored function within the BlueBrick library </param>
		/// <param name="prefix"> The function to be called </param>
		template<class Class, typename Ret, typename... Args>
		Hook& AttachPrefix(Ret(*func)(Args...), FuncData<std::remove_pointer_t<decltype(func)>>::PrefixType prefix) {
			using DataType = FuncData<std::remove_pointer_t<decltype(func)>>&;
			DataType data = dynamic_cast<DataType>(GetFuncData<Class>(func));

			auto hook = new HookPatch<decltype(prefix)>(mod, prefix);
			data.AddPrefix(hook);
			data.ApplyHook();

			return *hook;
		}

		/// <summary>
		/// Adds a function to call after the given member function
		/// </summary>
		/// <typeparam name="...Args"> The return type of the function </typeparam>
		/// <typeparam name="Ret"> The argument types of the function </typeparam>
		/// <param name="func"> The mirrored function within the BlueBrick library </param>
		/// <param name="postfix"> The function to be called </param>
		template<class Class, typename Ret, typename... Args>
		Hook& AttachPostfix(Ret(Class::* func)(Args...), FuncData<decltype(func)>::PostfixType postfix) {
			using DataType = FuncData<decltype(func)>&;
			DataType data = dynamic_cast<DataType>(GetFuncData(func));

			auto hook = new HookPatch<decltype(postfix)>(mod, postfix);
			data.AddPostfix(hook);
			data.ApplyHook();

			return *hook;
		}

		/// <summary>
		/// Adds a function to call after the given static function
		/// </summary>
		/// <typeparam name="...Args"> The return type of the function </typeparam>
		/// <typeparam name="Ret"> The argument types of the function </typeparam>
		/// <param name="func"> The mirrored function within the BlueBrick library </param>
		/// <param name="postfix"> The function to be called </param>
		template<class Class, typename Ret, typename... Args>
		Hook& AttachPostfix(Ret(*func)(Args...), FuncData<std::remove_pointer_t<decltype(func)>>::PostfixType postfix) {
			using DataType = FuncData<std::remove_pointer_t<decltype(func)>>&;
			DataType data = dynamic_cast<DataType>(GetFuncData<Class>(func));

			auto hook = new HookPatch<decltype(postfix)>(mod, postfix);
			data.AddPostfix(hook);
			data.ApplyHook();

			return *hook;
		}
	};

}
