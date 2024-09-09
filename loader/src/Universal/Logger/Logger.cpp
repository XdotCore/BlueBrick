#include "Logger/Logger.hpp"
#include "Logger/Color.hpp"
#include "Mod/Mod.hpp"
#include "Mod/Overlay.hpp"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <regex>
#include <vector>

// The logger instance used for main BlueBrick logs
BlueBrick::Logger MainLogger = BlueBrick::Logger(nullptr);

namespace BlueBrick {

	static bool canUseColor = false;

	static void EnableColor(HANDLE outputHandle) {
		DWORD dwMode = 0;
		if (!GetConsoleMode(outputHandle, &dwMode)) {
			canUseColor = false;
			return;
		}

		dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		if (!SetConsoleMode(outputHandle, dwMode)) {
			canUseColor = false;
			return;
		}

		canUseColor = true;
	}

	static bool InitConsole() {
		static HANDLE outputHandle = nullptr;

		if (outputHandle)
			return true;

		if (!AllocConsole())
			return false;

		// Get output handle
		outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		if (!outputHandle) {
			outputHandle = CreateFileA("CONOUT$", GENERIC_WRITE, 0, nullptr, 0, 0, nullptr);
			HANDLE inputHandle = CreateFileA("CONIN$", GENERIC_READ, 0, nullptr, 0, 0, nullptr);
			SetStdHandle(STD_OUTPUT_HANDLE, outputHandle);
			SetStdHandle(STD_INPUT_HANDLE, inputHandle);
			SetStdHandle(STD_ERROR_HANDLE, outputHandle);
		}

		// check if output handle is invalid
		if (!outputHandle || outputHandle == INVALID_HANDLE_VALUE) {
			outputHandle = nullptr;
			FreeConsole();
			return false;
		}

		// connect std to new output handle
		FILE* discard;
		if (freopen_s(&discard, "CONOUT$", "w", stdout) ||
			freopen_s(&discard, "CONIN$", "r", stdin) ||
			freopen_s(&discard, "CONOUT$", "w", stderr)) {
			outputHandle = nullptr;
			FreeConsole();
			return false;
		}

		// enable output types
		SetConsoleCP(CP_UTF8);
		SetConsoleOutputCP(CP_UTF8);
		EnableColor(outputHandle);

		return true;
	}

	Logger::Logger(Mod* mod) : mod(mod) {
	}

	bool Logger::CanUseColor() {
		InitConsole();
		return canUseColor;
	}

	void Logger::Message(const std::string& message) {
		Logger::Message(Severity::Info, message);
	}

	void Logger::Message(Severity severity, const std::string& message) {
		std::stringstream string;

		bool isMain = this == &MainLogger;

		// colors
		std::string timeColor;
		std::string typeColor;
		std::string textColor;
		std::string nameColor = isMain ? Color::DeepSkyBlue().Start() : mod->GetInfo().NameColor.Start();

		switch (severity) {
			case Severity::Debug: {
			// the web color "gray" is darker than "dark gray" ...
				timeColor = Color::Gray().Start();
				typeColor = textColor = Color::DarkGray().Start();
			} break;
			case Severity::Warning: {
				timeColor = Color::Goldenrod().Start();
				typeColor = textColor = Color::Yellow().Start();
			} break;
			case Severity::Error: {
				timeColor = Color::Firebrick().Start();
				typeColor = textColor = Color::Red().Start();
			} break;
			default: {
				timeColor = Color::Gray().Start();
				typeColor = textColor = Color::End();
			} break;
		}

		// time
		auto now = std::chrono::system_clock::now();
		auto nowLocal = std::chrono::current_zone()->to_local(now);
		std::string timeText = std::format("{:%H:%M:%OS} ", nowLocal);
		string << timeColor << timeText << Color::End();

		// message type
		std::string typeText = isMain ? "Loader" : "Mod";
		string << "[" << typeColor << typeText << Color::End() << "] ";

		// mod name
		std::string nameText = isMain ? "BlueBrick" : mod->GetInfo().Name;
		string << "[" << nameColor << nameText << Color::End() << "]: ";

		// message
		string << textColor << message << Color::End() << std::endl;

		std::string msg = string.str();

		// output to console
		if (InitConsole())
			std::cout << msg;

		// output to overlay
		static std::regex overlayItemReg("\x1b\\[(38;2);([\\d]{1,3});([\\d]{1,3});([\\d]{1,3})m|\x1b\\[0m|[\r\n]+");
		std::vector<Overlay::LogItemTypes> overlayItems;

		std::smatch colorMatch;
		std::string::const_iterator searchStart = msg.cbegin();
		while (std::regex_search(searchStart, msg.cend(), colorMatch, overlayItemReg)) {
			// push string between matches
			overlayItems.push_back(colorMatch.prefix().str());

			// allow me to do newlines seperately
			if (colorMatch.str()[0] == '\n' || colorMatch.str()[0] == '\r') {
				overlayItems.push_back("\n");
			}
			// color
			if (colorMatch[1].str() == "38;2") {
				byte r = std::stoi(colorMatch[2].str());
				byte g = std::stoi(colorMatch[3].str());
				byte b = std::stoi(colorMatch[4].str());
				Color c(r, g, b);

				overlayItems.push_back(c);
			}
			// reset color
			else {
				overlayItems.push_back(Color::None());
			}

			searchStart = colorMatch.suffix().first;
		}

		// add trailing text after last item
		if (searchStart != msg.cend())
			overlayItems.push_back(std::string(searchStart, msg.cend()));

		Overlay::instance().AddLogItems(overlayItems);

		// output to log
		static std::ofstream file("BlueBrick/log.txt", std::ofstream::trunc);
		static std::regex escapes("\x1b.*?m");
		std::string colorless = std::regex_replace(msg, escapes, "");
		file << colorless;
		file.flush();
	}

}
