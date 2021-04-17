#include "init.hpp"

#include "../util/fileutil.hpp"
#include "adventurer.hpp"
#include "area.hpp"
#include "audio.hpp"
#include "autopick.hpp"
#include "blending.hpp"
#include "building.hpp"
#include "character.hpp"
#include "character_status.hpp"
#include "config.hpp"
#include "crafting.hpp"
#include "ctrl_file.hpp"
#include "data/init.hpp"
#include "data/types/type_crafting_material.hpp"
#include "deferred_event.hpp"
#include "draw.hpp"
#include "enchantment.hpp"
#include "fov.hpp"
#include "game.hpp"
#include "god.hpp"
#include "i18n.hpp"
#include "initialize_map.hpp"
#include "item.hpp"
#include "keybind/keybind.hpp"
#include "loading_screen.hpp"
#include "lua_env/api_manager.hpp"
#include "lua_env/console.hpp"
#include "lua_env/data_manager.hpp"
#include "lua_env/event_manager.hpp"
#include "lua_env/lua_env.hpp"
#include "lua_env/lua_event/base_event.hpp"
#include "lua_env/mod_manager.hpp"
#include "map.hpp"
#include "mapgen.hpp"
#include "mef.hpp"
#include "menu.hpp"
#include "quest.hpp"
#include "randomgen.hpp"
#include "save.hpp"
#include "scene.hpp"
#include "set_item_info.hpp"
#include "text.hpp"
#include "trait.hpp"
#include "ui.hpp"
#include "variables.hpp"
#include "world.hpp"



