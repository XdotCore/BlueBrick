mod dx9;
mod win32;

use std::{error::Error, path::PathBuf, ptr};

use bluebrick_proxy_base::{Platform, Renderer};
use imgui::{Condition, DrawData, Key, StyleColor, StyleVar};

pub struct Overlay {
    imgui: imgui::Context,
    #[allow(unused)] // for future use
    platform: Platform,
    #[allow(unused)] // for future use
    renderer: Renderer,
    show_hide_key: Key,
    is_showing: bool,
}

static mut OVERLAY_INSTANCE: *mut Overlay = ptr::null_mut();

impl Overlay {
    // TODO: clean and minimize the unsafe innards
    pub fn start(platform: Platform, renderer: Renderer) -> Result<(), Box<dyn Error>> {
        unsafe { OVERLAY_INSTANCE = Box::into_raw(Box::new(Self::new(platform, renderer))) };

        match platform {
            Platform::Win32 => win32::init(),
        }?;

        match renderer {
            Renderer::DX9 => dx9::init(),
        }?;

        Ok(())
    }

    fn new(platform: Platform, renderer: Renderer) -> Self {
        let mut imgui = imgui::Context::create();
        imgui.style_mut().use_dark_colors();

        imgui.set_ini_filename(Some(PathBuf::from("bluebrick/imgui.ini")));

        // TODO: add docking

        // TODO: add fonts

        Self {
            imgui,
            platform,
            renderer,
            show_hide_key: Key::F3,
            is_showing: true,
        }
    }

    pub fn draw(&mut self) -> &DrawData {
        let ui = self.imgui.new_frame();

        if ui.is_key_pressed(self.show_hide_key) {
            self.is_showing = !self.is_showing;
        }

        if self.is_showing {
            ui.show_demo_window(&mut true);
        }

        ui.window("hello").size([700.0, 700.0], Condition::FirstUseEver).build(|| {
            let _spacing = ui.push_style_var(StyleVar::ItemSpacing([0.0, 2.0]));
            let _color = ui.push_style_color(StyleColor::Text, imgui::color::ImColor32::from_rgb(0xd1, 0x79, 0x15).to_rgba_f32s());
            ui.text("Hello");
            ui.text("World!");
        });

        ui.end_frame_early();
        self.imgui.render()
    }

    pub fn post_draw(&self) {}
}
