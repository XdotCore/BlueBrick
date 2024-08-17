#pragma once

#include "Hooking/FuncData.hpp"
#include "Logger/Logger.hpp"

extern BlueBrick::Logger MainLogger;

namespace BlueBrick {

	template<typename FuncType> requires std::is_function_v<FuncType> || std::is_member_function_pointer_v<FuncType>
	class HookableFuncData : public FuncData<FuncType> {
	protected:
		using PrefixType = FuncData<FuncType>::PrefixType;
		using PostfixType = FuncData<FuncType>::PostfixType;

		HookableFuncData(const std::string& name) : FuncData<FuncType>(name) {}

		template<typename Ret, typename... Args>
		void CallPrefixes(Args&&... args) {
			for (PrefixType prefix : this->prefixHooks) {
				try {
					prefix(args...);
				}
				catch (const std::exception& e) {
					MainLogger.Message(Severity::Error, "Exception in prefix to {}: {}", this->name, e.what());
				}
				catch (...) {
					MainLogger.Message(Severity::Error, "Unknown thrown in prefix to {}", this->name);
				}
			}
		}

		template<typename... Args>
		void CallPostfixes(Args&&... args) {
			for (PostfixType postfix : this->postfixHooks) {
				try {
					postfix(args...);
				}
				catch (const std::exception& e) {
					MainLogger.Message(Severity::Error, "Exception in postfix to {}: {}", this->name, e.what());
				}
				catch (...) {
					MainLogger.Message(Severity::Error, "Unknown thrown in postfix to {}", this->name);
				}
			}
		}

		template<typename Ret, typename... Args>
		void CallPostfixes(Ret& result, Args&&... args) {
			for (PostfixType postfix : this->postfixHooks) {
				try {
					result = postfix(args...);
				}
				catch (const std::exception& e) {
					MainLogger.Message(Severity::Error, "Exception in postfix to {}: {}", this->name, e.what());
				}
				catch (...) {
					MainLogger.Message(Severity::Error, "Unknown thrown in postfix to {}", this->name);
				}
			}
		}
	};

}