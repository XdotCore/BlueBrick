mod dx9;
mod win32;

use std::{error::Error, path::PathBuf, sync::mpsc::{self, Sender}};

use bluebrick_proxy::{Config, RequestedPlatform, RequestedRenderer};
use bluebrick::imgui::{self, Condition, ConfigFlags, FontConfig, FontGlyphRanges, FontSource, Key, Ui};

use crate::BBEvent;
use crate::logger::{MainLogger, main_log};
use crate::overlay::dx9::{DX9, DX9Event, DX9Handle};
use crate::overlay::win32::{Win32, Win32Event, Win32Handle};
use crate::subbrick::SubBrickManager;

pub enum OverlayEvent {
    Draw(Sender<()>),
    PostDraw(Sender<()>),
    Platform(PlatformEvent),
    Renderer(RendererEvent),
}

impl Into<BBEvent> for OverlayEvent {
    fn into(self) -> BBEvent {
        BBEvent::Overlay(self)
    }
}

pub enum PlatformEvent {
    Win32(Win32Event),
}

impl Into<BBEvent> for PlatformEvent {
    fn into(self) -> BBEvent {
        OverlayEvent::Platform(self).into()
    }
}

pub enum RendererEvent {
    DX9(DX9Event),
}

impl Into<BBEvent> for RendererEvent {
    fn into(self) -> BBEvent {
        OverlayEvent::Renderer(self).into()
    }
}

pub struct Overlay {
    imgui: imgui::Context,
    platform: SomePlatform,
    renderer: SomeRenderer,
    show_hide_key: Key,
    is_showing: bool,
    show_demo_window: bool,
    show_logs: bool,
    show_bricks: bool,
}

impl Overlay {
    pub fn new(config: Config) -> Result<Self, Box<dyn Error>> {

        let platform = match config.platform {
            RequestedPlatform::Win32 => SomePlatform::Win32(Win32::new()?),
        };

        let renderer = match config.renderer {
            RequestedRenderer::DX9 => SomeRenderer::DX9(DX9::new()?),
        };
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
            show_logs: false,
            show_bricks: false,
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

    pub fn draw(&mut self, subbrick_manager: &mut SubBrickManager) {
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
                    ui.menu_item_config("Show Bricks").build_with_ref(&mut self.show_bricks);

                    ui.separator();

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
                Self::show_logs(ui, &mut self.show_logs);
            }

            if self.show_bricks {
                Self::show_bricks(ui, &mut self.show_bricks, subbrick_manager);
            }

            if self.show_demo_window {
                ui.show_demo_window(&mut self.show_demo_window);
            }

            subbrick_manager.draw_all(ui);
        }
    }

    fn show_logs(ui: &Ui, opened: &mut bool) {
        ui.window("Log Window").size([900.0, 650.0], Condition::FirstUseEver).horizontal_scrollbar(true).opened(opened).build(|| {
            let mut main_logger = MainLogger::instance();

            main_logger.draw_logs(ui);
        });
    }

    fn show_bricks(ui: &Ui, opened: &mut bool, subbrick_manager: &mut SubBrickManager) {
        ui.window("Loaded Bricks").size([900.0, 650.0], Condition::FirstUseEver).opened(opened).build(|| {
            if let Some(tab_bar) = ui.tab_bar("BrickTabs") {
                if let Some(libraries) = ui.tab_item("Libraries") {
                    subbrick_manager.draw_library_list(ui);
                    
                    libraries.end();
                }

                if let Some(mods) = ui.tab_item("Mods") {
                    subbrick_manager.draw_mod_list(ui);

                    mods.end();
                }

                tab_bar.end();
            }
        });
    }

    pub fn post_draw(&self) {
    }

    pub fn handle_event(&mut self, event: OverlayEvent, subbrick_manager: &mut SubBrickManager) {
        match event {
            OverlayEvent::Draw(tx) => {
                _ = tx.send(self.draw(subbrick_manager));
            }
            OverlayEvent::PostDraw(tx) => {
                _ = tx.send(self.post_draw());
            }
            OverlayEvent::Platform(platform) => {
                match (platform, &mut self.platform) {
                    (PlatformEvent::Win32(win32_event), SomePlatform::Win32(win32)) => {
                        win32.handle_event(win32_event);
                    }
                    /*_ => {
                        msgbox::create("Mismatched platform types", "A BlueBrick event was triggered with the wrong platform type", msgbox::IconType::Error);
                    }*/
                }
            }
            OverlayEvent::Renderer(renderer) => {
                match (renderer, &mut self.renderer) {
                    (RendererEvent::DX9(dx9_event), SomeRenderer::DX9(dx9)) => {
                        dx9.handle_event(dx9_event);
                    }
                    /*_ => {
                        msgbox::create("Mismatched renderer types", "A BlueBrick event was triggered with the wrong renderer type", msgbox::IconType::Error);
                    }*/
                }
            }
        }
    }
}

pub struct OverlayHandle {
    tx: Sender<BBEvent>,
    platform: SomePlatformHandle,
    #[allow(unused)]
    renderer: SomeRendererHandle,
}

impl OverlayHandle {
    pub fn new(config: Config, tx: Sender<BBEvent>) -> Self {
        // TODO: move this match into the enums
        let platform = match config.platform {
            RequestedPlatform::Win32 => SomePlatformHandle::Win32(Win32Handle::new(tx.clone())),
        };

        let renderer = match config.renderer {
            RequestedRenderer::DX9 => SomeRendererHandle::DX9(DX9Handle::new(tx.clone())),
        };

        Self {
            tx,
            platform,
            renderer,
        }
    }

    pub fn draw(&self) {
        let (draw_tx, draw_rx) = mpsc::channel();
        self.tx.send(OverlayEvent::Draw(draw_tx).into()).expect("Bluebrick thread died");
        draw_rx.recv().expect("Bluebrick thread dropped draw_tx")
    }

    pub fn post_draw(&self) {
        let (pdtx, pdrx) = mpsc::channel();
        self.tx.send(OverlayEvent::PostDraw(pdtx).into()).expect("Bluebrick thread died");
        pdrx.recv().expect("Bluebrick thread dropped pdrx")
    }
}

trait Platform {
    fn new_frame(&self);
}

enum SomePlatform {
    Win32(Win32),
}

impl SomePlatform {
    fn get_inner(&self) -> &dyn Platform {
        match self {
            Self::Win32(win32) => win32,
        }
    }
}

impl Platform for SomePlatform {
    fn new_frame(&self) {
        self.get_inner().new_frame();
    }
}

trait PlatformHandle {
    fn new_frame(&self);
}

enum SomePlatformHandle {
    Win32(Win32Handle),
}

impl SomePlatformHandle {
    fn get_inner(&self) -> &dyn PlatformHandle {
        match self {
            Self::Win32(win32) => win32,
        }
    }
}

impl PlatformHandle for SomePlatformHandle {
    fn new_frame(&self) {
        self.get_inner().new_frame();
    }
}

trait Renderer {

}

enum SomeRenderer {
    DX9(DX9),
}

impl SomeRenderer {
    #[allow(unused)]
    fn get_inner(&self) -> &dyn Renderer {
        match self {
            Self::DX9(dx9) => dx9,
        }
    }
}

impl Renderer for SomeRenderer {

}

trait RendererHandle {

}

enum SomeRendererHandle {
    DX9(DX9Handle),
}

impl SomeRendererHandle {
    #[allow(unused)]
    fn get_inner(&self) -> &dyn RendererHandle {
        match self {
            Self::DX9(dx9) => dx9,
        }
    }
}

impl RendererHandle for SomeRendererHandle {

}
