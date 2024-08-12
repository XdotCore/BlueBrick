#pragma once

#include "Export.hpp"
#include <vector>
#include <string>

namespace BlueBrick {

	class FuncDataBase {
	public:
		std::string name;

		FuncDataBase(const std::string& name) : name(name) { }

		virtual void ApplyHook() = 0;
	};

	template<typename FuncType> requires std::is_function_v<FuncType> || std::is_member_function_pointer_v<FuncType>
	class FuncData : public FuncDataBase {};

	template<class Class, typename Ret, typename... Args>
	class FuncData<Ret(Class::*)(Args...)> : public FuncDataBase {
	protected:
		using PrefixType = Ret(*)(Class*, Args...);
		using PostfixType = Ret(*)(Class*, Args...);

		std::vector<PrefixType> prefixHooks;
		std::vector<PostfixType> postfixHooks;
	public:
		FuncData(const std::string& name) : FuncDataBase(name) { }

		virtual Ret Call(Class* _this, const Args&... args) = 0;

		void AddPrefix(PrefixType prefix) { prefixHooks.push_back(prefix); }
		void AddPostfix(PostfixType postfix) { postfixHooks.push_back(postfix); }
	};

	template<typename Ret, typename... Args>
	class FuncData<Ret(Args...)> : public FuncDataBase {
	protected:
		using PrefixType = Ret(*)(Args...);
		using PostfixType = Ret(*)(Args...);

		std::vector<PrefixType> prefixHooks;
		std::vector<PostfixType> postfixHooks;
	public:
		FuncData(const std::string& name) : FuncDataBase(name) { }

		virtual Ret Call(const Args&... args) = 0;

		void AddPrefix(PrefixType prefix) { prefixHooks.push_back(prefix); }
		void AddPostfix(PostfixType postfix) { postfixHooks.push_back(postfix); }
	};

}
