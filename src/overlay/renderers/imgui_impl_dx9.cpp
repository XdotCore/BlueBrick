#include "../imgui/backends/imgui_impl_dx9.h"

extern "C" {
    bool _ImGui_ImplDX9_Init(IDirect3DDevice9* device) { return ImGui_ImplDX9_Init(device); }

    void _ImGui_ImplDX9_NewFrame() { ImGui_ImplDX9_NewFrame(); }
    
    void _ImGui_ImplDX9_RenderDrawData(ImDrawData* draw_data) { return ImGui_ImplDX9_RenderDrawData(draw_data); }
    
    bool _ImGui_ImplDX9_CreateDeviceObjects() { return ImGui_ImplDX9_CreateDeviceObjects(); }

    void _ImGui_ImplDX9_InvalidateDeviceObjects() { ImGui_ImplDX9_InvalidateDeviceObjects(); }
}