#[cfg(windows)]
mod windows;

pub mod web_colors;

use std::{env, ffi::{c_char, CStr}, fs::File, io::Write, sync::{LazyLock, Mutex, OnceLock}};

use bluebrick::logger::Severity;
use colored::{Color, ColoredString, Colorize};
use regex::Regex;
use web_colors::WebColor;

macro_rules! main_log {
    ($($arg:tt)*) => {
        crate::logger::MainLogger::instance().lock().unwrap().log_with_severity(&format!($($arg)*), bluebrick::logger::Severity::Info);
    }
}
pub(crate) use main_log;

#[allow(unused_macros)]
macro_rules! main_log_debug {
    ($($arg:tt)*) => {
        crate::logger::MainLogger::instance().lock().unwrap().log_with_severity(&format!($($arg)*), bluebrick::logger::Severity::Debug);
    }
}
#[allow(unused_imports)]
pub(crate) use main_log_debug;

macro_rules! main_log_warning {
    ($($arg:tt)*) => {
        crate::logger::MainLogger::instance().lock().unwrap().log_with_severity(&format!($($arg)*), bluebrick::logger::Severity::Warning);
    }
}
pub(crate) use main_log_warning;

macro_rules! main_log_error {
    ($($arg:tt)*) => {
        crate::logger::MainLogger::instance().lock().unwrap().log_with_severity(&format!($($arg)*), bluebrick::logger::Severity::Error);
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

    pub fn instance() -> &'static Mutex<Self> {
        static MAIN_LOGGER: OnceLock<Mutex<MainLogger>> = OnceLock::new();
        MAIN_LOGGER.get_or_init(|| Mutex::new(MainLogger::new()))
    }
}

// TODO: implement name color
fn log_impl(kind: &str, name: *const c_char, msg: *const c_char, severity: Severity) {
    let name = unsafe { CStr::from_ptr(name) };
    let name = String::from_utf8_lossy(name.to_bytes()).to_string();
    let msg = unsafe { CStr::from_ptr(msg) };
    let msg = String::from_utf8_lossy(msg.to_bytes()).to_string();
    MainLogger::instance().lock().unwrap().log_impl::<Color>(kind, &name, None, &msg, severity);
}

#[unsafe(no_mangle)]
extern "C" fn log_library_impl(name: *const c_char, msg: *const c_char, severity: Severity) {
    log_impl("Library", name, msg, severity);
    log_impl("Mod", name, msg, severity);
}

#[unsafe(no_mangle)]
extern "C" fn log_mod_impl(name: *const c_char, msg: *const c_char, severity: Severity) {
    log_impl("Mod", name, msg, severity);
}
