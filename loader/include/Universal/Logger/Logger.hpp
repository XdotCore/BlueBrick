#pragma once

#include <string>
#include <format>
#include "Export.hpp"

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
		void Message(std::string msg);

		/// <summary>
		/// Writes a formatted message to the log, see std::format
		/// </summary>
		/// <param name="fmt"> The message format </param>
		/// <param name="args"> The message args </param>
		template <class... Args>
		void Message(std::string fmt, Args&&... args) {
			std::string formatted = std::vformat(fmt, std::make_format_args(args...));
			Message(formatted);
		}
	};

}