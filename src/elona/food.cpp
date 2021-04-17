#include "food.hpp"

#include "../util/strutil.hpp"
#include "audio.hpp"
#include "buff.hpp"
#include "buff_utils.hpp"
#include "calc.hpp"
#include "chara_db.hpp"
#include "character.hpp"
#include "character_status.hpp"
#include "data/types/type_buff.hpp"
#include "data/types/type_item.hpp"
#include "data/types/type_skill.hpp"
#include "debug.hpp"
#include "deferred_event.hpp"
#include "dmgheal.hpp"
#include "elona.hpp"
#include "fov.hpp"
#include "game.hpp"
#include "i18n.hpp"
#include "input.hpp"
#include "inventory.hpp"
#include "item.hpp"
#include "itemgen.hpp"
#include "lua_env/interface.hpp"
#include "map.hpp"
#include "map_cell.hpp"
#include "message.hpp"
#include "random.hpp"
#include "skill.hpp"
#include "status_ailment.hpp"
#include "text.hpp"
#include "trait.hpp"
#include "variables.hpp"



namespace
{

void _food_gets_rotten(int chara_idx, const ItemRef& food)
{
    if (food->material != "core.raw")
    {
        return;
    }
    if (food_is_rotten(food))
    {
        return; // Has already been rotten.
    }
    if (game_now() < food->__expiration_time)
    {
        return; // The expiration date has not come yet.
    }
    if (food->own_state > OwnState::none)
    {
        return; // On the field.
    }

    // Is it corpse(s) on a dryrock?
    if (chara_idx == -1 && food->id == "core.corpse" &&
        chip_data.for_cell(food->position().x, food->position().y).kind == 1)
    {
        if (game()->weather != "core.sunny")
        {
            return;
        }
        txt(i18n::s.get("core.misc.corpse_is_dried_up", food));
        food->__expiration_time = game_now() + 90_days;
        food->image = 337;
        food->id = "core.jerky";
        food->param1 = 0;
        food->param2 = 5;
        return;
    }

    if (0 <= chara_idx && chara_idx < 16)
    {
        txt(i18n::s.get("core.misc.get_rotten", food));
    }

    food_make_rotten(food);
    food->image = 336;

    if (chara_idx == 0 && cdata.player().religion == "core.kumiromi")
    {
        if (rnd(3) == 0)
        {
            txt(i18n::s.get("core.misc.extract_seed", food));
            const auto seed_num = rnd_capped(food->number()) + 1;
            food->remove();
            flt(calcobjlv(cdata.player().level));
            flttypeminor = 58500;
            itemcreate_player_inv(0, seed_num);
        }
    }
}

} // namespace



