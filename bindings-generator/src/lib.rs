use proc_macro::TokenStream;

mod bindings;

#[proc_macro]
pub fn include_bluebrick_bindings(item: TokenStream) -> TokenStream {
    bindings::include_bluebrick_bindings(item)
}
