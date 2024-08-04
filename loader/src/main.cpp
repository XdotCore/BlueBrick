#include <windows.h>
#include <format>
#include <cstdint>
#include <rcmp.hpp>

void AttachHooks() {
	intptr_t base = (intptr_t)GetModuleHandle(NULL);

	rcmp::hook_function<rcmp::cdecl_t<void(uint64_t*, uint64_t, bool, bool)>>(0x7E1070 + base, [](auto original, uint64_t* coinsPtr, uint64_t toAdd, bool multEnabled, bool param_4) {
		MessageBoxA(NULL, "Coin", "Coin", MB_OK);
		original(coinsPtr, toAdd, multEnabled, param_4);
	});
}

BOOL WINAPI DllMain(HINSTANCE dll, DWORD reason, LPVOID _) {
	switch (reason) {
		case DLL_PROCESS_ATTACH: {
			AttachHooks();
		} break;
	}

	return TRUE;
}