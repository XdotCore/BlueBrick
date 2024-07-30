#include <windows.h>
#include "detours/detours.h"
#include <format>

void AttachHooks() {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	DetourTransactionCommit();
}

BOOL WINAPI DllMain(HINSTANCE dll, DWORD reason, LPVOID _) {
	switch (reason) {
		case DLL_PROCESS_ATTACH: {
			AttachHooks();
		} break;
	}

	return TRUE;
}