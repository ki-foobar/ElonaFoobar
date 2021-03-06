local Data = require("core.Data")

-- Contains all possible character parameters for testing.
Data.add(
   "core.chara",
   {
      spiral_putit = {
         integer_id = 9999,
         item_type = 2,
         tags = {"slime", "ether"},
         level = 2,
         portrait = "core.zeome",
         ai_calm = 2,
         ai_move = 100,
         ai_dist = 1,
         ai_act_sub_freq = 0,
         ai_heal = 5,
         creaturepack = 3,
         normal_actions = {417, 434, 415, 454},
         special_actions = {636},
         race = "core.slime",
         class_ = "core.wizard",
         resistances = {["core.mind"] = 500},
         image = 430,
         female_image = 431,
         male_image = 432,
         cspecialeq = 0,
         damage_reaction_info = 20000,
         element_of_unarmed_attack = 30000,
         fltselect = 0,
         fixlv = 10,
         has_random_name = false,
         category = 3,
         rarity = 100000,
         coefficient = 0,
         eqammo_0 = 1,
         eqammo_1 = 2,
         eqmultiweapon = 3,
         eqrange_0 = 4,
         eqrange_1 = 5,
         eqring1 = 6,
         eqtwohand = 7,
         eqweapon1 = 8,

         color = "blue",
         sex = "female",
         original_relationship = "aggressive",
         flags = {"is_suitable_for_mount", "splits2"},
      }
   }
)
