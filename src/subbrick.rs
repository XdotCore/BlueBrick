use std::{ffi::{c_char, CStr, OsStr}, fs::{self, DirEntry}};

use dlopen::wrapper::{Container, WrapperApi};
use dlopen_derive::WrapperApi;

use crate::{log, log_error, log_warning, logger::{HasLogger, Logger}, MainLogger};

pub trait SubBrick : HasLogger {
    // TODO: add depends on function
    
    fn init();
    fn enable();
    fn disable();
}

pub trait Library : SubBrick {
}

pub trait Mod : SubBrick {
}

#[derive(WrapperApi)]
pub(crate) struct SubBrickApi {
    name: extern "C" fn() -> *const c_char,
    author: extern "C" fn() -> *const c_char,
    version: extern "C" fn() -> *const c_char,

    init: extern "C" fn(),
    enable: extern "C" fn(),
    disable: extern "C" fn(),
}

impl SubBrickApi {
    fn to_string(string: *const c_char) -> String {
        let string = unsafe { CStr::from_ptr(string) };
        String::from_utf8_lossy(string.to_bytes()).to_string()
    }

    fn name_string(&self) -> String { Self::to_string((self.name)()) }
    fn author_string(&self) -> String { Self::to_string((self.author)()) }
    fn version_string(&self) -> String { Self::to_string((self.version)()) }
}

pub(crate) struct SubBrickManager {
    libraries: Vec<Container<SubBrickApi>>,
    mods: Vec<Container<SubBrickApi>>,
}

impl SubBrickManager {
    pub fn new() -> Self {
        let mut new = Self {
            libraries: Vec::new(),
            mods: Vec::new()
        };

        log!(MainLogger::instance(), "Loading Libraries:");
        Self::load_subbricks("libraries", "bluebrick/libraries", &mut new.libraries);

        log!(MainLogger::instance(), "Loading Mods:");
        Self::load_subbricks("mods", "bluebrick/mods", &mut new.mods);

        new
    }

    pub fn load_subbricks(kind: &str, folder: &str, subbricks: &mut Vec<Container<SubBrickApi>>) {
        fn get_file_name(entry: &DirEntry) -> String {
            entry.file_name().to_string_lossy().to_string()
        }

        match fs::exists(folder) {
            Ok(true) => {}
            Ok(false) => {
                if let Err(e) = fs::create_dir_all(folder) {
                    log_error!(MainLogger::instance(), "BlueBrick {kind} folder missing, and could not be created: {e}");
                    return;
                }
            }
            Err(e) => {
                log_error!(MainLogger::instance(), "Unable to determine BlueBrick {kind} folder existence: {e}");
                return;
            }
        }

        let entries = match fs::read_dir(folder) {
            Ok(entries) => entries,
            Err(e) => {
                log_error!(MainLogger::instance(), "Unable to read BlueBrick {kind} folder entries: {e}");
                return;
            }
        }.filter_map(|entry| match entry {
            Ok(entry) => {
                let file_type = match entry.file_type() {
                    Ok(file_type) => file_type,
                    Err(e) => {
                        log_warning!(MainLogger::instance(), "Unable to read file type of {} in BlueBrick {} folder: {}", get_file_name(&entry), kind, e);
                        return None;
                    }
                };
                if file_type.is_file() && ["dll", "so", "dylib"].map(|s| OsStr::new(s)).contains(&entry.path().extension().unwrap_or_default()) {
                    Some(entry)
                } else {
                    None
                }
            }
            Err(e) => {
                log_warning!(MainLogger::instance(), "Unable to read entry in BlueBrick {kind} folder: {e}");
                None
            }
        });

        for entry in entries {
            let subbrick = match unsafe { Container::<SubBrickApi>::load(entry.path()) } {
                Ok(library) => library,
                Err(e) => {
                    log_warning!(MainLogger::instance(), "Unable to load {} in BlueBrick {kind} folder: {}", get_file_name(&entry), e);
                    continue;
                }
            };
            log!(MainLogger::instance(), "Loaded {} v{} by {} from {}", subbrick.name_string(), subbrick.version_string(), subbrick.author_string(), get_file_name(&entry));
            subbrick.init();
            subbrick.enable();
            subbricks.push(subbrick);
        }
    }
}
