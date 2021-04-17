#include "ui_menu_equipment.hpp"

#include "../character.hpp"
#include "../equipment.hpp"
#include "../game.hpp"
#include "../globals.hpp"
#include "../item.hpp"
#include "../lua_env/data_manager.hpp"
#include "../menu.hpp"
#include "../message.hpp"



namespace elona
{
namespace ui
{

static int _load_equipment_list(const Character& chara)
{
    int mainhand = 0;

    for (size_t i = 0; i < chara.body_parts.size(); ++i)
    {
        if (!chara.body_parts[i].is_unequippable())
        {
            if (mainhand == 0)
            {
                if (chara.body_parts[i].id == "core.hand")
                {
                    mainhand = i + 1;
                }
            }
            list(0, listmax) = i + 1;
            list(1, listmax) =
                lua::lua->get_data_manager()
                    .get()
                    .raw("core.body_part", chara.body_parts[i].id)
                    ->get<lua_int>("integer_id");
            ++listmax;
        }
    }
    sort_list_by_column1();

    return mainhand;
}

bool UIMenuEquipment::init()
{
    page = 0;
    pagesize = 14;
    listmax = 0;
    cs = 0;
    cs_bk = -1;
    if (_cs_prev != 0)
    {
        cs = _cs_prev;
        _cs_prev = 0;
    }

    ww = 690;
    wh = 380;
    wx = (windoww - 690) / 2 + inf_screenx;
    wy = winposy(380);

    // TODO refactor
    if (nowindowanime == 0)
    {
        snd("core.wear");
    }

    window_animation(wx, wy, ww, wh, 9, 4);
    asset_load("core.deco_wear");
    gsel(0);
    windowshadow = 1;

    _mainhand = _load_equipment_list(cdata.player());

    return true;
}

void UIMenuEquipment::update()
{
    cs_bk = -1;
    pagemax = (listmax - 1) / pagesize;
    if (page < 0)
    {
        page = pagemax;
    }
    else if (page > pagemax)
    {
        page = 0;
    }
}

void UIMenuEquipment::_draw_window_background()
{
    ui_display_window(
        i18n::s.get("core.ui.equip.title"),
        strhint5 + strhint5b + strhint3,
        (windoww - 690) / 2 + inf_screenx,
        winposy(428),
        690,
        428,
        64);
    display_topic(
        i18n::s.get("core.ui.equip.category") + "/" +
            i18n::s.get("core.ui.equip.name"),
        wx + 28,
        wy + 30);
}

void UIMenuEquipment::_draw_window_deco(bool show_additional_info)
{
    if (!show_additional_info)
    {
        display_topic(i18n::s.get("core.ui.equip.weight"), wx + 524, wy + 30);
    }
    draw_indexed("core.inventory_icon", wx + 46, wy - 16, 10);
    elona::draw("core.deco_wear_a", wx + ww - 106, wy);
    elona::draw("core.deco_wear_b", wx, wy + wh - 164);
    draw_additional_item_info_label(wx + 350, wy + 40);
}

void UIMenuEquipment::_draw_window_headers()
{
    display_note(
        i18n::s.get("core.ui.equip.equip_weight") + ": " +
        cnvweight(cdata.player().equipment_weight) +
        get_armor_class_name(cdata.player()) + " " +
        i18n::s.get("core.ui.equip.hit_bonus") + ":" +
        cdata.player().hit_bonus + " " +
        i18n::s.get("core.ui.equip.damage_bonus") + ":" +
        cdata.player().damage_bonus + "  DV/PV:"s + cdata.player().dv + "/"s +
        cdata.player().pv);
}

void UIMenuEquipment::_draw_window(bool show_additional_info)
{
    _draw_window_background();
    _draw_window_deco(show_additional_info);
    _draw_window_headers();
}

void UIMenuEquipment::_draw_key(int cnt, int p_, bool is_main_hand)
{
    if (cnt % 2 == 0)
    {
        boxf(wx + 100, wy + 60 + cnt * 19, 558, 18, {12, 14, 16, 16});
    }

    std::string body_part_name;
    if (is_main_hand)
    {
        body_part_name = i18n::s.get("core.ui.equip.main_hand");
    }
    else
    {
        body_part_name = i18n::s.get_data_text(
            "core.body_part",
            *lua::lua->get_data_manager().get().by_integer(
                "core.body_part", list(1, p_)),
            "name");
    }

    draw_indexed(
        "core.body_part_icon",
        wx + 22,
        wy + 60 + cnt * 19 - 4,
        list(1, p_) - 1);
    mes(wx + 46, wy + 60 + cnt * 19 + 3, body_part_name);
}

void UIMenuEquipment::_draw_keys(int main_hand)
{
    font(12 + sizefix - en * 2, snail::Font::Style::bold);
    gmode(2);
    keyrange = 0;
    f = 0;
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        int index = pagesize * page + cnt;
        if (index >= listmax)
        {
            break;
        }
        key_list(cnt) = key_select(cnt);
        ++keyrange;
        bool is_main_hand = list(0, index) == main_hand;
        _draw_key(cnt, index, is_main_hand);
    }
}

static void
_draw_single_list_entry(int cnt, int list_item, bool show_additional_info)
{
    display_key(wx + 88, wy + 60 + cnt * 19 - 2, cnt);

    const auto equipment = cdata.player().body_parts[list_item - 1].equipment();
    std::string item_name = "-    "s;
    std::string item_weight = "-"s;

    if (equipment)
    {
        item_name = itemname(equipment.unwrap());
        item_weight = cnvweight(equipment->weight);

        draw_item_with_portrait(
            equipment.unwrap(), wx + 126, wy + 70 + cnt * 19);

        draw_additional_item_info(
            equipment.unwrap(), wx + 350, wy + 60 + cnt * 19 + 2);
        if (show_additional_info)
        {
            item_name = cut_item_name_for_additional_info(item_name, 2);
        }
    }

    const auto text_color = equipment
        ? cs_list_get_item_color(equipment.unwrap())
        : snail::Color{10, 10, 10};
    cs_list(
        cs == cnt,
        item_name,
        wx + 140 - 26,
        wy + 60 + cnt * 19 - 1,
        30,
        text_color);

    mes(wx + 640 - strlen_u(item_weight) * 7,
        wy + 60 + cnt * 19 + 2,
        item_weight,
        text_color);
}

void UIMenuEquipment::_draw_list_entries(bool show_additional_info)
{
    font(14 - en * 2);
    cs_listbk();
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        int index = pagesize * page + cnt;
        if (index >= listmax)
        {
            break;
        }

        int list_item = list(0, index);
        _draw_single_list_entry(cnt, list_item, show_additional_info);
    }
    cs_bk = cs;
}

