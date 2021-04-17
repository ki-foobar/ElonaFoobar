local I18N = require("core.I18N")

I18N.add {
   keybind = {
      menu = {
         hint = "Enter [Bind Key] ",

         topics = {
            name = "Name",
            primary = "Primary",
            alternate = "Alternate",
            joystick = "Joystick",
         },

         conflict = {
            text = "The following keybindings are conflicting:",
            prompt = "Press Enter to unbind them, Escape to cancel.",
         },

         prompt = [=[
Please press a key.
Enter to unbind, Escape to cancel.
]=],
      },

      category = {
         default = "Default",
         shortcut = "Shortcut",
         selection = "Selection",
         menu = "Menu",
         game = "Game",
         wizard = "Wizard",
      },

      escape = "Escape",
      cancel = "Cancel",
      enter = "Enter",
      north = "North",
      south = "South",
      east = "East",
      west = "West",
      northwest = "Northwest",
      northeast = "Northeast",
      southwest = "Southwest",
      southeast = "Southeast",

      -- NOTE: has special name formatting behavior
      shortcut = "Shortcut ",

      -- NOTE: has special name formatting behavior
      select = "Select ",

      next_page = "Next Page",
      previous_page = "Previous Page",
      next_menu = "Next Menu",
      previous_menu = "Previous Menu",
      switch_mode = "Switch Mode",
      switch_mode_2 = "Switch Mode (2)",
      identify = "See Details",
      portrait = "Portrait Menu",

      wait = "Wait",
      quick_menu = "Quick Menu",
      zap = "Zap",
      inventory = "Inventory",
      quick_inventory = "Quick Inventory",
      get = "Get",
      drop = "Drop",
      chara_info = "Character Sheet",
      eat = "Eat",
      wear = "Wear",
      cast = "Cast",
      drink = "Drink",
      read = "Read",
      fire = "Fire",
      go_down = "Go Down",
      go_up = "Go Up",
      save = "Save",
      search = "Search",
      interact = "Interact",
      skill = "Skill",
      close = "Close",
      rest = "Rest",
      target = "Target",
      dig = "Dig",
      use = "Use",
      bash = "Bash",
      open = "Open",
      dip = "Dip",
      pray = "Pray",
      offer = "Offer",
      journal = "Journal",
      material = "Material",
      trait = "Trait",
      look = "Look",
      give = "Give",
      throw = "Throw",
      ammo = "Ammo",
      autodig = "Autodig",
      quicksave = "Quicksave",
      quickload = "Quickload",
      help = "Help",
      message_log = "Message Log",
      chat_box = "Chat Box",
      tcg = "Start TCG",
      update_screen = "Update Screen",
      dump_player_info = "Dump Player Info",
      reload_autopick = "Reload Autopick",
      screenshot = "Take Screenshot",
      open_console = "Open Console",

      wizard_mewmewmew = "Mewmewmew!",
      wizard_wish = "Wish",
      wizard_advance_time = "Advance Time",
      wizard_delete_map = "Delete Map",
   },
}
