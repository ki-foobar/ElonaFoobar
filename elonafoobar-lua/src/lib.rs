#![feature(maybe_uninit_extra, ptr_as_uninit)]

pub use elonafoobar_lua_proc_macros as macros;

pub mod conv;
mod ffi;
pub mod state;
pub mod types;
pub mod utils;

pub use crate::state::Lua;
pub use crate::types::{AsLuaInt, LuaUserdata};

pub use crate::conv::{FromLuaInt, IntoLuaInt};

// It is public for `elonafoobar_lua_proc_macros`.
pub mod __internal;
