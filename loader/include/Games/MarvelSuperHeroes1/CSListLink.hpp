#pragma once

#include "Export.hpp"

namespace Lego {
	
	template<class T>
	class BLUEBRICK_DLL CSListLink {
	public:
		T* prev;
		T* next;
	};

}