#include "Events/IEventListener.hpp"

namespace Lego::Events {

	void IEventListener::IEventListener_dtor() {

	}

	void IEventListener::RecieveEvent(Event* event, NuEventData * data) {

	}

	bool IEventListener::vfunction3(void* unk) {
		return false;
	}

}