#pragma once

#include "Export.hpp"
#include "Severity.hpp"
#include <string>
#include <format>

namespace BlueBrick {
	class Mod;

	class BLUEBRICK_DLL Logger final {
	private:
		Mod* mod;
	public:
		Logger(Mod* mod);

		/// <summary>
		/// Determines if the console has been set up to use color
		/// </summary>
		bool CanUseColor();

		/// <summary>
		/// Writes a message to the log
		/// </summary>
		/// <param name="msg"> The message </param>
		void Message(const std::string& msg);

		/// <summary>
		/// Writes a message to the log, colored by the severity
		/// </summary>
		/// <param name="severity"> The severity of the message </param>
		/// <param name="msg"> The message </param>
		void Message(Severity severity, const std::string& msg);

		/// <summary>
		/// Writes a formatted message to the log, see std::format
		/// </summary>
		/// <param name="fmt"> The message format </param>
		/// <param name="args"> The message args </param>
		template<class... Args>
		void Message(const std::string& fmt, Args&&... args) {
			Message(Severity::Info, fmt, std::forward<Args>(args)...);
		}

		/// <summary>
		/// Writes a formatted message to the log, colored by severity, see std::format
		/// </summary>
		/// <param name="severity"> The severity of the message </param>
		/// <param name="fmt"> The message format </param>
		/// <param name="...args"> The message args </param>
		template<class... Args>
		void Message(Severity severity, const std::string& fmt, Args&&... args) {
			try {
				std::string formatted = std::vformat(fmt, std::make_format_args(args...));
				Message(severity, formatted);
			}
			catch (std::format_error formatError) {
				// should not be a recursive loop as this should not throw an error itself
				Message(Severity::Error, "Format Error: {}", formatError.what());
			}
		}
	};

}