namespace elona
{

int cieat = 0;
int nutrition = 0;
int fdmax = 0;
elona_vector2<int> fdlist;



void show_eating_message(const Character& eater)
{
    if (eater.nutrition >= 12000)
    {
        txt(i18n::s.get("core.food.eating_message.bloated"),
            Message::color{ColorIndex::green});
    }
    else if (eater.nutrition >= 10000)
    {
        txt(i18n::s.get("core.food.eating_message.satisfied"),
            Message::color{ColorIndex::green});
    }
    else if (eater.nutrition >= 5000)
    {
        txt(i18n::s.get("core.food.eating_message.normal"),
            Message::color{ColorIndex::green});
    }
    else if (eater.nutrition >= 2000)
    {
        txt(i18n::s.get("core.food.eating_message.hungry"),
            Message::color{ColorIndex::green});
    }
    else if (eater.nutrition >= 1000)
    {
        txt(i18n::s.get("core.food.eating_message.very_hungry"),
            Message::color{ColorIndex::green});
    }
    else
    {
        txt(i18n::s.get("core.food.eating_message.starving"),
            Message::color{ColorIndex::green});
    }
}



void eat_rotten_food(Character& eater)
{
    if (eater.can_digest_rotten_food() == 1)
    {
        txt(i18n::s.get("core.food.not_affected_by_rotten", eater));
        return;
    }
    fdmax = 0;
    if (fdmax < 10)
    {
        fdlist(0, fdmax) = 10;
        fdlist(1, fdmax) = -100;
        ++fdmax;
    }
    if (fdmax < 10)
    {
        fdlist(0, fdmax) = 11;
        fdlist(1, fdmax) = -100;
        ++fdmax;
    }
    if (fdmax < 10)
    {
        fdlist(0, fdmax) = 17;
        fdlist(1, fdmax) = -100;
        ++fdmax;
    }
    if (fdmax < 10)
    {
        fdlist(0, fdmax) = 16;
        fdlist(1, fdmax) = -100;
        ++fdmax;
    }
    if (fdmax < 10)
    {
        fdlist(0, fdmax) = 12;
        fdlist(1, fdmax) = -100;
        ++fdmax;
    }
    if (fdmax < 10)
    {
        fdlist(0, fdmax) = 13;
        fdlist(1, fdmax) = -100;
        ++fdmax;
    }
    if (fdmax < 10)
    {
        fdlist(0, fdmax) = 14;
        fdlist(1, fdmax) = -100;
        ++fdmax;
    }
    if (fdmax < 10)
    {
        fdlist(0, fdmax) = 15;
        fdlist(1, fdmax) = -100;
        ++fdmax;
    }
    nutrition = 1000;
    status_ailment_damage(eater, StatusAilment::paralyzed, 100);
    status_ailment_damage(eater, StatusAilment::confused, 200);
}



void cure_anorexia(Character& chara)
{
    if (!chara.has_anorexia())
        return;

    chara.has_anorexia() = false;
    if (is_in_fov(chara) || chara.is_player_or_ally())
    {
        txt(i18n::s.get("core.food.anorexia.recovers_from", chara));
        snd("core.offer1");
    }
}



void chara_vomit(Character& chara)
{
    ++chara.anorexia_count;

    if (is_in_fov(chara))
    {
        snd("core.vomit");
        txt(i18n::s.get("core.food.vomits", chara));
    }

    if (chara.is_pregnant())
    {
        chara.is_pregnant() = false;
        if (is_in_fov(chara))
        {
            txt(i18n::s.get("core.food.spits_alien_children", chara));
        }
    }

    // Lose food buff.
    buff_remove_if(chara, [](const auto& buff) {
        return the_buff_db[buff.id]->type == BuffType::food;
    });

    // Vomit.
    if (map_data.type != mdata_t::MapType::world_map)
    {
        auto p = 2;
        for (const auto& item : *inv_map())
        {
            if (item->id == "core.vomit")
            {
                ++p;
            }
        }
        if (rnd_capped(p * p * p) == 0 || chara.is_player())
        {
            flt();
            if (const auto item = itemcreate_map_inv(704, chara.position, 0))
            {
                if (!chara.is_player())
                {
                    item->subname =
                        the_character_db.ensure(chara.id).integer_id;
                }
            }
        }
    }

    if (chara.has_anorexia())
    {
        chara_gain_fixed_skill_exp(chara, 10, -50);
        chara_gain_fixed_skill_exp(chara, 11, -75);
        chara_gain_fixed_skill_exp(chara, 17, -100);
    }
    else
    {
        if ((chara.is_player_or_ally() && chara.anorexia_count > 10) ||
            (!chara.is_player_or_ally() && rnd(4) == 0))
        {
            if (rnd(5) == 0)
            {
                chara.has_anorexia() = true;
                if (is_in_fov(chara))
                {
                    txt(i18n::s.get("core.food.anorexia.develops", chara));
                    snd("core.offer1");
                }
            }
        }
    }

    status_ailment_damage(chara, StatusAilment::dimmed, 100);
    modify_weight(chara, -(1 + rnd(5)));
    if (chara.nutrition <= 0)
    {
        damage_hp(chara, 9999, -3);
    }
    chara.nutrition -= 3000;
}



void food_apply_curse_state(Character& eater, CurseState curse_state)
{
    if (eater.state() != Character::State::alive)
        return;

    if (is_cursed(curse_state))
    {
        eater.nutrition -= 1500;
        if (is_in_fov(eater))
        {
            txt(i18n::s.get("core.food.eat_status.bad", eater));
        }
        chara_vomit(eater);
    }
    else if (curse_state == CurseState::blessed)
    {
        if (is_in_fov(eater))
        {
            txt(i18n::s.get("core.food.eat_status.good", eater));
        }
        if (rnd(5) == 0)
        {
            buff_add(eater, "core.luck", 100, 500 + rnd(500));
        }
        heal_insanity(eater, 2);
    }
}



void chara_anorexia(Character& chara)
{
    if (chara.has_anorexia())
    {
        chara_vomit(chara);
    }
}



void get_sick_if_cursed(CurseState curse_state, Character& drinker)
{
    if (!is_cursed(curse_state))
        return;

    if (is_in_fov(drinker))
    {
        txt(i18n::s.get("core.food.eat_status.cursed_drink", drinker));
    }
    status_ailment_damage(drinker, StatusAilment::sick, 200);
}



void get_hungry(Character& chara)
{
    if ((cdata.player().traits().level("core.slow_digestion") && rnd(3) == 0))
        return;
    if (debug_has_wizard_flag("core.wizard.no_hungry"))
        return;

    int p = chara.nutrition / 1000;
    chara.nutrition -= 8;
    if (chara.nutrition / 1000 != p)
    {
        if (p == 1)
        {
            input_halt_input(HaltInput::alert);
            txt(i18n::s.get("core.food.hunger_status.starving"));
        }
        else if (p == 2)
        {
            input_halt_input(HaltInput::alert);
            txt(i18n::s.get("core.food.hunger_status.very_hungry"));
        }
        else if (p == 5)
        {
            input_halt_input(HaltInput::alert);
            txt(i18n::s.get("core.food.hunger_status.hungry"));
        }
        chara_refresh_speed(chara);
    }
}



void food_cook(Character& cook, const ItemRef& cook_tool, const ItemRef& food)
{
    snd("core.cook1");
    item_separate(food);

    const auto item_name_prev = itemname(food);

    int dish_rank = rnd_capped(cook.skills().level("core.cooking") + 6) +
        rnd(cook_tool->param1 / 50 + 1);
    if (dish_rank > cook.skills().level("core.cooking") / 5 + 7)
    {
        dish_rank = cook.skills().level("core.cooking") / 5 + 7;
    }
    dish_rank = rnd(dish_rank + 1);
    if (dish_rank > 3)
    {
        dish_rank = rnd(dish_rank);
    }
    if (cook.skills().level("core.cooking") >= 5)
    {
        if (dish_rank < 3)
        {
            if (rnd(4) == 0)
            {
                dish_rank = 3;
            }
        }
    }
    if (cook.skills().level("core.cooking") >= 10)
    {
        if (dish_rank < 3)
        {
            if (rnd(3) == 0)
            {
                dish_rank = 3;
            }
        }
    }
    dish_rank += cook_tool->param1 / 100;
    dish_rank = clamp(dish_rank, 1, 9);

    make_dish(food, dish_rank);
    txt(i18n::s.get("core.food.cook", item_name_prev, cook_tool, food));
    inv_stack(inv_player(), food, true);
    const auto rank = food->param2;
    if (rank > 2)
    {
        chara_gain_skill_exp(cook, 184, 30 + rank * 5);
    }
    refresh_burden_state();
}



void make_dish(const ItemRef& food, int dish_rank)
{
    food->image = picfood(dish_rank, food->param1 / 1000);
    food->weight = 500;
    food->param2 = dish_rank;
    if (food->material == "core.raw" && !food_is_rotten(food))
    {
        food->__expiration_time = game_now() + 3_days;
    }
}



void apply_general_eating_effect(Character& eater, const ItemRef& food)
{
    DIM3(fdlist, 2, 10);
    for (int cnt = 0, cnt_end = (fdmax); cnt < cnt_end; ++cnt)
    {
        fdlist(0, cnt) = 0;
        fdlist(1, cnt) = 0;
    }
    nutrition = 2500;
    if (the_item_db[food->id]->is_cargo)
    {
        nutrition += 2500;
    }
    fdmax = 0;
    i = food->param1 / 1000;
    if (i == 1)
    {
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 10;
            fdlist(1, fdmax) = 30;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 11;
            fdlist(1, fdmax) = 40;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 17;
            fdlist(1, fdmax) = 10;
            ++fdmax;
        }
        nutrition = 3500;
    }
    if (i == 8)
    {
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 17;
            fdlist(1, fdmax) = 20;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 13;
            fdlist(1, fdmax) = 20;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 15;
            fdlist(1, fdmax) = 20;
            ++fdmax;
        }
        nutrition = 2000;
    }
    if (i == 2)
    {
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 15;
            fdlist(1, fdmax) = 50;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 14;
            fdlist(1, fdmax) = 50;
            ++fdmax;
        }
        nutrition = 2000;
    }
    if (i == 3)
    {
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 16;
            fdlist(1, fdmax) = 50;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 17;
            fdlist(1, fdmax) = 20;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 13;
            fdlist(1, fdmax) = 30;
            ++fdmax;
        }
        nutrition = 1500;
    }
    if (i == 7)
    {
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 10;
            fdlist(1, fdmax) = 25;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 12;
            fdlist(1, fdmax) = 25;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 14;
            fdlist(1, fdmax) = 25;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 11;
            fdlist(1, fdmax) = 25;
            ++fdmax;
        }
        nutrition = 2800;
    }
    if (i == 4)
    {
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 16;
            fdlist(1, fdmax) = 40;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 12;
            fdlist(1, fdmax) = 30;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 14;
            fdlist(1, fdmax) = 30;
            ++fdmax;
        }
        nutrition = 1500;
    }
    if (i == 6)
    {
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 14;
            fdlist(1, fdmax) = 40;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 12;
            fdlist(1, fdmax) = 40;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 13;
            fdlist(1, fdmax) = 20;
            ++fdmax;
        }
        nutrition = 3000;
    }
    if (i == 5)
    {
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 11;
            fdlist(1, fdmax) = 60;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 12;
            fdlist(1, fdmax) = 40;
            ++fdmax;
        }
        nutrition = 3500;
    }
    if (the_item_db[food->id]->category == ItemCategory::food)
    {
        nutrition = nutrition * (100 + food->param2 * 15) / 100;
    }
    for (int cnt = 0, cnt_end = (fdmax); cnt < cnt_end; ++cnt)
    {
        if (fdlist(1, cnt) > 0)
        {
            if (food->param2 < 3)
            {
                fdlist(1, cnt) = fdlist(1, cnt) / 2;
            }
            else
            {
                fdlist(1, cnt) =
                    fdlist(1, cnt) * (50 + food->param2 * 20) / 100;
            }
        }
        else if (food->param2 < 3)
        {
            fdlist(1, cnt) =
                fdlist(1, cnt) * ((3 - food->param2) * 100 + 100) / 100;
        }
        else
        {
            fdlist(1, cnt) = fdlist(1, cnt) * 100 / (food->param2 * 50);
        }
    }
    if (eater.is_player())
    {
        p = food->param1 / 1000;
        for (int cnt = 0; cnt < 1; ++cnt)
        {
            if (eater.is_player())
            {
                if (cdata.player().traits().level("core.cannibalism"))
                {
                    if (food->id == "core.corpse")
                    {
                        s = chara_db_get_filter(
                            *the_character_db.get_id_from_integer(
                                food->subname));
                        if (strutil::contains(s(0), "/man/"))
                        {
                            txt(i18n::s.get(
                                "core.food.effect.human.delicious"));
                            break;
                        }
                    }
                }
            }
            if (food->material == "core.raw")
            {
                if (food_is_rotten(food))
                {
                    txt(i18n::s.get("core.food.effect.rotten"));
                    break;
                }
            }
            if (food->param2 == 0)
            {
                if (p == 1)
                {
                    txt(i18n::s.get("core.food.effect.raw_meat"));
                    break;
                }
                if (p == 7)
                {
                    txt(i18n::s.get("core.food.effect.powder"));
                    break;
                }
                if (p == 5)
                {
                    txt(i18n::s.get("core.food.effect.raw"));
                    break;
                }
                txt(i18n::s.get("core.food.effect.boring"));
                break;
            }
            if (food->param2 < 3)
            {
                txt(i18n::s.get("core.food.effect.quality.bad"));
                break;
            }
            if (food->param2 < 5)
            {
                txt(i18n::s.get("core.food.effect.quality.so_so"));
                break;
            }
            if (food->param2 < 7)
            {
                txt(i18n::s.get("core.food.effect.quality.good"));
                break;
            }
            if (food->param2 < 9)
            {
                txt(i18n::s.get("core.food.effect.quality.great"));
                break;
            }
            txt(i18n::s.get("core.food.effect.quality.delicious"));
        }
    }
    else if (food->material == "core.raw")
    {
        if (food_is_rotten(food))
        {
            txt(i18n::s.get("core.food.effect.raw_glum", eater));
        }
    }
    if (food->id == "core.curaria")
    {
        fdmax = 0;
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 10;
            fdlist(1, fdmax) = 100;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 11;
            fdlist(1, fdmax) = 100;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 17;
            fdlist(1, fdmax) = 100;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 16;
            fdlist(1, fdmax) = 100;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 12;
            fdlist(1, fdmax) = 100;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 13;
            fdlist(1, fdmax) = 100;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 14;
            fdlist(1, fdmax) = 100;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 15;
            fdlist(1, fdmax) = 100;
            ++fdmax;
        }
        nutrition = 2500;
        if (eater.is_player())
        {
            txt(i18n::s.get("core.food.effect.herb.curaria"),
                Message::color{ColorIndex::green});
        }
    }
    if (food->id == "core.morgia")
    {
        fdmax = 0;
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 10;
            fdlist(1, fdmax) = 900;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 11;
            fdlist(1, fdmax) = 700;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 17;
            fdlist(1, fdmax) = 10;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 16;
            fdlist(1, fdmax) = 10;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 12;
            fdlist(1, fdmax) = 10;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 13;
            fdlist(1, fdmax) = 10;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 14;
            fdlist(1, fdmax) = 10;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 15;
            fdlist(1, fdmax) = 10;
            ++fdmax;
        }
        nutrition = 500;
        skill_add_potential(eater, "core.stat_strength", 2);
        skill_add_potential(eater, "core.stat_constitution", 2);
        if (eater.is_player())
        {
            txt(i18n::s.get("core.food.effect.herb.morgia"),
                Message::color{ColorIndex::green});
        }
    }
    if (food->id == "core.mareilon")
    {
        fdmax = 0;
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 10;
            fdlist(1, fdmax) = 10;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 11;
            fdlist(1, fdmax) = 10;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 17;
            fdlist(1, fdmax) = 10;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 16;
            fdlist(1, fdmax) = 800;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 12;
            fdlist(1, fdmax) = 10;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 13;
            fdlist(1, fdmax) = 10;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 14;
            fdlist(1, fdmax) = 10;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 15;
            fdlist(1, fdmax) = 800;
            ++fdmax;
        }
        nutrition = 500;
        skill_add_potential(eater, "core.stat_magic", 2);
        skill_add_potential(eater, "core.stat_will", 2);
        if (eater.is_player())
        {
            txt(i18n::s.get("core.food.effect.herb.mareilon"),
                Message::color{ColorIndex::green});
        }
    }
    if (food->id == "core.spenseweed")
    {
        fdmax = 0;
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 10;
            fdlist(1, fdmax) = 10;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 11;
            fdlist(1, fdmax) = 10;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 17;
            fdlist(1, fdmax) = 10;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 16;
            fdlist(1, fdmax) = 10;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 12;
            fdlist(1, fdmax) = 750;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 13;
            fdlist(1, fdmax) = 800;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 14;
            fdlist(1, fdmax) = 10;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 15;
            fdlist(1, fdmax) = 10;
            ++fdmax;
        }
        skill_add_potential(eater, "core.stat_dexterity", 2);
        skill_add_potential(eater, "core.stat_perception", 2);
        nutrition = 500;
        if (eater.is_player())
        {
            txt(i18n::s.get("core.food.effect.herb.spenseweed"),
                Message::color{ColorIndex::green});
        }
    }
    if (food->id == "core.alraunia")
    {
        fdmax = 0;
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 10;
            fdlist(1, fdmax) = 10;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 11;
            fdlist(1, fdmax) = 10;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 17;
            fdlist(1, fdmax) = 850;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 16;
            fdlist(1, fdmax) = 10;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 12;
            fdlist(1, fdmax) = 10;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 13;
            fdlist(1, fdmax) = 10;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 14;
            fdlist(1, fdmax) = 700;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 15;
            fdlist(1, fdmax) = 10;
            ++fdmax;
        }
        nutrition = 500;
        skill_add_potential(eater, "core.stat_charisma", 2);
        skill_add_potential(eater, "core.stat_learning", 2);
        if (eater.is_player())
        {
            txt(i18n::s.get("core.food.effect.herb.alraunia"),
                Message::color{ColorIndex::green});
        }
    }
    if (food->id == "core.stomafillia")
    {
        fdmax = 0;
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 10;
            fdlist(1, fdmax) = 50;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 11;
            fdlist(1, fdmax) = 50;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 17;
            fdlist(1, fdmax) = 50;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 16;
            fdlist(1, fdmax) = 50;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 12;
            fdlist(1, fdmax) = 50;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 13;
            fdlist(1, fdmax) = 50;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 14;
            fdlist(1, fdmax) = 50;
            ++fdmax;
        }
        if (fdmax < 10)
        {
            fdlist(0, fdmax) = 15;
            fdlist(1, fdmax) = 50;
            ++fdmax;
        }
        nutrition = 20000;
    }
    if (food->id == "core.fortune_cookie")
    {
        nutrition = 750;
    }
    if (eater.is_player())
    {
        if (food->material == "core.raw")
        {
            if (food_is_rotten(food))
            {
                eat_rotten_food(eater);
            }
        }
    }
    if (food->id == "core.corpse")
    {
        s = chara_db_get_filter(
            *the_character_db.get_id_from_integer(food->subname));
        if (eater.is_player())
        {
            if (strutil::contains(s(0), "/man/"))
            {
                if (cdata.player().traits().level("core.cannibalism"))
                {
                    txt(i18n::s.get("core.food.effect.human.like"));
                }
                else
                {
                    txt(i18n::s.get("core.food.effect.human.dislike"));
                    damage_insanity(eater, 15);
                    status_ailment_damage(eater, StatusAilment::insane, 150);
                    if (cdata.player().traits().level("core.cannibalism") == 0)
                    {
                        if (rnd(5) == 0)
                        {
                            trait_get_info(0, 41);
                            txt(traitrefn(0),
                                Message::color{ColorIndex::green});
                            cdata.player().traits().set_level(
                                "core.cannibalism", 1);
                        }
                    }
                }
            }
            else if (cdata.player().traits().level("core.cannibalism"))
            {
                txt(i18n::s.get(
                    "core.food.effect.human.would_have_rather_eaten"));
                nutrition = nutrition * 2 / 3;
            }
        }
    }
    if (food->id == "core.corpse" ||
        ((food->id == "core.jerky" || food->id == "core.egg") && rnd(3) == 0))
    {
        chara_db_invoke_eating_effect(
            eater, *the_character_db.get_id_from_integer(food->subname));
    }
    for (int cnt = 0, cnt_end = (fdmax); cnt < cnt_end; ++cnt)
    {
        i = 100;
        if (eater.nutrition >= 5000)
        {
            p = (eater.nutrition - 5000) / 25;
            i = i * 100 / (100 + p);
        }
        if (!eater.is_player())
        {
            i = 1500;
            if (food->material == "core.raw")
            {
                if (food_is_rotten(food))
                {
                    i = 500;
                }
            }
        }
        if (i > 0)
        {
            chara_gain_skill_exp(
                eater, fdlist(0, cnt), fdlist(1, cnt) * i / 100);
        }
    }
    if (food->curse_state == CurseState::blessed)
    {
        nutrition = nutrition * 150 / 100;
    }
    if (is_cursed(food->curse_state))
    {
        nutrition = nutrition * 50 / 100;
    }
    eater.nutrition += nutrition;
    if (nutrition >= 3000)
    {
        if (rnd(10) == 0 || eater.nutrition >= 12000)
        {
            modify_weight(
                eater,
                rnd(3) + 1,
                eater.nutrition >= 20000 &&
                    rnd(30000 / std::max(lua_int{1}, eater.nutrition) + 2) ==
                        0);
        }
    }
    if (eater.id == "core.cute_fairy")
    {
        if (nutrition >= 2000)
        {
            flt(calcobjlv(eater.level));
            flttypeminor = 58500;
            if (const auto item = itemcreate_map_inv(0, eater.position, 0))
            {
                txt(i18n::s.get(
                        "core.food.effect.bomb_fish", eater, item.unwrap()),
                    Message::color{ColorIndex::cyan});
            }
        }
    }
    if (food->id == "core.corpse")
    {
        if (food->subname == 319)
        {
            txt(i18n::s.get("core.food.effect.little_sister", eater),
                Message::color{ColorIndex::green});
            if (rnd_capped(
                    eater.skills().base_level("core.stat_life") *
                        eater.skills().base_level("core.stat_life") +
                    1) < 2000)
            {
                chara_gain_fixed_skill_exp(eater, 2, 1000);
            }
            if (rnd_capped(
                    eater.skills().base_level("core.stat_mana") *
                        eater.skills().base_level("core.stat_mana") +
                    1) < 2000)
            {
                chara_gain_fixed_skill_exp(eater, 3, 1000);
            }
            for (int cnt = 100; cnt < 400; ++cnt)
            {
                if (!the_skill_db[cnt] ||
                    the_skill_db[cnt]->related_basic_attribute == 0 ||
                    eater.skills().base_level(
                        *the_skill_db.get_id_from_integer(cnt)) == 0)
                {
                    continue;
                }
                skill_add_potential(
                    eater, *the_skill_db.get_id_from_integer(cnt), rnd(10) + 1);
            }
        }
    }
    if (food->id == "core.kagami_mochi")
    {
        txt(i18n::s.get("core.food.effect.hero_cheese"));
        chara_gain_fixed_skill_exp(eater, 19, 2000);
    }
    if (food->id == "core.rabbits_tail")
    {
        chara_gain_fixed_skill_exp(eater, 19, 1000);
    }
    if (food->id == "core.happy_apple")
    {
        chara_gain_fixed_skill_exp(eater, 19, 20000);
    }
    if (food->id == "core.hero_cheese")
    {
        chara_gain_fixed_skill_exp(eater, 2, 3000);
    }
    if (food->id == "core.magic_fruit")
    {
        chara_gain_fixed_skill_exp(eater, 3, 3000);
    }
    if (food->id == "core.fortune_cookie")
    {
        if (eater.is_player_or_ally())
        {
            txt(i18n::s.get("core.food.effect.fortune_cookie", eater));
            read_talk_file("%COOKIE2");
            if (food->curse_state == CurseState::blessed ||
                (food->curse_state == CurseState::none && rnd(2)))
            {
                read_talk_file("%COOKIE1");
            }
            txt(""s + buff, Message::color{ColorIndex::orange});
        }
    }
    if (food->id == "core.sisters_love_fueled_lunch")
    {
        txt(i18n::s.get("core.food.effect.sisters_love_fueled_lunch", eater));
        heal_insanity(eater, 30);
    }
    if (food->is_poisoned)
    {
        if (is_in_fov(eater))
        {
            txt(i18n::s.get("core.food.effect.poisoned.text", eater));
            txt(i18n::s.get("core.food.effect.poisoned.dialog"));
        }
        damage_hp(eater, rnd(250) + 250, -4);
        if (eater.state() != Character::State::alive)
        {
            if (!eater.is_player())
            {
                if (eater.relationship >= Relationship::friendly)
                {
                    modify_karma(cdata.player(), -1);
                }
            }
            return;
        }
    }
    if (food->is_aphrodisiac)
    {
        if (eater.is_player())
        {
            txt(i18n::s.get("core.food.effect.spiked.self"));
        }
        else
        {
            txt(i18n::s.get("core.food.effect.spiked.other", eater),
                Message::color{ColorIndex::cyan});
            eater.emotion_icon = 317;
            chara_modify_impression(eater, 30);
            modify_karma(cdata.player(), -10);
            lovemiracle(eater);
        }
        status_ailment_damage(eater, StatusAilment::dimmed, 500);
        eater.emotion_icon = 317;
    }
    for (const auto& enc : food->enchantments)
    {
        int enc_id = enc.id;
        if (enc_id == 36)
        {
            p = rnd_capped(enc.power / 50 + 1) + 1;
            heal_sp(eater, p);
            continue;
        }
        if (enc_id == 38)
        {
            p = rnd_capped(enc.power / 25 + 1) + 1;
            heal_mp(eater, p / 5);
            continue;
        }
        if (enc_id == 37)
        {
            deferred_event_add(DeferredEvent{
                "core.ragnarok",
                0,
                lua::create_table("core.chara", eater.index)});
            continue;
        }
        if (enc_id == 40)
        {
            if (game()->frozen_turns == 0)
            {
                txt(i18n::s.get("core.action.time_stop.begins", eater),
                    Message::color{ColorIndex::cyan});
                game()->frozen_turns = enc.power / 100 + 2;
                continue;
            }
        }
        const auto enc2 = enc_id / 10000;
        if (enc2 != 0)
        {
            enc_id = enc_id % 10000;
            if (enc2 == 1)
            {
                if (is_in_fov(eater))
                {
                    if (enc.power / 50 + 1 >= 0)
                    {
                        txt(i18n::s.get(
                            "core.food.effect.ability.develops",
                            eater,
                            the_skill_db.get_text(enc_id, "name")));
                    }
                    else
                    {
                        txt(i18n::s.get(
                            "core.food.effect.ability.deteriorates",
                            eater,
                            the_skill_db.get_text(enc_id, "name")));
                    }
                }
                chara_gain_skill_exp(
                    eater,
                    enc_id,
                    (enc.power / 50 + 1) * 100 *
                        (1 + (!eater.is_player()) * 5));
                continue;
            }
            if (enc2 == 6)
            {
                if (is_in_fov(eater))
                {
                    txt(the_buff_db.get_text(enc_id + 10, "apply", eater));
                }

                int integer_id = 20 + (enc_id - 10);
                auto buff_id = the_buff_db.get_id_from_integer(integer_id);
                assert(buff_id);

                buff_add(
                    eater,
                    *buff_id,
                    (enc.power / 50 + 1) * 5 * (1 + (!eater.is_player()) * 2),
                    2000);

                continue;
            }
        }
    }
    food_apply_curse_state(eater, food->curse_state);
}



