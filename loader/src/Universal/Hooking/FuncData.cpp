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

	FuncData::FuncData(void* ptr, BlueBrick::CallConv callConv) : isVirtual(false), callConv(callConv), ptr(ptr), vftable(nullptr), index(0) {}

	void** FuncData::Vftable() const {
		return vftable;
	}

	unsigned int FuncData::Index() const {
		return index;
	}

	FuncData::FuncData(void** vftable, unsigned int index, BlueBrick::CallConv callConv) : isVirtual(true), callConv(callConv), ptr(nullptr), vftable(vftable), index(index) {}

	void* FuncData::GetFunc() const {
		static intptr_t base = (intptr_t)GetModuleHandle(NULL);

		if (isVirtual) {
			void** mem = (void**)((intptr_t)vftable + base);
			return mem[index];
		}
		else
			return (void*)((intptr_t)ptr + base);
	}

}
