pub mod logger;
mod memutils;
mod overlay;
pub mod subbrick;

use std::{error::Error, fmt, mem, ptr};

use bluebrick::proxy::Config;
use logger::{main_log_error, MainLogger};
use overlay::Overlay;
use retour::static_detour;
use subbrick::SubBrickManager;

static_detour! {
    static AddToCoins: unsafe extern "cdecl" fn(*mut u64, u64, i32, bool);
}

fn hook() -> std::result::Result<(), Box<dyn Error>> {
    let real_add_to_coins = memutils::get_executable_base()? + 0x7E1070;
    let real_add_to_coins = unsafe { mem::transmute(real_add_to_coins) };

    unsafe {
        AddToCoins.initialize(real_add_to_coins, |coins_ptr, to_add, mult, round_to_10s| {
            let _ = msgbox::create("Got coin", &format!("worth: {to_add}, mult: {mult}"), msgbox::IconType::Info);
            AddToCoins.call(coins_ptr, 0, mult, round_to_10s);
        })?;
        AddToCoins.enable()?;
    }

    Ok(())
}

type Result<T> = std::result::Result<T, StartupErr>;

#[derive(Debug)]
enum StartupErr {
    Overlay(Box<dyn Error>),
    Hooks(Box<dyn Error>),
    NotInitialized
}

impl fmt::Display for StartupErr {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        use StartupErr::*;
        let message = match self {
            Overlay(e) => format!("Problem attaching imgui: {e}"),
            Hooks(e) => format!("Problem hooking functions: {e}"),
            NotInitialized => format!("Attempted to use BlueBrick before it is initialized"),
        };
        write!(f, "Error starting up BlueBrick: {}", message)
    }
}

struct BlueBrick {
    overlay: Overlay,
    #[allow(dead_code)] // TODO: use this to (un)load mods
    subbrick_manager: SubBrickManager,
}

impl BlueBrick {
    fn get_or_init(config: Option<Config>) -> Result<&'static mut BlueBrick> {
        static mut INSTANCE: *mut BlueBrick = ptr::null_mut();
        if unsafe { INSTANCE.is_null() }  {
            match config {
                Some(config) => {
                    let overlay = match Overlay::new(config) {
                        Ok(overlay) => overlay,
                        Err(e) => {
                            let e = StartupErr::Overlay(e);
                            main_log_error!("{e}");
                            return Err(e);
                        }
                    };

                    if let Err(e) = hook() {
                        let e = StartupErr::Hooks(e);
                        main_log_error!("{e}");
                        return Err(e);
                    }

                    let subbrick_manager = SubBrickManager::new();

                    unsafe {
                        INSTANCE = Box::into_raw(Box::new(BlueBrick {
                            overlay,
                            subbrick_manager
                        }));
                    }
                }
                None => {
                    let e = StartupErr::NotInitialized;
                    let _ = msgbox::create("Error getting BlueBrick", &format!("{e}"), msgbox::IconType::Error);
                    return Err(e);
                }
            }
        }
        Ok(unsafe { &mut *INSTANCE })
    }

    pub fn instance() -> &'static mut BlueBrick {
        match Self::get_or_init(None) {
            Ok(bb) => bb,
            Err(e) => panic!("{e}")
        }
    }
}

#[unsafe(no_mangle)]
extern "C" fn start_bluebrick(config: Config) {
    if let Err(_) = BlueBrick::get_or_init(Some(config)) {
        let _ = msgbox::create("BlueBrick Failed To Start", "Continuing to the game without BlueBrick", msgbox::IconType::Info);
    }
}
