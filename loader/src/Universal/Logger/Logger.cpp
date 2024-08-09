#include "Logger/Logger.hpp"
#include "Logger/Color/Color.hpp"
#include "Mod/Mod.hpp"
#include <windows.h>
#include <iostream>
#include <chrono>

// The logger instance used for main BlueBrick logs
extern BlueBrick::Logger MainLogger;

namespace BlueBrick {

	bool canUseColor = false;

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

	static bool InitLogger() {
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
		InitLogger();
		return canUseColor;
	}

	void Logger::Message(const std::string& message) {
		Logger::Message(Severity::Info, message);
	}

	void Logger::Message(Severity severity, const std::string& message) {
		if (InitLogger()) {
			bool isMain = this == &MainLogger;

			// colors
			std::string timeColor;
			std::string typeColor;
			std::string textColor;
			std::string nameColor = isMain ? Color::DeepSkyBlue().Start() : mod->GetInfo().StartNameColor();

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
			std::cout << timeColor << timeText << Color::End();

			// message type
			std::string typeText = isMain ? "Loader" : "Mod";
			std::cout << "[" << typeColor << typeText << Color::End() << "] ";

			// mod name
			std::string nameText = isMain ? "BlueBrick" : mod->GetInfo().Name;
			std::cout << "[" << nameColor << nameText << Color::End() << "]: ";

			// message
			std::cout << textColor << message << Color::End() << std::endl;
		}
	}

}
