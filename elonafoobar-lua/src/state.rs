use crate::conv::ToLuaValues;
use crate::types::{LuaFloat, LuaFnPtr, LuaInt};
use crate::{ffi, LuaUserdata};
use anyhow::{format_err, Result};
use std::path::{self, Path, PathBuf};
use std::ptr::NonNull;

pub struct Lua {
    inner: ffi::State,
}

impl Lua {
    pub fn new() -> Result<Lua> {
        Ok(Lua {
            inner: ffi::luaL_newstate()?,
        })
    }

    pub fn open_builtin_libraries(&mut self) {
        ffi::luaL_openlibs(self.inner);
    }

    pub fn load(&mut self, path: &Path) -> Result<()> {
        ffi::lua_load(self.inner, path)
    }

    pub fn call<A>(&mut self, args: A) -> Result<()>
    where
        A: ToLuaValues,
    {
        let nargs = args.push_all(self.inner)?;
        ffi::lua_pcall(self.inner, nargs, 0)?;
        Ok(())
    }

    pub fn set_load_path(&mut self, new_load_path: &Path) -> Result<()> {
        // TODO:
        // Lua does not support UTF-8 path by default on Windows.
        // We need to use a patched version of Lua.

        let new_load_path = new_load_path.to_str().ok_or_else(|| {
            format_err!(
                "path '{}' contains invalid Unicode sequence",
                new_load_path.display()
            )
        })?;
        let new_load_path = format!("{}{}?.lua", new_load_path, path::MAIN_SEPARATOR);

        // package.path = new_load_path
        ffi::lua_getglobal(self.inner, "package", ffi::LUA_TTABLE)?;
        ffi::lua_pushstring(self.inner, "path")?;
        ffi::lua_pushstring(self.inner, &new_load_path)?;
        ffi::lua_rawset(self.inner, -3);
        ffi::lua_pop_one(self.inner);

        Ok(())
    }

    pub fn set_function(&mut self, func_name: &str, func: LuaFnPtr) -> Result<()> {
        ffi::lua_pushstring(self.inner, func_name)?;
        ffi::lua_pushcfunction(self.inner, func);
        ffi::lua_rawset(self.inner, -3);
        Ok(())
    }

    pub fn set_string(&mut self, name: &str, string: &str) -> Result<()> {
        ffi::lua_pushstring(self.inner, name)?;
        ffi::lua_pushstring(self.inner, string)?;
        ffi::lua_rawset(self.inner, -3);
        Ok(())
    }

    pub fn set_integer(&mut self, name: &str, value: LuaInt) -> Result<()> {
        ffi::lua_pushstring(self.inner, name)?;
        ffi::lua_pushinteger(self.inner, value);
        ffi::lua_rawset(self.inner, -3);
        Ok(())
    }

    pub fn set_float(&mut self, name: &str, value: LuaFloat) -> Result<()> {
        ffi::lua_pushstring(self.inner, name)?;
        ffi::lua_pushnumber(self.inner, value);
        ffi::lua_rawset(self.inner, -3);
        Ok(())
    }

    pub fn set_userdata_metatable(&mut self, name: &str, userdata_name: &str) -> Result<()> {
        ffi::lua_pushstring(self.inner, name)?;
        ffi::get_userdata_metatable(self.inner, userdata_name)?;
        ffi::lua_pushstring(self.inner, "__index")?;
        ffi::lua_rawget(self.inner, -2);
        ffi::lua_swap(self.inner);
        ffi::lua_pop_one(self.inner);
        ffi::lua_rawset(self.inner, -3);
        Ok(())
    }

    pub fn get_caller_path(&mut self) -> Result<PathBuf> {
        let source = ffi::get_debug_info_source(self.inner, 2)?;
        if source.is_empty() {
            return Err(format_err!("invalid source file"));
        }
        if source.as_bytes()[0] != b'@' {
            return Err(format_err!(
                "the caller function is defined by evaluating a string"
            ));
        }
        let caller_path = std::str::from_utf8(&source.as_bytes()[1..])?; // trim the leading '@'
        Ok(caller_path.into())
    }

    pub fn bind_class<T, F>(&mut self, bind: F) -> Result<()>
    where
        T: LuaUserdata,
        F: FnOnce(&mut Self) -> Result<()>,
    {
        extern "C" fn free<T>(lua_state: *mut ffi::lua_State) -> ffi::Nresults
        where
            T: LuaUserdata,
        {
            unsafe {
                let lua_state = NonNull::new(lua_state).expect("Lua state is null");

                let object = ffi::lua_touserdata(lua_state, -1);
                let object = object as *mut T;
                std::ptr::drop_in_place(object);

                0
            }
        }

        self.new_metatable(T::NAME)?;
        self.new_table();

        bind(self)?;

        self.set("__index")?;
        ffi::lua_pushstring(self.inner, "__gc")?;
        ffi::lua_pushcfunction(self.inner, free::<T>);
        ffi::lua_rawset(self.inner, -3);
        self.pop_one();

        Ok(())
    }

    pub fn bind_module<F>(&mut self, module_name: &str, bind: F) -> Result<()>
    where
        F: FnOnce(&mut Self) -> Result<()>,
    {
        self.new_table();

        bind(self)?;

        self.set(module_name)?;
        Ok(())
    }

    pub fn bind_toplevel_module<F>(&mut self, module_name: &str, bind: F) -> Result<()>
    where
        F: FnOnce(&mut Self) -> Result<()>,
    {
        self.new_table();

        bind(self)?;

        self.set_global(module_name)?;
        Ok(())
    }

    fn new_table(&mut self) {
        ffi::lua_newtable(self.inner);
    }

    fn new_metatable(&mut self, userdata_name: &str) -> Result<()> {
        ffi::new_metatable(self.inner, userdata_name)?;
        Ok(())
    }

    fn set(&mut self, name: &str) -> Result<()> {
        ffi::lua_pushstring(self.inner, name)?;
        ffi::lua_swap(self.inner);
        ffi::lua_rawset(self.inner, -3);
        Ok(())
    }

    fn pop_one(&mut self) {
        ffi::lua_pop_one(self.inner);
    }

    fn set_global(&mut self, name: &str) -> Result<()> {
        ffi::set_global(self.inner, name)?;
        Ok(())
    }

    pub(crate) fn wrap(inner: ffi::State) -> Lua {
        Lua { inner }
    }
}
