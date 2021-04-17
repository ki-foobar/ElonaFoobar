#include "ui_menu_charamake_race.hpp"

#include "../audio.hpp"
#include "../character_making.hpp"
#include "../draw.hpp"
#include "../i18n.hpp"
#include "../menu.hpp"
#include "../race.hpp"

namespace elona
{
namespace ui
{

static void _load_race_list()
{
    listmax = 0;
    for (const auto& race : race_get_available(false))
    {
        listn(1, listmax) = race.get().id.get();
        list(0, listmax) = 0;
        ++listmax;
    }
    if (g_config.extra_race())
    {
        for (const auto& race : race_get_available(true))
        {
            listn(1, listmax) = race.get().id.get();
            list(0, listmax) = 1;
            ++listmax;
        }
    }
    for (int cnt = 0, cnt_end = (listmax); cnt < cnt_end; ++cnt)
    {
        listn(0, cnt) =
            the_race_db.get_text(data::InstanceId{listn(1, cnt)}, "name");
        if (list(0, cnt) == 1)
        {
            listn(0, cnt) = "(extra)"s + listn(0, cnt);
        }
    }
}

bool UIMenuCharamakeRace::init()
{
    cs = 0;
    cs_bk = -1;
    pagesize = 16;
    page = 0;

    character_making_draw_background("core.chara_making.select_race.caption");

    windowshadow = 1;

    _load_race_list();

    return true;
}

void UIMenuCharamakeRace::update()
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
    if (listmax <= page * pagesize + cs)
    {
        cs = listmax % pagesize - 1;
    }
}



void UIMenuCharamakeRace::_draw_race_info(data::InstanceId race_id)
{
    const auto& race_data = the_race_db.ensure(race_id);

    // male
    draw_chara(race_data.male_image, wx + 480, wy + 96, 2, 40);
    // female
    draw_chara(race_data.female_image, wx + 350, wy + 96, 2, 40);

    gmode(2);
    draw_race_or_class_info(
        the_race_db.get_text_optional(race_id, "description").value_or(""));
}



void UIMenuCharamakeRace::_draw_window()
{
    ui_display_window(
        i18n::s.get("core.chara_making.select_race.title"),
        strhint3b,
        (windoww - 680) / 2 + inf_screenx,
        winposy(500, 1) + 20,
        680,
        500);
    ++cmbg;
    x = ww / 5 * 2;
    y = wh - 80;
    gmode(2, 50);
    gcopy_c(
        2,
        cmbg / 4 % 4 * 180,
        cmbg / 4 / 4 % 2 * 300,
        180,
        300,
        wx + ww / 4,
        wy + wh / 2,
        x,
        y);
    gmode(2);
    display_topic(
        i18n::s.get("core.chara_making.select_race.race"), wx + 28, wy + 30);
    display_topic(
        i18n::s.get("core.chara_making.select_race.detail"), wx + 188, wy + 30);
}

void UIMenuCharamakeRace::_draw_choice(int cnt, const std::string& text)
{
    display_key(wx + 38, wy + 66 + cnt * 19 - 2, cnt);
    cs_list(cs == cnt, text, wx + 64, wy + 66 + cnt * 19 - 1);
}



void UIMenuCharamakeRace::_draw_choices()
{
    font(14 - en * 2);
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        int index = page * pagesize + cnt;
        if (index >= listmax)
        {
            break;
        }
        key_list(cnt) = key_select(cnt);
        keyrange = cnt + 1;

        const std::string& text = listn(0, index);
        _draw_choice(cnt, text);
    }
    cs_bk = cs;
}



static void _reload_selected_race(data::InstanceId race_id)
{
    chara_delete(cdata.player());
    race_init_chara(cdata.player(), race_id);
}



void UIMenuCharamakeRace::draw()
{
    if (cs == cs_bk)
    {
        return;
    }

    _draw_window();
    _draw_choices();

    const auto selected_race = data::InstanceId{listn(1, page * pagesize + cs)};
    _reload_selected_race(selected_race);

    _draw_race_info(selected_race);
}



optional<UIMenuCharamakeRace::ResultType> UIMenuCharamakeRace::on_key(
    const std::string& action)
{
    if (auto race_index = get_selected_index())
    {
        return UIMenuCharamakeRace::Result::finish(
            UIMenuCharamakeRaceResult{data::InstanceId{listn(1, *race_index)}});
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
        return UIMenuCharamakeRace::Result::cancel();
    }
    else if (getkey(snail::Key::f1))
    {
        show_game_help();
        return UIMenuCharamakeRace::Result::finish();
    }

    return none;
}

} // namespace ui
} // namespace elona
