#include "Mod/ModLoader.hpp"
#include "Logger/Logger.hpp"
#include <filesystem>
#include <windows.h>

extern BlueBrick::Logger MainLogger;

namespace BlueBrick {

	ModLoader& ModLoader::instance() {
		static ModLoader instance;
		return instance;
	}

	static char const* exceptionToString(DWORD code) {
#define LOG_CODE(x) case x: return #x
		switch (code) {
			LOG_CODE(EXCEPTION_ACCESS_VIOLATION);
			LOG_CODE(EXCEPTION_DATATYPE_MISALIGNMENT);
			LOG_CODE(EXCEPTION_BREAKPOINT);
			LOG_CODE(EXCEPTION_SINGLE_STEP);
			LOG_CODE(EXCEPTION_ARRAY_BOUNDS_EXCEEDED);
			LOG_CODE(EXCEPTION_FLT_DENORMAL_OPERAND);
			LOG_CODE(EXCEPTION_FLT_DIVIDE_BY_ZERO);
			LOG_CODE(EXCEPTION_FLT_INEXACT_RESULT);
			LOG_CODE(EXCEPTION_FLT_INVALID_OPERATION);
			LOG_CODE(EXCEPTION_FLT_OVERFLOW);
			LOG_CODE(EXCEPTION_FLT_STACK_CHECK);
			LOG_CODE(EXCEPTION_FLT_UNDERFLOW);
			LOG_CODE(EXCEPTION_INT_DIVIDE_BY_ZERO);
			LOG_CODE(EXCEPTION_INT_OVERFLOW);
			LOG_CODE(EXCEPTION_PRIV_INSTRUCTION);
			LOG_CODE(EXCEPTION_IN_PAGE_ERROR);
			LOG_CODE(EXCEPTION_ILLEGAL_INSTRUCTION);
			LOG_CODE(EXCEPTION_NONCONTINUABLE_EXCEPTION);
			LOG_CODE(EXCEPTION_STACK_OVERFLOW);
			LOG_CODE(EXCEPTION_INVALID_DISPOSITION);
			LOG_CODE(EXCEPTION_GUARD_PAGE);
			LOG_CODE(EXCEPTION_INVALID_HANDLE);
			default: return "Unknown";
		}
#undef LOG_CODE
	}

	static long __stdcall HandleSEH(LPEXCEPTION_POINTERS ExceptionInfo) {
		unsigned int code = ExceptionInfo->ExceptionRecord->ExceptionCode;

		static constexpr unsigned int __fatalexception = 0x80000000; // codes below this are not actual errors
		static constexpr unsigned int __cppexception = 0xE06D7363; // these should be handled by try/catch blocks
		if (code >= __fatalexception && code != __cppexception) {
			const char* err = exceptionToString(code);
			MainLogger.Message(Severity::Error, "FATAL ERROR: {} (0x{:X}) found in BlueBrick or a mod.", err, code);
			MessageBoxA(NULL, std::format("{} (0x{:X}) found in BlueBrick or a mod.", err, code).c_str(), "FATAL ERROR", MB_ICONERROR);
		}

		return EXCEPTION_CONTINUE_SEARCH;
	}

	void ModLoader::Start() {
		MainLogger.Message("Running BlueBrick v{} in {}", BLUEBRICK_VERSION, GAME_NAME);

		AddVectoredExceptionHandler(0, HandleSEH);

		LoadMods();
		Overlay::instance().AttachHooks();
	}

	void ModLoader::LoadMods() {
		const std::filesystem::path modsDir = "BlueBrick/Mods";
		std::filesystem::create_directories(modsDir);

		// first pass: load dlls
		for (const std::filesystem::directory_entry& modFile : std::filesystem::directory_iterator(modsDir)) {
			if (!std::filesystem::is_regular_file(modFile) || !modFile.path().has_extension() || modFile.path().extension().string() != ".dll")
				continue;

			currentDll = modFile.path().filename().string();
			LoadLibraryA(modFile.path().string().c_str());
		}

		// second pass: process mods that added themselves
		for (Mod* mod : loadedMods) {
			ModInfo* info;
			try {
				info = &mod->GetInfo();
			}
			catch (const std::exception& e) {
				MainLogger.Message(Severity::Error, "Could not load info for mod in {}, mod not loaded. (Exception {})", mod->Dll, e.what());
				continue;
			}
			catch (...) {
				MainLogger.Message(Severity::Error, "Could not load info for mod in {}, mod not loaded. (Unknown thrown)", mod->Dll);
				continue;
			}

			try {
				mod->OnInitialized();
				MainLogger.Message("Loaded {1}{0}{5} v{2} by {4}{3}{5} in {6}",
				                   /*0*/	info->Name,
				                   /*1*/	info->StartNameColor(),
				                   /*2*/	info->Version,
				                   /*3*/	info->Author,
				                   /*4*/	info->StartAuthorColor(),
				                   /*5*/	ColorBase::End(),
				                   /*6*/	mod->Dll);
			}
			catch (const std::exception& e) {
				MainLogger.Message(Severity::Error, "Failed to load {} v{} by {} from {}. (Exception {})", info->Name, info->Version, info->Author, mod->Dll, e.what());
			}
			catch (...) {
				MainLogger.Message(Severity::Error, "Failed to load {} v{} by {} from {}. (Unknown thrown)", info->Name, info->Version, info->Author, mod->Dll);
			}
		}
	}

	void ModLoader::AddMod(Mod* mod) {
		loadedMods.push_back(mod);
	}

	const std::string& ModLoader::GetDll() {
		return currentDll;
	}

}
