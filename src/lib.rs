pub mod logger;
mod memutils;
mod overlay;
pub mod proxy;

use std::{error::Error, mem, ptr};

use colored::Colorize;
use proxy::Config;
use logger::init_terminal;
use overlay::Overlay;
use retour::static_detour;

static_detour! {
    static AddToCoins: unsafe extern "cdecl" fn(*mut u64, u64, i32, bool);
}

fn hook() -> Result<(), Box<dyn Error>> {
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

struct BlueBrick {
    overlay: overlay::Overlay
}

impl BlueBrick {
    // TODO: make thread safe if it ever becomes worth it
    fn get_or_init(config: Option<Config>) -> &'static mut BlueBrick {
        static mut INSTANCE: *mut BlueBrick = ptr::null_mut();
        match config {
            Some(config) => {
                if let Err(e) = init_terminal() {
                    let msg = &format!("Problem starting terminal:\n{e:?}");
                    let _ = msgbox::create("Error Starting Up BlueBrick", msg, msgbox::IconType::Error);
                    panic!("{msg}");
                }
                println!("{}", "hello world! 🤡🍄🤯👨🏿🏳️‍🌈".red());
                // TODO: replace all msgbox from recoverable errors with messages to the log

                let overlay = match Overlay::new(config) {
                    Ok(overlay) => overlay,
                    Err(e) => {
                        let msg = &format!("Problem attaching imgui:\n{e:?}");
                        let _ = msgbox::create("Error Starting Up BlueBrick", msg, msgbox::IconType::Error);
                        panic!("{msg}");
                    }
                };

                if let Err(e) = hook() {
                    let msg = &format!("Problem hooking functions:\n{e:?}");
                    let _ = msgbox::create("Error Starting Up BlueBrick", msg, msgbox::IconType::Error);
                    panic!("{msg}");
                }

                unsafe {
                    INSTANCE = Box::into_raw(Box::new(BlueBrick {
                        overlay
                    }));
                }
            }
            None => {
                if unsafe { INSTANCE.is_null() } {
                    let msg = "Attempted to use BlueBrick before it is initialized";
                    let _ = msgbox::create("Failed to get BlueBrick instance", msg, msgbox::IconType::Error);
                    panic!("{msg}");
                }
            }
        }
        unsafe { &mut *INSTANCE }
    }

    pub fn instance() -> &'static mut BlueBrick {
        Self::get_or_init(None)
    }
}

#[unsafe(no_mangle)]
pub extern "C" fn start_bluebrick(config: Config) {
    let _ = msgbox::create("For debugging", "For debugging", msgbox::IconType::None);

    BlueBrick::get_or_init(Some(config));
}
