#include "Hooking/FuncData.hpp"
#include <windows.h>

namespace BlueBrick {

	bool FuncData::IsVirtual() const {
		return isVirtual;
	}

	CallConv FuncData::CallConv() const {
		return callConv;
	}

	void* FuncData::Ptr() const {
		return ptr;
	}

	FuncData::FuncData(void* ptr, BlueBrick::CallConv callConv) :
		isVirtual(false),
		callConv(callConv),
		ptr(ptr),
		vftable(nullptr),
		index(0) { }

	void** FuncData::Vftable() const {
		return vftable;
	}

	unsigned int FuncData::Index() const {
		return index;
	}

	FuncData::FuncData(void** vftable, unsigned int index, BlueBrick::CallConv callConv) :
		isVirtual(true),
		callConv(callConv),
		ptr(nullptr),
		vftable(vftable),
		index(index) { }

	intptr_t offsetByBase(void* ptr) {
		static intptr_t base = (intptr_t)GetModuleHandle(NULL);

		return (intptr_t)ptr + base;
	}

	void* FuncData::GetFunc() const {
		if (isVirtual) {
			void** mem = (void**)offsetByBase(vftable);
			return mem[index];
		}
		else
			return (void*)offsetByBase(ptr);
	}

	void* FuncData::GetVftableAtIndex() const {
		if (isVirtual)
			return (void*)(offsetByBase(vftable) + sizeof(void*) * index);
		else
			return nullptr;
	}

	void FuncData::AddPrefix(void* prefix) {
		prefixHooks.push_back(prefix);
	}

	const std::vector<void*>& FuncData::GetPrefixes() const {
		return prefixHooks;
	}

	void FuncData::AddPostfix(void* postfix) {
		postfixHooks.push_back(postfix);
	}

	const std::vector<void*>& FuncData::GetPostfixes() const {
		return postfixHooks;
	}

}
