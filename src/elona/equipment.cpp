#include "equipment.hpp"

#include "adventurer.hpp"
#include "calc.hpp"
#include "chara_db.hpp"
#include "character.hpp"
#include "character_status.hpp"
#include "class.hpp"
#include "data/types/type_item.hpp"
#include "draw.hpp"
#include "enchantment.hpp"
#include "food.hpp"
#include "globals.hpp"
#include "i18n.hpp"
#include "inventory.hpp"
#include "item.hpp"
#include "itemgen.hpp"
#include "random.hpp"
#include "variables.hpp"



namespace elona
{

namespace
{

snail::Color _get_element_color(int element)
{
    switch (element)
    {
    case 50: return {150, 0, 0};
    case 51: return {0, 0, 150};
    case 52: return {150, 150, 0};
    case 59: return {150, 0, 150};
    case 53: return {100, 80, 80};
    case 55: return {0, 150, 0};
    case 60: return {150, 100, 100};
    case 57: return {50, 100, 150};
    case 58: return {100, 150, 50};
    case 54: return {150, 100, 50};
    case 56: return {150, 50, 0};
    default: return {0, 0, 0};
    }
}



void draw_additional_item_info_resistance(const ItemRef& equip, int x, int y)
{
    for (const auto& enc : equip->enchantments)
    {
        if (enc.id / 10000 != 2)
            continue;

        const auto power_level = std::abs(enc.power / 2 / 50) + 1;
        const auto element = enc.id % 10000;
        const auto color = _get_element_color(element);
        if (jp)
        {
            const auto s = power_level > 5 ? "+"s : std::to_string(power_level);
            const auto glyph = enc.power >= 0 ? "●" : "▼";
            mes(x + (element - 50) * 20, y, glyph, color);
            mes(x + (element - 50) * 20 + 5, y + 1, s, color);
            mes(x + (element - 50) * 20 + 4, y, s, {255, 255, 255});
        }
        else
        {
            mes(x + (element - 50) * 20 + 5,
                y,
                std::to_string(power_level),
                {80, 60, 40});
            mes(x + (element - 50) * 20 + 4,
                y,
                std::to_string(power_level),
                color);
        }
    }
}



void draw_additional_item_info_maintenance_and_ailment(
    const ItemRef& equip,
    int x,
    int y)
{
    for (int i = 0; i < 10; ++i)
    {
        if (enchantment_find(equip, 60000 + 10 + i))
        {
            mes(x - 100 + i * 20, y, jp ? "●" : "#", {0, 100, 100});
        }
        else
        {
            mes(x - 100 + i * 20, y, "-", {0, 100, 100});
        }
    }
    for (int i = 0; i < 6; ++i)
    {
        if (enchantment_find(equip, 23 + i))
        {
            mes(x + 100 + i * 20, y, jp ? "●" : "#", {100, 32, 0});
        }
        else
        {
            mes(x + 100 + i * 20, y, "-", {100, 32, 0});
        }
    }
}

} // namespace



int i_at_m66 = 0;



void draw_additional_item_info_label(int x, int y)
{
    switch (g_show_additional_item_info)
    {
    case AdditionalItemInfo::none:
    case AdditionalItemInfo::_size: break;
    case AdditionalItemInfo::resistance:
        for (int i = 0; i < 11; ++i)
        {
            mes(x + 20 * i, y, i18n::s.get_enum("core.ui.equip.resist", i));
        }
        break;
    case AdditionalItemInfo::maintenance_and_ailment:
        for (int i = 0; i < 10; ++i)
        {
            mes(x - 100 + 20 * i,
                y,
                i18n::s.get_enum("core.ui.equip.maintenance", i));
        }
        for (int i = 0; i < 6; ++i)
        {
            mes(x + 100 + 20 * i,
                y,
                i18n::s.get_enum("core.ui.equip.ailment", i));
        }
        break;
    }
}



void draw_additional_item_info(const ItemRef& equip, int x, int y)
{
    if (equip->identify_state != IdentifyState::completely)
        return;

    switch (g_show_additional_item_info)
    {
    case AdditionalItemInfo::none:
    case AdditionalItemInfo::_size: break;
    case AdditionalItemInfo::resistance:
        draw_additional_item_info_resistance(equip, x, y);
        break;
    case AdditionalItemInfo::maintenance_and_ailment:
        draw_additional_item_info_maintenance_and_ailment(equip, x, y);
        break;
    }
}



std::string cut_item_name_for_additional_info(
    const std::string& name,
    size_t adjustment)
{
    size_t width;
    switch (g_show_additional_item_info)
    {
    case AdditionalItemInfo::none:
    case AdditionalItemInfo::_size: return name;
    case AdditionalItemInfo::resistance: width = 24; break;
    case AdditionalItemInfo::maintenance_and_ailment: width = 14; break;
    }
    width -= adjustment;

    return strutil::take_by_width(name, width);
}



int eqweaponlight()
{
    if (rnd(2))
    {
        return 10003;
    }
    if (rnd(2))
    {
        return 10009;
    }
    return 10004;
}



int eqweaponheavy()
{
    if (rnd(3) == 0)
    {
        return 10001;
    }
    if (rnd(3) == 0)
    {
        return 10010;
    }
    if (rnd(3) == 0)
    {
        return 10008;
    }
    return 10005;
}



void eqrandweaponmage()
{
    if (rnd(2))
    {
        eqweapon1(0) = 10006;
        eqweapon1(1) = 2;
    }
    else
    {
        eqweapon1(0) = 10003;
        eqweapon1(1) = 2;
    }
}



void wear_most_valuable_equipment_for_all_body_parts(Character& chara)
{
    for (const auto& item : *chara.inventory())
    {
        if (item->is_equipped())
        {
            continue;
        }
        wear_most_valuable_equipment(chara, item);
    }
}



void wear_most_valuable_equipment(Character& chara, ItemRef equipment)
{
    const auto equipment_body_part = iequiploc(equipment);
    if (equipment_body_part == "")
        return;

    std::vector<size_t> equipment_slot_indice;
    for (size_t i = 0; i < chara.body_parts.size(); ++i)
    {
        if (chara.body_parts[i].id == equipment_body_part)
        {
            equipment_slot_indice.emplace_back(i);
        }
    }
    if (equipment_slot_indice.empty())
        return;

    for (size_t i = 0; i < equipment_slot_indice.size(); ++i)
    {
        const auto body_part_index = equipment_slot_indice[i];
        auto& body_part = chara.body_parts[body_part_index];
        if (!body_part.is_equip())
        {
            equip_item(chara, body_part_index, equipment);
            break;
        }
        bool equip = false;
        if (equipment->value >= body_part.equipment()->value)
        {
            equip = true;
        }
        if (equipment_slot_indice.size() > i + 1)
        {
            if (!chara.body_parts[equipment_slot_indice[i + 1]].is_equip())
            {
                equip = false;
            }
            else if (
                body_part.equipment()->value >=
                chara.body_parts[equipment_slot_indice[i + 1]]
                    .equipment()
                    ->value)
            {
                equip = false;
            }
        }
        if (equip)
        {
            unequip_item(chara, body_part_index);
            equip_item(chara, body_part_index, equipment);
            break;
        }
    }
}



void supply_new_equipment(Character& chara)
{
    int haveweapon = 0;
    int mustequip = 0;
    haveweapon = 0;
    for (int cnt = 0; cnt < 100; ++cnt)
    {
        f = 0;
        for (int _j = 0; _j < 4; ++_j)
        {
            const auto inv = chara.inventory();
            const auto item = inv->at(inv_get_random_slot(inv));
            if (!item)
            {
                f = 1;
                break;
            }
            if (item->is_equipped())
            {
                continue;
            }
            if (item->is_quest_target)
            {
                continue;
            }
            if (item->number() != 0)
            {
                item->remove();
                f = 1;
                break;
            }
        }
        if (f == 0)
        {
            break;
        }
        if (chara.role == Role::adventurer)
        {
            flt(chara.level, Quality::great);
        }
        else
        {
            flt(chara.level, calcfixlv(Quality::bad));
        }
        mustequip = 0;
        for (const auto& body_part : chara.body_parts)
        {
            const auto body_part_id = body_part.id;
            if (body_part.is_equip())
            {
                if (body_part_id == "core.hand")
                {
                    if (haveweapon == 0)
                    {
                        if (the_item_db[body_part.equipment()->id]->category ==
                            ItemCategory::melee_weapon)
                        {
                            haveweapon = 1;
                        }
                    }
                }
                continue;
            }
            if (body_part_id == "core.hand")
            {
                if (haveweapon == 0)
                {
                    flttypemajor = 10000;
                    mustequip = 1;
                    break;
                }
            }
            if (body_part_id == "core.head")
            {
                flttypemajor = 12000;
                mustequip = 1;
                break;
            }
            if (body_part_id == "core.body")
            {
                flttypemajor = 16000;
                mustequip = 1;
                break;
            }
            if (body_part_id == "core.shoot")
            {
                flttypeminor = 24001;
                mustequip = 1;
                break;
            }
            if (body_part_id == "core.ammo")
            {
                flttypeminor = 25001;
                mustequip = 1;
                break;
            }
        }
        if (mustequip == 0)
        {
            break;
        }
        if (const auto item = itemcreate_chara_inv(chara, 0, 0))
        {
            item->identify_state = IdentifyState::completely;
            if (item->quality >= Quality::miracle)
            {
                if (is_equipment(the_item_db[item->id]->category))
                {
                    if (chara.role == Role::adventurer)
                    {
                        adventurer_add_news(
                            NewsType::discovery,
                            chara,
                            itemname(item.unwrap()));
                    }
                }
            }
            wear_most_valuable_equipment(chara, item.unwrap());
            if (chara.role != Role::adventurer)
            {
                if (rnd(3))
                {
                    break;
                }
            }
        }
        else
        {
            break;
        }
    }
}



void supply_initial_equipments(Character& chara)
{
    elona_vector1<int> eqhelm;
    elona_vector1<int> eqshield;
    elona_vector1<int> eqweapon2;
    elona_vector1<int> eqarmor;
    elona_vector1<int> eqglove;
    elona_vector1<int> eqboots;
    elona_vector1<int> eqcloack;
    elona_vector1<int> eqgirdle;
    elona_vector1<int> eqring2;
    elona_vector1<int> eqamulet1;
    elona_vector1<int> eqamulet2;
    int fixeq = 0;
    int probeq = 0;
    int eqtwowield = 0;
    if (chara.race == "core.mutant")
    {
        const auto n_lims = clamp(chara.level / 3, 0, 12);
        for (int _i = 0; _i < n_lims; ++_i)
        {
            body_part_gain_new_slot(chara, !cm);
        }
    }

    eqhelm.clear();
    eqweapon1.clear();
    eqshield.clear();
    eqrange.clear();
    eqammo.clear();
    eqweapon2.clear();
    eqarmor.clear();
    eqglove.clear();
    eqboots.clear();
    eqcloack.clear();
    eqgirdle.clear();
    eqring1.clear();
    eqring2.clear();
    eqamulet1.clear();
    eqamulet2.clear();

    if (g_mode == 1)
    {
        fixeq = 0;
        probeq = 10;
    }
    else if (chara.quality <= Quality::good)
    {
        probeq = 3;
        fixeq = 0;
    }
    else if (chara.quality == Quality::great)
    {
        probeq = 6;
        fixeq = 0;
    }
    else if (chara.quality == Quality::miracle)
    {
        probeq = 8;
        fixeq = 1;
    }
    else
    {
        probeq = 10;
        fixeq = 1;
    }

    switch (class_get_equipment_type(chara.class_))
    {
    case 0: break;
    case 1:
        generatemoney(chara);
        eqweapon1(0) = eqweaponheavy();
        eqweapon1(1) = 2;
        eqshield(0) = 14003;
        eqshield(1) = 1;
        if (rnd(10) < probeq)
        {
            eqarmor(0) = 16001;
            eqarmor(1) = 1;
        }
        if (rnd(10) < probeq)
        {
            eqhelm(0) = 12001;
            eqhelm(1) = 1;
        }
        if (rnd(10) < probeq)
        {
            eqboots(0) = 18001;
            eqboots(1) = 1;
        }
        if (rnd(10) < probeq)
        {
            eqgirdle(0) = 19001;
            eqgirdle(1) = 1;
        }
        eqrange(0) = 24030;
        eqrange(1) = 1;
        break;
    case 2:
        generatemoney(chara);
        eqrandweaponmage();
        eqamulet1(0) = 34001;
        eqamulet1(1) = 1;
        eqring1(0) = 32001;
        eqring1(1) = 2;
        eqring2(0) = 32001;
        eqring2(1) = 1;
        if (rnd(10) < probeq)
        {
            eqarmor(0) = 16003;
            eqarmor(1) = 1;
        }
        if (rnd(10) < probeq)
        {
            eqcloack(0) = 20001;
            eqcloack(1) = 1;
        }
        break;
    case 3:
        generatemoney(chara);
        eqweapon1(0) = 10002;
        eqweapon1(1) = 1;
        eqrange(0) = 24001;
        eqrange(1) = 1;
        eqammo(0) = 25001;
        eqammo(1) = 1;
        eqcloack(0) = 20001;
        eqcloack(1) = 1;
        if (rnd(10) < probeq)
        {
            eqarmor(0) = 16001;
            eqarmor(1) = 1;
        }
        if (rnd(10) < probeq)
        {
            eqglove(0) = 22001;
            eqglove(1) = 1;
        }
        if (rnd(10) < probeq)
        {
            eqboots(0) = 18001;
            eqboots(1) = 1;
        }
        break;
    case 4:
        generatemoney(chara);
        eqweapon1(0) = 10002;
        eqweapon1(1) = 1;
        if (rnd(4) != 0)
        {
            eqrange(0) = 24020;
            eqrange(1) = 2;
            eqammo(0) = 25020;
            eqammo(1) = 1;
        }
        else
        {
            eqrange(0) = 24021;
            eqrange(1) = 2;
            eqammo(0) = 25030;
            eqammo(1) = 1;
        }
        eqcloack(0) = 20001;
        eqcloack(1) = 1;
        if (rnd(10) < probeq)
        {
            eqarmor(0) = 16001;
            eqarmor(1) = 1;
        }
        if (rnd(10) < probeq)
        {
            eqglove(0) = 22001;
            eqglove(1) = 1;
        }
        if (rnd(10) < probeq)
        {
            eqboots(0) = 18001;
            eqboots(1) = 1;
        }
        break;
    case 5:
        generatemoney(chara);
        eqrandweaponmage();
        eqamulet1(0) = 34001;
        eqamulet1(1) = 1;
        eqring1(0) = 32001;
        eqring1(1) = 2;
        eqring2(0) = 32001;
        eqring2(1) = 1;
        if (rnd(10) < probeq)
        {
            eqarmor(0) = 16001;
            eqarmor(1) = 1;
        }
        if (rnd(10) < probeq)
        {
            eqcloack(0) = 20001;
            eqcloack(1) = 1;
        }
        break;
    case 6:
        generatemoney(chara);
        eqweapon1(0) = 10004;
        eqweapon1(1) = 1;
        if (rnd(10) < probeq)
        {
            eqshield(0) = 14003;
            eqshield(1) = 1;
        }
        if (rnd(10) < probeq)
        {
            eqarmor(0) = 16001;
            eqarmor(1) = 1;
        }
        if (rnd(10) < probeq)
        {
            eqhelm(0) = 12001;
            eqhelm(1) = 1;
        }
        if (rnd(10) < probeq)
        {
            eqboots(0) = 18001;
            eqboots(1) = 1;
        }
        break;
    case 7:
        generatemoney(chara);
        eqweapon1(0) = eqweaponlight();
        eqweapon1(1) = 2;
        eqweapon2(0) = eqweaponlight();
        eqweapon2(1) = 2;
        if (rnd(10) < probeq)
        {
            eqarmor(0) = 16001;
            eqarmor(1) = 1;
        }
        if (rnd(10) < probeq)
        {
            eqhelm(0) = 12001;
            eqhelm(1) = 1;
        }
        if (rnd(10) < probeq)
        {
            eqboots(0) = 18001;
            eqboots(1) = 1;
        }
        if (rnd(10) < probeq)
        {
            eqgirdle(0) = 19001;
            eqgirdle(1) = 1;
        }
        eqrange(0) = 24030;
        eqrange(1) = 1;
        eqtwowield = 1;
        break;
    case 8:
        generatemoney(chara);
        eqweapon1(0) = 232;
        eqweapon1(1) = 3;
        if (rnd(10) < probeq)
        {
            eqboots(0) = 18001;
            eqboots(1) = 1;
        }
        if (rnd(10) < probeq)
        {
            eqgirdle(0) = 19001;
            eqgirdle(1) = 1;
        }
        if (rnd(10) < probeq)
        {
            eqcloack(0) = 20001;
            eqcloack(1) = 2;
        }
        eqrange(0) = 24030;
        eqrange(1) = 1;
        eqtwohand = 1;
        break;
    }

    if (chara.quality >= Quality::miracle)
    {
        for (int cnt = 0; cnt < 2; ++cnt)
        {
            if (rnd(2) == 0)
            {
                switch (rnd(20))
                {
                case 0: eqhelm(1) = 3; break;
                case 1: eqweapon1(1) = 3; break;
                case 2: eqshield(1) = 3; break;
                case 3: eqrange(1) = 3; break;
                case 4: eqammo(1) = 3; break;
                case 5: eqweapon2(1) = 3; break;
                case 6: eqarmor(1) = 3; break;
                case 7: eqglove(1) = 3; break;
                case 8: eqboots(1) = 3; break;
                case 9: eqcloack(1) = 3; break;
                case 10: eqgirdle(1) = 3; break;
                case 11: eqring1(1) = 3; break;
                case 12: eqring2(1) = 3; break;
                case 13: eqamulet1(1) = 3; break;
                case 14: eqamulet2(1) = 3; break;
                default: break;
                }
            }
            if (rnd(2) == 0)
            {
                --cnt;
                continue;
            }
        }
    }
    if (cspecialeq)
    {
        chara_db_get_special_equipments(chara.id);
    }
    if (eqtwohand)
    {
        eqshield = 0;
    }
    if (chara.id == "core.beggar")
    {
        if (rnd(120) == 0)
        {
            eqamulet1 = 705;
        }
    }
    if (chara.id == "core.noble")
    {
        if (rnd(100) == 0)
        {
            eqrange = 718;
        }
    }
    if (chara.id == "core.rock_thrower")
    {
        if (rnd(200) == 0)
        {
            eqrange = 716;
        }
    }
    if (chara.id == "core.blade" || chara.id == "core.blade_alpha" ||
        chara.id == "core.blade_omega")
    {
        if (rnd(800) == 0)
        {
            eqgirdle = 728;
        }
    }
    if (chara.id == "core.silver_eyed_witch")
    {
        if (rnd(150) == 0)
        {
            eqweapon1 = 719;
        }
    }
    if (chara.id == "core.asura" || chara.id == "core.mitra" ||
        chara.id == "core.varuna")
    {
        if (rnd(600) == 0)
        {
            eqamulet1 = 723;
        }
    }
    if (chara.id == "core.rogue_archer")
    {
        if (rnd(250) == 0)
        {
            eqamulet1 = 722;
        }
    }
    for (size_t i = 0; i < 30; ++i)
    {
        if (chara.body_parts.size() <= i)
            continue;

        const auto body_part_id = chara.body_parts[i].id;
        if (body_part_id == "core.neck")
        {
            if (eqamulet1)
            {
                int item_id;
                if (eqamulet1 >= 10000)
                {
                    flt(calcobjlv(chara.level),
                        calcfixlv(static_cast<Quality>(fixeq + eqamulet1(1))));
                    flttypeminor = eqamulet1;
                    item_id = -1;
                }
                else
                {
                    flt();
                    item_id = eqamulet1;
                }
                if (const auto equipment =
                        itemcreate_chara_inv(chara, item_id, 0))
                {
                    equip_item(chara, i, equipment.unwrap());
                }
                eqamulet1 = 0;
                continue;
            }
            if (eqamulet2)
            {
                int item_id;
                if (eqamulet2 >= 10000)
                {
                    flt(calcobjlv(chara.level),
                        calcfixlv(static_cast<Quality>(fixeq + eqamulet2(1))));
                    flttypeminor = eqamulet2;
                    item_id = -1;
                }
                else
                {
                    flt();
                    item_id = eqamulet2;
                }
                if (const auto equipment =
                        itemcreate_chara_inv(chara, item_id, 0))
                {
                    equip_item(chara, i, equipment.unwrap());
                }
                eqamulet2 = 0;
                continue;
            }
        }
        if (body_part_id == "core.ring")
        {
            if (eqring1)
            {
                int item_id;
                if (eqring1 >= 10000)
                {
                    flt(calcobjlv(chara.level),
                        calcfixlv(static_cast<Quality>(fixeq + eqring1(1))));
                    flttypeminor = eqring1;
                    item_id = -1;
                }
                else
                {
                    flt();
                    item_id = eqring1;
                }
                if (const auto equipment =
                        itemcreate_chara_inv(chara, item_id, 0))
                {
                    equip_item(chara, i, equipment.unwrap());
                }
                eqring1 = 0;
                continue;
            }
            if (eqring2)
            {
                int item_id;
                if (eqring2 >= 10000)
                {
                    flt(calcobjlv(chara.level),
                        calcfixlv(static_cast<Quality>(fixeq + eqring2(1))));
                    flttypeminor = eqring2;
                    item_id = -1;
                }
                else
                {
                    flt();
                    item_id = eqring2;
                }
                if (const auto equipment =
                        itemcreate_chara_inv(chara, item_id, 0))
                {
                    equip_item(chara, i, equipment.unwrap());
                }
                eqring2 = 0;
                continue;
            }
        }
        if (body_part_id == "core.back")
        {
            if (eqcloack)
            {
                int item_id;
                if (eqcloack >= 10000)
                {
                    flt(calcobjlv(chara.level),
                        calcfixlv(static_cast<Quality>(fixeq + eqcloack(1))));
                    flttypeminor = eqcloack;
                    item_id = -1;
                }
                else
                {
                    flt();
                    item_id = eqcloack;
                }
                if (const auto equipment =
                        itemcreate_chara_inv(chara, item_id, 0))
                {
                    equip_item(chara, i, equipment.unwrap());
                }
                eqcloack = 0;
                continue;
            }
            continue;
        }
        if (body_part_id == "core.waist")
        {
            if (eqgirdle)
            {
                int item_id;
                if (eqgirdle >= 10000)
                {
                    flt(calcobjlv(chara.level),
                        calcfixlv(static_cast<Quality>(fixeq + eqgirdle(1))));
                    flttypeminor = eqgirdle;
                    item_id = -1;
                }
                else
                {
                    flt();
                    item_id = eqgirdle;
                }
                if (const auto equipment =
                        itemcreate_chara_inv(chara, item_id, 0))
                {
                    equip_item(chara, i, equipment.unwrap());
                }
                eqgirdle = 0;
                continue;
            }
            continue;
        }
        if (body_part_id == "core.head")
        {
            if (eqhelm)
            {
                int item_id;
                if (eqhelm >= 10000)
                {
                    flt(calcobjlv(chara.level),
                        calcfixlv(static_cast<Quality>(fixeq + eqhelm(1))));
                    flttypeminor = eqhelm;
                    item_id = -1;
                }
                else
                {
                    flt();
                    item_id = eqhelm;
                }
                if (const auto equipment =
                        itemcreate_chara_inv(chara, item_id, 0))
                {
                    equip_item(chara, i, equipment.unwrap());
                }
                eqhelm = 0;
                continue;
            }
            continue;
        }
        if (body_part_id == "core.body")
        {
            if (eqarmor)
            {
                int item_id;
                if (eqarmor >= 10000)
                {
                    flt(calcobjlv(chara.level),
                        calcfixlv(static_cast<Quality>(fixeq + eqarmor(1))));
                    flttypeminor = eqarmor;
                    item_id = -1;
                }
                else
                {
                    flt();
                    item_id = eqarmor;
                }
                if (const auto equipment =
                        itemcreate_chara_inv(chara, item_id, 0))
                {
                    equip_item(chara, i, equipment.unwrap());
                }
                eqarmor = 0;
                continue;
            }
            continue;
        }
        if (body_part_id == "core.arm")
        {
            if (eqglove)
            {
                int item_id;
                if (eqglove >= 10000)
                {
                    flt(calcobjlv(chara.level),
                        calcfixlv(static_cast<Quality>(fixeq + eqglove(1))));
                    flttypeminor = eqglove;
                    item_id = -1;
                }
                else
                {
                    flt();
                    item_id = eqglove;
                }
                if (const auto equipment =
                        itemcreate_chara_inv(chara, item_id, 0))
                {
                    equip_item(chara, i, equipment.unwrap());
                }
                eqglove = 0;
                continue;
            }
            continue;
        }
        if (body_part_id == "core.leg")
        {
            if (eqboots)
            {
                int item_id;
                if (eqboots >= 10000)
                {
                    flt(calcobjlv(chara.level),
                        calcfixlv(static_cast<Quality>(fixeq + eqboots(1))));
                    flttypeminor = eqboots;
                    item_id = -1;
                }
                else
                {
                    flt();
                    item_id = eqboots;
                }
                if (const auto equipment =
                        itemcreate_chara_inv(chara, item_id, 0))
                {
                    equip_item(chara, i, equipment.unwrap());
                }
                eqboots = 0;
                continue;
            }
            continue;
        }
        if (body_part_id == "core.hand")
        {
            if (eqmultiweapon)
            {
                OptionalItemRef equipment;
                for (int cnt = 0; cnt < 15; ++cnt)
                {
                    flt(calcobjlv(chara.level),
                        calcfixlv(static_cast<Quality>(2 + fixeq)));
                    equipment = itemcreate_chara_inv(chara, eqmultiweapon, 0);
                    assert(equipment);
                    if (equipment->weight > 1500)
                    {
                        if (cnt < 14)
                        {
                            equipment->remove();
                            continue;
                        }
                    }
                    break;
                }
                if (equipment)
                {
                    equip_item(chara, i, equipment.unwrap());
                }
                eqweapon1 = 0;
                continue;
            }
            if (eqweapon1)
            {
                OptionalItemRef equipment;
                if (eqweapon1 >= 10000)
                {
                    for (int cnt = 0; cnt < 15; ++cnt)
                    {
                        flt(calcobjlv(chara.level),
                            calcfixlv(
                                static_cast<Quality>(fixeq + eqweapon1(1))));
                        flttypeminor = eqweapon1;
                        equipment = itemcreate_chara_inv(chara, -1, 0);
                        assert(equipment);
                        if (eqtwohand)
                        {
                            if (equipment->weight < 4000)
                            {
                                if (cnt < 14)
                                {
                                    equipment->remove();
                                    continue;
                                }
                            }
                        }
                        if (eqtwowield)
                        {
                            if (equipment->weight > 1500)
                            {
                                if (cnt < 14)
                                {
                                    equipment->remove();
                                    continue;
                                }
                            }
                        }
                        break;
                    }
                }
                else
                {
                    flt();
                    equipment = itemcreate_chara_inv(chara, eqweapon1, 0);
                }
                if (equipment)
                {
                    equip_item(chara, i, equipment.unwrap());
                }
                eqweapon1 = 0;
                continue;
            }
            if (eqweapon2)
            {
                OptionalItemRef equipment;
                if (eqweapon2 >= 10000)
                {
                    for (int cnt = 0; cnt < 15; ++cnt)
                    {
                        flt(calcobjlv(chara.level),
                            calcfixlv(
                                static_cast<Quality>(fixeq + eqweapon2(1))));
                        flttypeminor = eqweapon2;
                        equipment = itemcreate_chara_inv(chara, -1, 0);
                        assert(equipment);
                        if (eqtwowield)
                        {
                            if (equipment->weight > 1500)
                            {
                                if (cnt < 14)
                                {
                                    equipment->remove();
                                    continue;
                                }
                            }
                        }
                        break;
                    }
                }
                else
                {
                    flt();
                    equipment = itemcreate_chara_inv(chara, eqweapon2, 0);
                }
                if (equipment)
                {
                    equip_item(chara, i, equipment.unwrap());
                }
                eqweapon2 = 0;
                continue;
            }
            if (eqshield)
            {
                int item_id;
                if (eqshield >= 10000)
                {
                    flt(calcobjlv(chara.level),
                        calcfixlv(static_cast<Quality>(fixeq + eqshield(1))));
                    flttypeminor = eqshield;
                    item_id = -1;
                }
                else
                {
                    flt();
                    item_id = eqshield;
                }
                if (const auto equipment =
                        itemcreate_chara_inv(chara, item_id, 0))
                {
                    equip_item(chara, i, equipment.unwrap());
                }
                eqshield = 0;
                continue;
            }
            continue;
        }
        if (body_part_id == "core.shoot")
        {
            if (eqrange)
            {
                int item_id;
                if (eqrange >= 10000)
                {
                    flt(calcobjlv(chara.level),
                        calcfixlv(static_cast<Quality>(fixeq + eqrange(1))));
                    flttypeminor = eqrange;
                    item_id = -1;
                }
                else
                {
                    flt();
                    item_id = eqrange;
                }
                if (const auto equipment =
                        itemcreate_chara_inv(chara, item_id, 0))
                {
                    equip_item(chara, i, equipment.unwrap());
                }
                eqrange = 0;
                continue;
            }
            continue;
        }
        if (body_part_id == "core.ammo")
        {
            if (eqammo)
            {
                int item_id;
                if (eqammo >= 10000)
                {
                    flt(calcobjlv(chara.level),
                        calcfixlv(static_cast<Quality>(fixeq + eqammo(1))));
                    flttypeminor = eqammo;
                    item_id = -1;
                }
                else
                {
                    flt();
                    item_id = eqammo;
                }
                if (const auto equipment =
                        itemcreate_chara_inv(chara, item_id, 0))
                {
                    equip_item(chara, i, equipment.unwrap());
                }
                eqammo = 0;
                continue;
            }
            continue;
        }
    }
    if (chara.id == "core.bard")
    {
        if (rnd(150) == 0)
        {
            flt();
            itemcreate_chara_inv(chara, 707, 0);
        }
        else
        {
            flt(calcobjlv(chara.level), calcfixlv());
            flttypeminor = 60005;
            itemcreate_chara_inv(chara, 0, 0);
        }
    }
    if (chara.id == "core.the_leopard_warrior" || chara.id == "core.silvia")
    {
        for (int cnt = 0; cnt < 6; ++cnt)
        {
            flt();
            nostack = 1;
            if (const auto item = itemcreate_chara_inv(chara, 772, 0))
            {
                item->modify_number(rnd(4));
                if (rnd(2))
                {
                    food_make_rotten(item.unwrap());
                    item->image = 336;
                }
            }
        }
    }
    eqtwohand = 0;
    eqtwowield = 0;
    eqmultiweapon = 0;
}



void equip_item(Character& chara, size_t body_part_index, ItemRef equipment)
{
    auto& body_part = chara.body_parts[body_part_index];
    if (body_part.is_equip())
    {
        return;
    }
    if (equipment->is_equipped())
    {
        return;
    }
    item_separate(equipment);
    if (chara.is_player())
    {
        item_identify(equipment, IdentifyState::almost);
    }
    equipment->_equipped_slot = lua_index::from_0_based(body_part_index);
    body_part.equip(equipment);
}



void unequip_item(Character& chara, size_t body_part_index)
{
    auto& body_part = chara.body_parts[body_part_index];
    if (!body_part.is_equip())
    {
        return;
    }
    body_part.equipment()->_equipped_slot = lua_index::nil();
    inv_stack(chara.inventory(), body_part.equipment().unwrap());
    body_part.unequip();
}

} // namespace elona
