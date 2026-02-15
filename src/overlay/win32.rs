#![allow(nonstandard_style)]

use std::{error::Error, slice::from_raw_parts_mut, sync::mpsc::{self, Sender}};

use bluebrick::imgui;
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

use crate::{BBEvent, BLUEBRICK_HANDLE};
use crate::overlay::{OverlayEvent, Platform, PlatformEvent, PlatformHandle, SomePlatformHandle};

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

pub enum Win32Event {
    SetWindow { ptr: usize, tx: Sender<()> },
    GetTrueWNDPROC { tx: Sender<WNDPROC> },
    NewFrame { tx: Sender<()> },
}

impl Into<BBEvent> for Win32Event {
    fn into(self) -> BBEvent {
        BBEvent::Overlay(OverlayEvent::Platform(PlatformEvent::Win32(self)))
    }
}

pub struct Win32 {
    window: HWND,
    true_wndproc: WNDPROC,
}

impl Win32 {
    fn FakeWndProc(hwnd: HWND, mut msg: u32, mut wparam: WPARAM, lparam: LPARAM) -> LRESULT {
        let result = unsafe {
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

            let true_wndproc = if let Some(bb) = BLUEBRICK_HANDLE.get() {
                #[allow(irrefutable_let_patterns)]
                if let SomePlatformHandle::Win32(win32) = &bb.overlay.platform {
                    win32.get_true_wndproc()
                } else {
                    return DefWindowProcA(hwnd, msg, wparam, lparam);
                }
            } else {
                return DefWindowProcA(hwnd, msg, wparam, lparam);
            };
            CallWindowProcA(true_wndproc, hwnd, msg, wparam, lparam)
        };
        result
    }

    fn set_window(&mut self, ptr: *mut ()) {
        unsafe {
            if !self.window.is_invalid() && self.true_wndproc.is_some() {
                SetWindowLongPtrA(self.window, GWLP_WNDPROC, self.true_wndproc.unwrap() as *const () as _);
            }

            self.window = HWND(ptr.cast());

            self.true_wndproc = std::mem::transmute(SetWindowLongPtrA(self.window, GWLP_WNDPROC, Self::FakeWndProc as *const () as _));

            _ImGui_ImplWin32_Init(self.window);
        }
    }

    fn get_true_wndproc(&self) -> WNDPROC {
        self.true_wndproc
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

    fn init() -> Result<(), Box<dyn Error>> {
        Self::attach_hooks()?;

        Ok(())
    }

    pub fn new() -> Result<Self, Box<dyn Error>> {
        Self::init()?;
        Ok(Self {
            window: Default::default(),
            true_wndproc: None,
        })
    }

    pub fn handle_event(&mut self, event: Win32Event) {
        match event {
            Win32Event::SetWindow { ptr, tx } => {
                _ = tx.send(self.set_window(ptr as _));
            }
            Win32Event::GetTrueWNDPROC { tx } => {
                _ = tx.send(self.get_true_wndproc());
            }
            Win32Event::NewFrame { tx } => {
                _ = tx.send(self.new_frame());
            }
        }
    }
}

impl Platform for Win32 {
    fn new_frame(&self) {
        unsafe {
            _ImGui_ImplWin32_NewFrame();
        }
    }
}

pub struct Win32Handle {
    tx: Sender<BBEvent>,
}

impl Win32Handle {
    pub fn new(tx: Sender<BBEvent>) -> Self {
        Self  {
            tx
        }
    }

    pub fn set_window(&self, ptr: *mut ()) {
        let (swtx, swrx) = mpsc::channel();
        _ = self.tx.send(Win32Event::SetWindow { ptr: ptr as _, tx: swtx }.into());
        swrx.recv().expect("Bluebrick thread dropped swtx")
    }

    pub fn get_true_wndproc(&self) -> WNDPROC {
        let (gtwtx, gtwrx) = mpsc::channel();
        _ = self.tx.send(Win32Event::GetTrueWNDPROC { tx: gtwtx }.into());
        gtwrx.recv().expect("Bluebrick thread dropped gtwtx")
    }
}

impl PlatformHandle for Win32Handle {
    fn new_frame(&self) {
        let (nftx, nfrx) = mpsc::channel();
        _ = self.tx.send(Win32Event::NewFrame { tx: nftx }.into());
        nfrx.recv().expect("Bluebrick thread dropped nfrx")
    }
}
