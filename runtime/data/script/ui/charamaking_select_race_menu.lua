local prelude = _ENV.prelude
local class = prelude.class

local audio = require("audio")
local config = require("config")
local graphics = require("graphics")
local i18n = require("i18n")
local input = require("input")
local race = require("race")
local ui = require("ui")
local Cursor = ui.Cursor
local ListView = require("ui.list_view")

local CharamakingSelectRaceMenu = class("core.ui.CharamakingSelectRaceMenu")

local PREV_CURSOR = nil

local CharamakingSelectRaceListView = class("core.ui.CharamakingSelectRaceListView", ListView)

function CharamakingSelectRaceListView:draw_item(index, item)
   --[[
   local is_selected = self.cursor.position == index
   local x = 40
   local y = (index - 1) * 35 + 50

   ui.selection_key(input.get_nth_selection_key(index), x, y)
   if i18n.language() == "en" then
      graphics.set_font(14)
      ui.list_item(is_selected, item.text, x + 40, y + 1)
   else
      local x_offset = ui.compat.wx()
      local y_offset = ui.compat.wy()
      graphics.set_font(11)
      graphics.set_draw_color(0, 0, 0)
      graphics.draw_text(item.text_en, x + 40 + x_offset, y - 4 + y_offset)
      graphics.set_font(13)
      ui.list_item(is_selected, item.text, x + 40, y + 8)
   end
   --]]
end

function CharamakingSelectRaceListView:on_item_selected(selected_item)
   --[[
   -- TODO
   -- audio.play_sound("core.ok1")
   selected_item.on_selected()
   --]]
end

--- @tparam Gene? gene
function CharamakingSelectRaceMenu:__init(gene)
   if gene then
      todo()
   end

   -- TODO:
   -- g_mode = 1
   -- cm = 1

   self._menu = CharamakingSelectRaceListView.new(self:_build_race_list())
end

function CharamakingSelectRaceMenu:on_shown()
   self._races.cursor = PREV_CURSOR or Cursor.new(1, 1)
end

function CharamakingSelectRaceMenu:on_closed()
   PREV_CURSOR = self._menu.cursor
end

function CharamakingSelectRaceMenu:on_key_down(event)
   self._menu:on_key_down(event)
end

function CharamakingSelectRaceMenu:update()
   self._menu:update()
end

function CharamakingSelectRaceMenu:draw()
   graphics.draw_image("core.void", 0, 0, graphics.screen_width(), graphics.screen_height())

   -- TODO
   -- asset_load("core.deco_cm")

   -- TODO
   -- character_making_draw_background("core.chara_making.select_race.caption")

   self:_draw_window()
   self:_draw_choices()

   -- TODO
   --[[
   const auto selected_race = data::InstanceId{listn(1, page * pagesize + cs)}
   self:_reload_selected_race(selected_race)

   self:_draw_race_info(selected_race)
   --]]
end

function CharamakingSelectRaceMenu:_build_race_list()
   local races = {}
   -- Primary races
   for _, id, race in race.primary_races() do
      races[#races + 1] = {
         id = id,
         name = i18n.get_data_text("core.race", id, "name"),
         data = race,
      }
   end
   -- Extra races
   if config.get("core.balance.extra_race") then
      for _, id, _ in race.extra_races() do
         races[#races + 1] = {
            id = id,
            name = i18n.get_data_text("core.race", id, "name"),
            data = race,
         }
      end
   end
   return races
end

function CharamakingSelectRaceMenu:_draw_race_info(race_id)
   --[[
    const auto& race_data = the_race_db.ensure(race_id)

    // male
    draw_chara(race_data.male_image, wx + 480, wy + 96, 2, 40)
    // female
    draw_chara(race_data.female_image, wx + 350, wy + 96, 2, 40)

    gmode(2)
    draw_race_or_class_info(
        the_race_db.get_text_optional(race_id, "description").value_or(""))
   --]]
end

function CharamakingSelectRaceMenu:_draw_window()
        --[[
    ui_display_window(
        i18n::s.get("core.chara_making.select_race.title"),
        strhint3b,
        (windoww - 680) / 2 + inf_screenx,
        winposy(500, 1) + 20,
        680,
        500)
    ++cmbg
    x = ww / 5 * 2
    y = wh - 80
    gmode(2, 50)
    gcopy_c(
        2,
        cmbg / 4 % 4 * 180,
        cmbg / 4 / 4 % 2 * 300,
        180,
        300,
        wx + ww / 4,
        wy + wh / 2,
        x,
        y)
    gmode(2)
    display_topic(
        i18n::s.get("core.chara_making.select_race.race"), wx + 28, wy + 30)
    display_topic(
        i18n::s.get("core.chara_making.select_race.detail"), wx + 188, wy + 30)
        --]]
end

function CharamakingSelectRaceMenu:_draw_choice(cnt, text)
    -- display_key(wx + 38, wy + 66 + cnt * 19 - 2, cnt)
    -- cs_list(cs == cnt, text, wx + 64, wy + 66 + cnt * 19 - 1)
 end

function CharamakingSelectRaceMenu:_draw_choices()
   --[[
    font(14 - en * 2)
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        int index = page * pagesize + cnt
        if (index >= listmax)
        {
            break
        }
        key_list(cnt) = key_select(cnt)
        keyrange = cnt + 1

        const std::string& text = listn(0, index)
        _draw_choice(cnt, text)
    }
    cs_bk = cs
   --]]
end

function CharamakingSelectRaceMenu:_reload_selected_race(race_id)
   --[[
    chara_delete(cdata.player())
    race_init_chara(cdata.player(), race_id)
   --]]
end

function CharamakingSelectRaceMenu:on_key(action)
   --[[
    if (auto race_index = get_selected_index())
    {
        return self:Result::finish(
            UIMenuCharamakeRaceResult{data::InstanceId{listn(1, *race_index)}})
    }
    else if (action == "next_page")
    {
        if (pagemax != 0)
        {
            snd("core.pop1")
            ++page
            set_reupdate()
        }
    }
    else if (action == "previous_page")
    {
        if (pagemax != 0)
        {
            snd("core.pop1")
            --page
            set_reupdate()
        }
    }
    else if (action == "cancel")
    {
        return self:Result::cancel()
    }
    else if (getkey(snail::Key::f1))
    {
        show_game_help()
        return self:Result::finish()
    }

    return none
   --]]
end

return CharamakingSelectRaceMenu
