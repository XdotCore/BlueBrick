#pragma once

namespace BlueBrick {

	class Overlay {
	private:
		bool isShowing = true;

		bool showDemoWindow = false;

	protected: // stuff needed for platform specifics can go in here and below
		bool isSetUp = false;

		bool fullscreen = true;
		int windowWidth = -1;
		int windowHeight = -1;

		void Start();
		void Draw();

		virtual void ToggleFullscreen() = 0;

	public:
		static Overlay& instance();

		virtual void AttachHooks() = 0;
	};

}
