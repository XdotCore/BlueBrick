use dlopen::wrapper::{Container, WrapperApi};
use dlopen_derive::WrapperApi;

#[repr(C)]
#[derive(PartialEq, Clone, Copy)]
pub enum RequestedRenderer {
    DX9,
}

#[repr(C)]
#[derive(PartialEq, Clone, Copy)]
pub enum RequestedPlatform {
    Win32,
}

#[repr(C)]
pub struct Config {
    pub platform: RequestedPlatform,
    pub renderer: RequestedRenderer
}

#[derive(WrapperApi)]
struct BBApi {
    start_bluebrick: extern "C" fn(config: Config),
}

pub fn load_bluebrick(config: Config) {
    let bluebrick = match unsafe { Container::<BBApi>::load("bluebrick/bluebrick") } {
        Ok(bb) => bb,
        Err(e) => {
            let _ = msgbox::create("Error Loading BlueBrick", &format!("Problem opening loader:\n{e:?}"), msgbox::IconType::Error);
            return;
        }
    };

    bluebrick.start_bluebrick(config);

    std::mem::forget(bluebrick); // keeps from dropping, which would unload bluebrick
}
