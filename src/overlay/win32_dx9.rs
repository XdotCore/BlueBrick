#![allow(nonstandard_style)]

use dlopen::wrapper::{Container, WrapperApi};
use dlopen_derive::WrapperApi;
use retour::static_detour;
use std::{error::Error, ptr, sync::OnceLock};
use windows::{
    Win32::{
        Foundation::{HWND, LPARAM, LRESULT, RECT, WPARAM},
        Graphics::{Direct3D9::*, Gdi::RGNDATA},
        UI::WindowsAndMessaging::{CallWindowProcA, DefWindowProcA, GWLP_WNDPROC, SetWindowLongPtrA, WA_ACTIVE, WM_ACTIVATE, WM_KEYFIRST, WM_KEYLAST, WM_KILLFOCUS, WM_MOUSEFIRST, WM_MOUSELAST, WM_SETFOCUS, WM_SYSKEYDOWN, WNDPROC},
    },
    core::{BOOL, HRESULT},
};

use crate::overlay::OVERLAY_INSTANCE;

// statically linked imgui backend functions
unsafe extern "C" {
    fn _ImGui_ImplWin32_Init(hwnd: HWND) -> bool;
    fn _ImGui_ImplWin32_NewFrame();
    fn _ImGui_ImplWin32_WndProcHandler(hWnd: HWND, msg: u32, wParam: WPARAM, lParam: LPARAM) -> BOOL;

    fn _ImGui_ImplDX9_Init(device: *mut *const IDirect3DDevice9_Vtbl) -> bool;
    fn _ImGui_ImplDX9_NewFrame();
    fn _ImGui_ImplDX9_RenderDrawData(draw_data: &imgui::DrawData);
    fn _ImGui_ImplDX9_CreateDeviceObjects() -> bool;
    fn _ImGui_ImplDX9_InvalidateDeviceObjects();
}

#[derive(WrapperApi)]
struct D3D9Api {
    Direct3DCreate9: unsafe extern "system" fn(sdkversion: u32) -> *mut *const IDirect3D9_Vtbl,
    #[allow(non_snake_case)]
    Direct3DCreate9Ex: unsafe extern "system" fn(sdkversion: u32, id3d9ex: *mut *mut *const IDirect3D9Ex_Vtbl) -> HRESULT,
}

static_detour! {
    static Direct3DCreate9Hook: unsafe extern "system" fn(u32) -> *mut *const IDirect3D9_Vtbl;
    static Direct3DCreate9ExHook: unsafe extern "system" fn(u32, *mut *mut *const IDirect3D9Ex_Vtbl) -> HRESULT;
    static Direct3D9_CreateDeviceHook: unsafe extern "system" fn(*mut *const IDirect3D9_Vtbl, i32, D3DDEVTYPE, HWND, i32, *mut D3DPRESENT_PARAMETERS, *mut *mut *const IDirect3DDevice9_Vtbl) -> HRESULT;
    static Direct3D9_Device_Reset: unsafe extern "system" fn(*mut *const IDirect3DDevice9_Vtbl, *mut D3DPRESENT_PARAMETERS) -> HRESULT;
    static Direct3D9_Device_Present: unsafe extern "system" fn(*mut *const IDirect3DDevice9_Vtbl, *const RECT, *const RECT, HWND, *const RGNDATA) -> HRESULT;
}

static mut WINDOW: HWND = unsafe { std::mem::zeroed() };
static mut PARAMS: D3DPRESENT_PARAMETERS = unsafe { std::mem::zeroed() };
static mut DEVICE: *mut *const IDirect3DDevice9_Vtbl = ptr::null_mut();
static mut TRUEWNDPROC: WNDPROC = None;

fn FakeWndProc(hwnd: HWND, mut msg: u32, mut wparam: WPARAM, lparam: LPARAM) -> LRESULT {
    unsafe {
        let io = imgui::sys::igGetIO();

        if _ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam).as_bool() {
            return LRESULT(true as isize);
        }

        // eat message to disable mouse and keyboard passing through imgui
        if ((*io).WantCaptureMouse && msg >= WM_MOUSEFIRST && msg <= WM_MOUSELAST) || ((*io).WantCaptureKeyboard && msg >= WM_KEYFIRST && msg <= WM_KEYLAST) {
            return LRESULT(true as isize);
        }

        match msg {
            // keep the game from not rendering
            WM_ACTIVATE => {
                wparam = WPARAM(WA_ACTIVE as usize);
            }
            WM_KILLFOCUS => {
                msg = WM_SETFOCUS;
            }

            // allow alt f4
            WM_SYSKEYDOWN => {
                return DefWindowProcA(hwnd, msg, wparam, lparam);
            }

            _ => {}
        }

        return CallWindowProcA(TRUEWNDPROC, hwnd, msg, wparam, lparam);
    }
}

