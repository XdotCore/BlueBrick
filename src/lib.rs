use ctor::ctor;

#[ctor]
fn hello() {
    #[cfg(test)]
    return;

    msgbox::create("Wassup", "Bitches", msgbox::IconType::None);
}
