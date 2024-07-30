#include <windows.h>
#include <string>

struct XINPUT_STATE;
struct XINPUT_VIBRATION;
struct XINPUT_CAPABILITIES;

constexpr static size_t MAX_PATH_UNICODE = 32767;

static HMODULE getRealXInput() {
	std::wstring path(MAX_PATH_UNICODE, '\0');
	size_t length = GetSystemDirectoryW(const_cast<wchar_t*>(path.data()), MAX_PATH_UNICODE);

	if (length > 0) {
		path.resize(length);
		path += L"\\XInput1_3.dll";
		return LoadLibraryW(path.c_str());
	}

	return NULL;
}

static FARPROC getRealFunc(const std::string& name) {
	static HMODULE xInput = getRealXInput();

	if (xInput)
		return GetProcAddress(xInput, name.c_str());

	return NULL;
}

extern "C" DWORD XInputGetState(DWORD dwUserIndex, XINPUT_STATE* pState) {
	using XInputGetStateFunc = decltype(&XInputGetState);
	static XInputGetStateFunc realXInputGetState =
		reinterpret_cast<XInputGetStateFunc>(getRealFunc("XInputGetState"));

	if (realXInputGetState)
		return realXInputGetState(dwUserIndex, pState);

	return ERROR_DEVICE_NOT_CONNECTED;
}

extern "C" DWORD XInputSetState(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration) {
	using XInputSetStateFunc = decltype(&XInputSetState);
	static XInputSetStateFunc realXInputSetState =
		reinterpret_cast<XInputSetStateFunc>(getRealFunc("XInputSetState"));

	if (realXInputSetState)
		return realXInputSetState(dwUserIndex, pVibration);

	return ERROR_DEVICE_NOT_CONNECTED;
}

extern "C" DWORD XInputGetCapabilities(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES* pCapabilities) {
	using XInputGetCapabilitiesFunc = decltype(&XInputGetCapabilities);
	static XInputGetCapabilitiesFunc realXInputGetCapabilities =
		reinterpret_cast<XInputGetCapabilitiesFunc>(getRealFunc("XInputGetCapabilities"));

	if (realXInputGetCapabilities)
		return realXInputGetCapabilities(dwUserIndex, dwFlags, pCapabilities);

	return ERROR_DEVICE_NOT_CONNECTED;
}

BOOL WINAPI DllMain(HINSTANCE dll, DWORD reason, LPVOID _) {
	switch (reason) {
		case DLL_PROCESS_ATTACH:
			if (!LoadLibraryW(L"BlueBrick.dll"))
				MessageBoxW(NULL, L"Failed to load BlueBrick.dll.\nCheck if it has been installed correctly.", L"Error", MB_OK);

			break;
	}

	return TRUE;
}