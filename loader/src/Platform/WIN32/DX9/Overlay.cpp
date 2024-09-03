#include "Overlay.hpp"
#include "Logger/Logger.hpp"
#include "Logger/Color/Color.hpp"
#include "rcmp.hpp"
#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx9.h"

extern BlueBrick::Logger MainLogger;

// copied here as instructed by the imgui docs gods
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace BlueBrick {

	LRESULT CALLBACK Overlay_WIN32_DX9::FakeWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		static ImGuiIO& io = ImGui::GetIO();

		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return true;

		// eat message through default proc to disable mouse passing through imgui
		if (io.WantCaptureMouse) {
			if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
				return true;
			return false;
		}

		// keep the window from not rendering
		switch (msg) {
			case WM_ACTIVATE:
				wParam = WA_ACTIVE;
				break;
			case WM_KILLFOCUS:
				msg = WM_SETFOCUS;
				break;
		}

		return TrueWndProc(hWnd, msg, wParam, lParam);
	}

	void Overlay_WIN32_DX9::HookDevice(IDirect3DDevice9* device) {
		// reset imgui when the game does
		rcmp::hook_function<decltype(device->lpVtbl->Reset)>(device->lpVtbl->Reset, [](auto original, IDirect3DDevice9* This, D3DPRESENT_PARAMETERS* pPresentationParameters) -> HRESULT {
			// I know I will sometimes be copying over what is already there, but it doesn't really matter too much
			Params = *pPresentationParameters;

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
				ImGui_ImplWin32_Init(Window);
				ImGui_ImplDX9_Init(Device);

				isSetUp = true;
			}

			ImGui_ImplWin32_NewFrame();
			ImGui_ImplDX9_NewFrame();

			Draw();

			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

			return original(This);
		});

		// keep cursor visible
		rcmp::hook_function<decltype(device->lpVtbl->ShowCursor)>(device->lpVtbl->ShowCursor, [](auto original, IDirect3DDevice9* This, BOOL bShow) -> BOOL {
			return original(This, true);
		});
	}

	void Overlay_WIN32_DX9::HookDirectX9(IDirect3D9* id3d9) {
		// get game device
		rcmp::hook_function<decltype(id3d9->lpVtbl->CreateDevice)>(id3d9->lpVtbl->CreateDevice, [this](auto original, IDirect3D9* This, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface) -> HRESULT {
			HRESULT result = original(This, Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);

			Params = *pPresentationParameters;
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
			ID3D9 = reinterpret_cast<IDirect3D9*>(id3d9ex);
			HookDirectX9(ID3D9);
			return original(SDKVersion, id3d9ex);
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
			constexpr DWORD unusable = RIDEV_CAPTUREMOUSE | RIDEV_NOLEGACY;
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
		if (fullscreen) {

		}
		else {

		}
	}

}