use std::ffi::CString;

use proc_macro::TokenStream;
use quote::quote;
use syn::{parse::{Parse, ParseStream}, parse_macro_input, ItemStruct, LitCStr, LitStr, Result, Token};

struct SubBrickNames {
    name: LitStr,
    author: LitStr,
}

impl Parse for SubBrickNames {
    fn parse(input: ParseStream) -> Result<Self> {
        let name = input.parse()?;
        let _ = input.parse::<Token![,]>();
        let author = input.parse()?;
        Ok(SubBrickNames { name, author })
    }
}

pub fn bluebrick_library(args: TokenStream, item: TokenStream) -> TokenStream {
    let SubBrickNames {
        name,
        author,
    } = parse_macro_input!(args as SubBrickNames);

    let name_cstr = LitCStr::new(&CString::new(name.value()).unwrap(), name.span());
    let author_cstr = LitCStr::new(&CString::new(author.value()).unwrap(), author.span());

    let lib = parse_macro_input!(item as ItemStruct);
    let lib_name = &lib.ident;

    let result = quote! {
        use bluebrick::logger::HasLogger; // this pollution is necessary for ease of use

        const _: () = {
            #[unsafe(no_mangle)]
            extern "C" fn name() -> *const std::ffi::c_char {
                const name: &std::ffi::CStr = #name_cstr;
                name.as_ptr()
            }

            #[unsafe(no_mangle)]
            extern "C" fn author() -> *const std::ffi::c_char {
                const author: &std::ffi::CStr = #author_cstr;
                author.as_ptr()
            }

            #[unsafe(no_mangle)]
            extern "C" fn version() -> *const std::ffi::c_char {
                static VERSION: std::sync::OnceLock<std::ffi::CString> = std::sync::OnceLock::new();
                VERSION.get_or_init(|| std::ffi::CString::new(std::env!("CARGO_PKG_VERSION")).unwrap()).as_ptr()
            }

            #[unsafe(no_mangle)]
            extern "C" fn init() { #lib_name::init(); }

            #[unsafe(no_mangle)]
            extern "C" fn enable() -> bool { #lib_name::enable() }

            #[unsafe(no_mangle)]
            extern "C" fn disable() -> bool { #lib_name::disable() }

            #[unsafe(no_mangle)]
            extern "C" fn set_imgui_ctx(ctx: *mut bluebrick::imgui::sys::ImGuiContext) { unsafe { bluebrick::imgui::sys::igSetCurrentContext(ctx) }; }

            #[unsafe(no_mangle)]
            extern "C" fn draw(ui: &mut bluebrick::imgui::Ui) { #lib_name::draw(ui); }
        };

        #lib

        impl HasLogger for #lib_name {
            fn logger() -> &'static impl bluebrick::logger::Logger {
                static LOGGER: std::sync::OnceLock<bluebrick::logger::LibraryLogger> = std::sync::OnceLock::new();
                LOGGER.get_or_init(|| bluebrick::logger::LibraryLogger::new(#name))
            }
        }

        trait IsLibrary : bluebrick::subbrick::Library {}
        impl IsLibrary for #lib_name {}
    };

    TokenStream::from(result)
}
