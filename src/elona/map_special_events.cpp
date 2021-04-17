#include "area.hpp"
#include "audio.hpp"
#include "character.hpp"
#include "character_status.hpp"
#include "data/types/type_skill.hpp"
#include "dmgheal.hpp"
#include "food.hpp"
#include "game.hpp"
#include "i18n.hpp"
#include "itemgen.hpp"
#include "map.hpp"
#include "map_cell.hpp"
#include "message.hpp"
#include "quest.hpp"
#include "skill.hpp"
#include "variables.hpp"



namespace elona
{

static void _map_events_noyel()
{
    if (game()->released_fire_giant == 1)
    {
        if (cdata[game()->fire_giant].state() == Character::State::alive)
        {
            if (game()->crowd_density < 70)
            {
                if (rnd(4) == 0)
                {
                    flt();
                    objlv = 1;
                    chara_create(-1, 204, -3, 0);
                }
                if (rnd(10) == 0)
                {
                    flt();
                    objlv = 1;
                    chara_create(-1, 185, -3, 0);
                }
            }
        }
    }
}

static void _map_events_quest_party()
{
    if (quest_data.immediate().progress != 3)
    {
        if (game()->crowd_density <
            game()->left_minutes_of_executing_quest / 60)
        {
            int chara_id = 0;
            if (rnd(4) == 0)
            {
                chara_id = 204;
            }
            if (rnd(10) == 0)
            {
                chara_id = 185;
            }
            if (chara_id != 0)
            {
                flt();
                objlv = 1;
                if (const auto chara = chara_create(-1, chara_id, -3, 0))
                {
                    chara->relationship = Relationship::neutral;
                    chara->original_relationship = Relationship::neutral;
                    chara->hate = 100;
                    chara->enemy_id = quest_data.immediate().extra_info_2;
                }
            }
        }
    }
}

static void _map_events_tower_of_fire()
{
    if (rnd(5) == 0)
    {
        r = cdata.player().skills().level("core.element_fire") / 50;
        if (r < 6)
        {
            dmg = (6 - r) * (6 - r) * 2;
            txt(i18n::s.get("core.action.exit_map.it_is_hot"),
                Message::color{ColorIndex::red});
            damage_hp(cdata.player(), dmg, -9);
        }
    }
}

static void _map_events_port_kapul()
{
    if (game()->current_dungeon_level == 25)
    {
        story_quest_add_ext("core.kamikaze_attack", "core.elapsed_time", 1);
        x = 1;
        y = rnd(map_data.height);
        if (rnd(4) == 0)
        {
            x = map_data.width - 2;
            y = rnd(map_data.height);
        }
        if (rnd(5) == 0)
        {
            x = rnd(map_data.width);
            y = 1;
        }
        if (rnd(6) == 0)
        {
            x = rnd(map_data.width);
            y = map_data.height - 2;
        }
        p = 237;
        if (story_quest_get_ext<lua_int>(
                "core.kamikaze_attack", "core.elapsed_time") > 50)
        {
            if (rnd(10) == 0)
            {
                p = 245;
            }
        }
        if (story_quest_get_ext<lua_int>(
                "core.kamikaze_attack", "core.elapsed_time") > 100)
        {
            if (rnd(10) == 0)
            {
                p = 244;
            }
        }
        if (story_quest_get_ext<lua_int>(
                "core.kamikaze_attack", "core.elapsed_time") > 150)
        {
            if (rnd(10) == 0)
            {
                p = 244;
            }
        }
        if (story_quest_get_ext<lua_int>(
                "core.kamikaze_attack", "core.elapsed_time") == 250)
        {
            quest_update_journal_msg();
            story_quest_set_progress("core.kamikaze_attack", 3);
            txt(i18n::s.get("core.misc.quest.kamikaze_attack.message"),
                Message::color{ColorIndex::cyan});
            txt(i18n::s.get("core.misc.quest.kamikaze_attack.stairs_appear"));
            cell_featset(18, 9, tile_downstairs, 11, 1);
        }
        flt();
        if (const auto chara = chara_create(-1, p, x, y))
        {
            chara->hate = 1000;
            chara->enemy_id = 0;
        }
        return;
    }
}

static void _map_events_jail()
{
    if (rnd(50) == 0)
    {
        if (cdata.player().karma < -30)
        {
            txt(i18n::s.get("core.misc.map.jail.repent_of_sin"));
            modify_karma(cdata.player(), 1);
            p = rnd(8) + 10;
            if (cdata.player().skills().base_level(
                    *the_skill_db.get_id_from_integer(p)) >= 10)
            {
                chara_gain_fixed_skill_exp(cdata.player(), p, -300);
            }
        }
    }
    if (cdata.player().karma >= -30)
    {
        if (rnd(1000) == 0)
        {
            cell_featread(29, 6);
            if (feat(1) == 21)
            {
                txt(i18n::s.get("core.misc.map.jail.guard_approaches"));
                txt(i18n::s.get("core.misc.map.jail.leave_here"));
                txt(i18n::s.get("core.misc.map.jail.unlocks_your_cell"));
                cell_featset(29, 6, tile_dooropen, 20, 0, -1);
                snd("core.door1");
            }
        }
    }
}

static void _map_events_shelter()
{
    if (game()->weather == "core.snow" || game()->weather == "core.hard_rain" ||
        game()->weather == "core.etherwind")
    {
        if (cdata.player().nutrition < 5000)
        {
            if (cdata.player().has_anorexia() == 0)
            {
                snd("core.eat1");
                txt(i18n::s.get("core.misc.map.shelter.eat_stored_food"));
                cdata.player().nutrition += 5000;
                show_eating_message(cdata.player());
            }
        }
        if (cdata.player().sleepiness >= 15)
        {
            cdata.player().sleepiness = 13;
        }
        map_data.turn_cost = 1000000;
    }
    else if (map_data.turn_cost == 1000000)
    {
        map_data.turn_cost = 10000;
        for (auto&& cnt : cdata.all())
        {
            cnt.turn_energy = 0;
        }
        txt(i18n::s.get("core.misc.map.shelter.no_longer_need_to_stay"));
    }
}



static void _map_events_museum()
{
    if (game()->crowd_density > 0)
    {
        if (rnd(25) == 0)
        {
            txt(i18n::s.get("core.misc.map.museum.chats"),
                Message::color{ColorIndex::cyan});
        }
        if (rnd(25) == 0)
        {
            txt(i18n::s.get("core.misc.map.museum.chats2"),
                Message::color{ColorIndex::cyan});
        }
        if (rnd(15) == 0)
        {
            std::string locale_key;
            if (game()->ranks.at(3) > 8000)
            {
                locale_key = "core.map.museum.rank_lowest";
            }
            else if (game()->ranks.at(3) > 5000)
            {
                locale_key = "core.map.museum.rank_low";
            }
            else if (game()->ranks.at(3) > 2500)
            {
                locale_key = "core.map.museum.rank_middle";
            }
            else if (game()->ranks.at(3) > 500)
            {
                locale_key = "core.map.museum.rank_high";
            }
            else
            {
                locale_key = "core.map.museum.rank_highest";
            }
            if (const auto msg = i18n::s.get_optional(locale_key))
            {
                txt(*msg, Message::color{ColorIndex::cyan});
            }
        }
    }
}



static void _map_events_shop()
{
    if (game()->crowd_density > 0)
    {
        if (rnd(25) == 0)
        {
            txt(i18n::s.get("core.misc.map.shop.chats"),
                Message::color{ColorIndex::cyan});
        }
        return;
    }
}

void map_proc_special_events()
{
    if (game()->current_map == mdata_t::MapId::noyel)
    {
        _map_events_noyel();
    }
    if (game()->executing_immediate_quest_type == 1008)
    {
        _map_events_quest_party();
    }
    if (game()->current_map == mdata_t::MapId::tower_of_fire)
    {
        _map_events_tower_of_fire();
        return;
    }
    if (game()->current_map == mdata_t::MapId::port_kapul)
    {
        _map_events_port_kapul();
    }
    if (game()->current_map == mdata_t::MapId::jail)
    {
        _map_events_jail();
    }
    if (game()->current_map == mdata_t::MapId::shelter_)
    {
        _map_events_shelter();
    }
    if (area_data[game()->current_map].id == mdata_t::MapId::museum)
    {
        _map_events_museum();
    }
    if (area_data[game()->current_map].id == mdata_t::MapId::shop)
    {
        _map_events_shop();
    }
}

} // namespace elona
