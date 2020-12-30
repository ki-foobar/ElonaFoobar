local i18n = require("core.i18n")

i18n.add {
   god = {
      eyth_name = "無のエイス",
      indifferent = "あなたの信仰は既に限界まで高まっている。",
      enraged = "{$1}は激怒した。", -- TODO duplicate?

      switch = {
         unbeliever = "あなたは今や無信仰者だ。",
         follower = "あなたは今や{$1}の信者だ！",
      },

      pray = {
         do_not_believe = "{you()}は神を信仰していないが、試しに祈ってみた。",
         prompt = "あなたの神に祈りを乞う？",
         you_pray_to = "{$1}に祈った。",
         indifferent = "{$1}はあなたに無関心だ。",

         servant = {
            no_more = "神の使徒は2匹までしか仲間にできない。",
            party_is_full = "仲間が一杯で、神からの贈り物を受け取ることができなかった。",
            prompt_decline = "この贈り物を諦める？",

            desc = {
               mani = "このアンドロイドはブーストした時に恐るべき力を発揮するようだ。",
               lulwy = "この黒天使はブーストした時に恐るべき力を発揮するようだ。",
               itzpalt = "この追放者は連続魔法を使えるようだ。",
               ehekatl = "この猫に舐められた武具は、エヘカトルの祝福を授かるようだ。祝福を受けた武具にはエンチャントが一つ付与される。",
               opatos = "この騎士はある程度重いものをもたせても文句をいわないようだ。",
               jure = "この防衛者は致死ダメージを受けた仲間をレイハンドで回復できるようだ。レイハンドは眠るたびに再使用可能になる。",
               kumiromi = "この妖精は食後に種を吐き出すようだ。",
            },
         },
      },

      desc = {
         window = {
            title = "《 {$1} 》",

            abandon = "信仰を捨てる",
            convert = "{$1}に改宗する",
            believe = "{$1}を信仰する",
            cancel = "やめる",
         },
      },
   },
}



