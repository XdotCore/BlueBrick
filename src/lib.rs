pub mod logger;
mod memutils;
mod overlay;
pub mod subbrick;

use std::{error::Error, fmt, mem, sync::{OnceLock, mpsc::{self, Sender}}, thread};

use bluebrick_proxy::Config;
use retour::static_detour;

use crate::logger::{main_log_debug, main_log_error};
use crate::overlay::{Overlay, OverlayEvent, OverlayHandle};
use crate::subbrick::SubBrickManager;

fn hook() -> std::result::Result<(), Box<dyn Error>> {
    static_detour! {
        static AddToCoins: unsafe extern "cdecl" fn(*mut u64, u64, i32, bool);
        static AddToCoins2: unsafe extern "cdecl" fn(*mut u64, u64, i32, bool);
    }

    let real_add_to_coins = memutils::get_executable_base()? + 0x7E1070;
    let real_add_to_coins = unsafe { mem::transmute(real_add_to_coins) };

    unsafe {
        AddToCoins.initialize(real_add_to_coins, move |coins_ptr, to_add, mult, round_to_10s| {
            main_log_debug!("1: Got coin worth: {to_add}, mult: {mult}");
            AddToCoins.call(coins_ptr, 0, mult, round_to_10s);
        })?;
        AddToCoins.enable()?;
        AddToCoins2.initialize(real_add_to_coins, move |coins_ptr, to_add, mult, round_to_10s| {
            main_log_debug!("2: Got coin worth: {to_add}, mult: {mult}");
            AddToCoins2.call(coins_ptr, 0, mult, round_to_10s);
        })?;
        AddToCoins2.enable()?;
    }

    Ok(())
}

type Result<T> = std::result::Result<T, StartupErr>;

#[derive(Debug)]
enum StartupErr {
    Overlay(Box<dyn Error>),
    Hooks(Box<dyn Error>),
}

impl fmt::Display for StartupErr {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        use StartupErr::*;
        let message = match self {
            Overlay(e) => format!("Problem attaching imgui: {e}"),
            Hooks(e) => format!("Problem hooking functions: {e}"),
        };
        write!(f, "Error starting up BlueBrick: {}", message)
    }
}

pub enum BBEvent {
    Overlay(OverlayEvent),
    SubBrick//(SubBrickEvent),
}

struct BlueBrick {
    overlay: Overlay,
    subbrick_manager: SubBrickManager,
}

impl BlueBrick {
    fn new(config: Config) -> Result<Self> {
        let overlay = match Overlay::new(config) {
            Ok(overlay) => overlay,
            Err(e) => return Err(StartupErr::Overlay(e)),
        };

        if let Err(e) = hook() {
            return Err(StartupErr::Hooks(e));
        }

        let subbrick_manager = SubBrickManager::new();

        Ok(BlueBrick {
            overlay,
            subbrick_manager
        })
    }

    fn handle_event(&mut self, event: BBEvent) {
        match event {
            BBEvent::Overlay(event) => self.overlay.handle_event(event, &mut self.subbrick_manager),
            BBEvent::SubBrick => {}//(subbrick) => self.subbrick_manager.handle_event(event),
        }
    }
}

pub struct BlueBrickHandle {
    overlay: OverlayHandle,
    #[allow(unused)]
    tx: Sender<BBEvent>,
}

pub static BLUEBRICK_HANDLE: OnceLock<BlueBrickHandle> = OnceLock::new();
impl BlueBrickHandle {
    fn start(config: Config) {
        let (tx, rx) = mpsc::channel();

        thread::spawn(move || {
            let mut bb = match BlueBrick::new(config) {
                Ok(bb) => bb,
                Err(e) => {
                    main_log_error!("{e}");
                    let _ = msgbox::create("BlueBrick Failed To Start", "Continuing to the game without BlueBrick", msgbox::IconType::Info);
                    return;
                }
            };

            while let Ok(msg) = rx.recv() {
                bb.handle_event(msg);
            }
        });
        
        _ = BLUEBRICK_HANDLE.set(Self {
            overlay: OverlayHandle::new(config, tx.clone()),
            tx
        });
    }
}

#[unsafe(no_mangle)]
extern "C" fn start_bluebrick(config: Config) {
    BlueBrickHandle::start(config);
}
