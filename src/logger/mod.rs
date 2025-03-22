#[cfg(windows)]
mod windows;
#[cfg(windows)]
pub use windows::*;

pub struct Logger;
