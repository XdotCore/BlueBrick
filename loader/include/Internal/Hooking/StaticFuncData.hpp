#pragma once

#include "HookableFuncData.hpp"
#include "Logger/Logger.hpp"
#include "rcmp.hpp"
#include <windows.h>

extern BlueBrick::Logger MainLogger;

namespace BlueBrick {

#if RCMP_GET_ARCH() == RCMP_ARCH_X86
	template<intptr_t ptr, rcmp::cconv callconv, typename FuncType> requires std::is_function_v<FuncType>
	class StaticFuncData : HookableFuncData<FuncType> {};

	template<intptr_t ptr, rcmp::cconv callconv, typename Ret, typename... Args>
	class StaticFuncData<ptr, callconv, Ret(Args...)> : public HookableFuncData<Ret(Args...)> {
	public:
		using RcmpType = rcmp::generic_signature_t<Ret(Args...), callconv>;
#else
	template<intptr_t ptr, typename FuncType> requires std::is_function_v<FuncType>
	class StaticFuncData : HookableFuncData<FuncType> {};

	template<intptr_t ptr, typename Ret, typename... Args>
	class StaticFuncData<ptr, Ret(Args...)> : public HookableFuncData<Ret(Args...)> {
	public:
		using RcmpType = rcmp::generic_signature_t<Ret(Args...), rcmp::cconv::native_x64>;
#endif
		using CallType = rcmp::from_generic_signature<RcmpType>;

		using base = HookableFuncData<Ret(Args...)>;

		StaticFuncData(const std::string& name) : base(name) {}

		void* GetPtr() {
			intptr_t base = (intptr_t)GetModuleHandle(NULL);
			return (void*)(ptr + base);
		}

		Ret Call(const Args&... args) override {
			return reinterpret_cast<CallType>(GetPtr())(args...);
		}

		void ApplyHook() override {
			static bool applied = false;
			if (applied)
				return;
			applied = true;

			MainLogger.Message(Severity::Debug, "Hooking static function {}", this->name);

			rcmp::hook_function<decltype(*this), RcmpType>(GetPtr(), [this](auto original, Args&&... args) -> Ret {
				this->template CallPrefixes<Ret, Args...>(std::forward<Args>(args)...);
				if constexpr (std::is_same_v<Ret, void>) {
					original(args...);
					this->template CallPostfixes<Args...>(std::forward<Args>(args)...);
				}
				else {
					Ret result = original(args...);
					this->template CallPostfixes<Ret, Args...>(result, std::forward<Args>(args)...);
					return result;
				}
			});
		}
	};

}
