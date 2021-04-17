#pragma once

#include "../../thirdparty/sol2/sol.hpp"



namespace elona
{
namespace lua
{

class APIManager;
class DataManager;
class EventManager;
class HandleManager;
class I18NManager;
class ModManager;
class Console;
class ConfigManager;



/***
 * Main singleton encapsulating various Lua subsystems. Delegates
 * responsibility for specific Lua-related operations to invidual
 * subsystems.
 */
class LuaEnv
{
public:
    explicit LuaEnv();
    ~LuaEnv();

    /***
     * Returns a shared pointer to the Lua environment's internal Lua
     * state.
     */
    std::shared_ptr<sol::state> get_state()
    {
        return lua_;
    }

    //********** Individual API manager retrieval methods **********//

    APIManager& get_api_manager()
    {
        return *api_mgr;
    }

    EventManager& get_event_manager()
    {
        return *event_mgr;
    }

    HandleManager& get_handle_manager()
    {
        return *handle_mgr;
    }

    I18NManager& get_i18n_manager()
    {
        return *i18n_mgr;
    }

    ModManager& get_mod_manager()
    {
        return *mod_mgr;
    }

    DataManager& get_data_manager()
    {
        return *data_mgr;
    }

    Console& get_console()
    {
        return *console;
    }

    ConfigManager& get_config_manager()
    {
        return *config_mgr;
    }


    void load_mods();


    /****************** Methods for testing use *******************/

    /***
     * Unloads all characters and items tracked by handles.
     *
     * For testing use only.
     */
    void clear();

private:
    /***
     * The underlying Lua state shared across all mod/API
     * environments.
     */
    std::shared_ptr<sol::state> lua_;

    std::unique_ptr<ModManager> mod_mgr;
    std::unique_ptr<APIManager> api_mgr;
    std::unique_ptr<EventManager> event_mgr;
    std::unique_ptr<HandleManager> handle_mgr;
    std::unique_ptr<DataManager> data_mgr;
    std::unique_ptr<I18NManager> i18n_mgr;
    std::unique_ptr<Console> console;
    std::unique_ptr<ConfigManager> config_mgr;
};



extern std::unique_ptr<LuaEnv> lua;

} // namespace lua
} // namespace elona
