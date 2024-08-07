#pragma once

#include "Export.hpp"
#include "CallConv.hpp"

namespace BlueBrick {

	/// <summary>
	/// Represents the information needed to hook a function
	/// </summary>
	struct BLUEBRICK_DLL FuncData final {
	private:
		bool isVirtual;
		CallConv callConv;

		// non-virtual
		void* ptr;

		// virtual
		void** vftable;
		unsigned int index;
	public:
		/// <summary>
		/// Gets whether or not this function comes from a vftable
		/// </summary>
		bool IsVirtual() const;

		/// <summary>
		/// Gets the calling convention of the function
		/// </summary>
		CallConv CallConv() const;

		/// <summary>
		/// Gets the function pointer for a non-virtual function
		/// </summary>
		void* Ptr() const;

		/// <summary>
		/// Instantiates data for a non-virtual function
		/// </summary>
		/// <param name="ptr"> The function pointer for a non-virtual function </param>
		FuncData(void* ptr, BlueBrick::CallConv callConv = BlueBrick::CallConv::None);

		/// <summary>
		/// Gets the vftable for a virtual function
		/// </summary>
		void** Vftable() const;

		/// <summary>
		/// Gets the index in the vftable for the virtual function
		/// </summary>
		unsigned int Index() const;

		/// <summary>
		/// Instantiates data for a virtual function
		/// </summary>
		/// <param name="vftable"> The vftable pointer for a virtual function </param>
		/// <param name="index"> The index in the vftable for the virtual function </param>
		FuncData(void** vftable, unsigned int index, BlueBrick::CallConv callConv = BlueBrick::CallConv::None);

		/// <summary>
		/// Gets the pointer for the function
		/// </summary>
		void* GetFunc() const;
	};

}
