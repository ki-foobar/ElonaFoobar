local I18N = require("core.I18N")

I18N.add {
   wish = {
      what_do_you_wish_for = "何を望む？",
      your_wish = "「{$1}！！」",

      it_is_sold_out = "あ、それ在庫切れ。",
      something_appears = "足元に{itemname($1)}が転がってきた。",
      something_appears_from_nowhere = "足元に{itemname($1)}が転がってきた。",
      you_learn_skill = "{$1}の技術を会得した！",
      your_skill_improves = "{$1}が上昇した！",

      wish_gold = "金貨が降ってきた！",
      wish_platinum = "プラチナ硬貨が降ってきた！",
      wish_small_medal = "小さなメダルが降ってきた！",
      wish_sex = "{name($1)}は{$2}になった！ …もう後戻りはできないわよ。",
      wish_youth = "ふぅん…そんな願いでいいんだ。",
      wish_man_inside = "中の神も大変…あ…中の神なんているわけないじゃない！…ねえ、聞かなかったことにしてね。",
      wish_god_inside = "中の人も大変ね。",
      wish_ehekatl = "「うみみゅみゅぁ！」",
      wish_lulwy = "「アタシを呼びつけるとは生意気ね。」",
      wish_opatos = "工事中。",
      wish_kumiromi = "工事中。",
      wish_mani = "工事中。",
      wish_death = "それがお望みなら…",
      wish_redemption = {
         you_are_not_a_sinner = "…罪なんて犯してないじゃない。",
         what_a_convenient_wish = "あら…都合のいいことを言うのね。",
      },
      wish_alias = {
         impossible = "だめよ。",
         what_is_your_new_alias = "新しい異名は？",
         new_alias = "あなたの新しい異名は「{$1}」。満足したかしら？",
         no_change = "あら、そのままでいいの？",
      },

      general_wish = {
         skill = "スキル",
         item = "アイテム",
         card = "カード",
         summon = "召喚",
         figure = {"剥製", "はく製"},
      },

      special_wish = {
         god_inside = "中の神",
         man_inside = "中の人",
         ehekatl = "エヘカトル",
         lulwy = "ルルウィ",
         opatos = "オパートス",
         kumiromi = "クミロミ",
         mani = "マニ",
         youth = {"若さ", "若返り", "年", "美貌"},
         alias = {"通り名", "異名"},
         sex = {"性転換", "性", "異性"},
         redemption = "贖罪",
         death = "死",
         ally = "仲間",
         gold = {"金", "金貨", "富", "財産"},
         small_medal = {"メダル", "小さなメダル", "ちいさなメダル"},
         platinum = {"プラチナ", "プラチナ硬貨"},
         fame = "名声",
      },
   },
}
