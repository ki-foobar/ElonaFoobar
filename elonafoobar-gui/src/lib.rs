pub mod app;
mod audio;
mod error;
mod event;
mod font_cache;
mod image;

pub use app::{App, TextAlignment, TextBaseline};
pub use event::{Event, EventExt, EventKind, Key};
pub use image::Image;
pub use sdl2::pixels::Color;
pub use sdl2::rect::{Point, Rect};
pub use sdl2::ttf::FontStyle;
pub use sdl2::video::FullscreenType;
