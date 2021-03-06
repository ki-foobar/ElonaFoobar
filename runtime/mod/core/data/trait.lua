local Data = require("core.Data")

Data.define_prototype("trait")
Data.add(
   "core.trait",
   {
      stamina_feat = {
         integer_id = 24,
         trait_type = "feat",
         min = 0,
         max = 3,
      },
      vimpire = {
         integer_id = 3,
         trait_type = "feat",
         min = 0,
         max = 1,
      },
      leader = {
         integer_id = 40,
         trait_type = "feat",
         min = 0,
         max = 1,
      },
      dimensional_move = {
         integer_id = 13,
         trait_type = "feat",
         min = 0,
         max = 1,
      },
      fire_breath = {
         integer_id = 14,
         trait_type = "feat",
         min = 0,
         max = 1,
      },
      hypnotism = {
         integer_id = 22,
         trait_type = "feat",
         min = 0,
         max = 1,
      },
      poison_nail = {
         integer_id = 23,
         trait_type = "feat",
         min = 0,
         max = 1,
      },
      charisma_feat = {
         integer_id = 21,
         trait_type = "feat",
         min = 0,
         max = 2,
      },
      strength_feat = {
         integer_id = 5,
         trait_type = "feat",
         min = 0,
         max = 3,
      },
      accountant = {
         integer_id = 38,
         trait_type = "feat",
         min = 0,
         max = 2,
      },
      quartermaster = {
         integer_id = 39,
         trait_type = "feat",
         min = 0,
         max = 2,
      },
      exorcist = {
         integer_id = 42,
         trait_type = "feat",
         min = 0,
         max = 1,
      },
      constitution_feat = {
         integer_id = 9,
         trait_type = "feat",
         min = 0,
         max = 3,
      },
      martial_arts_feat = {
         integer_id = 20,
         trait_type = "feat",
         min = 0,
         max = 2,
      },
      casting_feat = {
         integer_id = 12,
         trait_type = "feat",
         min = 0,
         max = 2,
      },
      shield_bash = {
         integer_id = 43,
         trait_type = "feat",
         min = 0,
         max = 1,
      },
      gentle_face = {
         integer_id = 44,
         trait_type = "feat",
         min = 0,
         max = 1,
      },
      dual_wield_feat = {
         integer_id = 19,
         trait_type = "feat",
         min = 0,
         max = 2,
      },
      darkness_resistance_feat = {
         integer_id = 15,
         trait_type = "feat",
         min = 0,
         max = 2,
      },
      poison_resistance_feat = {
         integer_id = 18,
         trait_type = "feat",
         min = 0,
         max = 2,
      },
      negotiation_feat = {
         integer_id = 16,
         trait_type = "feat",
         min = 0,
         max = 2,
      },
      faith_feat = {
         integer_id = 17,
         trait_type = "feat",
         min = 0,
         max = 2,
      },
      luck_feat = {
         integer_id = 1,
         trait_type = "feat",
         min = 0,
         max = 3,
      },
      life_feat = {
         integer_id = 2,
         trait_type = "feat",
         min = 0,
         max = 5,
      },
      mana_feat = {
         integer_id = 11,
         trait_type = "feat",
         min = 0,
         max = 5,
      },
      detection_feat = {
         integer_id = 6,
         trait_type = "feat",
         min = 0,
         max = 2,
      },
      speed_feat = {
         integer_id = 4,
         trait_type = "feat",
         min = 0,
         max = 2,
      },
      pv_feat = {
         integer_id = 7,
         trait_type = "feat",
         min = 0,
         max = 3,
      },
      dv_feat = {
         integer_id = 8,
         trait_type = "feat",
         min = 0,
         max = 3,
      },
      evade_feat = {
         integer_id = 10,
         trait_type = "feat",
         min = 0,
         max = 3,
      },
      cannibalism = {
         integer_id = 41,
         trait_type = "mutation",
         min = 0,
         max = 1,
      },
      pv_mutation = {
         integer_id = 25,
         trait_type = "mutation",
         min = -3,
         max = 3,
      },
      dexterity_mutation = {
         integer_id = 26,
         trait_type = "mutation",
         min = -3,
         max = 3,
      },
      healing_mutation = {
         integer_id = 27,
         trait_type = "mutation",
         min = -2,
         max = 2,
      },
      speed_mutation = {
         integer_id = 28,
         trait_type = "mutation",
         min = -3,
         max = 3,
      },
      strength_mutation = {
         integer_id = 29,
         trait_type = "mutation",
         min = -3,
         max = 3,
      },
      charisma_mutation = {
         integer_id = 30,
         trait_type = "mutation",
         min = -2,
         max = 2,
      },
      memorization_mutation = {
         integer_id = 31,
         trait_type = "mutation",
         min = -2,
         max = 2,
      },
      magic_resistance_mutation = {
         integer_id = 32,
         trait_type = "mutation",
         min = -1,
         max = 1,
      },
      sound_resistance_mutation = {
         integer_id = 33,
         trait_type = "mutation",
         min = -1,
         max = 1,
      },
      fire_resistance_mutation = {
         integer_id = 34,
         trait_type = "mutation",
         min = -1,
         max = 1,
      },
      cold_resistance_mutation = {
         integer_id = 35,
         trait_type = "mutation",
         min = -1,
         max = 1,
      },
      lightning_resistance_mutation = {
         integer_id = 36,
         trait_type = "mutation",
         min = -1,
         max = 1,
      },
      perception_mutation = {
         integer_id = 37,
         trait_type = "mutation",
         min = -2,
         max = 2,
      },
      fire_resistance_nature = {
         integer_id = 150,
         trait_type = "nature",
         min = -2,
         max = 2,
      },
      cold_resistance_nature = {
         integer_id = 151,
         trait_type = "nature",
         min = -2,
         max = 2,
      },
      poison_resistance_nature = {
         integer_id = 152,
         trait_type = "nature",
         min = -2,
         max = 2,
      },
      darkness_resistance_nature = {
         integer_id = 155,
         trait_type = "nature",
         min = -2,
         max = 2,
      },
      less_mana_reaction = {
         integer_id = 156,
         trait_type = "nature",
         min = 0,
         max = 1,
      },
      outstanding_resistances = {
         integer_id = 160,
         trait_type = "nature",
         min = 0,
         max = 1,
      },
      cannot_wear_heavy_equipments = {
         integer_id = 161,
         trait_type = "nature",
         min = 0,
         max = 1,
      },
      evil_man = {
         integer_id = 162,
         trait_type = "nature",
         min = 0,
         max = 1,
      },
      good_man = {
         integer_id = 169,
         trait_type = "nature",
         min = 0,
         max = 1,
      },
      ehekatl_blessing = {
         integer_id = 163,
         trait_type = "nature",
         min = 0,
         max = 1,
      },
      opatos_blessing = {
         integer_id = 164,
         trait_type = "nature",
         min = 0,
         max = 1,
      },
      itzpalt_blessing = {
         integer_id = 165,
         trait_type = "nature",
         min = 0,
         max = 1,
      },
      jure_blessing = {
         integer_id = 166,
         trait_type = "nature",
         min = 0,
         max = 1,
      },
      so_cute = {
         integer_id = 167,
         trait_type = "nature",
         min = 0,
         max = 1,
      },
      will_not_dimmed = {
         integer_id = 157,
         trait_type = "nature",
         min = 0,
         max = 1,
      },
      slow_digestion = {
         integer_id = 158,
         trait_type = "nature",
         min = 0,
         max = 1,
      },
      more_materials = {
         integer_id = 159,
         trait_type = "nature",
         min = 0,
         max = 1,
      },
      more_bonus_points = {
         integer_id = 154,
         trait_type = "nature",
         min = 0,
         max = 1,
      },
      magic_resistance_nature = {
         integer_id = 153,
         trait_type = "nature",
         min = -2,
         max = 2,
      },
      changing_body = {
         integer_id = 0,
         trait_type = "nature",
         min = 0,
         max = 1,
      },
      slow_ether_disease_progress = {
         integer_id = 168,
         trait_type = "nature",
         min = 0,
         max = 1,
      },
      gravity = {
         integer_id = 201,
         trait_type = "ether_disease",
         min = -3,
         max = 0,
      },
      festered_face = {
         integer_id = 202,
         trait_type = "ether_disease",
         min = -3,
         max = 0,
      },
      hooves = {
         integer_id = 203,
         trait_type = "ether_disease",
         min = -1,
         max = 0,
      },
      four_eyes = {
         integer_id = 204,
         trait_type = "ether_disease",
         min = -1,
         max = 0,
      },
      feathers = {
         integer_id = 205,
         trait_type = "ether_disease",
         min = -1,
         max = 0,
      },
      thick_neck = {
         integer_id = 206,
         trait_type = "ether_disease",
         min = -1,
         max = 0,
      },
      desire_for_violence = {
         integer_id = 207,
         trait_type = "ether_disease",
         min = -1,
         max = 0,
      },
      huge_head = {
         integer_id = 208,
         trait_type = "ether_disease",
         min = -1,
         max = 0,
      },
      rainy_clouds = {
         integer_id = 209,
         trait_type = "ether_disease",
         min = -1,
         max = 0,
      },
      potion_addiction = {
         integer_id = 210,
         trait_type = "ether_disease",
         min = -1,
         max = 0,
      },
      debilitation = {
         integer_id = 211,
         trait_type = "ether_disease",
         min = -1,
         max = 0,
      },
      dementia = {
         integer_id = 212,
         trait_type = "ether_disease",
         min = -1,
         max = 0,
      },
      heavy_carapace = {
         integer_id = 213,
         trait_type = "ether_disease",
         min = -1,
         max = 0,
      },
      random_teleportation = {
         integer_id = 214,
         trait_type = "ether_disease",
         min = -1,
         max = 0,
      },
      mana_battery = {
         integer_id = 215,
         trait_type = "ether_disease",
         min = -1,
         max = 0,
      },
      poisonous_hand = {
         integer_id = 216,
         trait_type = "ether_disease",
         min = -1,
         max = 0,
      },
   }
)
