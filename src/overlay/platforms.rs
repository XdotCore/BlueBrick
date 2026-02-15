pub mod win32;

use std::error::Error;
use std::sync::mpsc::Sender;

use bluebrick_proxy::{Config, RequestedPlatform};

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
    pub fn new(config: Config) -> Result<Self, Box<dyn Error>> {
        Ok(match config.platform {
            RequestedPlatform::Win32 => SomePlatform::Win32(Win32::new()?),
        })
    }

    fn get_inner(&self) -> &dyn Platform {
        match self {
            Self::Win32(win32) => win32,
        }
    }

    pub fn handle_event(&mut self, event: PlatformEvent) {
        match (event, self) {
            (PlatformEvent::Win32(win32_event), SomePlatform::Win32(win32)) => {
                win32.handle_event(win32_event);
            }
            // uncomment when there is more than 1 platform supported
            /*_ => {
                msgbox::create("Mismatched platform types", "A BlueBrick event was triggered with the wrong platform type", msgbox::IconType::Error);
            }*/
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
    pub fn new(config: Config, tx: Sender<BBEvent>) -> Self {
        match config.platform {
            RequestedPlatform::Win32 => SomePlatformHandle::Win32(Win32Handle::new(tx)),
        }
    }

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