#pragma once

#include "Export.hpp"
#include "FuncData.hpp"

namespace BlueBrick {

	/// <summary>
	/// A static class that manages the hooking of classes
	/// </summary>
	/// <typeparam name="Class"> The class that it is working on </typeparam>
	template<class Class>
	class ClassManager final {
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

		static inline bool calledInit = false;
		static void Init() { }

	public:
		
		/// <summary>
		/// Gets the information about the member function for hooking
		/// </summary>
		/// <typeparam name="Ret"> The return type of the function </typeparam>
		/// <typeparam name="...Args"> The argument types of the function </typeparam>
		/// <param name="func"> The mirrored function within the BlueBrick library </param>
		/// <returns> The function information, or nullptr if not found </returns>
		template<typename Ret, typename... Args>
		static FuncData* GetFuncData(Ret(Class::* func)(Args...)) {
			return nullptr;
		}

		/// <summary>
		/// Calls the original member function (with hooks)
		/// </summary>
		/// <typeparam name="Ret"> The return type of the function </typeparam>
		/// <typeparam name="...Args"> The argument types of the function </typeparam>
		/// <param name="func"> The mirrored function within the BlueBrick library </param>
		/// <param name="_this"> The class instance to be passed into the function </param>
		/// <param name="args"> The arguments to be passed into the function </param>
		/// <returns> The result of calling the function, or a default value if the function is not found </returns>
		template<typename Ret, typename... Args>
		static Ret CallFunc(Ret(Class::* func)(Args...), Class* _this, Args... args) {
			FuncData* data = GetFuncData(func);
			if (data == nullptr) {
				if constexpr (std::is_same_v<Ret, void>)
					return;
				else
					return {};
			}

			void* toCall = data->GetFunc();

			// TODO: find a way to make this into templates possibly
			switch (data->CallConv()) {
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

		/// <summary>
		/// Adds a function to call before the given function
		/// </summary>
		/// <typeparam name="...Args"> The return type of the function </typeparam>
		/// <typeparam name="Ret"> The argument types of the function </typeparam>
		/// <param name="func"> The mirrored function within the BlueBrick library </param>
		/// <param name="prefix"> The function to be called </param>
		template<typename Ret, typename... Args>
		static void AttachPrefix(Ret(Class::* func)(Args...), Ret(*prefix)(Class*, Args...)) {
			FuncData* data = GetFuncData(func);
			if (data == nullptr)
				return;

			data->AddPrefix(prefix);

			if (!calledInit) {
				Init();
				calledInit = true;
			}
		}

		/// <summary>
		/// Adds a function to call after the given function
		/// </summary>
		/// <typeparam name="...Args"> The return type of the function </typeparam>
		/// <typeparam name="Ret"> The argument types of the function </typeparam>
		/// <param name="func"> The mirrored function within the BlueBrick library </param>
		/// <param name="postfix"> The function to be called </param>
		template<typename Ret, typename... Args>
		static void AttachPostfix(Ret(Class::* func)(Args...), Ret(*postfix)(Class*, Args...)) {
			FuncData* data = GetFuncData(func);
			if (data == nullptr)
				return;

			data->AddPostfix(postfix);

			if (!calledInit) {
				Init();
				calledInit = true;
			}
		}
	};

}