i18n.add_data_text(
   "core.god",
   {
      mani = {
         name = "機械のマニ",
         description = [=[マニは機械仕掛けの神です。マニを信仰した者は、機械や罠に対する膨大な知識を得、またそれらを効果的に利用する術を知ります。

  捧げ物: 死体 / 銃器 / 機械
ボーナス: 器用 / 感覚 / 銃 / 治癒 / 探知 / 宝石細工 / 鍵開け / 大工
特殊能力: マニの分解術(自動:罠からマテリアルを取り出す)]=],
         random = {
            "「お前も体を機械化したらどうだ？」",
            "「常に、私の名に恥じぬよう振舞え」",
            "「焦るな。すぐに機械が全てを支配する時代が来る」",
         },
         kill = {
            "「いいぞ」",
            "「分解してみたくならないか？」",
            "「その魂なら質の良い機械が組めそうだ」",
         },
         night = "「短い命の多くを無駄な眠りに費やすとは、生身の体とは不自由なものだ。だが今はそう、休むがいい。また私に仕えるために」",
         welcome = "「戻ってきたか。案外ホネのある奴だな」",
         believe = "「入信者か。私の名を貶めないよう励むがいい」",
         betray = "「やってくれたな。裏切り者め！」",
         take_over = "「やるじゃないか。見直したよ」",
         fail_to_take_over = "「馬鹿にしてくれたじゃないか」",
         offer = "「なかなかの贈り物だ」",
         receive_gift = "「お前は忠実なるシモベだ。これを有効に使うがいい」",
         ready_to_receive_gift = "「お前はまさに信者の模範だな」",
         ready_to_receive_gift2 = "「お前は最愛のシモベだ。その魂を私に捧げろ。お前を必ず守ってみせよう」",
      },
      lulwy = {
         name = "風のルルウィ",
         description = [=[ルルウィは風を司る女神です。ルルウィを信仰した者は、風の恩恵を受け素早く動くことが可能になります。

　捧げ物: 死体 / 弓
ボーナス: 感覚 / 速度 / 弓 / クロスボウ / 隠密 / 魔道具
特殊能力: ルルウィの憑依(スキル:瞬間的に高速になる)]=],
         random = {
            "「みじめなブタども」",
            "「マニ？その名を再び口にしたらミンチよ、子猫ちゃん」",
            "「前の下僕は、八つ裂きにしてシルフ達の餌にしたわ。髪型がちょっと気に食わなかったから。アハハ！」",
            "「私の子供達は風の声、何事にも縛られてはいけない。オマエもよ」",
         },
         kill = {
            "「不潔ね。血を拭いなさいよ」",
            "「アハハ！ミンチミンチィ！」",
            "「まあ、いけない子猫ちゃん」",
         },
         night = "「いいわ、少しの間だけ首枷を外してあげる。存分に休息を楽しみなさい」",
         welcome = "「どこホッツキ歩いてたのよ。もっと調教が必要ね」",
         believe = "「私を選んだのは正解よ。たっぷり可愛がってあげるわ、子猫ちゃん」",
         betray = "「アハハ。馬鹿ね。私なしで生きていくの？」",
         take_over = "「褒めてあげるわ。私の可愛い小さなお人形さん」",
         fail_to_take_over = "「やってくれたわね、ゴミの分際で。お仕置きよ」",
         offer = "「あら、気の利いたものをくれるわね。下心でもあるの？」",
         receive_gift = "「下僕のオマエにご褒美よ。大事に使いなさい。」",
         ready_to_receive_gift = "「どこまでも私のために尽くしなさい。オマエは私の一番の奴隷なんだから」",
         ready_to_receive_gift2 = "「私に従え。全てを委ねろ。オマエの綺麗な顔を傷つけるブタどもは、私がミンチにしてあげるわ」",
      },
      itzpalt = {
         name = "元素のイツパロトル",
         description = [=[イツパロトルは元素を司る神です。イツパロトルを信仰した者は、魔力を大気から吸収し、元素に対する保護を受けることができます。

　捧げ物: 死体 / 杖
ボーナス: 魔力 / 瞑想 / 火炎耐性 / 冷気耐性 / 電撃耐性
特殊能力: 魔力の吸収(スキル:周囲の空気からマナを吸い出す)]=],
         -- Duplicate them to emulate the original Elona's behavior.
         random = {
            "「汝油断することなかれ」",
            "「汝油断することなかれ」",
            "「我々の抱える痛みを、定命の者が理解することはないであろう」",
            "「我々の抱える痛みを、定命の者が理解することはないであろう」",
            "「我が名はイツパロトル。元素の起源にて、最古の炎を従えし王、全ての神の主なり」",
            "「我が名はイツパロトル。元素の起源にて、最古の炎を従えし王、全ての神の主なり」",
            "「神々の戦いに終わりはない。来るべき時は、汝も我が軍門の元で働いてもらうであろう」",
            "「神々の戦いに終わりはない。来るべき時は、汝も我が軍門の元で働いてもらうであろう」",
            "「神々の戦いに終わりはない。来るべき時は、汝も我が軍門の元で働いてもらうであろう」",
         },
         kill = {
            "「なかなかのものだ」",
            "「そして魂は元素へと還る」",
            "「高らかに我が名を唱えよ。屍に炎と安息を」",
         },
         night = "「旅の疲れを癒すがよい。我が費えることなき紅蓮の炎が、夜のとばりに紛れる邪なる者から汝を守るであろう」",
         welcome = "「定命の者よ、よくぞ戻ってきた」",
         believe = "「我が期待に応えて見せよ」",
         betray = "「我を裏切ると？愚かなり」",
         take_over = "「良し。汝の行いは覚えておこう」",
         fail_to_take_over = "「汝の愚かな試みの代償を味わうがよい」",
         offer = "「汝の贈り物に感謝しよう」",
         receive_gift = "「汝の忠誠に答え、贈り物を授けよう」",
         ready_to_receive_gift = "「我が名を語るに相応しい者が絶えて久しい。汝ならば、あるいは…」",
         ready_to_receive_gift2 = "「見事なり、定命の者よ。汝を、我が信頼に値する唯一の存在として認めよう」",
      },
      ehekatl = {
         name = "幸運のエヘカトル",
         description = [=[エヘカトルは幸運の女神です。エヘカトルを信仰した者は、運を味方につけます。

　捧げ物: 死体 / 魚
ボーナス: 魅力 / 運 / 回避 / 魔力の限界 / 釣り/ 鍵開け
特殊能力: エヘカトル流魔術(自動:マナの消費がランダムになる)]=],
         random = "「たらばがに！」",
         kill = {
            "「もっと！もっと！」",
            "「死んじゃったよ！たよ！」",
            "「うっみゅーうみゅうみゅ」",
         },
         night = "「寝るの？本当に寝るの？おやつみ！」",
         welcome = "「みゃみゃ？帰って来たの？来たの？たくさん待ってたよ！」",
         believe = "「わ〜ほんとに信じてくれるの？くれるの？」",
         betray = "「うみみやゅっ！裏切っちゃうの？ちゃうの？」",
         take_over = "「わおっ♪嬉しい！好き！大好き！」",
         fail_to_take_over = "「バカバカバカ！バカ！」",
         offer = "「うみみゃぁ！」",
         receive_gift = "「これあげるぅ…大切にしてね！…してね！」",
         ready_to_receive_gift = "「どきどきどき。君のこと、好きだよ。だよ！」",
         ready_to_receive_gift2 = "「好き！好き好き好きっ！だいすき！君とは死ぬまでずっと一緒だよ。だよ！」",
      },
      opatos = {
         name = "地のオパートス",
         description = [=[オパートスは大地の神です。オパートスを信仰した者は、高い防御力と破壊力を身につけます。

　捧げ物: 死体 / 鉱石
ボーナス: 筋力 / 耐久 / 盾 / 重量挙げ / 採掘 / 魔道具
特殊能力: オパートスの甲殻(自動:受ける物理ダメージを減らす)]=],
         random = "「フハハハハ」",
         kill = {
            "「フハハハ！」",
            "「逝け！逝け！フハハハッ！」",
            "「フハーン！」",
         },
         night = "「フハハハハ。付いて行くぞぉ、夢の中までも」",
         welcome = "「フハハハハハ！！おかえり」",
         believe = "「フハッハハハハ。逃がさんぞ！」",
         betray = "「フフッフフッフハハハハハ！」",
         take_over = "「フハハッ、いいぞいいぞ」",
         fail_to_take_over = "「フハハハハッ、弱い弱い」",
         offer = "「フッハッハハー！」",
         receive_gift = "「フハハハァ！受け取れィ」",
         ready_to_receive_gift = "「フハァッハハハハハハ！愉快愉快！」",
         ready_to_receive_gift2 = "「ファハハハハハハハハハハハハハー！フワハァー！」",
      },
      jure = {
         name = "癒しのジュア",
         description = [=[ジュアは癒しの女神です。ジュアを信仰した者は、傷ついた身体を癒すことができます。

　捧げ物: 死体 / 鉱石
ボーナス: 意思 / 治癒 / 瞑想 / 解剖学 / 料理 / 魔道具 / 魔力の限界
特殊能力: ジュアの祈り(スキル:失った体力を回復)]=],
         random = {
            "「な、なによバカ！」",
            "「私はこの仕事に向いているのかなあ」",
         },
         kill = {
            "「や、やるじゃない」",
            "「や、やめてよ。気持ち悪い」",
            "「こっち来ないで！」",
         },
         night = "「お、おやすみのキスなんて…絶対にやだからね！」",
         welcome = "「べ、別にアンタが帰ってくるのを待ってたんじゃないからね！」",
         believe = "「べ、別にアンタの活躍なんて期待してないんだからねっ」",
         betray = "「な、何よ。アンタなんか、いなくたって寂しくないんだからね！」",
         take_over = "「な、なによ。ほ、褒めてなんかあげないわよ！」",
         fail_to_take_over = "「な、なにするのよ！」",
         offer = "「べ、別に嬉しくなんかないんだからねっ！」",
         receive_gift = "「ア、アンタのためにしてあげるんじゃないんだからっ」",
         ready_to_receive_gift = "「や、やめてよ。アンタのことなんか、大好きじゃないんだからねっ。バカ！」",
         ready_to_receive_gift2 = "「べ、別にアンタのこと愛してなんかいないんだからっ。あたしの傍から離れたらしょうちしないからねっ！このバカぁ…！",
      },
      kumiromi = {
         name = "収穫のクミロミ",
         description = [=[クミロミは収穫の神です。クミロミを信仰した者は、大地の恵みを収穫し、それを加工する術を知ります。

　捧げ物: 死体 / 野菜 / 種
ボーナス: 感覚 / 器用 / 習得 / 栽培 / 錬金術 / 裁縫 / 読書
特殊能力: 生命の輪廻(自動：腐った作物から種を取り出す)]=],
         random = {
            "「木々のさえずり…森の命が奏でる歌…耳をすませば…」",
            "「争いごとは…醜い」",
            "「僕のエヘカトル…今はもう…かつての面影さえない」",
            "「僕は与えよう…君たちが奪う以上のものを」",
         },
         kill = {
            "「汚れたね」",
            "「ほどほどに」",
            "「…これが…君の望んでいた結果？」",
         },
         night = "「おやすみ…明日…また朗らかな芽を吹いておくれ」",
         welcome = "「おかえり…待っていた」",
         believe = "「よくきたね…期待…しているよ」",
         betray = "「裏切り…許さない…」",
         take_over = "「よくやった…ほんとに…」",
         fail_to_take_over = "「敵には…容赦しない…絶対」",
         offer = "「ありがたい…とてもいいよ…これ」",
         receive_gift = "「いいもの…あげる…」",
         ready_to_receive_gift = "「君は…大切なしもべだ…」",
         ready_to_receive_gift2 = "「ずっと一緒…だよね？…もう離さない…君が死ぬまで」",
      },
   }
)
