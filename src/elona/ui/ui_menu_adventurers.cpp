#include "ui_menu_adventurers.hpp"

#include "../audio.hpp"
#include "../character.hpp"
#include "../draw.hpp"
#include "../i18n.hpp"
#include "../text.hpp"

namespace elona
{
namespace ui
{

bool UIMenuAdventurers::init()
{
    listmax = 0;
    page = 0;
    pagesize = 16;
    cs = 0;
    cs_bk = -1;
    for (int cnt = 0; cnt < 56; ++cnt)
    {
        if (cdata[cnt].state() == Character::State::empty)
        {
            continue;
        }
        list(0, listmax) = cnt;
        list(1, listmax) = -cdata[cnt].fame;
        ++listmax;
    }
    sort_list_by_column1();
    windowshadow = 1;

    return true;
}

void UIMenuAdventurers::update()
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

void UIMenuAdventurers::_draw_window()
{
    ui_display_window(
        i18n::s.get("core.ui.adventurers.title"),
        strhint2 + strhint3,
        (windoww - 640) / 2 + inf_screenx,
        winposy(448),
        640,
        448);
    display_topic(
        i18n::s.get("core.ui.adventurers.name_and_rank"), wx + 28, wy + 36);
    display_topic(
        i18n::s.get("core.ui.adventurers.fame_lv"), wx + 320, wy + 36);
    display_topic(
        i18n::s.get("core.ui.adventurers.location"), wx + 420, wy + 36);
}

void UIMenuAdventurers::_draw_key(int cnt)
{
    if (cnt % 2 == 0)
    {
        boxf(wx + 70, wy + 66 + cnt * 19, 540, 18, {12, 14, 16, 16});
    }
    display_key(wx + 58, wy + 66 + cnt * 19 - 2, cnt);
}

void UIMenuAdventurers::_draw_keys()
{
    keyrange = 0;
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        key_list(cnt) = key_select(cnt);
        ++keyrange;
        _draw_key(cnt);
    }
}

static void
_draw_list_entry_pic_and_rank(int cnt, const Character& chara, int _p)
{
    draw_chara_scale_height(chara, wx + 40, wy + 74 + cnt * 19 - 8);

    mes(wx + 84,
        wy + 66 + cnt * 19 + 2,
        cnvrank(_p + 1) + i18n::s.get("core.ui.adventurers.rank_counter"));
}

void UIMenuAdventurers::_draw_list_entry_name(int cnt, const Character& chara)
{
    const auto name =
        strutil::take_by_width(chara.alias + " "s + chara.name, 26);
    cs_list(cs == cnt, name, wx + 118, wy + 66 + cnt * 19 - 1);
}

void UIMenuAdventurers::_draw_list_entry_level(int cnt, const Character& chara)
{
    std::string level = ""s + chara.fame + "("s + chara.level + ")"s;
    mes(wx + 402 - strlen_u(level) * 7, wy + 66 + cnt * 19 + 2, level);
}

void UIMenuAdventurers::_draw_list_entry_map_name(
    int cnt,
    const Character& chara)
{
    std::string map_name = mapname(chara.current_map);

    if (map_name == ""s)
    {
        map_name = i18n::s.get("core.ui.adventurers.unknown");
    }
    if (chara.state() == Character::State::adventurer_dead)
    {
        map_name = i18n::s.get("core.ui.adventurers.hospital");
    }

    mes(wx + 435, wy + 66 + cnt * 19 + 2, map_name);
}

void UIMenuAdventurers::_draw_list_entry(
    int cnt,
    const Character& chara,
    int _p)
{
    _draw_list_entry_pic_and_rank(cnt, chara, _p);
    _draw_list_entry_name(cnt, chara);
    _draw_list_entry_level(cnt, chara);
    _draw_list_entry_map_name(cnt, chara);
}

void UIMenuAdventurers::_draw_list_entries()
{
    font(14 - en * 2);
    cs_listbk();
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }

        int chara_index = list(0, p);
        const Character& chara = cdata[chara_index];

        _draw_list_entry(cnt, chara, p(0));
    }
    if (keyrange != 0)
    {
        cs_bk = cs;
    }
}

void UIMenuAdventurers::draw()
{
    _draw_window();
    _draw_keys();
    _draw_list_entries();
}

optional<UIMenuAdventurers::ResultType> UIMenuAdventurers::on_key(
    const std::string& action)
{
    if (action == "next_page")
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
        return UIMenuAdventurers::Result::finish();
    }

    return none;
}
} // namespace ui
} // namespace elona
