#[cfg(all(feature = "win32", feature = "dx9"))]
mod win32_dx9;

use std::{error::Error, path::PathBuf, ptr};

use imgui::{DrawData, Key};
use win32_dx9::init_imgui;

pub struct Overlay {
    imgui: imgui::Context,
    show_hide_key: Key,
    is_showing: bool,
}

// there might be a better way to do this
static mut OVERLAY_INSTANCE: *mut Overlay = ptr::null_mut();

impl Overlay {
    // TODO: add shutdown method
    // TODO: refactor all the unsafe innards
    pub fn start() -> Result<(), Box<dyn Error>> {
        if unsafe { OVERLAY_INSTANCE.is_null() } {
            let mut imgui = imgui::Context::create();
            imgui.style_mut().use_dark_colors();

            imgui.set_ini_filename(Some(PathBuf::from("bluebrick/imgui.ini")));

            // TODO: add docking

            // TODO: add fonts

            let overlay = Overlay { imgui, show_hide_key: Key::F3, is_showing: true };

            unsafe {
                OVERLAY_INSTANCE = Box::into_raw(Box::new(overlay));
            };

            init_imgui()?;
        }

        Ok(())
    }

    pub fn draw(&mut self) -> &DrawData {
        let ui = self.imgui.new_frame();

        if ui.is_key_pressed(self.show_hide_key) {
            self.is_showing = !self.is_showing;
        }

        if self.is_showing {
            ui.show_demo_window(&mut true);
        }

        ui.end_frame_early();
        self.imgui.render()
    }

    pub fn post_draw(&self) {}
}
