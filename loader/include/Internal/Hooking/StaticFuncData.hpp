#pragma once

#include "Hooking/FuncData.hpp"
#include "Logger/Logger.hpp"
#include "rcmp.hpp"
#include <windows.h>

extern BlueBrick::Logger MainLogger;

namespace BlueBrick {

#if RCMP_GET_ARCH() == RCMP_ARCH_X86
	template<const char* Name, rcmp::cconv callconv, typename FuncType> requires std::is_function_v<FuncType>
	class StaticFuncData : FuncData<FuncType> {};

	template<const char* Name, rcmp::cconv callconv, typename Ret, typename... Args>
	class StaticFuncData<Name, callconv, Ret(Args...)> : public FuncData<Ret(Args...)> {
	public:
		using RcmpType = rcmp::generic_signature_t<Ret(Args...), callconv>;
#else
	template<const char* Name, typename FuncType> requires std::is_function_v<FuncType>
	class StaticFuncData : FuncData<FuncType> {};

	template<const char* Name, typename Ret, typename... Args>
	class StaticFuncData<Name, Ret(Args...)> : public FuncData<Ret(Args...)> {
	public:
		using RcmpType = rcmp::generic_signature_t<Ret(Args...), rcmp::cconv::native_x64>;
#endif
		using CallType = rcmp::from_generic_signature<RcmpType>;

		using base = FuncData<Ret(Args...)>;

		using PrefixType = base::PrefixType;
		using PostfixType = base::PostfixType;

		StaticFuncData(void* ptr) : base(Name, ptr) {}

		void* GetPtr() {
			intptr_t base = (intptr_t)GetModuleHandle(NULL);
			return (void*)((intptr_t)this->ptr + base);
		}

		Ret Call(const Args&... args) override {
			return static_cast<CallType>(GetPtr())(args...);
		}

		void ApplyHook() override {
			static bool applied = false;
			if (applied)
				return;
			applied = true;

			MainLogger.Message(Severity::Debug, "Hooking static function {}", this->name);

			rcmp::hook_function<decltype(*this), RcmpType>(GetPtr(), [this](auto original, Args&&... args) -> Ret {
				for (PrefixType prefix : this->prefixHooks)
					prefix(args...);

				if constexpr (std::is_same_v<Ret, void>) {
					original(args...);

					for (PostfixType postfix : this->postfixHooks)
						postfix(args...);
				}
				else {
					Ret result = original(args...);

					for (PostfixType postfix : this->postfixHooks)
						result = postfix(args...);

					return result;
				}
			});
		}
	};

}
