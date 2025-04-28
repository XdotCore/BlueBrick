use proc_macro::TokenStream;

mod subbricks;

#[proc_macro_attribute]
pub fn bluebrick_library(args: TokenStream, item: TokenStream) -> TokenStream {
    subbricks::bluebrick_library(args, item)
}
