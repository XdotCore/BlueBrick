#include "Logger/Logger.hpp"
#include "Logger/Color.hpp"
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

	bool InitLogger() {
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
		if (InitLogger()) {
			bool isMain = this == &MainLogger;

			//time
			auto now = std::chrono::system_clock::now();
			auto nowLocal = std::chrono::current_zone()->to_local(now);
			std::cout << std::format("{:%H:%M:%OS} ", nowLocal);

			// mod name
			std::cout << "[" << (isMain ? "Loader" : "Mod") << "] ";
			std::cout << "[" << (isMain ? "BlueBrick" : mod->GetInfo().Name) << "]: ";

			// message
			std::cout << message << Color::End() << std::endl;
		}
	}

}
