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

		Mod* mod;

	public:
		HookManager(Mod* mod);
		
		/// <summary>
		/// Gets the information about the member function for hooking
		/// </summary>
		/// <typeparam name="Ret"> The return type of the function </typeparam>
		/// <typeparam name="...Args"> The argument types of the function </typeparam>
		/// <param name="func"> The mirrored function within the BlueBrick library </param>
		/// <returns> The function information if found </returns>
		template<class Class, typename Ret, typename... Args>
		static FuncDataBase& GetFuncData(Ret(Class::* func)(Args...)) {
			throw noFuncDataException;
		}

		/// <summary>
		/// Gets the information about the static function for hooking
		/// </summary>
		/// <typeparam name="Ret"> The return type of the function </typeparam>
		/// <typeparam name="...Args"> The argument types of the function </typeparam>
		/// <param name="func"> The mirrored function within the BlueBrick library </param>
		/// <returns> The function information if found </returns>
		template<class Class, typename Ret, typename... Args>
		static FuncDataBase& GetFuncData(Ret(*func)(Args...)) {
			throw noFuncDataException;
		}

		/// <summary>
		/// Adds a function to call before the given member function
		/// </summary>
		/// <typeparam name="...Args"> The return type of the function </typeparam>
		/// <typeparam name="Ret"> The argument types of the function </typeparam>
		/// <param name="func"> The mirrored function within the BlueBrick library </param>
		/// <param name="prefix"> The function to be called </param>
		template<class Class, typename Ret, typename... Args>
		Hook& AttachPrefix(Ret(Class::* func)(Args...), Ret(*prefix)(Class*, Args...)) {
			using DataType = FuncData<Ret(Class::*)(Args...)>&;
			DataType data = dynamic_cast<DataType>(GetFuncData(func));

			auto hook = new HookPatch<Ret(*)(Class*, Args...)>(mod, prefix);
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
		Hook& AttachPrefix(Ret(*func)(Args...), Ret(*prefix)(Args...)) {
			using DataType = FuncData<Ret(Args...)>&;
			DataType data = dynamic_cast<DataType>(GetFuncData<Class>(func));

			auto hook = new HookPatch<Ret(*)(Args...)>(mod, prefix);
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
		Hook& AttachPostfix(Ret(Class::* func)(Args...), Ret(*postfix)(Class*, Args...)) {
			using DataType = FuncData<Ret(Class::*)(Args...)>&;
			DataType data = dynamic_cast<DataType>(GetFuncData(func));

			auto hook = new HookPatch<Ret(*)(Class*, Args...)>(mod, postfix);
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
		Hook& AttachPostfix(Ret(*func)(Args...), Ret(*postfix)(Args...)) {
			using DataType = FuncData<Ret(Args...)>&;
			DataType data = dynamic_cast<DataType>(GetFuncData<Class>(func));

			auto hook = new HookPatch<Ret(*)(Args...)>(mod, postfix);
			data.AddPostfix(hook);
			data.ApplyHook();

			return *hook;
		}
	};

}
