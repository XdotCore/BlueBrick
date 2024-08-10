#pragma once

#include "Export.hpp"
#include "Event.hpp"
#include "EventData/NuEventData.hpp"

namespace Lego::Events {

	class BLUEBRICK_DLL IEventListener {
	public:
		void* ptr_0x0;
		void* ptr_0x4;

		virtual void IEventListener_dtor();
		virtual void RecieveEvent(Event* event, NuEventData* data);
		virtual bool vfunction3(void* unk);
	};

}