void UIMenuEquipment::draw()
{
    _draw_window(g_show_additional_item_info != AdditionalItemInfo::none);
    _draw_keys(_mainhand);
    _draw_list_entries(g_show_additional_item_info != AdditionalItemInfo::none);
}



static void _unequip_item(int index)
{
    g_player_is_changing_equipment = true;
    const auto equipment =
        cdata.player().body_parts[index - 1].equipment().unwrap();
    if (is_cursed(equipment->curse_state))
    {
        txt(i18n::s.get("core.ui.equip.cannot_be_taken_off", equipment));
        return;
    }
    unequip_item(cdata.player(), index - 1);
    chara_refresh(cdata.player());
    snd("core.equip1");
    Message::instance().linebreak();
    txt(i18n::s.get("core.ui.equip.you_unequip", equipment));
    if (cdata.player().body_parts[index - 1].id == "core.hand")
    {
        equip_melee_weapon(cdata.player());
    }
}



static void _equip_item(int index)
{
    // Push equipment selection screen onto call stack and pop it off
    // after.
    nowindowanime = 1;
    menucycle = 0;
    invctrl = 6;
    snd("core.inv");
    CtrlInventoryOptions opts;
    opts.body_part_index = lua_index::from_0_based(index);
    ctrl_inventory(opts);
}

static bool _on_list_entry_select(int index)
{
    if (cdata.player().body_parts[index - 1].is_equip())
    {
        _unequip_item(index);
        render_hud();
        return false;
    }

    _equip_item(index);
    return true;
}

static void _show_item_desc(int body_)
{
    item_show_description(
        cdata.player().body_parts[body_ - 1].equipment().unwrap());
    nowindowanime = 1;
    returnfromidentify = 0;
    screenupdate = -1;
    update_screen();
}

optional<UIMenuEquipment::ResultType> UIMenuEquipment::on_key(
    const std::string& action)
{
    if (auto id = get_selected_item())
    {
        _cs_prev = cs;

        bool equipped = _on_list_entry_select(*id);

        if (equipped)
        {
            // Reinitialize "list" after returning from ctrl_inventory().
            set_reinit();
        }
        else
        {
            set_reupdate();
        }
        return none;
    }
    else if (action == "identify")
    {
        int body_ = list(0, pagesize * page + cs);
        if (cdata.player().body_parts[body_ - 1].is_equip())
        {
            _cs_prev = cs;
            _show_item_desc(body_);

            // Modifies "list", so run init() again in-place.
            set_reinit();
            return none;
        }
    }
    else if (action == "switch_mode")
    {
        g_show_additional_item_info =
            get_next_enum(g_show_additional_item_info);
        snd("core.pop1");
        set_reupdate();
    }
    else if (action == "next_page")
    {
        if (pagemax != 0)
        {
            snd("core.pop1");
            ++page;
            set_reupdate();
        }
    }
    else if (action == "previous_page")
    {
        if (pagemax != 0)
        {
            snd("core.pop1");
            --page;
            set_reupdate();
        }
    }
    else if (action == "cancel")
    {
        menucycle = 0;
        create_pcpic(cdata.player());
        update_screen();
        // result.turn_result = TurnResult::pc_turn_user_error
        return UIMenuEquipment::Result::cancel();
    }

    return none;
}

} // namespace ui
} // namespace elona