std::string
foodname(int type, const std::string& ingredient_, int rank, int character_id)
{
    std::string ingredient = ingredient_;
    if (type == 1 || type == 8)
    {
        // Food created from character drops (meat and eggs)
        if (character_id == 0)
        {
            ingredient = i18n::s.get_enum_property(
                "core.food.names", "default_origin", type);
        }
        else
        {
            ingredient = chara_db_get_name(
                *the_character_db.get_id_from_integer(character_id));
        }
    }
    else if (type == 5 || type == 7)
    {
        // Food that always always has a default origin (bread and noodles)
        ingredient = i18n::s.get_enum_property(
            "core.food.names", "default_origin", type);
    }
    else if (ingredient == ""s)
    {
        // No ingredient name was provided
        ingredient = i18n::s.get_enum_property(
            "core.food.names", "default_origin", type);
    }

    if ((type < 1 || type > 8) || (rank < 1 || rank > 9))
    {
        return ingredient;
    }
    else
    {
        return i18n::s.get_enum_property(
            "core.food.names", "_" + std::to_string(rank), type, ingredient);
    }
}



void foods_get_rotten()
{
    for (auto&& chara : cdata.all())
    {
        if (chara.state() == Character::State::empty)
        {
            continue;
        }

        for (const auto& item : *chara.inventory())
        {
            _food_gets_rotten(chara.index, item);
        }
    }
    for (const auto& item : *inv_map())
    {
        _food_gets_rotten(-1, item);
    }
}



bool food_is_rotten(const ItemRef& food)
{
    // return food->__expiration_time == "ROTTEN";
    return food->__is_rotten;
}



void food_make_rotten(const ItemRef& food)
{
    // food->__expiration_time = "ROTTEN";
    food->__expiration_time = time::Instant{};
    food->__is_rotten = true;
}

} // namespace elona
