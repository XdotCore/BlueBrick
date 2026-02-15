pub mod dx9;

use std::error::Error;
use std::sync::mpsc::Sender;

use bluebrick_proxy::{Config, RequestedRenderer};

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
    pub fn new(config: Config) -> Result<Self, Box<dyn Error>> {
        Ok(match config.renderer {
            RequestedRenderer::DX9 => SomeRenderer::DX9(DX9::new()?),
        })
    }

    #[allow(unused)]
    fn get_inner(&self) -> &dyn Renderer {
        match self {
            Self::DX9(dx9) => dx9,
        }
    }

    pub fn handle_event(&mut self, event: RendererEvent) {
        match (event, self) {
            (RendererEvent::DX9(dx9_event), SomeRenderer::DX9(dx9)) => {
                dx9.handle_event(dx9_event);
            }
            // uncomment when there is more than 1 renderer supported
            /*_ => {
                msgbox::create("Mismatched renderer types", "A BlueBrick event was triggered with the wrong renderer type", msgbox::IconType::Error);
            }*/
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
    pub fn new(config: Config, tx: Sender<BBEvent>) -> Self {
        match config.renderer {
            RequestedRenderer::DX9 => SomeRendererHandle::DX9(DX9Handle::new(tx)),
        }
    }

    #[allow(unused)]
    fn get_inner(&self) -> &dyn RendererHandle {
        match self {
            Self::DX9(dx9) => dx9,
        }
    }
}

impl RendererHandle for SomeRendererHandle {

}
