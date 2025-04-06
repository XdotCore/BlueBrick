use std::{ffi::{c_char, CString}, sync::OnceLock};

use dlopen::wrapper::{Container, WrapperApi};
use dlopen_derive::WrapperApi;

#[repr(C)]
#[derive(Clone, Copy)]
pub enum Severity {
    Info,
    Debug,
    Warning,
    Error
}

// TODO: add documentation
pub trait Logger {
    fn log_with_severity(&self, msg: &str, severity: Severity);
    
    fn log(&self, msg: &str) {
        self.log_with_severity(msg, Severity::Info);
    }

    fn log_debug(&self, msg: &str) {
        self.log_with_severity(msg, Severity::Debug);
    }

    fn log_warning(&self, msg: &str) {
        self.log_with_severity(msg, Severity::Warning);
    }

    fn log_error(&self, msg: &str) {
        self.log_with_severity(msg, Severity::Error);
    }
}

pub trait HasLogger {
    fn logger() -> &'static impl Logger;
}

#[macro_export]
macro_rules! log {
    ($dst:expr, $($arg:tt)*) => {
        $dst.log(&format!($($arg)*));
    }
}

#[macro_export]
macro_rules! log_debug {
    ($dst:expr, $($arg:tt)*) => {
        $dst.log_debug(&format!($($arg)*));
    }
}

#[macro_export]
macro_rules! log_warning {
    ($dst:expr, $($arg:tt)*) => {
        $dst.log_warning(&format!($($arg)*));
    }
}

#[macro_export]
macro_rules! log_error {
    ($dst:expr, $($arg:tt)*) => {
        $dst.log_error(&format!($($arg)*));
    }
}

#[derive(WrapperApi)]
struct BBLoggerApi {
    log_library_impl: extern "C" fn (name: *const c_char, msg: *const c_char, severity: Severity),
    log_mod_impl: extern "C" fn (name: *const c_char, msg: *const c_char, severity: Severity),
}

fn get_bb_logger_api() -> &'static Container<BBLoggerApi> {
    static GET_API: OnceLock<Container<BBLoggerApi>> = OnceLock::new();
    GET_API.get_or_init(|| {
        match unsafe { Container::<BBLoggerApi>::load("bluebrick/bluebrick") } {
            Ok(api) => api,
            Err(e) => panic!("{e}")
        }
    })
}

pub struct LibraryLogger {
    name: &'static str,
    log_impl: extern "C" fn(*const c_char, *const c_char, Severity),
}

impl LibraryLogger {
    pub fn new(name: &'static str) -> Self {
        Self {
            name,
            log_impl: get_bb_logger_api().log_library_impl
        }
    }
}

impl Logger for LibraryLogger {
    fn log_with_severity(&self, msg: &str, severity: Severity) {
        let name = self.name.replace("\0", "");
        let name = CString::new(name).unwrap();
        let msg = msg.replace("\0", "");
        let msg = CString::new(msg).unwrap();
        (self.log_impl)(name.as_ptr(), msg.as_ptr(), severity);
    }
}
