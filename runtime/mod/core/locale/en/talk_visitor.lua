local I18N = require("core.I18N")

I18N.add {
   talk = {
      visitor = {
         wanted_to_say_hi = "I just wanted to say hi.",
         receive = "You receive {itemname($1, 1)} from {name($2)}!",

         choices = {
            yes = "Yes.",
            no = "No.",
         },

         adventurer = {
            new_year = {
               happy_new_year = "Happy new year!",
               gift = "I've brought you a gift today, here.",
               throws = "{name($1)} throws you {itemname($2, 1)}.",
            },

            hate = {
               dialog = "You scum! You won't get away from me now!",
               text = "\"Eat this!\"",
               throws = "{name($1)} throws molotov.",
            },

            like = {
               dialog = "Here, take this!",
               wonder_if = "Wonder if we can reach 100 friends? ♪",
            },

            train = {
               choices = {
                  learn = "Teach me the skill.",
                  train = "Train me.",
                  pass = "I think I'll pass.",
               },

               learn = {
                  dialog = "I can teach you the art of {$1} for a friendly price of {$2} platinum pieces. Do you want me to train you?",
                  after = "Fantastic! You've learned the skill in no time. I'm glad I could help.",
               },

               train = {
                  dialog = "I can train your {$1} skill for a friendly price of {$2} platinum pieces. Do you want me to train you?",
                  after = "Marvelous! The training is now complete. I think you've improved some potential.",
               },

               pass = "I see. I'll ask you again at some time in the future.",
            },

            friendship = {
               dialog = "As a pledge of friendship, here's something for you!",
               no_empty_spot = "Your home has no empty spot...",
            },

            souvenir = {
               dialog = "I just stopped by to see you. Oh, I happen to have a gift for you too.",
               inventory_is_full = "Your inventory is full...",
               receive = "You receive {itemname($1, 1)}.",
            },

            materials = {
               dialog = "I found these during my journey. Thought you could find them useful.",
               receive = "{name($1)} gives you a bag full of materials.",
            },

            favorite_skill = {
               dialog = "{$1} is one of my favorite skills.",
            },

            favorite_stat = {
               dialog = "I'm proud of my good {$1}.",
            },

            conversation = {
               dialog = "Hey {basename($1)}, how's your journey? I was bored to death so I decided to make a visit to you!",
               hold = "You hold an amusing conversation with {name($1)}!",
            },

            drink = {
               dialog = "Let's have a drink and deepen our friendship!",
               cheers = "\"Cheers!\"",
            },
         },

         trainer = {
            no_more_this_month = "No more training in this month.",

            dialog = {
               member = "As a member of {$1} you have to forge your talent to live up to our reputation. For only {$2} platinum coins, I'll improve the potential of your talent.",
               nonmember = "Training! Training! At the end, only thing that saves your life is training! For only {$1} platinum coins, I'll improve the potential of your talent.",
            },

            choices = {
               not_today = "Not today.",
               improve = "I want to improve {$1}.",
            },

            regret = "You'll regret this!",
            potential_expands = "{name($1)}{his_owned($1)} potential of {$2} greatly expands.",
            after = "Good. You show a lot of potential.",
         },

         beggar = {
            dialog = "I got no money to buy food. Will you spare me some coins?",
            spare = "You spare {him($2)} {$1} gold pieces.",
            after = "Thanks! I'll never forget this.",
            cheap = "You're so cheap!",
         },

         punk = {
            dialog = "So, are you ready?",
            hump = "Hump!",
         },

         mysterious_producer = {
            want_to_be_star = "You want to be a star?",
            no_turning_back = "Okay, no turning back now!",
         },

         merchant = {
            choices = {
               buy = "I want to buy something.",
               sell = "I want to sell something.",
               not_now = "Not now.",
            },

            dialog = "This is your lucky day. I wouldn't normally show my discounted goods to commoners but since I feel so good today...",
            regret = "I hope you won't regret it later.",
         },
      },
   },
}
