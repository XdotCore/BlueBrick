mod dx9;
mod win32;

use std::{error::Error, path::PathBuf, ptr};

use bluebrick::proxy::{Config, RequestedPlatform, RequestedRenderer};
use colored::Color;
use imgui::{Condition, ConfigFlags, DrawData, FontConfig, FontGlyphRanges, FontSource, Key, StyleColor, StyleVar, Ui};

use crate::{logger::{main_log, LogItem}, MainLogger};

pub struct Overlay {
    imgui: imgui::Context,
    platform: Box<dyn Platform>,
    #[allow(unused)] // for possible future use
    renderer: Box<dyn Renderer>,
    show_hide_key: Key,
    is_showing: bool,
    show_demo_window: bool,
    show_logs: bool
}

impl Overlay {
    pub fn new(config: Config) -> Result<Self, Box<dyn Error>> {
        let platform = Box::new(match config.platform {
            RequestedPlatform::Win32 => win32::Platform::new(),
        }?);

        let renderer = Box::new(match config.renderer {
            RequestedRenderer::DX9 => dx9::Renderer::new(),
        }?);

        let mut imgui = imgui::Context::create();
        imgui.style_mut().use_dark_colors();

        imgui.set_ini_filename(Some(PathBuf::from("bluebrick/imgui.ini")));

        imgui.io_mut().config_flags |= ConfigFlags::DOCKING_ENABLE;

        Self::add_fonts(&mut imgui);

        Ok(Self {
            imgui,
            platform,
            renderer,
            show_hide_key: Key::F3,
            is_showing: true,
            show_demo_window: false,
            show_logs: false
        })
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

        if ui.is_key_down(Key::T) {
            main_log!("test");
        }

        if self.is_showing {
            
            ui.main_menu_bar(|| {
                ui.menu("Blue Brick", || {
                    ui.menu_item_config("Show Logs").build_with_ref(&mut self.show_logs);

                    ui.menu_item_config("Show ImGui Demo").build_with_ref(&mut self.show_demo_window);

                    ui.separator();

                    if ui.menu_item_config("Quit").shortcut("Alt + F4").build() {
                        // TODO: ask are you sure
                        std::process::exit(0);
                    }
                });

                ui.menu("Bricks", || {
                    if ui.menu_item("Hello")  {

                    }
                });
            });

            if self.show_logs {
                Self::show_logs(ui);
            }

            if self.show_demo_window {
                ui.show_demo_window(&mut self.show_demo_window);
            }
        }

        ui.end_frame_early();
        self.imgui.render()
    }

    fn show_logs(ui: &Ui) {
        ui.window("Log Window").size([700.0, 650.0], Condition::FirstUseEver).horizontal_scrollbar(true).build(|| {
            let mut main_logger = MainLogger::instance().lock().unwrap();

            let _spacing = ui.push_style_var(StyleVar::ItemSpacing([0.0, 0.2]));

            let mut _current_color = None;
            for item in &main_logger.log_items {
                match item {
                    LogItem::Text(msg) => {
                        ui.text(msg);
                        ui.same_line();
                    }
                    LogItem::NewLine => ui.text(""), // better than new_line() because it can stack multiple new lines
                    LogItem::Color(color) => _current_color = Some(ui.push_style_color(StyleColor::Text, color_to_f32_4(*color))),
                    LogItem::StyleReset => _current_color = None,
                }
            }

            // scroll to end, including logic for the scrollbar covering part of the window
            if main_logger.log_scroll_changed && (ui.scroll_y() + ui.clone_style().scrollbar_size) >= ui.scroll_max_y() {
                ui.set_scroll_here_y_with_ratio(1.0);
                main_logger.log_scroll_changed = false;
            }
        });
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

    fn get_overlay() -> &'static mut Overlay {
        &mut super::BlueBrick::instance().overlay
    }
}

trait PlatformHelper<P: PlatformHelper<P> + Platform> : BackendHelper<P> {
    fn instance() -> &'static P {
        Self::cast(Self::get_overlay().platform.as_ref())
    }
}

#[allow(unused)] // for possible future use
trait RendererHelper<R: RendererHelper<R> + Renderer> : BackendHelper<R> {
    fn instance() -> &'static R {
        Self::cast(Self::get_overlay().renderer.as_ref())
    }
}

fn u8_to_f32(byte: u8) -> f32 {
    byte as f32 / u8::MAX as f32
}

fn color_to_f32_4(color: Color) -> [f32; 4] {
    use Color::*;
    match color {
        Black => [0.0, 0.0, 0.0, 1.0 ],
        Red => [205.0, 0.0, 0.0, 1.0 ],
        Green => [0.0, 205.0, 0.0, 1.0 ],
        Yellow => [205.0, 205.0, 0.0, 1.0 ],
        Blue => [0.0, 0.0, 238.0, 1.0 ],
        Magenta => [205.0, 0.0, 205.0, 1.0 ],
        Cyan => [0.0, 205.0, 205.0, 1.0 ],
        White => [229.0, 229.0, 229.0, 1.0 ],
        BrightBlack => [127.0, 127.0, 127.0, 1.0 ],
        BrightRed => [255.0, 0.0, 0.0, 1.0 ],
        BrightGreen => [0.0, 255.0, 0.0, 1.0 ],
        BrightYellow => [255.0, 255.0, 0.0, 1.0 ],
        BrightBlue => [92.0, 92.0, 255.0, 1.0 ],
        BrightMagenta => [255.0, 0.0, 255.0, 1.0 ],
        BrightCyan => [0.0, 255.0, 255.0, 1.0 ],
        BrightWhite => [255.0, 255.0, 255.0, 1.0 ],
        TrueColor { r, g, b } => [u8_to_f32(r), u8_to_f32(g), u8_to_f32(b), 1.0],
    }
}
