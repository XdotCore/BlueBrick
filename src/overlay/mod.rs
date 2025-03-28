mod dx9;
mod win32;

use std::{error::Error, path::PathBuf, ptr};

use bluebrick_proxy_base::{RequestedPlatform, RequestedRenderer};
use imgui::{Condition, DrawData, Key, StyleColor, StyleVar};

pub struct Overlay {
    imgui: imgui::Context,
    platform: Box<dyn Platform>,
    #[allow(unused)] // for possible future use
    renderer: Box<dyn Renderer>,
    show_hide_key: Key,
    is_showing: bool,
}

static mut OVERLAY_INSTANCE: *mut Overlay = ptr::null_mut();

trait Backend {}

trait Platform: Backend {
    fn new_frame(&self);
    fn set_window(&mut self, ptr: *mut ());
}

trait Renderer: Backend {}

trait BackendHelper<B: BackendHelper<B> + Backend> {
    fn cast(backend: &dyn Backend) -> &B {
        unsafe { (ptr::from_ref(backend).cast::<B>()).as_ref().unwrap() }
    }

    fn get_overlay() -> &'static mut Overlay {
        unsafe { &mut *OVERLAY_INSTANCE }
    }
}

trait PlatformHelper<P: PlatformHelper<P> + Platform> : BackendHelper<P> {
    fn get_instance() -> &'static P {
        Self::cast(Self::get_overlay().platform.as_ref())
    }
}

#[allow(unused)] // for possible future use
trait RendererHelper<R: RendererHelper<R> + Renderer> : BackendHelper<R> {
    fn get_instance() -> &'static R {
        Self::cast(Self::get_overlay().renderer.as_ref())
    }
}

impl Overlay {
    // TODO: clean and minimize the unsafe innards
    pub fn start(platform: RequestedPlatform, renderer: RequestedRenderer) -> Result<(), Box<dyn Error>> {
        let platform = Box::new(match platform {
            RequestedPlatform::Win32 => win32::Platform::new(),
        }?);

        let renderer = Box::new(match renderer {
            RequestedRenderer::DX9 => dx9::Renderer::new(),
        }?);

        unsafe { OVERLAY_INSTANCE = Box::into_raw(Box::new(Self::new(platform, renderer))) };

        Ok(())
    }

    fn new(platform: Box<dyn Platform>, renderer: Box<dyn Renderer>) -> Self {
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
