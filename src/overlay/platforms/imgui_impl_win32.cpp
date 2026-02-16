#include "../imgui/backends/imgui_impl_win32.h"
#include <windows.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

extern "C" {
    bool _ImGui_ImplWin32_Init(HWND hwnd) { return ImGui_ImplWin32_Init(hwnd); }

    void _ImGui_ImplWin32_NewFrame() { ImGui_ImplWin32_NewFrame(); }

    LRESULT _ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) { return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam); }
}