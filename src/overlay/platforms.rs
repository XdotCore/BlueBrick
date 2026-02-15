pub mod win32;

use crate::BBEvent;
use crate::overlay::OverlayEvent;
use crate::overlay::platforms::win32::{Win32, Win32Event, Win32Handle};

pub enum PlatformEvent {
    Win32(Win32Event),
}

impl Into<BBEvent> for PlatformEvent {
    fn into(self) -> BBEvent {
        OverlayEvent::Platform(self).into()
    }
}

pub trait Platform {
    fn new_frame(&self);
}

pub enum SomePlatform {
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

pub trait PlatformHandle {
    fn new_frame(&self);
}

pub enum SomePlatformHandle {
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