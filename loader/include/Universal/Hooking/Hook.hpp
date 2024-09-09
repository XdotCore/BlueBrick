#pragma once

#include <type_traits>

namespace BlueBrick {
	class Mod;

	class Hook {
	private:
		Mod* const owner;
		bool enabled;

	public:
		Hook(Mod* owner) : owner(owner), enabled(true) { }

		Mod* Owner() const { return owner; }

		bool Enabled() const { return enabled; }
		void SetEnabled(bool enabled) { this->enabled = enabled; }
	};

	template<typename T>
	class HookPatch final : public Hook {
	private:
		const T patch;

	public:
		HookPatch(Mod* owner, T patch) : Hook(owner), patch(patch) {}

		T Patch() const { return patch; }
	};

}
