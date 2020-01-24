#include "lua_env.hpp"

#include "../character.hpp"
#include "../config.hpp"
#include "../item.hpp"
#include "api_manager.hpp"
#include "config_manager.hpp"
#include "console.hpp"
#include "data_manager.hpp"
#include "event_manager.hpp"
#include "export_manager.hpp"
#include "handle_manager.hpp"
#include "i18n_function_manager.hpp"
#include "mod_manager.hpp"



namespace elona
{
namespace lua
{

std::unique_ptr<LuaEnv> lua;

LuaEnv::LuaEnv()
{
    lua_ = std::make_shared<sol::state>();
    lua_->open_libraries(
        sol::lib::base,
        sol::lib::package,
        sol::lib::table,
        sol::lib::debug,
        sol::lib::string,
        sol::lib::math,
        sol::lib::io,
        sol::lib::os,
        sol::lib::coroutine);

    // Add executable directory to package.path
    fs::path exe_path = filesystem::dirs::data() / "script" / "kernel";
    std::string normalized = filepathutil::to_forward_slashes(exe_path);
    lua_->safe_script(
        u8"package.path = \""s + normalized + u8"/?.lua;\"..package.path"s);

    (*lua_)["require_relative"] = (*lua_)["require"];
    (*lua_)["require"] = sol::lua_nil;

    // Make sure the API environment is initialized first so any
    // dependent managers can add new internal C++ methods to it (like
    // the event manager registering Elona.Event)
    api_mgr = std::make_unique<APIManager>(*this);
    event_mgr = std::make_unique<EventManager>(*this);
    mod_mgr = std::make_unique<ModManager>(*this);
    handle_mgr = std::make_unique<HandleManager>(*this);
    data_mgr = std::make_unique<DataManager>(*this);
    export_mgr = std::make_unique<ExportManager>(*this);
    i18n_function_mgr = std::make_unique<I18NFunctionManager>(*this);
    console = std::make_unique<Console>(*this);
    config_mgr = std::make_unique<ConfigManager>(*this);
}



// The destructor cannot be automatically defined because some "T" of
// "std::unique_ptr<T>" are incomplete types and just forward-declared in the
// header file to reduce compilation time.
LuaEnv::~LuaEnv() = default;



void LuaEnv::load_mods()
{
    const auto list = ModList::from_file(filesystem::files::mod_list());
    const auto lock = ModLock{};
    // const auto index = ModIndex::traverse(filesystem::dirs::mod());
    const auto index = ModIndex{
        {{"core", {ModIndex::IndexEntry{semver::Version{0, 2, 6}, {}}}}}};

    ModVersionResolver resolver;
    const auto resolve_result = resolver.resolve(list, lock, index);
    if (resolve_result)
    {
        mod_mgr->load_mods(resolve_result.right());
    }
    else
    {
        throw std::runtime_error{resolve_result.left()};
    }
}



void LuaEnv::clear()
{
    for (auto&& item : inv.all())
    {
        if (item.number() != 0)
        {
            handle_mgr->remove_item_handle(item);
        }
    }

    for (int i = 0; i < ELONA_MAX_CHARACTERS; i++)
    {
        if (cdata[i].state() != Character::State::empty)
        {
            handle_mgr->remove_chara_handle(cdata[i]);
        }
    }

    event_mgr->clear();
    mod_mgr->clear_mod_stores();
    data_mgr->clear();
    handle_mgr->clear_all_handles();
    // ConfigManager::clear() will be called elsewhere.
    lua_->collect_garbage();
}

} // namespace lua
} // namespace elona