fn hook_device(device: *const *const IDirect3DDevice9_Vtbl) {
    static CALL_ONLY_ONCE: OnceLock<()> = OnceLock::new();

    CALL_ONLY_ONCE.get_or_init(|| {
        unsafe {
            // reset imgui when the game does
            let real_reset = (**device).Reset;
            let real_reset = std::mem::transmute(real_reset);

            match Direct3D9_Device_Reset.initialize(real_reset, |this, presentation_parameters| {
                PARAMS = *presentation_parameters;

                _ImGui_ImplDX9_InvalidateDeviceObjects();
                let result = Direct3D9_Device_Reset.call(this, presentation_parameters);
                _ImGui_ImplDX9_CreateDeviceObjects();

                return result;
            }) {
                Err(e) => {
                    let _ = msgbox::create("Could not hook dx9 Reset", &format!("Error: {e:}"), msgbox::IconType::Error);
                }
                Ok(_) => {
                    if let Err(e) = Direct3D9_Device_Reset.enable() {
                        let _ = msgbox::create("Could not enable dx9 Reset hook", &format!("Error: {e:}"), msgbox::IconType::Error);
                    }
                }
            }

            // get present for rendering
            let real_present = (**device).Present;
            let real_present = std::mem::transmute(real_present);

            match Direct3D9_Device_Present.initialize(real_present, |this, source_rect, dest_rect, dest_window_override, dirty_region| {
                static CALL_ONLY_ONCE: OnceLock<()> = OnceLock::new();
                CALL_ONLY_ONCE.get_or_init(|| {
                    DEVICE = this;
                    let mut params = Default::default();
                    let _ = ((**DEVICE).GetCreationParameters)(DEVICE.cast(), &mut params);
                    WINDOW = params.hFocusWindow;

                    TRUEWNDPROC = std::mem::transmute(SetWindowLongPtrA(WINDOW, GWLP_WNDPROC, std::mem::transmute(FakeWndProc as *const ())));

                    //ImGui_ImplWin32_EnableDpiAwareness();
                    _ImGui_ImplWin32_Init(WINDOW);
                    _ImGui_ImplDX9_Init(DEVICE);
                });

                _ImGui_ImplWin32_NewFrame();
                _ImGui_ImplDX9_NewFrame();

                let draw_data = (*OVERLAY_INSTANCE).draw();

                _ImGui_ImplDX9_RenderDrawData(draw_data);

                let result = Direct3D9_Device_Present.call(this, source_rect, dest_rect, dest_window_override, dirty_region);

                (*OVERLAY_INSTANCE).post_draw();

                return result;
            }) {
                Err(e) => {
                    let _ = msgbox::create("Could not hook dx9 Present", &format!("Error: {e:}"), msgbox::IconType::Error);
                }
                Ok(_) => {
                    if let Err(e) = Direct3D9_Device_Present.enable() {
                        let _ = msgbox::create("Could not enable dx9 Present hook", &format!("Error: {e:}"), msgbox::IconType::Error);
                    }
                }
            }
        }
    });
}

fn hook_directx9(id3d9: *const *const IDirect3D9_Vtbl) {
    static CALL_ONLY_ONCE: OnceLock<()> = OnceLock::new();

    CALL_ONLY_ONCE.get_or_init(|| {
        // get game device
        unsafe {
            let real_create_device = (**id3d9).CreateDevice;
            let real_create_device = std::mem::transmute(real_create_device);

            match Direct3D9_CreateDeviceHook.initialize(real_create_device, |this, adapter, device_type, focus_window, behavior_flags, presentation_parameters, returned_device_interface| {
                WINDOW = focus_window;
                PARAMS = *presentation_parameters;

                let result = Direct3D9_CreateDeviceHook.call(this, adapter, device_type, focus_window, behavior_flags, presentation_parameters, returned_device_interface);

                hook_device(*returned_device_interface);

                result
            }) {
                Err(e) => {
                    let _ = msgbox::create("Could not hook dx9 CreateDevice", &format!("Error: {e:}"), msgbox::IconType::Error);
                }
                Ok(_) => {
                    if let Err(e) = Direct3D9_CreateDeviceHook.enable() {
                        let _ = msgbox::create("Could not enable dx9 CreateDevice hook", &format!("Error: {e:}"), msgbox::IconType::Error);
                    }
                }
            }
        }
    });
}

fn attach_hooks() -> Result<(), Box<dyn Error>> {
    let cont: Container<D3D9Api> = unsafe { Container::load("d3d9.dll")? };

    unsafe {
        Direct3DCreate9Hook.initialize(cont.Direct3DCreate9, |sdkversion| {
            let id3d9 = Direct3DCreate9Hook.call(sdkversion);
            hook_directx9(id3d9);
            id3d9
        })?;
        Direct3DCreate9Hook.enable()?;

        Direct3DCreate9ExHook.initialize(cont.Direct3DCreate9Ex, |sdkversion, id3d9ex| {
            let result = Direct3DCreate9ExHook.call(sdkversion, id3d9ex);
            hook_directx9(*id3d9ex.cast());
            result
        })?;
        Direct3DCreate9ExHook.enable()?;
    }

    Ok(())
}

pub fn init_imgui() -> Result<(), Box<dyn Error>> {
    attach_hooks()?;

    Ok(())
}
