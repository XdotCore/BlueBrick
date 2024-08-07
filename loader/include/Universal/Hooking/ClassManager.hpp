#pragma once

#include "Export.hpp"
#include "FuncData.hpp"
#include <utility>

namespace BlueBrick {

	/// <summary>
	/// A static class that manages the hooking of classes
	/// </summary>
	/// <typeparam name="Class"> The class that it is working on </typeparam>
	template<class Class>
	class BLUEBRICK_DLL ClassManager final {
	private:
		ClassManager() = delete;
		ClassManager(const ClassManager&) = delete;
		ClassManager& operator =(const ClassManager&) = delete;
		ClassManager(ClassManager&&) = delete;
		ClassManager& operator =(ClassManager&&) = delete;

		template<typename A, typename B>
		static bool IsSameAndEqual(A a, B b) {
			if constexpr (std::is_same_v<A, B>) {
				if (a == b)
					return true;
			}
			return false;
		}

	public:
		
		/// <summary>
		/// Gets the information about the member function for hooking
		/// </summary>
		/// <typeparam name="Ret"> The return type of the function </typeparam>
		/// <typeparam name="...Args"> The argument types of the function </typeparam>
		/// <param name="func"> The mirrored function within the BlueBrick library </param>
		/// <returns> The function information </returns>
		template<typename Ret, typename... Args>
		static FuncData GetFuncData(Ret(Class::* func)(Args...));

		/// <summary>
		/// Calls the original member function (with hooks)
		/// </summary>
		/// <typeparam name="Ret"> The return type of the function </typeparam>
		/// <typeparam name="...Args"> The argument types of the function </typeparam>
		/// <param name="func"> The mirrored function within the BlueBrick library </param>
		/// <returns> The result of calling the function </returns>
		template<typename Ret, typename... Args>
		static Ret CallFunc(Ret(Class::* func)(Args...), Class* _this, Args... args) {
			FuncData data = GetFuncData(func);
			void* toCall = data.GetFunc();

			// TODO: find a way to make this into generics possibly
			switch (data.CallConv()) {
				case CallConv::Cdecl: {
					using CallType = Ret(__cdecl*)(Class*, Args...);
					return ((CallType)toCall)(_this, std::forward<Args>(args)...);
				}
				case CallConv::Stdcall: {
					using CallType = Ret(__stdcall*)(Class*, Args...);
					return ((CallType)toCall)(_this, std::forward<Args>(args)...);
				}
				case CallConv::Fastcall: {
					using CallType = Ret(__fastcall*)(Class*, Args...);
					return ((CallType)toCall)(_this, std::forward<Args>(args)...);
				}
				case CallConv::Thiscall: {
					using CallType = Ret(__thiscall*)(Class*, Args...);
					return ((CallType)toCall)(_this, std::forward<Args>(args)...);
				}
				default: {
					using CallType = Ret(*)(Class*, Args...);
					return ((CallType)toCall)(_this, std::forward<Args>(args)...);
				}
			}
		}
	};

}
