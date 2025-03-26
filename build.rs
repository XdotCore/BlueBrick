fn add_backend_to_files(files: &mut Vec<String>, backend: &str) {
    let c = format!("src/overlay/imgui_impl_{backend}.cpp");
    let cpp = format!("src/overlay/imgui/backends/imgui_impl_{backend}.cpp");

    println!("cargo:rerun-if-changed={c}");
    println!("cargo:rerun-if-changed={cpp}");

    files.push(c);
    files.push(cpp);
}

fn main() {
    let mut files = Vec::new();

    #[cfg(feature = "win32")]
    add_backend_to_files(&mut files, "win32");

    #[cfg(feature = "dx9")]
    add_backend_to_files(&mut files, "dx9");

    cc::Build::new().files(files).include("src/overlay/imgui/").cpp(true).compile("imgui");
}
