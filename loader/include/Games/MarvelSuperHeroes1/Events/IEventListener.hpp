#pragma once

#include "Export.hpp"
#include "Events/Event.hpp"

namespace Lego::Events {

	class BLUEBRICK_DLL IEventListener {
	public:
		void* ptr0x0;
		void* ptr0x4;

		virtual void IEventListener_dtor();
		virtual void RecieveEvent(Event* event, int a);
		virtual bool vfunction3(void* unk);
	};

}