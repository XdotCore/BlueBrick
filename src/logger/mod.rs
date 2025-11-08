#[cfg(windows)]
mod windows;

pub mod web_colors;

use std::{env, ffi::{CStr, c_char}, fs::File, io::Write, sync::{LazyLock, Mutex, MutexGuard, OnceLock}};

use bluebrick::{imgui::{StyleColor, StyleVar, Ui}, logger::Severity};
use colored::{Color, ColoredString, Colorize};
use regex::Regex;
use web_colors::WebColor;

macro_rules! main_log {
    ($($arg:tt)*) => {
        crate::logger::MainLogger::instance().log_with_severity(&format!($($arg)*), bluebrick::logger::Severity::Info);
    }
}
pub(crate) use main_log;

#[allow(unused_macros)]
macro_rules! main_log_debug {
    ($($arg:tt)*) => {
        crate::logger::MainLogger::instance().log_with_severity(&format!($($arg)*), bluebrick::logger::Severity::Debug);
    }
}
#[allow(unused_imports)]
pub(crate) use main_log_debug;

macro_rules! main_log_warning {
    ($($arg:tt)*) => {
        crate::logger::MainLogger::instance().log_with_severity(&format!($($arg)*), bluebrick::logger::Severity::Warning);
    }
}
pub(crate) use main_log_warning;

macro_rules! main_log_error {
    ($($arg:tt)*) => {
        crate::logger::MainLogger::instance().log_with_severity(&format!($($arg)*), bluebrick::logger::Severity::Error);
    }
}
pub(crate) use main_log_error;

pub(crate) enum LogItem {
    Text(String),
    NewLine,
    Color(Color),
    StyleReset,
}

pub(crate) struct MainLogger {
    pub log_items: Vec<LogItem>,
    pub log_scroll_changed: bool,
    file: File,
}

impl MainLogger {
    // TODO: don't panic on fail
    fn new() -> Self {
        #[cfg(windows)] {
            if let Err(e) = windows::init_terminal() {
                let msg = format!("Failed to create BlueBrick logger: {e}");
                let _ = msgbox::create("Error creating BlueBrick logger", &msg, msgbox::IconType::Error);
                panic!("{msg}");
            }
        }

        // needed for colored to use truecolor
        // should be safe since no other threads should be running during BlueBrick startup
        unsafe { env::set_var("COLORTERM", "truecolor") };

