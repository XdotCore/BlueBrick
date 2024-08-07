#pragma once

#include "rcmp.hpp"
#include "Hooking/ClassManager.hpp"
#include "Logger/Logger.hpp"
#include <string>

extern BlueBrick::Logger MainLogger;

namespace BlueBrick::HookHelpers {

	template<class Tag, class Class, typename Ret, typename... Args>
	void HookFunction(Ret(Class::* func)(Args...), const std::string& name) {
		FuncData* funcData = ClassManager<Class>::GetFuncData(func);

		auto hook = [funcData, name](auto original, Class* _this, Args... args) -> Ret {
			using PrefixCallType = Ret(*)(Class*, Args...);
			using PostfixCallType = Ret(*)(Class*, Args...);

			MainLogger.Message("Calling {}", name);

			for (void* prefix : *funcData->GetPrefixes()) {
				((PrefixCallType)prefix)(_this, std::forward<Args>(args)...);
			}

			if constexpr (std::is_same_v<Ret, void>) {
				original(_this, std::forward<Args>(args)...);

				for (void* postfix : *funcData->GetPostfixes())
					((PostfixCallType)postfix)(_this, std::forward<Args>(args)...);
			}
			else {
				Ret result = original(_this, std::forward<Args>(args)...);

				for (void* postfix : *funcData->GetPostfixes())
					result = ((PostfixCallType)postfix)(_this, std::forward<Args>(args)...);

				return result;
			}
		};

		if (funcData->IsVirtual()) {
			switch (funcData->CallConv()) {
				case CallConv::Cdecl:
					rcmp::hook_indirect_function<Tag, rcmp::cdecl_t<Ret(Class*, Args...)>>(funcData->GetVftableAtIndex(), hook);
					break;
				case CallConv::Stdcall:
					rcmp::hook_indirect_function<Tag, rcmp::stdcall_t<Ret(Class*, Args...)>>(funcData->GetVftableAtIndex(), hook);
					break;
				case CallConv::Fastcall:
					rcmp::hook_indirect_function<Tag, rcmp::fastcall_t<Ret(Class*, Args...)>>(funcData->GetVftableAtIndex(), hook);
					break;
				case CallConv::Thiscall:
					rcmp::hook_indirect_function<Tag, rcmp::thiscall_t<Ret(Class*, Args...)>>(funcData->GetVftableAtIndex(), hook);
					break;
				default:
					rcmp::hook_indirect_function<Tag, Ret(Class*, Args...)>(funcData->GetVftableAtIndex(), hook);
					break;
			}
		}
		else {
			switch (funcData->CallConv()) {
				case CallConv::Cdecl:
					rcmp::hook_function<Tag, rcmp::cdecl_t<Ret(Class*, Args...)>>(funcData->GetFunc(), hook);
					break;
				case CallConv::Stdcall:
					rcmp::hook_function<Tag, rcmp::stdcall_t<Ret(Class*, Args...)>>(funcData->GetFunc(), hook);
					break;
				case CallConv::Fastcall:
					rcmp::hook_function<Tag, rcmp::fastcall_t<Ret(Class*, Args...)>>(funcData->GetFunc(), hook);
					break;
				case CallConv::Thiscall:
					rcmp::hook_function<Tag, rcmp::thiscall_t<Ret(Class*, Args...)>>(funcData->GetFunc(), hook);
					break;
				default:
					rcmp::hook_function<Tag, Ret(Class*, Args...)>(funcData->GetFunc(), hook);
					break;
			}
		}
	}

}