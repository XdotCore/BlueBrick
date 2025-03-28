pub mod logger;
mod memutils;
mod overlay;

use std::{error::Error, mem};

use bluebrick_proxy_base::{Platform, Renderer};
use colored::Colorize;
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
        AddToCoins.initialize(real_add_to_coins, |coins_ptr, to_add, mult_enabled, round_to_10s| {
            let _ = msgbox::create("Got coin", &format!("worth: {to_add}"), msgbox::IconType::Info);
            AddToCoins.call(coins_ptr, 0, mult_enabled, round_to_10s);
        })?;
        AddToCoins.enable()?;
    }

    Ok(())
}

#[unsafe(no_mangle)]
pub extern "C" fn start_bluebrick(platform: Platform, renderer: Renderer) {
    let _ = msgbox::create("For debugging", "For debugging", msgbox::IconType::None);

    if let Err(e) = init_terminal() {
        let _ = msgbox::create("Error Starting Up BlueBrick", &format!("Problem starting terminal:\n{e:?}"), msgbox::IconType::Error);
    }
    println!("{}", "hello world! 🤡🍄🤯👨🏿🏳️‍🌈".red());
    // TODO: replace all msgbox from recoverable errors with messages to the log

    if let Err(e) = Overlay::start(platform, renderer) {
        let _ = msgbox::create("Error Starting Up BlueBrick", &format!("Problem attaching imgui:\n{e:?}"), msgbox::IconType::Error);
    }

    if let Err(e) = hook() {
        let _ = msgbox::create("Error Starting Up BlueBrick", &format!("Problem hooking functions:\n{e:?}"), msgbox::IconType::Error);
    }
}
