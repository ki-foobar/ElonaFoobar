use crate::error::SdlError;
use anyhow::Result;
pub use sdl2::event::Event;
use sdl2::{EventPump, Sdl};

pub(crate) struct EventQueue(EventPump);

impl EventQueue {
    pub fn with_sdl(sdl: &Sdl) -> Result<Self> {
        sdl.event_pump().sdl_error().map(Self)
    }

    pub fn poll(&mut self) -> Option<Event> {
        self.0.poll_event()
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(C)]
pub enum EventKind {
    Quit,
    AppTerminating,
    AppLowMemory,
    AppWillEnterBackground,
    AppDidEnterBackground,
    AppWillEnterForeground,
    AppDidEnterForeground,
    Window,
    KeyDown,
    KeyUp,
    TextEditing,
    TextInput,
    MouseMotion,
    MouseButtonDown,
    MouseButtonUp,
    MouseWheel,
    JoyAxisMotion,
    JoyBallMotion,
    JoyHatMotion,
    JoyButtonDown,
    JoyButtonUp,
    JoyDeviceAdded,
    JoyDeviceRemoved,
    ControllerAxisMotion,
    ControllerButtonDown,
    ControllerButtonUp,
    ControllerDeviceAdded,
    ControllerDeviceRemoved,
    ControllerDeviceRemapped,
    FingerDown,
    FingerUp,
    FingerMotion,
    DollarGesture,
    DollarRecord,
    MultiGesture,
    ClipboardUpdate,
    DropFile,
    DropText,
    DropBegin,
    DropComplete,
    AudioDeviceAdded,
    AudioDeviceRemoved,
    RenderTargetsReset,
    RenderDeviceReset,
    User,
    Unknown,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(C)]
pub enum Key {
    Unknown,
    Up,
    Down,
    Enter,
}

pub trait EventExt {
    fn kind(&self) -> EventKind;
    fn key(&self) -> Option<Key>;
}

impl EventExt for Event {
    fn kind(&self) -> EventKind {
        match self {
            Event::Quit { .. } => EventKind::Quit,
            Event::AppTerminating { .. } => EventKind::AppTerminating,
            Event::AppLowMemory { .. } => EventKind::AppLowMemory,
            Event::AppWillEnterBackground { .. } => EventKind::AppWillEnterBackground,
            Event::AppDidEnterBackground { .. } => EventKind::AppDidEnterBackground,
            Event::AppWillEnterForeground { .. } => EventKind::AppWillEnterForeground,
            Event::AppDidEnterForeground { .. } => EventKind::AppDidEnterForeground,
            Event::Window { .. } => EventKind::Window,
            Event::KeyDown { .. } => EventKind::KeyDown,
            Event::KeyUp { .. } => EventKind::KeyUp,
            Event::TextEditing { .. } => EventKind::TextEditing,
            Event::TextInput { .. } => EventKind::TextInput,
            Event::MouseMotion { .. } => EventKind::MouseMotion,
            Event::MouseButtonDown { .. } => EventKind::MouseButtonDown,
            Event::MouseButtonUp { .. } => EventKind::MouseButtonUp,
            Event::MouseWheel { .. } => EventKind::MouseWheel,
            Event::JoyAxisMotion { .. } => EventKind::JoyAxisMotion,
            Event::JoyBallMotion { .. } => EventKind::JoyBallMotion,
            Event::JoyHatMotion { .. } => EventKind::JoyHatMotion,
            Event::JoyButtonDown { .. } => EventKind::JoyButtonDown,
            Event::JoyButtonUp { .. } => EventKind::JoyButtonUp,
            Event::JoyDeviceAdded { .. } => EventKind::JoyDeviceAdded,
            Event::JoyDeviceRemoved { .. } => EventKind::JoyDeviceRemoved,
            Event::ControllerAxisMotion { .. } => EventKind::ControllerAxisMotion,
            Event::ControllerButtonDown { .. } => EventKind::ControllerButtonDown,
            Event::ControllerButtonUp { .. } => EventKind::ControllerButtonUp,
            Event::ControllerDeviceAdded { .. } => EventKind::ControllerDeviceAdded,
            Event::ControllerDeviceRemoved { .. } => EventKind::ControllerDeviceRemoved,
            Event::ControllerDeviceRemapped { .. } => EventKind::ControllerDeviceRemapped,
            Event::FingerDown { .. } => EventKind::FingerDown,
            Event::FingerUp { .. } => EventKind::FingerUp,
            Event::FingerMotion { .. } => EventKind::FingerMotion,
            Event::DollarGesture { .. } => EventKind::DollarGesture,
            Event::DollarRecord { .. } => EventKind::DollarRecord,
            Event::MultiGesture { .. } => EventKind::MultiGesture,
            Event::ClipboardUpdate { .. } => EventKind::ClipboardUpdate,
            Event::DropFile { .. } => EventKind::DropFile,
            Event::DropText { .. } => EventKind::DropText,
            Event::DropBegin { .. } => EventKind::DropBegin,
            Event::DropComplete { .. } => EventKind::DropComplete,
            Event::AudioDeviceAdded { .. } => EventKind::AudioDeviceAdded,
            Event::AudioDeviceRemoved { .. } => EventKind::AudioDeviceRemoved,
            Event::RenderTargetsReset { .. } => EventKind::RenderTargetsReset,
            Event::RenderDeviceReset { .. } => EventKind::RenderDeviceReset,
            Event::User { .. } => EventKind::User,
            Event::Unknown { .. } => EventKind::Unknown,
        }
    }

    fn key(&self) -> Option<Key> {
        use sdl2::keyboard::Keycode;

        match self {
            Event::KeyDown {
                keycode: Some(k), ..
            } => Some(match k {
                Keycode::Up => Key::Up,
                Keycode::Down => Key::Down,
                Keycode::Return => Key::Enter,
                _ => Key::Unknown,
            }),
            Event::KeyUp {
                keycode: Some(k), ..
            } => Some(match k {
                Keycode::Up => Key::Up,
                Keycode::Down => Key::Down,
                Keycode::Return => Key::Enter,
                _ => Key::Unknown,
            }),
            _ => None,
        }
    }
}
