mod dx9;
mod win32;

use std::{error::Error, path::PathBuf, ptr};

use bluebrick_proxy_base::{RequestedPlatform, RequestedRenderer};
use imgui::{Condition, DrawData, FontConfig, FontGlyphRanges, FontSource, Key, StyleColor, StyleVar};

pub struct Overlay {
    imgui: imgui::Context,
    platform: Box<dyn Platform>,
    #[allow(unused)] // for possible future use
    renderer: Box<dyn Renderer>,
    show_hide_key: Key,
    is_showing: bool,
}

static mut OVERLAY_INSTANCE: *mut Overlay = ptr::null_mut();

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

        Self::add_fonts(&mut imgui);

        Self {
            imgui,
            platform,
            renderer,
            show_hide_key: Key::F3,
            is_showing: true,
        }
    }

    fn add_fonts(imgui: &mut imgui::Context) {
        const FONT_BYTES: &[u8] = include_bytes!("fonts/CascadiaCode/CascadiaCode.ttf");
        const FONT_ITALIC_BYTES: &[u8] = include_bytes!("fonts/CascadiaCode/CascadiaCodeItalic.ttf");
        const FONT_EMOJI_BYTES: &[u8] = include_bytes!("fonts/FluentUIEmoji/FluentUIEmojiFlat.ttf");
        const FONT_SIZE: f32 = 16.0;
        let font_range: FontGlyphRanges = FontGlyphRanges::from_slice(&[0x1, 0x1FFFF, 0]);

        // Regular with emojis
        imgui.fonts().add_font(&[
            FontSource::TtfData {
                data: FONT_BYTES,
                size_pixels: FONT_SIZE,
                config: Some(FontConfig {
                    glyph_ranges: font_range.clone(),
                    name: Some(String::from("Cascadia Code")),
                    ..Default::default()
                }),
            },
            FontSource::TtfData {
                data: FONT_EMOJI_BYTES,
                size_pixels: FONT_SIZE,
                config: Some(FontConfig {
                    oversample_h: 1,
                    oversample_v: 1,
                    font_builder_flags: imgui::sys::ImGuiFreeTypeBuilderFlags_LoadColor,
                    glyph_ranges: font_range.clone(),
                    name: Some(String::from("Fluent UI Emoji Flat")),
                    ..Default::default()
                })
            }
        ]);

        // bold
        imgui.fonts().add_font(&[FontSource::TtfData {
            data: FONT_BYTES,
            size_pixels: FONT_SIZE,
            config: Some(FontConfig {
                font_builder_flags: imgui::sys::ImGuiFreeTypeBuilderFlags_Bold,
                glyph_ranges: font_range.clone(),
                name: Some(String::from("Cascadia Code Bold")),
                ..Default::default()
            })
        }]);

        // italic
        imgui.fonts().add_font(&[FontSource::TtfData {
            data: FONT_ITALIC_BYTES,
            size_pixels: FONT_SIZE,
            config: Some(FontConfig {
                glyph_ranges: font_range.clone(),
                name: Some(String::from("Cascadia Code Italic")),
                ..Default::default()
            })
        }]);

        // bold + italic
        imgui.fonts().add_font(&[FontSource::TtfData {
            data: FONT_ITALIC_BYTES,
            size_pixels: FONT_SIZE,
            config: Some(FontConfig {
                font_builder_flags: imgui::sys::ImGuiFreeTypeBuilderFlags_Bold,
                glyph_ranges: font_range.clone(),
                name: Some(String::from("Cascadia Code Bold + Italic")),
                ..Default::default()
            })
        }]);
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
            ui.text("World! 🤯🍄");
        });

        ui.end_frame_early();
        self.imgui.render()
    }

    pub fn post_draw(&self) {}
}

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

    // TODO: make thread safe if it ever becomes worth it
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
