fn add_file(files: &mut Vec<String>, file_path: &str) {
    println!("cargo:rerun-if-changes={file_path}");
    files.push(String::from(file_path));
}

fn add_imgui_files(files: &mut Vec<String>, file_name: &str, imgui_folder: &str) {
    let c = format!("src/overlay/{file_name}.cpp");
    let cpp = format!("src/overlay/imgui/{imgui_folder}/{file_name}.cpp");

    add_file(files, &c);
    add_file(files, &cpp);
}

fn build_imgui_parts() {
    let mut files = Vec::new();

    add_imgui_files(&mut files, "imgui_impl_win32", "backends");
    add_imgui_files(&mut files, "imgui_impl_dx9", "backends");

    cc::Build::new()
        .files(files)
        .include("src/overlay/imgui/")
        .cpp(true)
        .compile("imgui");
}

fn main() {
    build_imgui_parts();
}
