use windows::{
    Win32::System::LibraryLoader::GetModuleHandleW,
    core::{Error, PCWSTR},
};

pub fn get_executable_base() -> Result<usize, Error> {
    unsafe { Ok(GetModuleHandleW(PCWSTR::null())?.0 as usize) }
}