namespace elona
{

namespace
{

void initialize_directories()
{
    const fs::path paths[] = {
        filesystem::dirs::save(),
        filesystem::dirs::screenshot(),
        filesystem::dirs::tmp(),
    };

    for (const auto& path : paths)
    {
        if (!fs::exists(path))
        {
            fs::create_directory(path);
        }
    }
}



void initialize_screen(const PreinitConfigOptions& opts)
{
    title(
        "Elona foobar version "s + latest_version.short_string(),
        opts.display_mode(),
        opts.fullscreen());
}



void init_game_clock()
{
    game()->epoch_year = 0;

    // 517/08/12 16:10
    game()->universal_clock.advance(517_years);
    game()->universal_clock.advance(8_months);
    game()->universal_clock.advance(12_days);
    game()->universal_clock.advance(16_hours);
    game()->universal_clock.advance(10_minutes);
}



void initialize_world()
{
    init_game_clock();

    game()->pc_x_in_world_map = 22;
    game()->pc_y_in_world_map = 21;

    game()->destination_outer_map = 4;
    game()->current_map = static_cast<int>(mdata_t::MapId::your_home);
    game()->current_dungeon_level = 1;
    game()->entrance_type = 4;

    game()->version = 1220;

    game()->home_scale = 0;

    initialize_adata();

    game()->weather = "core.rain";
    game()->weather_change_count = 6;

    for (int cnt = 0; cnt < 9; ++cnt)
    {
        game()->ranks.at(cnt) = 10000;
    }
}



void initialize_testbed()
{
    game()->current_map = 499;
    game()->current_dungeon_level = 2;
}

} // namespace



// see building.cpp
extern elona_vector1<int> fsetincome;
extern elona_vector1<int> fsetplantartifact;
extern elona_vector1<int> fsetplantunknown;

// see command.cpp
extern elona_vector1<int> fsetbarrel;
extern elona_vector1<int> isetgiftminor;
extern elona_vector1<int> isetgiftmajor;
extern elona_vector1<int> isetgiftgrand;

// see randomgen.cpp
extern elona_vector1<int> fsetrare;



void initialize_lua()
{
    // Load mods.
    lua::lua->load_mods();

    auto& data_manager = lua::lua->get_data_manager();
    data_manager.init_from_mods();

    // Set "data" table on all loaded mod environments.
    data::initialize(data_manager.get());

    // Remove unknown event types from the event tables.
    lua::lua->get_event_manager().remove_unknown_events();

    // Initialize console environment.
    lua::lua->get_console().init_environment();
}



void initialize_i18n()
{
    i18n::s.init();
}



void initialize_elona()
{
    initialize_ui_constants();
    gsel(0);
    boxf();
    redraw();

    mesbox(keylog);

    buffer(4, windoww, windowh);
    buffer(8, windoww, windowh);
    gsel(0);
    buffer(1, 1584, 1200);
    picload(filesystem::dirs::graphic() / "item.bmp", 0, 0, false);
    if (inf_tiles != 48)
    {
        gcopy(1, 0, 0, 1584, 1200, 0, 0, 33 * inf_tiles, 25 * inf_tiles);
    }
    buffer(2, 33 * inf_tiles, 25 * inf_tiles);
    buffer(6, 33 * inf_tiles, 25 * inf_tiles);
    buffer(7, 24, 24);
    buffer(9, 24, 24);

    DIM3(cmapdata, 5, 400);
    DIM2(fixeditemenc, 20);
    DIM2(dir, 5);
    DIM3(dblist, 2, 800);
    SDIM2(inputlog, 100);
    SDIM2(key, 20);
    SDIM2(keylog, 20);
    DIM2(rtval, 10);
    DIM3(list, 3, 500);
    SDIM4(listn, 40, 2, 500);
    DIM2(invctrl, 2);
    SDIM3(description, 1000, 3);
    DIM3(mef, 9, MEF_MAX);
    DIM3(adata, 40, 500);
    area_data.clear();
    DIM3(qdata, 20, 500);
    SDIM3(qname, 40, 500);
    DIM2(gdata, 1000);
    DIM2(genetemp, 1000);
    DIM2(mdatatmp, 100);
    map_data.clear();
    SDIM3(mdatan, 20, 2);
    SDIM2(s1, 1000);
    DIM2(mat, 400);
    DIM2(invmark, 35);
    DIM2(feat, 5);

    DIM3(pcc, 30, 20);

    SDIM1(fltnrace);
    DIM3(card, 4, 1000);
    DIM2(deck, 1000);

    DIM3(dirxy, 2, 4);

    dirxy(0, 3) = 0;
    dirxy(1, 3) = -1;
    dirxy(0, 0) = 0;
    dirxy(1, 0) = 1;
    dirxy(0, 1) = -1;
    dirxy(1, 1) = 0;
    dirxy(0, 2) = 1;
    dirxy(1, 2) = 0;

    DIM3(c_col, 3, 30);

    c_col(0, 0) = 0;
    c_col(1, 0) = 0;
    c_col(2, 0) = 0;
    c_col(0, 1) = 0;
    c_col(1, 1) = 0;
    c_col(2, 1) = 0;
    c_col(0, 2) = 80;
    c_col(1, 2) = 0;
    c_col(2, 2) = 80;
    c_col(0, 3) = 0;
    c_col(1, 3) = 100;
    c_col(2, 3) = 100;
    c_col(0, 4) = 80;
    c_col(1, 4) = 80;
    c_col(2, 4) = 0;
    c_col(0, 6) = 0;
    c_col(1, 6) = 40;
    c_col(2, 6) = 80;
    c_col(0, 5) = 0;
    c_col(1, 5) = 0;
    c_col(2, 5) = 80;
    c_col(0, 7) = 100;
    c_col(1, 7) = 101;
    c_col(2, 7) = 102;
    c_col(0, 8) = 70;
    c_col(1, 8) = 100;
    c_col(2, 8) = 40;
    c_col(0, 9) = 100;
    c_col(1, 9) = 50;
    c_col(2, 9) = 50;
    c_col(0, 10) = 0;
    c_col(1, 10) = 60;
    c_col(2, 10) = 70;
    c_col(0, 11) = 20;
    c_col(1, 11) = 40;
    c_col(2, 11) = 100;
    c_col(0, 12) = 0;
    c_col(1, 12) = 0;
    c_col(2, 12) = 0;
    c_col(0, 13) = 30;
    c_col(1, 13) = 40;
    c_col(2, 13) = 70;
    c_col(0, 14) = 150;
    c_col(1, 14) = 20;
    c_col(2, 14) = 150;
    c_col(0, 15) = 50;
    c_col(1, 15) = 50;
    c_col(2, 15) = 50;
    c_col(0, 16) = 0;
    c_col(1, 16) = 30;
    c_col(2, 16) = 30;
    c_col(0, 17) = 30;
    c_col(1, 17) = 30;
    c_col(2, 17) = 0;
    c_col(0, 18) = 30;
    c_col(1, 18) = 60;
    c_col(2, 18) = 0;
    c_col(0, 19) = 40;
    c_col(1, 19) = 0;
    c_col(2, 19) = 40;
    c_col(0, 20) = 45;
    c_col(1, 20) = 5;
    c_col(2, 20) = 95;

    SDIM3(key_list, 2, 20);
    SDIM2(playerheader, 100);
    DIM3(slight, inf_screenw + 4, inf_screenh + 4);

    keybind_regenerate_key_select();

    gsel(0);
    gmode(2);
    text_set();
    ctrl_file_temp_dir_delete();
    invctrl(0) = 0;
    invctrl(1) = 0;
    SDIM1(buffboard);
    notesel(buffboard);
    {
        buffboard(0).clear();
        std::ifstream in{
            lua::resolve_path_for_mod("<core>/locale/<LANGUAGE>/lazy/board.txt")
                .native(),
            std::ios::binary};
        std::string tmp;
        while (std::getline(in, tmp))
        {
            buffboard(0) += tmp + '\n';
        }
    }
    SDIM1(ioriginalnameref);
    SDIM1(ioriginalnameref2);
    SDIM1(iknownnameref);
    SDIM1(ialphanameref);
    DIM2(irandomname, 800);
    DIM2(trate, 8);
    SDIM1(filtern);
    initialize_all_chips();
    initialize_building_data();
    initialize_adata();
    map_init_cell_object_data();
    load_random_title_table();
    load_random_name_table();
    game()->random_seed = 1 + rnd(2000000000);
    game()->random_seed_offset = 0;
    set_item_info();
    clear_trait_data();
    initialize_rankn();
    initialize_post_data();
    initialize_ego_data();
    enchantment_init();
    initialize_set_of_random_generation();
    initialize_picfood();
    blending_init_recipe_data();
    initialize_nefia_names();
    initialize_home_adata();
    initialize_damage_popups();
    // if (g_config.sound())
    {
        bool ok = audio_init_sound();
        if (!ok)
        {
            throw std::runtime_error{"failed to init sound"};
        }
    }
    if (g_config.music())
    {
        bool ok = audio_init_music();
        if (!ok)
        {
            throw std::runtime_error{"failed to init music"};
        }
    }
    if (g_config.joypad())
    {
        DIINIT();
        if (DIGETJOYNUM() == 0)
        {
            throw std::runtime_error{"failed to init joypad"};
        }
    }
    mainskill(0) = 173;
    mainskill(1) = 106;
    mainskill(2) = 108;

    DIM3(cycle, 15, 5);
    cycle(0, 0) = 1;
    cycle(1, 0) = 2;
    cycle(2, 0) = 5;
    cycle(3, 0) = 7;
    cycle(4, 0) = 8;
    cycle(5, 0) = 9;
    cycle(6, 0) = 14;
    cycle(7, 0) = 15;
    cycle(8, 0) = 17;
    cycle(9, 0) = 26;
    cycle(10, 0) = -1;
    cycle(0, 1) = 1;
    cycle(1, 1) = 5;
    cycle(2, 1) = 7;
    cycle(3, 1) = 8;
    cycle(4, 1) = 14;
    cycle(5, 1) = -1;
    cycle(0, 2) = 10;
    cycle(1, 2) = 25;
    cycle(2, 2) = -1;
    cycle(0, 3) = 22;
    cycle(1, 3) = 24;
    cycle(2, 3) = -1;
    cycle(0, 4) = 2;
    cycle(1, 4) = -1;

    cyclemax(0) = 9;
    cyclemax(1) = 4;
    cyclemax(2) = 1;
    cyclemax(3) = 1;
    cyclemax(4) = 0;

    lastctrl = 1;

    DIM2(invicon, 40);
    SDIM3(invkey, 4, 40);
    invkey(0) = "";
    invkey(1) = keybind_get_bound_key_name("inventory");
    invkey(2) = keybind_get_bound_key_name("drop");
    invkey(3) = "";
    invkey(4) = "";
    invkey(5) = keybind_get_bound_key_name("eat");
    invkey(6) = "";
    invkey(7) = keybind_get_bound_key_name("read");
    invkey(8) = keybind_get_bound_key_name("drink");
    invkey(9) = keybind_get_bound_key_name("zap");
    invkey(10) = "";
    invkey(11) = "";
    invkey(12) = "";
    invkey(13) = "";
    invkey(14) = keybind_get_bound_key_name("use");
    invkey(15) = keybind_get_bound_key_name("open");
    invkey(16) = "";
    invkey(17) = keybind_get_bound_key_name("dip");
    invkey(26) = keybind_get_bound_key_name("throw");
    invicon(0) = -1;
    invicon(1) = 7;
    invicon(2) = 8;
    invicon(3) = -1;
    invicon(4) = -1;
    invicon(5) = 2;
    invicon(6) = -1;
    invicon(7) = 3;
    invicon(8) = 0;
    invicon(9) = 1;
    invicon(10) = 17;
    invicon(11) = -1;
    invicon(12) = -1;
    invicon(13) = -1;
    invicon(14) = 5;
    invicon(15) = 4;
    invicon(16) = -1;
    invicon(17) = 6;
    invicon(18) = -1;
    invicon(19) = -1;
    invicon(20) = -1;
    invicon(21) = -1;
    invicon(22) = 17;
    invicon(23) = -1;
    invicon(24) = 17;
    invicon(25) = 17;
    invicon(26) = 18;
    invicon(27) = -1;
    invicon(28) = -1;
    invicon(29) = -1;

    if (g_config.autodisable_numlock())
    {
        snail::Input::instance().disable_numlock();
    }

    // Calculate console text size (requires font to be loaded)
    lua::lua->get_console().init_constants();
}



void initialize_debug_globals()
{
    for (int cnt = 0; cnt < 9; ++cnt)
    {
        game()->ranks.at(cnt) = 5000;
    }
    game()->version = 1220;
    game()->next_inventory_serial_id = 1000;
    game()->next_shelter_serial_number = 100;
    game()->pc_x_in_world_map = 22;
    game()->pc_y_in_world_map = 21;
    game()->random_seed = 1 + rnd(2000000000);
    game()->random_seed_offset = 0;
    init_game_clock();
    game()->current_map = static_cast<int>(mdata_t::MapId::vernis);
    game()->current_dungeon_level = 1;
    game()->entrance_type = 7;
    mapstartx = 10;
    mapstarty = 23;
    initlv = 50;
    flt(100);
    chara_create(0, 84, -3, 0);
    initialize_pc_character();
    game()->played_scene = 50;
    game()->has_not_been_to_vernis = 1;
    area_data[static_cast<int>(mdata_t::MapId::your_home)].outer_map =
        static_cast<int>(mdata_t::MapId::north_tyris);
    game()->destination_outer_map = area_data[game()->current_map].outer_map;
    cdata.player().acquirable_feats = 2;
    story_quest_set_ext("core.little_sister", "core.save_count", lua_int{1000});
    game()->inheritance_rights = 1000;
    game()->home_scale = 0;
    game()->number_of_waiting_guests = 2;
    game()->charge_power = 1000;
    cdata.player().religion = "core.lulwy";
    cdata.player().piety_point = 1000;
    cdata.player().prayer_point = 1000;
    story_quest_set_progress("core.pael_and_her_mom", 1000);
    earn_gold(cdata.player(), 1000000);
    cdata.player().platinum = 30;
    cdata.player().fame = 65000;
    story_quest_set_progress("core.elona", 100);
    chara_refresh(cdata.player());

    cdata.player().can_cast_rapid_magic() = true;
    g_mode = 0;
    refresh_burden_state();
    for (const auto& [id, _] : the_crafting_material_db)
    {
        game()->crafting_materials.set_amount(id, 200);
    }
    create_all_adventurers();
    create_pcpic(cdata.player());
    cdata.player().alias = random_title(RandomTitleType::character);
    cdata.player().name = random_name();
}



void initialize_game(InitializeGameMode start_mode)
{
    bool script_loaded = false;
    bool will_load_script = false;
    Autopick::instance().load(playerid);

    mtilefilecur = -1;
    firstturn = 1;
    Message::instance().buffered_message_begin("   Welcome traveler! ");

    switch (start_mode)
    {
    case InitializeGameMode::continue_: g_mode = 3; break;
    case InitializeGameMode::new_: g_mode = 5; break;
    case InitializeGameMode::quickstart: g_mode = 6; break;
    default: assert(0); break;
    }

    if (start_mode == InitializeGameMode::new_)
    {
        initialize_world();
        create_all_adventurers();
        deferred_event_add("core.generate_game_world");
        deferred_event_add("core.lomias_talks");
        sceneid = 0;
        g_mode = 2;
        do_play_scene();
    }
    else if (start_mode == InitializeGameMode::quickstart)
    {
        playerid = "sav_testbed"s;
        initialize_debug_globals();
        initialize_testbed();
        will_load_script = true;
        g_mode = 2;
    }

    if (start_mode == InitializeGameMode::continue_)
    {
        save_load_game();

        if (config_get<bool>("core.foobar.run_script_in_save"))
        {
            will_load_script = true;
        }
    }
    else
    {
        game()->next_inventory_serial_id = 1000;
        game()->next_shelter_serial_number = 100;
        blending_clear_recipe_memory();
    }

    if (will_load_script && g_config.startup_script() != ""s)
    {
        lua::lua->get_mod_manager().run_startup_script(
            fs::u8path(g_config.startup_script()));
        script_loaded = true;
    }

    fov_init_fovlist();

    assert(g_mode == 2 || g_mode == 3);
    initialize_map();
    assert(g_mode == 0);

    if (script_loaded)
    {
        lua::lua->get_event_manager().trigger(
            lua::BaseEvent("core.script_loaded"));
    }
}



void init()
{
    const auto preinit_config_options = PreinitConfigOptions::from_file(
        filesystem::files::profile_local_config());

    initialize_screen(preinit_config_options);

    lua::lua = std::make_unique<lua::LuaEnv>();

    initialize_directories();

    initialize_lua();

    config_load_all_schema();
    config_load_options();

    // Load translations from scanned mods.
    initialize_i18n();

    lua::lua->get_api_manager().lock();

    if (g_config.font_filename().empty())
    {
        // If no font is specified in `config.json`, use a pre-defined font
        // depending on each language.
        g_config.set_font_filename(i18n::s.get("core.meta.default_font"));
        if (jp)
        {
            // TODO: work around
            elona::vfix = -3;
        }
    }

    initialize_keybindings();

    initialize_elona();

    config_save();

    // It is necessary to calculate PC's birth year correctly.
    init_game_clock();

    quickpage = 1;

    // TODO: Show each time mods are reloaded.
    show_loading_screen();
}



void initialize_post_data()
{
    DIM3(podata, 300, 20);
}



void initialize_picfood()
{
    DIM3(picfood, 10, 9);
    picfood(0, 8) = 230;
    picfood(1, 8) = 230;
    picfood(2, 8) = 230;
    picfood(3, 8) = 190;
    picfood(4, 8) = 229;
    picfood(5, 8) = 190;
    picfood(6, 8) = 342;
    picfood(7, 8) = 168;
    picfood(8, 8) = 347;
    picfood(9, 8) = 194;
    picfood(0, 1) = 230;
    picfood(1, 1) = 230;
    picfood(2, 1) = 230;
    picfood(3, 1) = 195;
    picfood(4, 1) = 227;
    picfood(5, 1) = 167;
    picfood(6, 1) = 167;
    picfood(7, 1) = 194;
    picfood(8, 1) = 229;
    picfood(9, 1) = 227;
    picfood(0, 2) = 230;
    picfood(1, 2) = 230;
    picfood(2, 2) = 230;
    picfood(3, 2) = 229;
    picfood(4, 2) = 342;
    picfood(5, 2) = 194;
    picfood(6, 2) = 229;
    picfood(7, 2) = 342;
    picfood(8, 2) = 229;
    picfood(9, 2) = 194;
    picfood(0, 3) = 230;
    picfood(1, 3) = 230;
    picfood(2, 3) = 230;
    picfood(3, 3) = 229;
    picfood(4, 3) = 346;
    picfood(5, 3) = 346;
    picfood(6, 3) = 345;
    picfood(7, 3) = 345;
    picfood(8, 3) = 347;
    picfood(9, 3) = 346;
    picfood(0, 4) = 230;
    picfood(1, 4) = 230;
    picfood(2, 4) = 230;
    picfood(3, 4) = 108;
    picfood(4, 4) = 346;
    picfood(5, 4) = 110;
    picfood(6, 4) = 346;
    picfood(7, 4) = 347;
    picfood(8, 4) = 347;
    picfood(9, 4) = 347;
    picfood(0, 5) = 230;
    picfood(1, 5) = 230;
    picfood(2, 5) = 229;
    picfood(3, 5) = 343;
    picfood(4, 5) = 344;
    picfood(5, 5) = 344;
    picfood(6, 5) = 343;
    picfood(7, 5) = 343;
    picfood(8, 5) = 344;
    picfood(9, 5) = 343;
    picfood(0, 6) = 230;
    picfood(1, 6) = 230;
    picfood(2, 6) = 230;
    picfood(3, 6) = 228;
    picfood(4, 6) = 342;
    picfood(5, 6) = 342;
    picfood(6, 6) = 228;
    picfood(7, 6) = 341;
    picfood(8, 6) = 228;
    picfood(9, 6) = 228;
    picfood(0, 7) = 230;
    picfood(1, 7) = 230;
    picfood(2, 7) = 110;
    picfood(3, 7) = 108;
    picfood(4, 7) = 110;
    picfood(5, 7) = 112;
    picfood(6, 7) = 114;
    picfood(7, 7) = 113;
    picfood(8, 7) = 115;
    picfood(9, 7) = 111;
}



void initialize_set_of_random_generation()
{
    fsetincome(0) = 52000;
    fsetincome(1) = 52000;
    fsetincome(2) = 52000;
    fsetincome(3) = 53000;
    fsetincome(4) = 53000;
    fsetincome(5) = 56000;
    fsetincome(6) = 54000;
    fsetincome(7) = 77000;
    fsetincome(8) = 57000;
    fsetincome(9) = 57000;
    fsetchest(0) = 10000;
    fsetchest(1) = 24000;
    fsetchest(2) = 12000;
    fsetchest(3) = 16000;
    fsetchest(4) = 20000;
    fsetchest(5) = 19000;
    fsetchest(6) = 18000;
    fsetchest(7) = 22000;
    fsetchest(8) = 14000;
    fsetchest(9) = 32000;
    fsetchest(10) = 34000;
    fsetchest(11) = 54000;
    fsetchest(12) = 59000;
    fsetwear(0) = 10000;
    fsetwear(1) = 10000;
    fsetwear(2) = 24000;
    fsetwear(3) = 24000;
    fsetwear(4) = 25000;
    fsetwear(5) = 12000;
    fsetwear(6) = 16000;
    fsetwear(7) = 20000;
    fsetwear(8) = 19000;
    fsetwear(9) = 18000;
    fsetwear(10) = 22000;
    fsetwear(11) = 14000;
    fsetwear(12) = 32000;
    fsetwear(13) = 34000;
    fsetitem(0) = 52000;
    fsetitem(1) = 52000;
    fsetitem(2) = 53000;
    fsetitem(3) = 53000;
    fsetitem(4) = 56000;
    fsetitem(5) = 68000;
    fsetitem(6) = 54000;
    fsetitem(7) = 64000;
    fsetitem(8) = 59000;
    fsetitem(9) = 55000;
    fsetrewardsupply(0) = 52000;
    fsetrewardsupply(1) = 53000;
    fsetrewardsupply(2) = 56000;
    fsetrewardsupply(3) = 54000;
    fsetrewardsupply(4) = 57000;
    fsetrare(0) = 60000;
    fsetrare(1) = 72000;
    fsetrare(2) = 77000;
    fsetrare(3) = 55000;
    fsetrare(4) = 57000;
    fsetdeliver(0) = 60000;
    fsetdeliver(1) = 77000;
    fsetdeliver(2) = 54000;
    fsetdeliver(3) = 64000;
    fsetsupply(0) = 60000;
    fsetsupply(1) = 77000;
    fsetsupply(2) = 56000;
    fsetsupply(3) = 54000;
    fsetsupply(4) = 64000;
    fsetmagic(0) = 53000;
    fsetmagic(1) = 56000;
    fsetmagic(2) = 54000;
    fsetarmor(0) = 12000;
    fsetarmor(1) = 16000;
    fsetarmor(2) = 20000;
    fsetarmor(3) = 19000;
    fsetarmor(4) = 18000;
    fsetarmor(5) = 22000;
    fsetarmor(6) = 14000;
    fsetweapon(0) = 10000;
    fsetweapon(1) = 24000;
    fsetweapon(2) = 25000;
    fsetplantartifact(0) = 32000;
    fsetplantartifact(1) = 34000;
    fsetplantunknown(0) = 57000;
    fsetplantunknown(1) = 57000;
    fsetplantunknown(2) = 54000;
    fsetplantunknown(3) = 64000;
    fsetplantunknown(4) = 77000;
    fsetbarrel(0) = 25000;
    fsetbarrel(1) = 57000;
    fsetbarrel(2) = 53000;
    fsetbarrel(3) = 52000;
    fsetbarrel(4) = 77000;
    fsetbarrel(5) = 64000;
    fsetcollect(0) = 64000;
    fsetcollect(1) = 60000;
    fsetcollect(2) = 57000;
    fsetcollect(3) = 77000;
    isetfruit(0) = 180;
    isetfruit(1) = 181;
    isetfruit(2) = 196;
    isetfruit(3) = 197;
    isetfruit(4) = 192;
    isetfruit(5) = 183;
    isetthrowpotionminor(0) = 27;
    isetthrowpotionminor(1) = 28;
    isetthrowpotionminor(2) = 376;
    isetthrowpotionminor(3) = 30;
    isetthrowpotionminor(4) = 262;
    isetthrowpotionminor(5) = 253;
    isetthrowpotionminor(6) = 379;
    isetthrowpotionminor(7) = 392;
    isetthrowpotionmajor(0) = 28;
    isetthrowpotionmajor(1) = 376;
    isetthrowpotionmajor(2) = 205;
    isetthrowpotionmajor(3) = 368;
    isetthrowpotionmajor(4) = 433;
    isetthrowpotionmajor(5) = 382;
    isetthrowpotionmajor(6) = 577;
    isetthrowpotionmajor(7) = 577;
    isetthrowpotiongreater(0) = 28;
    isetthrowpotiongreater(1) = 205;
    isetthrowpotiongreater(2) = 368;
    isetthrowpotiongreater(3) = 432;
    isetthrowpotiongreater(4) = 429;
    isetthrowpotiongreater(5) = 29;
    isetthrowpotiongreater(6) = 577;
    isetgiftminor(0) = 753;
    isetgiftminor(1) = 754;
    isetgiftminor(2) = 754;
    isetgiftminor(3) = 756;
    isetgiftminor(4) = 756;
    isetgiftminor(5) = 540;
    isetgiftminor(6) = 541;
    isetgiftminor(7) = 537;
    isetgiftminor(8) = 538;
    isetgiftminor(9) = 526;
    isetgiftminor(10) = 415;
    isetgiftminor(11) = 511;
    isetgiftminor(12) = 343;
    isetgiftminor(13) = 340;
    isetgiftminor(14) = 338;
    isetgiftminor(15) = 328;
    isetgiftminor(16) = 220;
    isetgiftminor(17) = 575;
    isetgiftminor(18) = 622;
    isetgiftminor(19) = 167;
    isetgiftmajor(0) = 755;
    isetgiftmajor(1) = 755;
    isetgiftmajor(2) = 633;
    isetgiftmajor(3) = 626;
    isetgiftmajor(4) = 502;
    isetgiftmajor(5) = 632;
    isetgiftmajor(6) = 667;
    isetgiftmajor(7) = 555;
    isetgiftmajor(8) = 748;
    isetgiftmajor(9) = 640;
    isetgiftgrand(0) = 757;
    isetgiftgrand(1) = 558;
    isetgiftgrand(2) = 664;
    isetgiftgrand(3) = 758;
    isetgiftgrand(4) = 759;
    isetgiftgrand(5) = 761;
    isetcrop(0) = 180;
    isetcrop(1) = 181;
    isetcrop(2) = 197;
    isetcrop(3) = 192;
    isetcrop(4) = 183;
    isetcrop(5) = 188;
    isetcrop(6) = 200;
    asettown(0) = 5;
    asettown(1) = 11;
    asettown(2) = 14;
    asettown(3) = 15;
    asettown(4) = 12;
    moneybox(0) = 500;
    moneybox(1) = 2000;
    moneybox(2) = 10000;
    moneybox(3) = 50000;
    moneybox(4) = 500000;
    moneybox(5) = 5000000;
    moneybox(6) = 100000000;
    giftvalue(0) = 10;
    giftvalue(1) = 20;
    giftvalue(2) = 30;
    giftvalue(3) = 50;
    giftvalue(4) = 75;
    giftvalue(5) = 100;

    bool in_booktitle_definition = false;
    int isetbook_index = 0;
    for (const auto& line : fileutil::read_by_line(lua::resolve_path_for_mod(
             "<core>/locale/<LANGUAGE>/lazy/book.txt")))
    {
        if (line == "%DEFINE")
        {
            in_booktitle_definition = true;
        }
        else if (line == "%END")
        {
            break;
        }
        else if (in_booktitle_definition)
        {
            // E.g., 7,Book Title,T
            const auto columns = strutil::split(line, ',');
            const auto id = elona::stoi(columns.at(0));
            const auto& title = columns.at(1);
            const auto is_generated = columns.at(2) == "T";

            booktitle(id) = title;
            if (is_generated)
            {
                isetbook(isetbook_index) = id;
                ++isetbook_index;
            }
        }
    }
}



void finish_elona()
{
    if (g_config.autodisable_numlock())
    {
        snail::Input::instance().restore_numlock();
    }
}

} // namespace elona
