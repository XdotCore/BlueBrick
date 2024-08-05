#pragma once

#include "Events/Event.hpp"

namespace Lego::Events {

	class IEventListener {
	public:
		void* ptr0x0;
		void* ptr0x4;

		virtual void IEventListener_dtor();
		virtual void RecieveEvent(Event* event);
		virtual bool vfunction3(void* unk);
	};

}