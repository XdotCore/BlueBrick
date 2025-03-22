use std::{error::Error, mem};

use colored::Colorize;
use ctor::ctor;
use logger::init_terminal;
use retour::static_detour;

pub mod hooking;
pub mod logger;
pub mod memutils;

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

#[ctor]
fn hello() {
    init_terminal();
    println!("{}", "hello world! 🤡🍄🤯👨🏿🏳️‍🌈".red());

    if let Err(e) = hook() {
        msgbox::create("Error Loading BlueBrick", &format!("Problem hooking functions:\n{e:?}"), msgbox::IconType::Error);
    }
}
