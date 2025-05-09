use imgui::Ui;

use crate::logger::HasLogger;

pub trait SubBrick : HasLogger {
    // TODO: add depends on function
    
    fn init();
    fn enable() -> bool;
    fn disable() -> bool;

    fn draw(ui: &mut Ui);
}

pub trait Library : SubBrick {
}

pub trait Mod : SubBrick {
}
