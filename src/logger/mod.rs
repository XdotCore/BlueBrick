#[cfg(windows)]
mod windows;

use std::error::Error;

pub struct Logger {

}

pub(crate) fn init_terminal() -> Result<(), Box<dyn Error>> {
    #[cfg(windows)]
    windows::init_terminal()?;

    Ok(())
}
