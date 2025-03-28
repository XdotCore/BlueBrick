#![allow(nonstandard_style)]

use std::{error::Error, slice::from_raw_parts_mut};

use dlopen::wrapper::{Container, WrapperApi};
use dlopen_derive::WrapperApi;
use retour::static_detour;
use windows::{
    Win32::{
        Foundation::{FALSE, HWND, LPARAM, LRESULT, TRUE, WPARAM},
        UI::{
            Input::{RAWINPUTDEVICE, RAWINPUTDEVICE_FLAGS, RIDEV_APPKEYS, RIDEV_CAPTUREMOUSE, RIDEV_NOHOTKEYS, RIDEV_NOLEGACY},
            WindowsAndMessaging::{CallWindowProcA, DefWindowProcA, GWLP_WNDPROC, SetWindowLongPtrA, WA_ACTIVE, WM_ACTIVATE, WM_KEYFIRST, WM_KEYLAST, WM_KILLFOCUS, WM_MOUSEFIRST, WM_MOUSELAST, WM_SETFOCUS, WM_SYSKEYDOWN, WNDPROC},
        },
    },
    core::BOOL,
};

unsafe extern "C" {
    fn _ImGui_ImplWin32_Init(hwnd: HWND) -> bool;
    fn _ImGui_ImplWin32_NewFrame();
    fn _ImGui_ImplWin32_WndProcHandler(hWnd: HWND, msg: u32, wParam: WPARAM, lParam: LPARAM) -> BOOL;
}

#[derive(WrapperApi)]
struct User32Api {
    RegisterRawInputDevices: unsafe extern "system" fn(raw_input_devices: *mut RAWINPUTDEVICE, num_devices: u32, size: u32) -> BOOL,
    ShowCursor: unsafe extern "system" fn(bshow: BOOL) -> i32,
    SetCursorPos: unsafe extern "system" fn(x: i32, y: i32) -> BOOL,
}

static_detour! {
    static RegisterRawInputDevicesHook: unsafe extern "system" fn(*mut RAWINPUTDEVICE, u32, u32) -> BOOL;
    static ShowCursorHook: unsafe extern "system" fn(BOOL) -> i32;
    static SetCursorPosHook: unsafe extern "system" fn (i32, i32) -> BOOL;
}

static mut WINDOW: HWND = unsafe { std::mem::zeroed() };
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

pub fn set_window(hwnd: HWND) {
    unsafe {
        WINDOW = hwnd;

        TRUEWNDPROC = std::mem::transmute(SetWindowLongPtrA(WINDOW, GWLP_WNDPROC, FakeWndProc as *const () as _));

        _ImGui_ImplWin32_Init(super::win32::WINDOW);
    }
}

pub fn new_frame() {
    unsafe {
        _ImGui_ImplWin32_NewFrame();
    }
}

fn attach_hooks() -> Result<(), Box<dyn Error>> {
    let cont = unsafe { Container::<User32Api>::load("user32.dll")? };

    unsafe {
        RegisterRawInputDevicesHook.initialize(cont.RegisterRawInputDevices, |raw_input_devices, num_devices, size| {
            let unusable: RAWINPUTDEVICE_FLAGS = RIDEV_NOLEGACY | RIDEV_CAPTUREMOUSE | RIDEV_APPKEYS | RIDEV_NOHOTKEYS;
            for input_device in from_raw_parts_mut(raw_input_devices, num_devices as usize) {
                input_device.dwFlags &= !unusable;
            }
            RegisterRawInputDevicesHook.call(raw_input_devices, num_devices, size)
        })?;
        RegisterRawInputDevicesHook.enable()?;

        ShowCursorHook.initialize(cont.ShowCursor, |_| ShowCursorHook.call(TRUE))?;
        ShowCursorHook.enable()?;

        SetCursorPosHook.initialize(cont.SetCursorPos, |_, _| FALSE)?;
        SetCursorPosHook.enable()?;
    }

    Ok(())
}

pub fn init() -> Result<(), Box<dyn Error>> {
    attach_hooks()?;

    Ok(())
}
