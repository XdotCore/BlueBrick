#pragma once

#include "Export.hpp"
#include "FuncData.hpp"
#include <optional>
#include <stdexcept>
#include <format>

namespace BlueBrick {

	/// <summary>
	/// A static class that manages the hooking of classes
	/// </summary>
	class HookManager final {
	private:
		HookManager() = delete;
		HookManager(const HookManager&) = delete;
		HookManager& operator =(const HookManager&) = delete;
		HookManager(HookManager&&) = delete;
		HookManager& operator =(HookManager&&) = delete;

		static inline std::invalid_argument noFuncDataException = std::invalid_argument("No data for argument func in HookManager::GetFuncData");

	public:
		
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
		static void AttachPrefix(Ret(Class::* func)(Args...), Ret(*prefix)(Class*, Args...)) {
			using DataType = FuncData<Ret(Class::*)(Args...)>&;
			DataType data = dynamic_cast<DataType>(GetFuncData(func));

			data.AddPrefix(prefix);
			data.ApplyHook();
		}

		/// <summary>
		/// Adds a function to call before the given static function
		/// </summary>
		/// <typeparam name="Ret"> The return type of the function </typeparam>
		/// <typeparam name="...Args"> The argument types of the function </typeparam>
		/// <param name="func"> The mirrored function within the BlueBrick library </param>
		/// <param name="prefix"> The function to be called </param>
		template<class Class, typename Ret, typename... Args>
		static void AttachPrefix(Ret(*func)(Args...), Ret(*prefix)(Args...)) {
			using DataType = FuncData<Ret(Args...)>&;
			DataType data = dynamic_cast<DataType>(GetFuncData<Class>(func));

			data.AddPrefix(prefix);
			data.ApplyHook();
		}

		/// <summary>
		/// Adds a function to call after the given member function
		/// </summary>
		/// <typeparam name="...Args"> The return type of the function </typeparam>
		/// <typeparam name="Ret"> The argument types of the function </typeparam>
		/// <param name="func"> The mirrored function within the BlueBrick library </param>
		/// <param name="postfix"> The function to be called </param>
		template<class Class, typename Ret, typename... Args>
		static void AttachPostfix(Ret(Class::* func)(Args...), Ret(*postfix)(Class*, Args...)) {
			using DataType = FuncData<Ret(Class::*)(Args...)>&;
			DataType data = dynamic_cast<DataType>(GetFuncData(func));

			data.AddPostfix(postfix);
			data.ApplyHook();
		}

		/// <summary>
		/// Adds a function to call after the given static function
		/// </summary>
		/// <typeparam name="...Args"> The return type of the function </typeparam>
		/// <typeparam name="Ret"> The argument types of the function </typeparam>
		/// <param name="func"> The mirrored function within the BlueBrick library </param>
		/// <param name="postfix"> The function to be called </param>
		template<class Class, typename Ret, typename... Args>
		static void AttachPostfix(Ret(*func)(Args...), Ret(*postfix)(Args...)) {
			using DataType = FuncData<Ret(Args...)>&;
			DataType data = dynamic_cast<DataType>(GetFuncData<Class>(func));

			data.AddPostfix(postfix);
			data.ApplyHook();
		}
	};

}
