local I18N = require("core.I18N")

-- NOTE: Will change drastically with the introduction of custom gods.

I18N.add {
   god = {
      eyth_name = "Eyth of Infidel",
      indifferent = " Your God becomes indifferent to your gift.",
      enraged = "{$1} is enraged.", -- TODO duplicate?

      switch = {
         unbeliever = "You are an unbeliever now.",
         follower = "You become a follower of {$1}!",
      },

      pray = {
         do_not_believe = "You don't believe in God.",
         prompt = "Really pray to your God?",
         you_pray_to = "You pray to {$1}.",
         indifferent = "{$1} is indifferent to you.",

         servant = {
            no_more = "No more than 2 God's servants are allowed in your party.",
            party_is_full = "Your party is full. The gift is reserved.",
            prompt_decline = "Do you want to decline this gift?",

            desc = {
               mani = "This android shows enormous strength when boosting.",
               lulwy = "This black angel shows enormous strength when boosting.",
               itzpalt = "This exile can cast several spells in a row.",
               ehekatl = "Weapons and armor licked by this cat receive a blessing of Ehekatl which adds an extra enchantment.",
               opatos = "This knight can hold really heavy stuff for you.",
               jure = "This defender can use Lay on hand to heal a mortally wounded ally. The ability becomes re-useable after sleeping.",
               kumiromi = "This fairy generates a seed after eating.",
            },
         },
      },

      desc = {
         window = {
            title = "< {$1} >",

            abandon = "Abandon God",
            convert = "Convert to {$1}",
            believe = "Believe in {$1}",
            cancel = "Cancel",
         },
      },
   },
}