        Self {
            log_items: Vec::new(),
            log_scroll_changed: false,
            file: match File::create("bluebrick/log.txt") {
                Ok(file) => file,
                Err(e) => {
                    let msg = format!("Failed to create or open BlueBrick log file: {e}");
                    let _ = msgbox::create("Error creating BlueBrick logger", &msg, msgbox::IconType::Error);
                    panic!("{msg}");
                }
            }
        }
    }

    pub fn log_with_severity(&mut self, msg: &str, severity: Severity) {
        self.log_impl("Loader", "BlueBrick", Some(WebColor::DeepSkyBlue), &msg, severity);
    }

    fn log_impl<C: Into<Color>>(&mut self, kind: &str, name: &str, name_color: Option<C>, msg: &str, severity: Severity) {
        let msg = msg.replace("\r", "");
        let msg = Self::format_msg(kind, name, name_color, &msg, severity);

        print!("{msg}");
        self.log_to_file(&msg);
        self.log_to_overlay(&msg);

        self.log_scroll_changed = true;
    }

    fn format_msg<C: Into<Color>>(kind: &str, name: &str, name_color: Option<C>, msg: &str, severity: Severity) -> String {
        // colors
        let time_color = match severity {
            Severity::Debug => Some(WebColor::Gray), // the web color "gray" is darker than "dark gray" ...
            Severity::Warning => Some(WebColor::Goldenrod),
            Severity::Error => Some(WebColor::Firebrick),
            _ => Some(WebColor::Gray),
        };
        let kind_color = match severity {
            Severity::Debug => Some(WebColor::DarkGray),
            Severity::Warning => Some(WebColor::Yellow),
            Severity::Error => Some(WebColor::Red),
            _ => None,
        };
        let text_color = match severity {
            Severity::Debug => Some(WebColor::DarkGray),
            Severity::Warning => Some(WebColor::Yellow),
            Severity::Error => Some(WebColor::Red),
            _ => None,
        };
        
        let time = chrono::Local::now();
        let time = format!("{}", time.format("%T"));
        let time = Self::apply_color(&time, time_color);

        let kind = Self::apply_color(&kind, kind_color);

        let name = Self::apply_color(name, name_color);

        let msg = Self::apply_color(msg, text_color);

        format!("{time} [{kind}] [{name}] {msg}\n")
    }

    fn apply_color<C: Into<Color>>(string: &str, color: Option<C>) -> ColoredString {
        match color {
            Some(color) => string.color(color),
            None => string.normal(),
        }
    }

    fn log_to_file(&mut self, msg: &str) {
        static RE: LazyLock<Regex> = LazyLock::new(|| Regex::new(r"\x1b.*?m").unwrap());
        let msg = RE.replace_all(msg, "");
        let _ = self.file.write_all(msg.as_bytes());
    }

    fn log_to_overlay(&mut self, msg: &str) {
        static RE: LazyLock<Regex> = LazyLock::new(|| Regex::new(r"\x1b\[(?P<color>(?:\d{1,3};)*\d{1,3})m|(?<newline>\n)|(?<text>[^\n\x1b]+)").unwrap());
        let log_items = RE.captures_iter(msg);

        for log_item in log_items {
            if let Some(text) = log_item.name("text").map(|m| m.as_str()) {
                self.log_items.push(LogItem::Text(String::from(text)));
            }
            else if let Some(_) = log_item.name("newline") {
                self.log_items.push(LogItem::NewLine);
            }
            else if let Some(color_txt) = log_item.name("color").map(|m| m.as_str()) {
                if let Some(color) = Self::parse_color_txt(color_txt) {
                    self.log_items.push(color);
                }
            }
        }
    }

    fn parse_color_txt(color_txt: &str) -> Option<LogItem> {
        let mut color = None;

        let nums = color_txt.split(';').map(|n| n.parse::<u8>().unwrap_or_default()).collect::<Vec<_>>();

        let mut i = 0;
        while i < nums.len() {
            let mut matched = true;

            match nums[i] {
                0 => color = Some(LogItem::StyleReset),
                30 => color = Some(LogItem::Color(Color::Black)),
                31 => color = Some(LogItem::Color(Color::Red)),
                32 => color = Some(LogItem::Color(Color::Green)),
                33 => color = Some(LogItem::Color(Color::Yellow)),
                34 => color = Some(LogItem::Color(Color::Blue)),
                35 => color = Some(LogItem::Color(Color::Magenta)),
                36 => color = Some(LogItem::Color(Color::Cyan)),
                37 => color = Some(LogItem::Color(Color::White)),
                90 => color = Some(LogItem::Color(Color::BrightBlack)),
                91 => color = Some(LogItem::Color(Color::BrightRed)),
                92 => color = Some(LogItem::Color(Color::BrightGreen)),
                93 => color = Some(LogItem::Color(Color::BrightYellow)),
                94 => color = Some(LogItem::Color(Color::BrightBlue)),
                95 => color = Some(LogItem::Color(Color::BrightMagenta)),
                96 => color = Some(LogItem::Color(Color::BrightCyan)),
                97 => color = Some(LogItem::Color(Color::BrightWhite)),
                _ => matched = false,
            }
            if matched {
                i += 1;
                continue;
            }
            matched = true;

            if i + 4 < nums.len() {
                match nums[i..i + 5] {
                    [38, 2, r, g, b] => color = Some(LogItem::Color(Color::TrueColor { r, g, b })),
                    _ => matched = false,
                }
                if matched {
                    i += 5;
                    continue;
                }
            }

            i += 1;
        }

        color
    }

    pub fn draw_logs(&mut self, ui: &Ui) {
        let _spacing = ui.push_style_var(StyleVar::ItemSpacing([0.0, 0.2]));

        let mut _current_color = None;
        for item in &self.log_items {
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
        if self.log_scroll_changed && (ui.scroll_y() + ui.clone_style().scrollbar_size) >= ui.scroll_max_y() {
            ui.set_scroll_here_y_with_ratio(1.0);
            self.log_scroll_changed = false;
        }
    }

    pub fn instance() -> MutexGuard<'static, Self> {
        static MAIN_LOGGER: OnceLock<Mutex<MainLogger>> = OnceLock::new();
        MAIN_LOGGER.get_or_init(|| Mutex::new(MainLogger::new())).lock().unwrap()
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

// TODO: implement name color
fn log_impl(kind: &str, name: *const c_char, msg: *const c_char, severity: Severity) {
    let name = unsafe { CStr::from_ptr(name) };
    let name = String::from_utf8_lossy(name.to_bytes()).to_string();
    let msg = unsafe { CStr::from_ptr(msg) };
    let msg = String::from_utf8_lossy(msg.to_bytes()).to_string();
    MainLogger::instance().log_impl::<Color>(kind, &name, None, &msg, severity);
}

#[unsafe(no_mangle)]
extern "C" fn log_library_impl(name: *const c_char, msg: *const c_char, severity: Severity) {
    log_impl("Library", name, msg, severity);
}

#[unsafe(no_mangle)]
extern "C" fn log_mod_impl(name: *const c_char, msg: *const c_char, severity: Severity) {
    log_impl("Mod", name, msg, severity);
}
