#pragma once

#include "Mod/Overlay.hpp"
#define CINTERFACE
#include <d3d9.h>
#undef CINTERFACE
#include <windows.h>

namespace BlueBrick {

	class Overlay_WIN32_DX9 final : public Overlay {
	private:
		static inline IDirect3D9* ID3D9 = nullptr;
		static inline IDirect3DDevice9* Device = nullptr;
		static inline D3DPRESENT_PARAMETERS Params = {};

		static inline HWND Window = nullptr;
		static inline WNDPROC TrueWndProc = nullptr;

		static LRESULT CALLBACK FakeWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		void HookDevice(IDirect3DDevice9* device);
		void HookDirectX9(IDirect3D9* id3d9);

	protected:
		void ToggleFullscreen() override;

	public:
		void AttachHooks() override;
	};

}