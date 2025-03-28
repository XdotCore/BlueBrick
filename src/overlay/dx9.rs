#![allow(nonstandard_style)]

use dlopen::wrapper::{Container, WrapperApi};
use dlopen_derive::WrapperApi;
use retour::static_detour;
use std::{error::Error, sync::OnceLock};
use windows::{
    Win32::{
        Foundation::{HWND, RECT},
        Graphics::{Direct3D9::*, Gdi::RGNDATA},
    },
    core::HRESULT,
};

use super::BackendHelper;

unsafe extern "C" {
    fn _ImGui_ImplDX9_Init(device: *mut *const IDirect3DDevice9_Vtbl) -> bool;
    fn _ImGui_ImplDX9_NewFrame();
    fn _ImGui_ImplDX9_RenderDrawData(draw_data: &imgui::DrawData);
    fn _ImGui_ImplDX9_CreateDeviceObjects() -> bool;
    fn _ImGui_ImplDX9_InvalidateDeviceObjects();
}

#[derive(WrapperApi)]
struct D3D9Api {
    Direct3DCreate9: unsafe extern "system" fn(sdkversion: u32) -> *mut *const IDirect3D9_Vtbl,
    Direct3DCreate9Ex: unsafe extern "system" fn(sdkversion: u32, id3d9ex: *mut *mut *const IDirect3D9Ex_Vtbl) -> HRESULT,
}

static_detour! {
    static Direct3DCreate9Hook: unsafe extern "system" fn(u32) -> *mut *const IDirect3D9_Vtbl;
    static Direct3DCreate9ExHook: unsafe extern "system" fn(u32, *mut *mut *const IDirect3D9Ex_Vtbl) -> HRESULT;
    static Direct3D9_CreateDeviceHook: unsafe extern "system" fn(*mut *const IDirect3D9_Vtbl, u32, D3DDEVTYPE, HWND, u32, *mut D3DPRESENT_PARAMETERS, *mut *mut *const IDirect3DDevice9_Vtbl) -> HRESULT;
    static Direct3D9_CreateDeviceExHook: unsafe extern "system" fn(*mut *const IDirect3D9Ex_Vtbl, u32, D3DDEVTYPE, HWND, u32, *mut D3DPRESENT_PARAMETERS, *mut D3DDISPLAYMODEEX, *mut *mut *const IDirect3DDevice9Ex_Vtbl) -> HRESULT;
    static Direct3D9_Device_Reset: unsafe extern "system" fn(*mut *const IDirect3DDevice9_Vtbl, *mut D3DPRESENT_PARAMETERS) -> HRESULT;
    static Direct3D9_Device_Present: unsafe extern "system" fn(*mut *const IDirect3DDevice9_Vtbl, *const RECT, *const RECT, HWND, *const RGNDATA) -> HRESULT;
}

pub struct Renderer {}

impl Renderer {
    fn hook_device(device: *const *const IDirect3DDevice9_Vtbl) {
        static CALL_ONLY_ONCE: OnceLock<()> = OnceLock::new();

        CALL_ONLY_ONCE.get_or_init(|| {
            unsafe {
                // reset imgui when the game does
                let real_reset = (**device).Reset;
                let real_reset = std::mem::transmute(real_reset);

                match Direct3D9_Device_Reset.initialize(real_reset, |this, presentation_parameters| {
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
                        let mut params = Default::default();
                        let _ = ((**this).GetCreationParameters)(this.cast(), &mut params);
                        Self::get_overlay().platform.set_window(params.hFocusWindow.0.cast());

                        _ImGui_ImplDX9_Init(this);
                    });

                    Self::get_overlay().platform.new_frame();
                    _ImGui_ImplDX9_NewFrame();

                    let draw_data = Self::get_overlay().draw();

                    _ImGui_ImplDX9_RenderDrawData(draw_data);

                    let result = Direct3D9_Device_Present.call(this, source_rect, dest_rect, dest_window_override, dirty_region);

                    Self::get_overlay().post_draw();

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
        // get game device
        unsafe {
            let real_create_device = (**id3d9).CreateDevice;
            let real_create_device = std::mem::transmute(real_create_device);

            match Direct3D9_CreateDeviceHook.initialize(real_create_device, |this, adapter, device_type, focus_window, behavior_flags, presentation_parameters, returned_device_interface| {
                let result = Direct3D9_CreateDeviceHook.call(this, adapter, device_type, focus_window, behavior_flags, presentation_parameters, returned_device_interface);

                Self::hook_device(*returned_device_interface);

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
    }

    fn hook_directx9_ex(id3d9ex: *const *const IDirect3D9Ex_Vtbl) {
        // get game device ex
        unsafe {
            let real_create_device_ex = (**id3d9ex).CreateDeviceEx;
            let real_create_device_ex = std::mem::transmute(real_create_device_ex);

            match Direct3D9_CreateDeviceExHook.initialize(
                real_create_device_ex,
                |this, adapter, device_type, focus_window, behavior_flags, presentation_parameters, fullscreen_display_mode, returned_device_interface| {
                    let result = Direct3D9_CreateDeviceExHook.call(this, adapter, device_type, focus_window, behavior_flags, presentation_parameters, fullscreen_display_mode, returned_device_interface);

                    Self::hook_device(*returned_device_interface.cast());

                    result
                },
            ) {
                Err(e) => {
                    let _ = msgbox::create("Could not hook dx9 CreateDeviceEx", &format!("Error: {e:}"), msgbox::IconType::Error);
                }
                Ok(_) => {
                    if let Err(e) = Direct3D9_CreateDeviceExHook.enable() {
                        let _ = msgbox::create("Could not enable dx9 CreateDeviceEx hook", &format!("Error: {e:}"), msgbox::IconType::Error);
                    }
                }
            };
        }
    }

    fn attach_hooks() -> Result<(), Box<dyn Error>> {
        let cont = unsafe { Container::<D3D9Api>::load("d3d9.dll")? };

        unsafe {
            Direct3DCreate9Hook.initialize(cont.Direct3DCreate9, |sdkversion| {
                let id3d9 = Direct3DCreate9Hook.call(sdkversion);
                Self::hook_directx9(id3d9);
                id3d9
            })?;
            Direct3DCreate9Hook.enable()?;

            Direct3DCreate9ExHook.initialize(cont.Direct3DCreate9Ex, |sdkversion, id3d9ex| {
                let result = Direct3DCreate9ExHook.call(sdkversion, id3d9ex);
                Self::hook_directx9(*id3d9ex.cast());
                Self::hook_directx9_ex(*id3d9ex);
                result
            })?;
            Direct3DCreate9ExHook.enable()?;
        }

        Ok(())
    }

    fn init() -> Result<(), Box<dyn Error>> {
        Self::attach_hooks()?;

        Ok(())
    }

    pub fn new() -> Result<Self, Box<dyn Error>> {
        Self::init()?;
        Ok(Self {})
    }
}

impl super::Backend for Renderer {}

impl super::Renderer for Renderer {}

impl super::BackendHelper<Self> for Renderer {}
impl super::RendererHelper<Self> for Renderer {}
