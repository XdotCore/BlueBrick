use imgui::Ui;

use crate::logger::HasLogger;

pub trait SubBrick : HasLogger {
    // TODO: add depends on function
    
    fn new() -> Self;
    fn init(&mut self);
    fn enable(&mut self) -> bool;
    fn disable(&mut self) -> bool;

    fn draw(&mut self, ui: &Ui);
}

pub trait Library : SubBrick {
}

pub trait Mod : SubBrick {
}
