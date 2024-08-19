#pragma once

#include "Hooking/FuncData.hpp"
#include "Logger/Logger.hpp"
#include "Mod/Mod.hpp"

extern BlueBrick::Logger MainLogger;

namespace BlueBrick {

	template<typename FuncType> requires std::is_function_v<FuncType> || std::is_member_function_pointer_v<FuncType>
	class HookableFuncData : public FuncData<FuncType> {
	private:
		void LogException(const char* fix, Mod* mod, const std::exception& e) {
			MainLogger.Message(Severity::Error, "Exception in {} to {} from {}: {}", fix, this->name, mod->GetInfo().Name, e.what());
		}

		void LogUnknown(const char* fix, Mod* mod) {
			MainLogger.Message(Severity::Error, "Unknown thrown in {} to {} from {}", fix, this->name, mod->GetInfo().Name);
		}

	protected:
		using PrefixType = FuncData<FuncType>::PrefixType;
		using PostfixType = FuncData<FuncType>::PostfixType;

		HookableFuncData(const std::string& name) : FuncData<FuncType>(name) {}

		template<typename Ret, typename... Args>
		void CallPrefixes(Args&&... args) {
			for (auto&& prefix : this->prefixHooks) {
				if (!prefix->Enabled())
					continue;

				try {
					prefix->Patch()(args...);
				}
				catch (const std::exception& e) {
					LogException("prefix", prefix->Owner(), e);
				}
				catch (...) {
					LogUnknown("prefix", prefix->Owner());
				}
			}
		}

		template<typename... Args>
		void CallPostfixes(Args&&... args) {
			for (auto&& postfix : this->postfixHooks) {
				if (!postfix->Enabled())
					continue;

				try {
					postfix->Patch()(args...);
				}
				catch (const std::exception& e) {
					LogException("postfix", postfix->Owner(), e);
				}
				catch (...) {
					LogUnknown("postfix", postfix->Owner());
				}
			}
		}

		template<typename Ret, typename... Args>
		void CallPostfixes(Ret& result, Args&&... args) {
			for (auto&& postfix : this->postfixHooks) {
				if (!postfix->Enabled())
					continue;

				try {
					postfix->Patch()(result, args...);
				}
				catch (const std::exception& e) {
					LogException("postfix", postfix->Owner(), e);
				}
				catch (...) {
					LogUnknown("postfix", postfix->Owner());
				}
			}
		}
	};

}