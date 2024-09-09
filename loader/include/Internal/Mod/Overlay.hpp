#pragma once

#include "Logger/Color.hpp"
#include <string>
#include <vector>
#include <variant>
#include <optional>

namespace BlueBrick {

	class Overlay {
	public:
		// TODO: add hyperlinks to mod
		using LogItemTypes = std::variant<std::string,
		                                  Color>;

	private:
		bool isShowing = true;

		bool showBlueBrickMenu = false;
		bool showBricksMenu = false;

		bool showDemoWindow = false;

		bool showLogs = false;
		std::vector<LogItemTypes> logItems;
		bool logChanged = false;
		void ShowLogs();

	protected: // stuff needed for platform specifics can go in here and below
		bool isSetUp = false;

		bool fullscreen = true;
		int windowWidth = -1;
		int windowHeight = -1;
		int windowX = -1;
		int windowY = -1;

		void Start();
		void Draw();
		void PostDraw();

		virtual void ToggleFullscreen() = 0;

	public:
		static Overlay& instance();

		virtual void AttachHooks() = 0;

		void AddLogItems(const std::vector<LogItemTypes>& items);
	};

}
