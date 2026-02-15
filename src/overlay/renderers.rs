pub mod dx9;

use crate::BBEvent;
use crate::overlay::OverlayEvent;
use crate::overlay::renderers::dx9::{DX9, DX9Event, DX9Handle};

pub enum RendererEvent {
    DX9(DX9Event),
}

impl Into<BBEvent> for RendererEvent {
    fn into(self) -> BBEvent {
        OverlayEvent::Renderer(self).into()
    }
}

pub trait Renderer {

}

pub enum SomeRenderer {
    DX9(DX9),
}

impl SomeRenderer {
    #[allow(unused)]
    fn get_inner(&self) -> &dyn Renderer {
        match self {
            Self::DX9(dx9) => dx9,
        }
    }
}

impl Renderer for SomeRenderer {

}

pub trait RendererHandle {

}

pub enum SomeRendererHandle {
    DX9(DX9Handle),
}

impl SomeRendererHandle {
    #[allow(unused)]
    fn get_inner(&self) -> &dyn RendererHandle {
        match self {
            Self::DX9(dx9) => dx9,
        }
    }
}

impl RendererHandle for SomeRendererHandle {

}
