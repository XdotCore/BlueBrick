#![allow(nonstandard_style)]

use windows::{
    Win32::{
        Foundation::{HWND, LPARAM, LRESULT, WPARAM}, UI::WindowsAndMessaging::{CallWindowProcA, DefWindowProcA, SetWindowLongPtrA, GWLP_WNDPROC, WA_ACTIVE, WM_ACTIVATE, WM_KEYFIRST, WM_KEYLAST, WM_KILLFOCUS, WM_MOUSEFIRST, WM_MOUSELAST, WM_SETFOCUS, WM_SYSKEYDOWN, WNDPROC}
    },
    core::BOOL
};

unsafe extern "C" {
    fn _ImGui_ImplWin32_Init(hwnd: HWND) -> bool;
    fn _ImGui_ImplWin32_NewFrame();
    fn _ImGui_ImplWin32_WndProcHandler(hWnd: HWND, msg: u32, wParam: WPARAM, lParam: LPARAM) -> BOOL;
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

pub fn init_imgui_implwin32(hwnd: HWND) {
    unsafe {
        WINDOW = hwnd;

        TRUEWNDPROC = std::mem::transmute(SetWindowLongPtrA(WINDOW, GWLP_WNDPROC, FakeWndProc as *const () as _));

        _ImGui_ImplWin32_Init(super::win32::WINDOW);
    }
}

pub fn imgui_implwin32_new_frame() {
    unsafe { _ImGui_ImplWin32_NewFrame(); }
}