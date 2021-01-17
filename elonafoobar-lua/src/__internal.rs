pub use crate::conv::{FromLuaValues, ToLuaValues};
use crate::ffi::State;
pub use crate::ffi::{luaL_error, lua_State, Nresults};
use crate::state::Lua;

pub fn wrap_lua(inner: State) -> Lua {
    Lua::wrap(inner)
}
