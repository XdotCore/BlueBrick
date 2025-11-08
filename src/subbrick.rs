use std::{ffi::{CStr, OsStr, c_char, c_void}, fs::{self, DirEntry}};

use dlopen::wrapper::{Container, WrapperApi};
use dlopen_derive::WrapperApi;
use bluebrick::{imgui::{self, Ui, sys::ImGuiContext}};

use crate::logger::{main_log_debug, main_log_error, main_log_warning};

fn get_file_name(entry: &DirEntry) -> String {
    entry.file_name().to_string_lossy().to_string()
}

#[derive(WrapperApi)]
pub(crate) struct SubBrickApi {
    name: extern "C" fn() -> *const c_char,
    author: extern "C" fn() -> *const c_char,
    version: extern "C" fn() -> *const c_char,

    new: extern "C" fn() -> *mut c_void,
    init: extern "C" fn(subbrick: *mut c_void),
    enable: extern "C" fn(subbrick: *mut c_void) -> bool,
    disable: extern "C" fn(subbrick: *mut c_void) -> bool,

    set_imgui_ctx: extern "C" fn(ctx: *mut ImGuiContext),
    draw: extern "C" fn(subbrick: *mut c_void, ui: &Ui),
}

pub(crate) struct SubBrick {
    ptr: *mut c_void,
    api: Container<SubBrickApi>,
    file: DirEntry,
    enabled: bool,
}

impl SubBrick {
    fn new(api: Container<SubBrickApi>, file: DirEntry) -> Self {
        Self {
            ptr: api.new(),
            api,
            file,
            enabled: false,
        }
    }

    fn to_string(string: *const c_char) -> String {
        let string = unsafe { CStr::from_ptr(string) };
        String::from_utf8_lossy(string.to_bytes()).to_string()
    }

    fn name(&self) -> String { Self::to_string((self.api.name)()) }
    fn author(&self) -> String { Self::to_string((self.api.author)()) }
    fn version(&self) -> String { Self::to_string((self.api.version)()) }
    fn file_name(&self) -> String { get_file_name(&self.file) }
    
    fn init(&self) {
        (self.api.init)(self.ptr);
        main_log_debug!("Loaded {}", self.string_info());
    }
    fn enable(&mut self) -> bool {
        let result = (self.api.enable)(self.ptr);
        if result {
            self.enabled = true;
            main_log_debug!("Enabled {}", self.string_info());
        } else {
            main_log_warning!("Failed to enable {}", self.string_info());
        }
        result
    }
    fn disable(&mut self) -> bool {
        let result = (self.api.disable)(self.ptr);
        if  result {
            self.enabled = false;
            main_log_debug!("Disabled {}", self.string_info());
        } else {
            main_log_warning!("Failed to disable {}", self.string_info());
        }
        result
    }

    fn set_imgui_ctx(&self, ctx: *mut ImGuiContext) { (self.api.set_imgui_ctx)(ctx) }
    fn draw(&self, ui: &Ui) { (self.api.draw)(self.ptr, ui) }

    fn string_info(&self) -> String {
        format!("{} v{} by {} from {}", self.name(), self.version(), self.author(), self.file_name())
    }

    fn draw_brick_list_item(&mut self, ui: &Ui) {
        ui.group(|| {
            ui.text(format!("{} v{}", self.name(), self.version()));
            ui.text(format!("by {}", self.author()));
            ui.same_line();
            ui.text_colored([0.5, 0.5, 0.5, 1.0], format!("from {}", self.file_name()));
        });
        
        ui.same_line();
        ui.dummy([50.0, 0.0]);
        ui.same_line();
        ui.group(|| {
            ui.disabled(self.enabled, || {
                if ui.button("Enable") {
                    self.enable();
                }
            });

            ui.disabled(!self.enabled, || {
                if ui.button("Disable") {
                    self.disable();
                }
            });
        });

        ui.same_line();
        ui.dummy([50.0, 0.0]);
        ui.same_line();
        ui.group(|| {
            if ui.button("Settings") {
                
            }
        })
    }
}

pub(crate) struct SubBrickManager {
    libraries: Vec<SubBrick>,
    mods: Vec<SubBrick>,
}

impl SubBrickManager {
    pub fn new() -> Self {
        let mut new = Self {
            libraries: Vec::new(),
            mods: Vec::new()
        };

        main_log_debug!("Loading Libraries:");
        Self::load_subbricks("libraries", "bluebrick/libraries", &mut new.libraries);

        main_log_debug!("Loading Mods:");
        Self::load_subbricks("mods", "bluebrick/mods", &mut new.mods);

        new
    }

    fn load_subbricks(kind: &str, folder: &str, subbricks: &mut Vec<SubBrick>) {
        match fs::exists(folder) {
            Ok(true) => {}
            Ok(false) => {
                if let Err(e) = fs::create_dir_all(folder) {
                    main_log_error!("BlueBrick {kind} folder missing, and could not be created: {e}");
                    return;
                }
            }
            Err(e) => {
                main_log_error!("Unable to determine BlueBrick {kind} folder existence: {e}");
                return;
            }
        }

        let entries = match fs::read_dir(folder) {
            Ok(entries) => entries,
            Err(e) => {
                main_log_error!("Unable to read BlueBrick {kind} folder entries: {e}");
                return;
            }
        }.filter_map(|entry| match entry {
            Ok(entry) => {
                let file_type = match entry.file_type() {
                    Ok(file_type) => file_type,
                    Err(e) => {
                        main_log_warning!("Unable to read file type of {} in BlueBrick {} folder: {}", get_file_name(&entry), kind, e);
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
                main_log_warning!("Unable to read entry in BlueBrick {kind} folder: {e}");
                None
            }
        });

        for entry in entries {
            let mut subbrick = SubBrick::new(match unsafe { Container::<SubBrickApi>::load(entry.path()) } {
                Ok(library) => library,
                Err(e) => {
                    main_log_warning!("Unable to load {} in BlueBrick {kind} folder: {}", get_file_name(&entry), e);
                    continue;
                }
            }, entry);

            subbrick.set_imgui_ctx(unsafe { imgui::sys::igGetCurrentContext() });
            subbrick.init();
            subbrick.enable();
            subbricks.push(subbrick);
        }
    }

    pub fn draw_all(&self, ui: &Ui) {
        for subbrick in self.libraries.iter().chain(self.mods.iter()) {
            subbrick.draw(ui);
        }
    }

    pub fn draw_library_list(&mut self, ui: &Ui) {
        for library in &mut self.libraries {
            library.draw_brick_list_item(ui);
        }
    }

    pub fn draw_mod_list(&mut self, ui: &Ui) {
        for mmod in &mut self.mods {
            mmod.draw_brick_list_item(ui);
        }
    }
}
