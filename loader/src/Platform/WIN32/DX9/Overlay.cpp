#include "Overlay.hpp"
#include "Logger/Logger.hpp"
#include "Logger/Color/Color.hpp"
#include "rcmp.hpp"
#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx9.h"
#include <format>

extern BlueBrick::Logger MainLogger;

// copied here as instructed by the imgui docs gods
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace BlueBrick {

	LRESULT CALLBACK Overlay_WIN32_DX9::FakeWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		static ImGuiIO& io = ImGui::GetIO();

		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return true;

		// eat message to disable mouse and keyboard passing through imgui
		if ((io.WantCaptureMouse && msg >= WM_MOUSEFIRST && msg <= WM_MOUSELAST)  ||
			(io.WantCaptureKeyboard && msg >= WM_KEYFIRST && msg <= WM_KEYLAST))
			return true;

		switch (msg) {
			// keep the game from not rendering
			case WM_ACTIVATE:
				wParam = WA_ACTIVE;
				break;
			case WM_KILLFOCUS:
				msg = WM_SETFOCUS;
				break;

			// allow alt f4
			case WM_SYSKEYDOWN:
				return DefWindowProcA(hWnd, msg, wParam, lParam);
		}

		return CallWindowProcA(TrueWndProc, hWnd, msg, wParam, lParam);
	}

	void Overlay_WIN32_DX9::HookDevice(IDirect3DDevice9* device) {
		static bool called = false;
		if (called)
			return;
		called = true;

		// reset imgui when the game does
		rcmp::hook_function<decltype(device->lpVtbl->Reset)>(device->lpVtbl->Reset, [this](auto original, IDirect3DDevice9* This, D3DPRESENT_PARAMETERS* pPresentationParameters) -> HRESULT {
			Params = *pPresentationParameters;

			ToggleFullscreen();
			*pPresentationParameters = Params;

			ImGui_ImplDX9_InvalidateDeviceObjects();
			HRESULT result = original(This, pPresentationParameters);
			ImGui_ImplDX9_CreateDeviceObjects();

			return result;
		});

		// get endscene for rendering
		rcmp::hook_function<decltype(device->lpVtbl->EndScene)>(device->lpVtbl->EndScene, [this](auto original, IDirect3DDevice9* This) -> HRESULT {
			if (!isSetUp) {
				Device = This;
				D3DDEVICE_CREATION_PARAMETERS params;
				Device->lpVtbl->GetCreationParameters(Device, &params);
				Window = params.hFocusWindow;

				TrueWndProc = (WNDPROC)SetWindowLongPtrA(Window, GWLP_WNDPROC, (LONG)FakeWndProc);

				Start();
				//ImGui_ImplWin32_EnableDpiAwareness();
				ImGui_ImplWin32_Init(Window);
				ImGui_ImplDX9_Init(Device);

				isSetUp = true;
			}

			ImGui_ImplWin32_NewFrame();
			ImGui_ImplDX9_NewFrame();

			Draw();

			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

			HRESULT result = original(This);

			PostDraw();

			return result;
		});

		// keep cursor visible
		rcmp::hook_function<decltype(device->lpVtbl->ShowCursor)>(device->lpVtbl->ShowCursor, [](auto original, IDirect3DDevice9* This, BOOL bShow) -> BOOL {
			return original(This, true);
		});
	}

	void Overlay_WIN32_DX9::HookDirectX9(IDirect3D9* id3d9) {
		static bool called = false;
		if (called)
			return;
		called = true;

		// get game device
		rcmp::hook_function<decltype(id3d9->lpVtbl->CreateDevice)>(id3d9->lpVtbl->CreateDevice, [this](auto original, IDirect3D9* This, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface) -> HRESULT {
			Window = hFocusWindow;
			Params = *pPresentationParameters;
			
			ToggleFullscreen();
			*pPresentationParameters = Params;

			HRESULT result = original(This, Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);

			HookDevice(*ppReturnedDeviceInterface);

			return result;
		});
	}

	void Overlay_WIN32_DX9::AttachHooks() {
		HMODULE d3d9 = LoadLibraryA("d3d9.dll");
		if (!d3d9) {
			MainLogger.Message(Severity::Error, "Could not load d3d9.dll");
			return;
		}
		FARPROC direct3DCreate9 = GetProcAddress(d3d9, "Direct3DCreate9");
		FARPROC direct3DCreate9Ex = GetProcAddress(d3d9, "Direct3DCreate9Ex");

		// hook create directx 9
		rcmp::hook_function<decltype(Direct3DCreate9)*>(direct3DCreate9, [this](auto original, UINT SDKVersion) -> IDirect3D9* {
			ID3D9 = original(SDKVersion);
			HookDirectX9(ID3D9);
			return ID3D9;
		});

		// hook create directx 9 ex
		rcmp::hook_function<decltype(Direct3DCreate9Ex)*>(direct3DCreate9Ex, [this](auto original, UINT SDKVersion, IDirect3D9Ex** id3d9ex) -> HRESULT {
			HRESULT result = original(SDKVersion, id3d9ex);
			ID3D9 = reinterpret_cast<IDirect3D9*>(*id3d9ex);
			HookDirectX9(ID3D9);
			return result;
		});

		// keep cursor visible
		rcmp::hook_function<SetCursor>([](auto original, auto...) -> HCURSOR {
			return original(LoadCursorA(NULL, IDC_ARROW));
		});

		rcmp::hook_function<ShowCursor>([](auto original, auto...) -> int {
			return original(true);
		});

		// stop mouse from being unusable
		rcmp::hook_function<RegisterRawInputDevices>([](auto original, PCRAWINPUTDEVICE pRawInputDevices, UINT uiNumDevices, UINT cbSize) -> BOOL {
			constexpr DWORD unusable = RIDEV_NOLEGACY | RIDEV_CAPTUREMOUSE | RIDEV_APPKEYS | RIDEV_NOHOTKEYS;
			for (int i = 0; i < uiNumDevices; i++) {
				if (pRawInputDevices[i].dwFlags & unusable)
					const_cast<RAWINPUTDEVICE*>(pRawInputDevices)[i].dwFlags &= ~unusable;
			}
			return original(pRawInputDevices, uiNumDevices, cbSize);
		});

		// don't move cursor outside of user control
		rcmp::hook_function<SetCursorPos>([](auto...) -> BOOL {
			return false;
		});
	}

	void Overlay_WIN32_DX9::ToggleFullscreen() {
		Params.Windowed = true;
		Params.FullScreen_RefreshRateInHz = 0;

		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);
		if (fullscreen) {
			SetWindowLongPtrA(Window, GWL_STYLE, WS_POPUP | WS_VISIBLE);

			SetWindowPos(Window, HWND_NOTOPMOST, 0, 0, screenWidth, screenHeight, SWP_NOSENDCHANGING | SWP_NOZORDER | SWP_ASYNCWINDOWPOS | SWP_NOREPOSITION | SWP_SHOWWINDOW | SWP_NOACTIVATE);
		}
		else {
			SetWindowLongPtrA(Window, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);

			if (windowWidth == -1)
				windowWidth = screenWidth * .75f;
			if (windowHeight == -1)
				windowHeight = screenHeight * .75f;

			if (windowX == -1)
				windowX = (screenWidth - windowWidth) / 2;
			if (windowY == -1)
				windowY = (screenHeight - windowHeight) / 2;

			Params.BackBufferWidth = windowWidth;
			Params.BackBufferHeight = windowHeight;

			SetWindowPos(Window, HWND_NOTOPMOST, windowX, windowY, windowWidth, windowHeight, SWP_NOSENDCHANGING | SWP_NOZORDER | SWP_ASYNCWINDOWPOS | SWP_NOREPOSITION | SWP_SHOWWINDOW | SWP_NOACTIVATE);
		}
	}

}