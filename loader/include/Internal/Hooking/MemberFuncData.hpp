#pragma once

#include "Hooking/FuncData.hpp"
#include "Logger/Logger.hpp"
#include "rcmp.hpp"
#include <windows.h>

extern BlueBrick::Logger MainLogger;

namespace BlueBrick {

#if RCMP_GET_ARCH() == RCMP_ARCH_X86
	template<const char* Name, rcmp::cconv callconv, typename FuncType, class Base> requires std::is_member_function_pointer_v<FuncType>
	class MemberFuncData : FuncData<FuncType> {};

	template<const char* Name, rcmp::cconv callconv, class Base, class Class, typename Ret, typename... Args> requires std::is_base_of_v<Base, Class>
	class MemberFuncData<Name, callconv, Ret(Class::*)(Args...), Base> : public FuncData<Ret(Class::*)(Args...)> {
	public:
		using RcmpType = rcmp::generic_signature_t<Ret(Base*, Args...), callconv>;
#else
	template<const char* Name, typename FuncType, class Base> requires std::is_member_function_pointer_v<FuncType>
	class MemberFuncData : FuncData<FuncType> {};

	template<const char* Name, class Base, class Class, typename Ret, typename... Args> requires std::is_base_of_v<Base, Class>
	class MemberFuncData<Name, Ret(Class::*)(Args...), Base> : public FuncData<Ret(Class::*)(Args...)> {
	public:
		using RcmpType = rcmp::generic_signature_t<Ret(Base*, Args...), rcmp::cconv::native_x64>;
#endif
		using CallType = rcmp::from_generic_signature<RcmpType>;

		using base = FuncData<Ret(Class::*)(Args...)>;

		using PrefixType = base::PrefixType;
		using PostfixType = base::PostfixType;

		MemberFuncData(void** vftable, int index) : base(Name, vftable, index) {}

		void* GetPtr() {
			intptr_t base = (intptr_t)GetModuleHandle(NULL);
			return ((void**)((intptr_t)this->vftable + base))[this->index];
		}

		void* GetVftableAtIndex() {
			intptr_t base = (intptr_t)GetModuleHandle(NULL);
			return (void*)((intptr_t)this->vftable + base + sizeof(void*) * this->index);
		}

		Ret Call(Class* _this, const Args&... args) override {
			Base* _base = static_cast<Base*>(_this);
			return static_cast<CallType>(GetPtr())(_base, args...);
		}

		void ApplyHook() override {
			static bool applied = false;
			if (applied)
				return;
			applied = true;

			MainLogger.Message(Severity::Debug, "Hooking member function {}", this->name);

			rcmp::hook_indirect_function<decltype(*this), RcmpType>(GetVftableAtIndex(), [this](auto original, Base* _base, Args&&... args) -> Ret {
				Class* _this = static_cast<Class*>(_base);

				for (PrefixType prefix : this->prefixHooks)
					prefix(_this, args...);

				if constexpr (std::is_same_v<Ret, void>) {
					original(_base, args...);

					for (PostfixType postfix : this->postfixHooks)
						postfix(_this, args...);
				}
				else {
					Ret result = original(_base, args...);

					for (PostfixType postfix : this->postfixHooks)
						result = postfix(_this, args...);

					return result;
				}
			});
		}
	};



}