I18N.add_data_text(
   "core.god",
   {
      mani = {
         name = "Mani of Machine",
         description = [=[Mani is a clockwork god of machinery. Those faithful to Mani<br>receive immense knowledge of machines and learn a way to use them<br>effectively.
Offering: Corpses/Guns/Machinery
   Bonus: DEX/PER/Firearm/Healing/Detection/Jeweler/Lockpick/Carpentry
 Ability: Mani's decomposition (Passive: Extract materials<br>from traps.)]=],
         random = {
            "\"You should mechanize your body.\"",
            "\"Always behave true to my name.\"",
            "\"No need to hurry. The day machines dominate the world is much closer than you think.\"",
         },
         kill = {
            "\"Nice kill.\"",
            "\"Don't you want to decompose it?\"",
            "\"Ah, this soul could be a good component for a new machine.\"",
         },
         night = "\"Flesh and blood, how pathetic to waste much of your limited life sleeping. But rest well for now, for you will need to serve me again.\"",
         welcome = "\"Ah, you've returned. You're tougher than I thought.\"",
         believe = "\"Oh, someone comes to me finally. Devote yourself to my tasks, you'll be rewarded greatly, maybe.\"",
         betray = "\"Great, a traitor.\"",
         take_over = "\"Well done. After all, you might be useful than I thought.\"",
         fail_to_take_over = "\"Heh, Nice try.\"",
         offer = "\"Cool. I like it.\"",
         receive_gift = "\"You've been a faithful servant of me. Here, use it wisely.\"",
         ready_to_receive_gift = "\"Truly, you are an ideal believer. I'm impressed.\"",
         ready_to_receive_gift2 = "\"Sacrifice your very soul to me and I shall protect you with my name.\"",
      },
      lulwy = {
         name = "Lulwy of Wind",
         description = [=[Lulwy is a goddess of wind. Those faithful to Lulwy receive<br>the blessing of wind and can move swiftly.
Offering: Corpses/Bows
   Bonus: PER/SPD/Bow/Crossbow/Stealth/Magic Device
 Ability: Lulwy's trick (Boost your speed for a short time.)]=],
         random = {
            "\"Pathetic pigs.\"",
            "\"Mani? Say that name again and I'll mince you, kitty.\"",
            "\"I've torn the former servant limb from limb to feed the sylphs. I just didn't like his hair style. Ahaha!\"",
            "\"My children are the voices of the wind, never tied to anything.\"",
         },
         kill = {
            "\"How dirty. Wipe the blood off your hands.\"",
            "\"Aha! Mince! Mince!.\"",
            "\"Bad kitty.\"",
         },
         night = "\"Fine, I'll unshackle you for a little while. Enjoy your sleep. But remember kitty, if you cheat on me in your dream, you'll never see a daylight again.\"",
         welcome = "\"Where were you until now? You need more breaking, it seems.\"",
         believe = "\"You know, you've made a right choice. I will treat you with great love, little kitty.\"",
         betray = "\"Huh, fool. A life without me is all but empty.\"",
         take_over = "\"Oh my little puppet, you serve me well.\"",
         fail_to_take_over = "\"Bad bad puppy. I need to punish you.\"",
         offer = "\"Oh, such a nice gift. Do you have a secret intention or something?\"",
         receive_gift = "\"My dear little kitty, you deserve a reward, don't you think?\"",
         ready_to_receive_gift = "\"Serve me to the end of this pathetic world, for you are my dearest slave.\"",
         ready_to_receive_gift2 = "\"Obey me and surrender everything. I shall mince all the pigs that try to hurt your beautiful face, my kitty.\"",
      },
      itzpalt = {
         name = "Itzpalt of Element",
         description = [=[Itzpalt is a god of elements. Those faithful to Itzpalt are<br>protected from elemental damages and learn to absorb mana from<br>their surroundings.
Offering: Corpses/Staves
   Bonus: MAG/Meditation/RES Fire/RES Cold/RES Lightning
 Ability: Absorb mana (Absorb mana from the air.)]=],
         -- Duplicate them to emulate the original Elona's behavior.
         random = {
            "\"Idleness is the devil's workshop.\"",
            "\"Idleness is the devil's workshop.\"",
            "\"You mortals will never understand the pain we hold.\"",
            "\"You mortals will never understand the pain we hold.\"",
            "\"My name is Itzpalt. I am the origin of elements, the king that bears the earliest flame and the master of all the Gods.\"",
            "\"My name is Itzpalt. I am the origin of elements, the king that bears the earliest flame and the master of all the Gods.\"",
            "\"The god's war never ends. In times to come, you shall be a warrior of my faction.\"",
            "\"The god's war never ends. In times to come, you shall be a warrior of my faction.\"",
            "\"The god's war never ends. In times to come, you shall be a warrior of my faction.\"",
         },
         kill = {
            "\"Impressive.\"",
            "\"And so the soul returns to the element.\"",
            "\"Chant my name proudly. Flame and eternal rest for the dead.\"",
         },
         night = "\"Relieve your fatigue. May the eternal flame protect you from filthy beings that crawl in the shroud of night.\"",
         welcome = "\"And so the mortal returns. You shall serve me again.\"",
         believe = "\"Do not fail me, servant.\"",
         betray = "\"Remember mortal, a betrayal is not something I can forgive.\"",
         take_over = "\"Impressive. Your action shall be remembered.\"",
         fail_to_take_over = "\"You shall pay a painful price for your foolish try.\"",
         offer = "\"I appreciate it, mortal.\"",
         receive_gift = "\"To answer your loyalty, you shall have this reward.\"",
         ready_to_receive_gift = "\"Long gone those worthy to bear my name are. But perhaps...\"",
         ready_to_receive_gift2 = "\"Very impressive, mortal. You shall be the one worthy to carry my name.\"",
      },
      ehekatl = {
         name = "Ehekatl of Luck",
         description = [=[Ehekatl is a goddess of luck. Those faithful to Ehekatl are<br>really lucky.

Offering: Corpses/Fish
   Bonus: CHR/LUCK/Evasion/Magic Capacity/Fishing/Lockpick
 Ability: Ehekatl school of magic (Passive: Randomize casting mana<br>cost.)]=],
         random = "\"Coconut crab!\"",
         kill = {
            "\"More! More!\"",
            "\"It's dead! Dead!\"",
            "\"Meoow mew mew.\"",
         },
         night = "\"Are you going to sleep? Really sleepy? Good night!\"",
         welcome = "\"Weeee♪ You are back? You are back!\"",
         believe = "\"Weee! You really have faith in me? Do you really?\"",
         betray = "\"Mewmewmew! Really betray me? Really betray me?\"",
         take_over = "\"Weeee! I'm so happy. I like you! I like you!\"",
         fail_to_take_over = "\"Stupid stupid! Stupid!\"",
         offer = "\"Mew mew mew meow!\"",
         receive_gift = "\"Here's a gift! For you! Mewmew!\"",
         ready_to_receive_gift = "\"My heart aches! I think I like you...I guess!\"",
         ready_to_receive_gift2 = "\"I love you! Love you! You will be with me forever...promise!\"",
      },
      opatos = {
         name = "Opatos of Earth",
         description = [=[Opatos is a god of earth. Those faithful to Opatos have massive<br>strength and defense.

Offering: Corpses/Ores
   Bonus: STR/CON/Shield/Weight Lifting/Mining/Magic Device
 Ability: Opatos' shell (Passive: Reduce any physical damage you<br>receive.)]=],
         random = "\"Muwahaha.\"",
         kill = {
            "\"Mwahaha!\"",
            "\"Die! Die! Mwahahaha!\"",
            "\"Muhan!\"",
         },
         night = "\"Muwahaha! I'll be following you, to your dream.\"",
         welcome = "\"Muwahahahahaha! Welcome back.\"",
         believe = "\"Muwahahahahaha. You're mine!\"",
         betray = "\"Muwahahahahahahaha!\"",
         take_over = "\"Muwahaha muwaha. Good. Good!\"",
         fail_to_take_over = "\"Muwahahahaha! Weak! Weak!\"",
         offer = "\"Muwahahahahaha!\"",
         receive_gift = "\"Muwahahahaha, take it!\"",
         ready_to_receive_gift = "\"Mwahaha haha! Fun! Fun!\"",
         ready_to_receive_gift2 = "\"Muwahahahaha! Mwaaaahahaha!\"",
      },
      jure = {
         name = "Jure of Healing",
         description = [=[Jure is a god of healing. Those faithful to Jure can heal wounds.

Offering: Corpses/Ores
   Bonus: WIL/Healing/Meditation/Anatomy/Cooking/Magic Device/Magic Capacity
 Ability: Prayer of Jure (Heal yourself.)]=],
         random = {
            "\"W-What? Silly!\"",
            "\"Am I really suitable for this job?\"",
         },
         kill = {
            "\"N-Not bad, for you\"",
            "\"S-Stop it. Disgusting...\"",
            "\"Don't look at me!\"",
         },
         night = "\"I-I'll never give you a good night kiss...ever!\"",
         welcome = "\"N-no...I-I wasn't looking for you! Silly!\"",
         believe = "\"I-I don't expect you to be my servant...I-I r-really don't, you silly!\"",
         betray = "\"I-I don't miss you. I-I really don't!\"",
         take_over = "\"W-What? I-I'm not gonna praise you. I'm not!\"",
         fail_to_take_over = "\"W-What have you done!\"",
         offer = "\"I-I'm not that pleased. I-I'm not, you silly!\"",
         receive_gift = "\"I-I'm not giving it to you as my gratitude! I-I r-really am not! Silly!\"",
         ready_to_receive_gift = "\"N-No! Cut it! I-I don't love you. Stupid!\"",
         ready_to_receive_gift2 = "\"I-I'm not falling l-love with you! D-don't you never ever leave me...okay? You stupid...!\"",
      },
      kumiromi = {
         name = "Kumiromi of Harvest",
         description = [=[Kumiromi is a god of harvest. Those faithful to Kumiromi receive<br>the blessings of nature.

Offering: Corpses/Vegetables/Seeds
   Bonus: PER/DEX/LER/Gardening/Alchemy/Tailoring/Literacy
 Ability: Kumiromi's recycle (Passive: Extract seeds from rotten
foods.)]=],
         random = {
            "\"Twitter of trees...song weaved by forests...if you strain your ears....\"",
            "\"This conflict between Gods...ugly.\"",
            "\"My Ehekatl...you are not what you used to be...\"",
            "\"I will give...more than you spoil...\"",
         },
         kill = {
            "\"You got dirty.\"",
            "\"Don't kill too much.\"",
            "\"...are you satisfied now?\"",
         },
         night = "\"Good night...May a bright sprout bring forth tomorrow... \"",
         welcome = "\"Welcome back...I was...waiting for you.\"",
         believe = "\"Welcome...I have...expectations of you...\"",
         betray = "\"A traitor...I can't...tolerate....\"",
         take_over = "\"Good work...truly...\"",
         fail_to_take_over = "\"I...will have no mercy..for enemies...absolutely.\"",
         offer = "\"This is...very good....Thank you.\"",
         receive_gift = "\"Here...there's something for...you...\"",
         ready_to_receive_gift = "\"You are...my precious valet...\"",
         -- This message is not translated in the original Elona. The following is
         -- machine-translated by Google translator.
         ready_to_receive_gift2 = {
            -- Original(Japanese): "「ずっと一緒…だよね？…もう離さない…君が死ぬまで」",
            "NOTE it is machine-translated: Together foreaver...right? I will not let you go...until you die.",
         },
      },
   }
)
