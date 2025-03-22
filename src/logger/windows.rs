use windows::{
    Win32::{Foundation::HANDLE, System::Console},
    core::{Error, Result},
};

fn enable_color() -> Result<()> {
    unsafe {
        let output_handle = Console::GetStdHandle(Console::STD_OUTPUT_HANDLE)?;
        if output_handle == HANDLE::default() {
            return Err(Error::from_win32());
        }

        let mut mode = Console::CONSOLE_MODE(0);
        Console::GetConsoleMode(output_handle, &mut mode)?;
        Console::SetConsoleMode(output_handle, mode | Console::ENABLE_VIRTUAL_TERMINAL_PROCESSING)?;

        Ok(())
    }
}

pub fn init_terminal() -> Result<()> {
    unsafe {
        Console::AllocConsole()?;
        enable_color()?;

        Ok(())
    }
}
