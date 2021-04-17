#include "text.hpp"

#include "../util/fileutil.hpp"
#include "../util/strutil.hpp"
#include "area.hpp"
#include "chara_db.hpp"
#include "character.hpp"
#include "data/types/type_skill.hpp"
#include "draw.hpp"
#include "elona.hpp"
#include "enchantment.hpp"
#include "fov.hpp"
#include "game.hpp"
#include "i18n.hpp"
#include "inventory.hpp"
#include "item.hpp"
#include "lua_env/interface.hpp"
#include "map.hpp"
#include "map_cell.hpp"
#include "random.hpp"
#include "variables.hpp"



namespace elona
{

namespace
{

using RandomNameTable =
    std::pair<std::vector<std::string>, std::vector<std::string>>;
RandomNameTable _random_name_table;



elona_vector2<std::string> _rnlist;
elona_vector1<std::string> txtbuff;



optional<std::string> _random_name_internal()
{
    auto ret = choice(_random_name_table.first);

    if (ret.empty())
    {
        return none;
    }
    if (jp)
    {
        if (rnd(8) == 0)
        {
            ret += "ー";
        }
    }
    if (rnd(5))
    {
        ret += choice(_random_name_table.second);
    }

    const auto length = ret.size();
    if (length < 4)
    {
        return none;
    }
    if (length < 6)
    {
        if (rnd(3))
        {
            return none;
        }
    }
    if (length < 8)
    {
        if (rnd(2))
        {
            return none;
        }
    }

    if (jp)
    {
        if (strutil::starts_with(ret, "ー") || strutil::contains(ret, "ーッ"))
        {
            return none;
        }
    }

    return ret;
}

} // namespace


int p_at_m34 = 0;
int talkref = 0;



// see attack.cpp
extern int cansee;

// see world.cpp
extern elona_vector1<int> ranknorma;



std::string zentohan(const std::string& str)
{
    // TODO: Implement.
    return str;
}



std::string cnvrank(int n)
{
    if (jp)
        return std::to_string(n);

    if (n % 10 == 1 && n != 11)
        return std::to_string(n) + "st";
    else if (n % 10 == 2 && n != 12)
        return std::to_string(n) + "nd";
    else if (n % 10 == 3 && n != 13)
        return std::to_string(n) + "rd";
    else
        return std::to_string(n) + "th";
}



std::string cnvarticle(const std::string& str)
{
    return i18n::s.get("core.ui.article", str);
}



std::string cnvitemname(int id)
{
    if (jp)
        return ioriginalnameref(id);

    if (ioriginalnameref2(id) == ""s)
    {
        return ioriginalnameref(id);
    }
    return ioriginalnameref2(id) + " of "s + ioriginalnameref(id);
}



std::string cnven(const std::string& source)
{
    if (jp)
        return source;
    if (source.empty())
        return source;

    std::string ret = source;
    if (ret[0] == '*')
    {
        if (source.size() == 1)
            return source;
        ret[1] = std::toupper(ret[1]);
    }
    else
    {
        ret[0] = std::toupper(ret[0]);
    }
    return ret;
}



std::string cnvfix(int n)
{
    return n >= 0 ? "+"s + std::to_string(n) : std::to_string(n);
}



std::string cnvdate(int datetime_id, bool show_hour)
{
    std::string ret;

    int hour = datetime_id % 24;
    int day = datetime_id / 24 % 30;
    if (day == 0)
    {
        day = 30;
        datetime_id -= 720;
    }
    int month = datetime_id / 24 / 30 % 12;
    if (month == 0)
    {
        month = 12;
        datetime_id -= 8640;
    }
    int year = datetime_id / 24 / 30 / 12;

    ret = i18n::s.get("core.ui.date", year, month, day);
    if (show_hour)
    {
        ret += i18n::s.get("core.ui.date_hour", hour);
    }

    return ret;
}



std::string cnvplaytime(int datetime_id)
{
    const int h = datetime_id / 60 / 60;
    const int m = datetime_id / 60 % 60;
    const int s = datetime_id % 60;
    return i18n::s.get("core.ui.playtime", h, m, s);
}



// Get rid of the job and extract the name.
// Lomias the general vendor => Lomias
std::string sncnv(const std::string& name_with_job)
{
    return name_with_job.substr(0, name_with_job.find(' ')) + ' ';
}



std::string sngeneral(const std::string& name)
{
    return i18n::s.get("core.chara.job.general_vendor", name);
}



std::string sninn(const std::string& name)
{
    return i18n::s.get("core.chara.job.innkeeper", name);
}



std::string sntrade(const std::string& name)
{
    return i18n::s.get("core.chara.job.trader", name);
}



std::string sngoods(const std::string& name)
{
    return i18n::s.get("core.chara.job.goods_vendor", name);
}



std::string snbakery(const std::string& name)
{
    return i18n::s.get("core.chara.job.baker", name);
}



std::string snmagic(const std::string& name)
{
    return i18n::s.get("core.chara.job.magic_vendor", name);
}



std::string snarmor(const std::string& name)
{
    return i18n::s.get("core.chara.job.blacksmith", name);
}



std::string sntrainer(const std::string& name)
{
    return i18n::s.get("core.chara.job.trainer", name);
}



std::string snfish(const std::string& name)
{
    return i18n::s.get("core.chara.job.fisher", name);
}



std::string snblack(const std::string& name)
{
    return i18n::s.get("core.chara.job.blackmarket", name);
}



std::string snfood(const std::string& name)
{
    return i18n::s.get("core.chara.job.food_vendor", name);
}



void initialize_nefia_names()
{
    SDIM4(mapnamerd, 20, 2, 5);
    for (int cnt = 0; cnt < 5; cnt++)
    {
        mapnamerd(0, cnt) =
            i18n::s.get_enum("core.map.nefia.prefix.type_a", cnt);
        mapnamerd(1, cnt) =
            i18n::s.get_enum("core.map.nefia.prefix.type_b", cnt);
    }
}



std::string maplevel(int)
{
    if (game()->current_map == mdata_t::MapId::your_home)
    {
        if (game()->current_dungeon_level != 1)
        {
            if (game()->current_dungeon_level > 0)
            {
                return "B."s + (game()->current_dungeon_level - 1);
            }
            else
            {
                return "L."s + (game()->current_dungeon_level - 2) * -1;
            }
        }
    }
    if (map_shows_floor_count_in_name())
    {
        return ""s +
            cnvrank(
                   (game()->current_dungeon_level -
                    area_data[game()->current_map].danger_level + 1)) +
            i18n::s.get("core.map.nefia.level");
    }

    return "";
}


std::string mapname_dungeon(int id)
{
    int suffix_id = area_data[id].type;
    std::string name = mapnamerd(
        area_data[id].dungeon_prefix,
        std::min(area_data[id].danger_level / 5, int(mapnamerd.j_size() - 1)));

    if (mdata_t::is_nefia(suffix_id))
    {
        name += i18n::s.get_enum("core.map.nefia.suffix", suffix_id);
    }
    return name;
}

std::string mapname(int id, bool description)
{
    std::string name;
    std::string desc;

    switch (static_cast<mdata_t::MapId>(area_data[id].id))
    {
    case mdata_t::MapId::quest:
        if (game()->executing_immediate_quest_type == 1001)
        {
            name = i18n::s.get("core.map.quest.outskirts");
        }
        if (game()->executing_immediate_quest_type == 1010 ||
            game()->executing_immediate_quest_type == 1008)
        {
            name = i18n::s.get("core.map.quest.urban_area");
        }
        break;
    case mdata_t::MapId::random_dungeon: name = mapname_dungeon(id); break;
    default:
        auto name_opt = i18n::s.get_enum_property_optional(
            "core.map.unique", "name", area_data[id].id);
        if (name_opt)
        {
            name = *name_opt;
        }
        else
        {
            name = "";
        }

        auto desc_opt = i18n::s.get_enum_property_optional(
            "core.map.unique", "desc", area_data[id].id);
        if (desc_opt)
        {
            desc = *desc_opt;
        }
        else
        {
            desc = "";
        }
    }

    if (description)
    {
        if (area_data[id].is_hidden_in_world_map())
        {
            return "";
        }
        else if (desc != ""s)
        {
            return desc;
        }
        else if (mdata_t::is_nefia(area_data[id].type))
        {
            return i18n::s.get(
                "core.map.you_see_an_entrance",
                name,
                area_data[id].danger_level);
        }
        else
        {
            return i18n::s.get("core.map.you_see", name);
        }
    }
    else
    {
        return name;
    }
}



std::string txtbuilding(int x, int y)
{
    const auto type = bddata(0, x, y);
    if (type == 0)
    {
        return "";
    }
    return i18n::s.get(
        "core.map.you_see", i18n::s.get_enum("core.map.misc_location", type));
}



std::string txtskillchange(const Character& chara, int id, bool increase)
{
    if (auto text = i18n::s.get_enum_property_optional(
            "core.skill", increase ? "increase" : "decrease", id, chara))
    {
        return *text;
    }
    else
    {
        if (increase)
        {
            return i18n::s.get(
                "core.skill.default.increase",
                chara,
                the_skill_db.get_text(id, "name"));
        }
        else
        {
            return i18n::s.get(
                "core.skill.default.decrease",
                chara,
                the_skill_db.get_text(id, "name"));
        }
    }
}



std::string _ka(const Character& speaker, int mark)
{
    std::vector<std::string> candidates[][2] = {
        {{"ですか"}, {"ですか"}},
        {{"かよ", "か"}, {"かい"}},
        {{"かい", "なの"}, {"なの"}},
        {{"か…", "かよ…"}, {"なの…"}},
        {{"かのう", "であるか"}, {"であるか"}},
        {{"でござるか"}, {"でござりまするか"}},
        {{"ッスか"}, {"かにゃ", "かニャン"}},
    };
    return choice(candidates[speaker.talk_type][speaker.sex]) +
        i18n::s.get_enum("core.ui.mark", mark);
}



std::string _da(const Character& speaker, int mark)
{
    std::vector<std::string> candidates[][2] = {
        {{"です", "ですね"}, {"ですわ", "です"}},
        {{"だぜ", "だ"}, {"ね", "よ"}},
        {{"だよ"}, {"だわ", "よ"}},
        {{"だ…", "さ…"}, {"よ…", "ね…"}},
        {{"じゃ", "でおじゃる"}, {"じゃ", "でおじゃるぞ"}},
        {{"でござる", "でござるよ"}, {"でござりまする"}},
        {{"ッス"}, {"みゃん", "ミャ"}},
    };
    return choice(candidates[speaker.talk_type][speaker.sex]) +
        i18n::s.get_enum("core.ui.mark", mark);
}



std::string _nda(const Character& speaker, int mark)
{
    std::vector<std::string> candidates[][2] = {
        {{"のです", "んです"}, {"のですわ", "のです"}},
        {{"", "んだ"}, {"の"}},
        {{"んだよ", "んだ"}, {"わ", "のよ"}},
        {{"…", "んだ…"}, {"の…", "わ…"}},
        {{"のじゃ", "のだぞよ"}, {"のじゃわ", "のだぞよ"}},
        {{"のでござる"}, {"のでございます"}},
        {{"んだッス"}, {"のニャ", "のにゃん"}},
    };
    return choice(candidates[speaker.talk_type][speaker.sex]) +
        i18n::s.get_enum("core.ui.mark", mark);
}



std::string _noka(const Character& speaker, int mark)
{
    std::vector<std::string> candidates[][2] = {
        {{"のですか", "んですか"}, {"のですか", "んですか"}},
        {{"のか", "のだな"}, {"の", "のかい"}},
        {{"のかい", "の"}, {"の"}},
        {{"のか…"}, {"の…"}},
        {{"のかのう", "のだな"}, {"のかね", "のだな"}},
        {{"のでござるか"}, {"のでございます"}},
        {{"のッスか"}, {"にゃんか", "ニャン"}},
    };
    return choice(candidates[speaker.talk_type][speaker.sex]) +
        i18n::s.get_enum("core.ui.mark", mark);
}



std::string _kana(const Character& speaker, int mark)
{
    std::vector<std::string> candidates[][2] = {
        {{"でしょうか", "ですか"}, {"かしら", "でしょう"}},
        {{"か", "かい"}, {"か", "かい"}},
        {{"かな", "かなぁ"}, {"かな", "かなー"}},
        {{"かな…", "か…"}, {"かな…", "か…"}},
        {{"かのう", "かの"}, {"かのう", "かの"}},
        {{"でござるか"}, {"でございますか"}},
        {{"ッスか"}, {"かにゃん", "かニャ"}},
    };
    return choice(candidates[speaker.talk_type][speaker.sex]) +
        i18n::s.get_enum("core.ui.mark", mark);
}



std::string _kimi(const Character& speaker, int mark)
{
    std::vector<std::string> candidates[][2] = {
        {{"貴方"}, {"貴方"}},
        {{"お前"}, {"お前"}},
        {{"君"}, {"君"}},
        {{"君"}, {"君"}},
        {{"お主"}, {"お主"}},
        {{"そこもと"}, {"そなた様"}},
        {{"アンタ"}, {"あにゃた"}},
    };
    return choice(candidates[speaker.talk_type][speaker.sex]) +
        i18n::s.get_enum("core.ui.mark", mark);
}



std::string _ru(const Character& speaker, int mark)
{
    std::vector<std::string> candidates[][2] = {
        {{"ます", "ますよ"}, {"ますわ", "ますの"}},
        {{"るぜ", "るぞ"}, {"るわ", "るよ"}},
        {{"るよ", "るね"}, {"るの", "るわ"}},
        {{"る…", "るが…"}, {"る…", "るわ…"}},
        {{"るぞよ", "るぞ"}, {"るぞよ", "るぞ"}},
        {{"るでござる", "るでござるよ"}, {"るのでございます"}},
        {{"るッス"}, {"るのニャ", "るにゃん"}},
    };
    return choice(candidates[speaker.talk_type][speaker.sex]) +
        i18n::s.get_enum("core.ui.mark", mark);
}



std::string _tanomu(const Character& speaker, int mark)
{
    std::vector<std::string> candidates[][2] = {
        {{"お願いします", "頼みます"}, {"お願いしますわ", "頼みますわ"}},
        {{"頼む", "頼むな"}, {"頼むよ", "頼む"}},
        {{"頼むね", "頼むよ"}, {"頼むわ", "頼むね"}},
        {{"頼む…", "頼むぞ…"}, {"頼むわ…", "頼むよ…"}},
        {{"頼むぞよ"}, {"頼むぞよ"}},
        {{"頼み申す", "頼むでござる"}, {"お頼み申し上げます"}},
        {{"頼むッス"}, {"おねがいにゃ", "おねがいニャン"}},
    };
    return choice(candidates[speaker.talk_type][speaker.sex]) +
        i18n::s.get_enum("core.ui.mark", mark);
}



std::string _ore(const Character& speaker, int mark)
{
    std::vector<std::string> candidates[][2] = {
        {{"私"}, {"私"}},
        {{"俺"}, {"あたし"}},
        {{"僕"}, {"わたし"}},
        {{"自分"}, {"自分"}},
        {{"麻呂"}, {"わらわ"}},
        {{"拙者"}, {"手前"}},
        {{"あっし"}, {"みゅー"}},
    };
    return choice(candidates[speaker.talk_type][speaker.sex]) +
        i18n::s.get_enum("core.ui.mark", mark);
}



std::string _ga(const Character& speaker, int mark)
{
    std::vector<std::string> candidates[][2] = {
        {{"ですが", "ですけど"}, {"ですが", "ですけど"}},
        {{"が", "がな"}, {"が"}},
        {{"けど", "が"}, {"が", "けど"}},
        {{"が…", "けど…"}, {"が…", "けど…"}},
        {{"であるが"}, {"であるが"}},
        {{"でござるが"}, {"でございますが"}},
        {{"ッスけど", "ッスが"}, {"ニャけど", "にゃが"}},
    };
    return choice(candidates[speaker.talk_type][speaker.sex]) +
        i18n::s.get_enum("core.ui.mark", mark);
}



std::string _dana(const Character& speaker, int mark)
{
    std::vector<std::string> candidates[][2] = {
        {{"ですね"}, {"ですわね", "ですね"}},
        {{"だな"}, {"だね", "ね"}},
        {{"だね"}, {"ね"}},
        {{"だな…"}, {"だね…", "ね…"}},
        {{"であるな"}, {"であるな"}},
        {{"でござるな"}, {"でございますね"}},
        {{"ッスね"}, {"にゃ", "みゃ"}},
    };
    return choice(candidates[speaker.talk_type][speaker.sex]) +
        i18n::s.get_enum("core.ui.mark", mark);
}



std::string _kure(const Character& speaker, int mark)
{
    std::vector<std::string> candidates[][2] = {
        {{"ください", "くださいよ"}, {"くださいな", "ください"}},
        {{"くれ", "くれよ"}, {"くれ", "よ"}},
        {{"ね", "よ"}, {"ね", "ね"}},
        {{"くれ…", "…"}, {"よ…", "…"}},
        {{"つかわせ", "たもれ"}, {"つかわせ", "たもれ"}},
        {{"頂きたいでござる"}, {"くださいませ"}},
        {{"くれッス"}, {"にゃ", "みゃ"}},
    };
    return choice(candidates[speaker.talk_type][speaker.sex]) +
        i18n::s.get_enum("core.ui.mark", mark);
}



std::string _daro(const Character& speaker, int mark)
{
    std::vector<std::string> candidates[][2] = {
        {{"でしょう"}, {"でしょう"}},
        {{"だろ"}, {"だろうね"}},
        {{"だろうね"}, {"でしょ"}},
        {{"だろ…"}, {"でしょ…"}},
        {{"であろう"}, {"であろうな"}},
        {{"でござろうな"}, {"でございましょう"}},
        {{"ッスね"}, {"にゃ", "みゃ"}},
    };
    return choice(candidates[speaker.talk_type][speaker.sex]) +
        i18n::s.get_enum("core.ui.mark", mark);
}



std::string _yo(const Character& speaker, int mark)
{
    std::vector<std::string> candidates[][2] = {
        {{"ですよ", "です"}, {"ですよ", "です"}},
        {{"ぜ", "ぞ"}, {"わ", "よ"}},
        {{"よ", "ぞ"}, {"わよ", "わ"}},
        {{"…", "ぞ…"}, {"わ…", "…"}},
        {{"であろう", "でおじゃる"}, {"であろうぞ", "でおじゃる"}},
        {{"でござろう"}, {"でございますわ"}},
        {{"ッスよ", "ッス"}, {"にゃぁ", "みゃぁ"}},
    };
    return choice(candidates[speaker.talk_type][speaker.sex]) +
        i18n::s.get_enum("core.ui.mark", mark);
}



std::string _aru(const Character& speaker, int mark)
{
    std::vector<std::string> candidates[][2] = {
        {{"あります", "ありますね"}, {"あります", "ありますわ"}},
        {{"ある", "あるな"}, {"あるね", "あるよ"}},
        {{"あるね", "あるよ"}, {"あるわ", "あるわね"}},
        {{"ある…", "あるぞ…"}, {"あるわ…"}},
        {{"あろう", "おじゃる"}, {"あろう", "おじゃる"}},
        {{"あるでござる", "あるでござるな"}, {"ござます"}},
        {{"あるッスよ", "あるッス"}, {"あにゅ", "あみぅ"}},
    };
    return choice(candidates[speaker.talk_type][speaker.sex]) +
        i18n::s.get_enum("core.ui.mark", mark);
}



std::string _u(const Character& speaker, int mark)
{
    std::vector<std::string> candidates[][2] = {
        {{"います", "いますよ"}, {"いますわ", "います"}},
        {{"うぜ", "うぞ"}, {"うわ", "うよ"}},
        {{"うよ", "う"}, {"うわ", "う"}},
        {{"う…", "うぞ…"}, {"うわ…", "う…"}},
        {{"うぞよ", "うぞ"}, {"うぞよ", "うぞ"}},
        {{"うでござる", "うでござるよ"}, {"うでございます"}},
        {{"うッスよ", "うッス"}, {"うにぁ", "うみぁ"}},
    };
    return choice(candidates[speaker.talk_type][speaker.sex]) +
        i18n::s.get_enum("core.ui.mark", mark);
}



std::string _na(const Character& speaker, int mark)
{
    std::vector<std::string> candidates[][2] = {
        {{"ですね", "です"}, {"ですわ", "ですね"}},
        {{"ぜ", "な"}, {"ね", "な"}},
        {{"ね", "なぁ"}, {"わ", "わね"}},
        {{"…", "な…"}, {"…", "わ…"}},
        {{"でおじゃるな", "のう"}, {"でおじゃるな", "のう"}},
        {{"でござるな"}, {"でございますわ"}},
        {{"ッスね", "ッス"}, {"ニァ", "ミァ"}},
    };
    return choice(candidates[speaker.talk_type][speaker.sex]) +
        i18n::s.get_enum("core.ui.mark", mark);
}



std::string _ta(const Character& speaker, int mark)
{
    std::vector<std::string> candidates[][2] = {
        {{"ました", "ましたね"}, {"ました", "ましたわ"}},
        {{"た", "たな"}, {"たね", "たよ"}},
        {{"たね", "たよ"}, {"たよ", "たね"}},
        {{"た…", "たぞ…"}, {"たわ…"}},
        {{"たぞよ", "たぞな"}, {"たぞよ"}},
        {{"たでござる"}, {"ましてございます"}},
        {{"たッスよ", "たッス"}, {"たにゃぁ", "たみゃぁ"}},
    };
    return choice(candidates[speaker.talk_type][speaker.sex]) +
        i18n::s.get_enum("core.ui.mark", mark);
}



std::string replace_tag(
    const std::string source,
    optional_ref<const Character> client)
{
    if (source == "ref"s && talkref == 1)
    {
        return i18n::s.get(
            "core.talk.tag.ref", game()->number_of_waiting_guests);
    }
    if (source == "you"s)
    {
        assert(client);
        return _kimi(*client, 3);
    }
    if (source == "sex"s)
    {
        return i18n::s.get_enum("core.ui.sex2", cdata.player().sex);
    }
    if (source == "player"s)
    {
        return cdata.player().name;
    }
    if (source == "aka"s)
    {
        return cdata.player().alias;
    }
    if (source == "npc"s)
    {
        assert(client);
        return client->name;
    }
    if (source == "ある"s)
    {
        assert(client);
        return _aru(*client, 3);
    }
    if (source == "が"s)
    {
        assert(client);
        return _ga(*client, 3);
    }
    if (source == "か"s)
    {
        assert(client);
        return _ka(*client, 3);
    }
    if (source == "かな"s)
    {
        assert(client);
        return _kana(*client, 3);
    }
    if (source == "だ"s)
    {
        assert(client);
        return _da(*client, 3);
    }
    if (source == "よ"s)
    {
        assert(client);
        return _yo(*client, 3);
    }
    if (source == "だな"s)
    {
        assert(client);
        return _dana(*client, 3);
    }
    if (source == "だろ"s)
    {
        assert(client);
        return _daro(*client, 3);
    }
    if (source == "る"s)
    {
        assert(client);
        return _ru(*client, 3);
    }
    if (source == "のだ"s)
    {
        assert(client);
        return _nda(*client, 3);
    }
    if (source == "な"s)
    {
        assert(client);
        return _na(*client, 3);
    }
    if (source == "くれ"s)
    {
        assert(client);
        return _kure(*client, 3);
    }
    return "Unknown Code"s;
}



void parse_talk_file(optional_ref<const Character> speaker)
{
    buff = strmid(buff, p, instr(buff, p, "%END"s));
    if (noteinfo() <= 1)
    {
        buff(0).clear();
        std::ifstream in{
            lua::resolve_path_for_mod("<core>/locale/<LANGUAGE>/lazy/talk.txt")
                .native(),
            std::ios::binary};
        std::string tmp;
        while (std::getline(in, tmp))
        {
            buff(0) += tmp + '\n';
        }
        p = instr(buff, 0, "%DEFAULT,"s + i18n::s.get("core.meta.tag"));
        buff = strmid(buff, p, instr(buff, p, "%END"s));
    }
    notedel(0);
    p = rnd(noteinfo());
    noteget(s, p);
    buff = s;
    text_replace_tags_in_quest_board(speaker);
}



void read_talk_file(const std::string& valn)
{
    buff = "";
    notesel(buff);
    {
        buff(0).clear();
        std::ifstream in{
            lua::resolve_path_for_mod("<core>/locale/<LANGUAGE>/lazy/talk.txt")
                .native(),
            std::ios::binary};
        std::string tmp;
        while (std::getline(in, tmp))
        {
            buff(0) += tmp + '\n';
        }
    }
    p = instr(buff, 0, valn + ","s + i18n::s.get("core.meta.tag"));
    parse_talk_file(none);
}



void get_npc_talk(Character& chara)
{
    buff = "";
    notesel(buff);
    {
        buff(0).clear();
        std::ifstream in{
            lua::resolve_path_for_mod("<core>/locale/<LANGUAGE>/lazy/talk.txt")
                .native(),
            std::ios::binary};
        std::string tmp;
        while (std::getline(in, tmp))
        {
            buff(0) += tmp + '\n';
        }
    }
    p = -1;
    for (int cnt = 0; cnt < 1; ++cnt)
    {
        if (chara.role == Role::maid)
        {
            if (game()->number_of_waiting_guests > 0)
            {
                talkref = 1;
                p = instr(buff, 0, "%MAID,"s + i18n::s.get("core.meta.tag"));
                break;
            }
        }
        if (chara.interest <= 0)
        {
            p = instr(buff, 0, "%BORED,"s + i18n::s.get("core.meta.tag"));
            break;
        }
        if (chara.is_player_or_ally())
        {
            p = instr(
                buff, 0, "%ALLY_DEFAULT,"s + i18n::s.get("core.meta.tag"));
            break;
        }
        if (chara.id == "core.prostitute")
        {
            p = instr(buff, 0, "%BITCH,"s + i18n::s.get("core.meta.tag"));
            break;
        }
        if (chara.role == Role::moyer)
        {
            p = instr(buff, 0, "%MOYER,"s + i18n::s.get("core.meta.tag"));
            break;
        }
        if (chara.role == Role::slave_master)
        {
            p = instr(buff, 0, "%SLAVEKEEPER,"s + i18n::s.get("core.meta.tag"));
            break;
        }
        if (is_shopkeeper(chara.role))
        {
            if (rnd(3))
            {
                p = instr(
                    buff, 0, "%SHOPKEEPER,"s + i18n::s.get("core.meta.tag"));
                break;
            }
        }
        if (chara.impression >= 100)
        {
            if (rnd(3) == 0)
            {
                p = instr(
                    buff, 0, "%RUMOR,LOOT,"s + i18n::s.get("core.meta.tag"));
                break;
            }
        }
        if (area_data[game()->current_map].christmas_festival)
        {
            if (game()->current_map == mdata_t::MapId::noyel)
            {
                if (rnd(3))
                {
                    p = instr(
                        buff,
                        0,
                        "%FEST,"s + game()->current_map + ","s +
                            i18n::s.get("core.meta.tag"));
                    break;
                }
            }
        }
        if (rnd(2))
        {
            p = instr(
                buff,
                0,
                "%PERSONALITY,"s + chara.personality + ","s +
                    i18n::s.get("core.meta.tag"));
            break;
        }
        if (rnd(3))
        {
            p = instr(
                buff,
                0,
                "%AREA,"s + game()->current_map + ","s +
                    i18n::s.get("core.meta.tag"));
            break;
        }
    }
    if (p == -1)
    {
        p = instr(buff, 0, "%DEFAULT,"s + i18n::s.get("core.meta.tag"));
    }
    parse_talk_file(chara);
}



std::string cnvweight(int weight)
{
    return ""s + std::abs(weight) / 1000 + '.' + std::abs(weight) % 1000 / 100 +
        i18n::s.get("core.ui.unit_of_weight");
}



std::string fltname(int category)
{
    if (auto text =
            i18n::s.get_enum_optional("core.item.filter_name", category))
    {
        return *text;
    }
    else
    {
        return i18n::s.get("core.item.filter_name.default");
    }
}



void quest_update_main_quest_journal()
{
    int progress;

    noteadd("@QM[" + i18n::s.get("core.quest.journal.main.title") + "]");
    if (story_quest_progress("core.elona") >= 0 &&
        story_quest_progress("core.elona") < 30)
    {
        progress = 0;
    }
    if (story_quest_progress("core.elona") >= 30 &&
        story_quest_progress("core.elona") < 50)
    {
        progress = 1;
    }
    if (story_quest_progress("core.elona") >= 50 &&
        story_quest_progress("core.elona") < 60)
    {
        progress = 2;
    }
    if (story_quest_progress("core.elona") >= 60 &&
        story_quest_progress("core.elona") < 100)
    {
        progress = 3;
    }
    if (story_quest_progress("core.elona") >= 100 &&
        story_quest_progress("core.elona") < 110)
    {
        progress = 4;
    }
    if (story_quest_progress("core.elona") >= 110 &&
        story_quest_progress("core.elona") < 125)
    {
        progress = 5;
    }
    if (story_quest_progress("core.elona") >= 125 &&
        story_quest_progress("core.elona") < 180)
    {
        progress = 6;
    }
    if (story_quest_progress("core.elona") >= 180 &&
        story_quest_progress("core.elona") < 1000)
    {
        progress = 7;
    }
    s1 = i18n::s.get_enum("core.quest.journal.main.progress", progress);
    talk_conv(s1, 40 - en * 4);
    buff += s1;
    noteadd(""s);
}



void append_subquest_journal(int val0)
{
    if (val0 == 0)
    {
        noteadd(""s);
        noteadd("@QM[" + i18n::s.get("core.quest.journal.sub.title") + "]");
    }
    p = 0;
    if (story_quest_progress("core.putit_attacks") != 0)
    {
        s = i18n::s.get("core.quest.journal.sub.putit_attacks.title");
        p = story_quest_progress("core.putit_attacks");
        if (val0 == 1)
        {
            if (p >= 1000)
            {
                noteadd(
                    "[" + i18n::s.get("core.quest.journal.sub.done") + "]" + s);
            }
        }
    }
    if (val0 == 0)
    {
        if (p == 1)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.putit_attacks.progress", 0);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    if (val0 == 0)
    {
        if (p == 2)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.putit_attacks.progress", 1);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    p = 0;
    if (story_quest_progress("core.thieves_hideout") != 0)
    {
        s = i18n::s.get("core.quest.journal.sub.thieves_hideout.title");
        p = story_quest_progress("core.thieves_hideout");
        if (val0 == 1)
        {
            if (p >= 1000)
            {
                noteadd(
                    "[" + i18n::s.get("core.quest.journal.sub.done") + "]" + s);
            }
        }
    }
    if (val0 == 0)
    {
        if (p == 1)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.thieves_hideout.progress", 0);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    if (val0 == 0)
    {
        if (p == 2)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.thieves_hideout.progress", 1);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    p = 0;
    if (story_quest_progress("core.puppys_cave") != 0)
    {
        s = i18n::s.get("core.quest.journal.sub.puppys_cave.title");
        p = story_quest_progress("core.puppys_cave");
        if (val0 == 1)
        {
            if (p >= 1000)
            {
                noteadd(
                    "[" + i18n::s.get("core.quest.journal.sub.done") + "]" + s);
            }
        }
    }
    if (val0 == 0)
    {
        if (p == 1)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.puppys_cave.progress", 0);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    p = 0;
    if (story_quest_progress("core.nightmare") != 0)
    {
        s = i18n::s.get("core.quest.journal.sub.nightmare.title");
        p = story_quest_progress("core.nightmare");
        if (val0 == 1)
        {
            if (p >= 1000)
            {
                noteadd(
                    "[" + i18n::s.get("core.quest.journal.sub.done") + "]" + s);
            }
        }
    }
    if (val0 == 0)
    {
        if (p == 1)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.nightmare.progress", 0);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    if (val0 == 0)
    {
        if (p == 2)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.nightmare.progress", 1);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    if (val0 == 0)
    {
        if (p == 3)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.nightmare.progress", 2);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    p = 0;
    if (story_quest_progress("core.pael_and_her_mom") != 0)
    {
        s = i18n::s.get("core.quest.journal.sub.pael_and_her_mom.title");
        p = story_quest_progress("core.pael_and_her_mom");
        if (val0 == 1)
        {
            if (p >= 1000)
            {
                noteadd(
                    "[" + i18n::s.get("core.quest.journal.sub.done") + "]" + s);
            }
        }
    }
    if (val0 == 0)
    {
        if (p == 1)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.pael_and_her_mom.progress", 0);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    if (val0 == 0)
    {
        if (p == 2)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.pael_and_her_mom.progress", 1);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    if (val0 == 0)
    {
        if (p == 3)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.pael_and_her_mom.progress", 0);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    if (val0 == 0)
    {
        if (p == 4)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.pael_and_her_mom.progress", 1);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    if (val0 == 0)
    {
        if (p == 5)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.pael_and_her_mom.progress", 0);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    if (val0 == 0)
    {
        if (p == 6)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.pael_and_her_mom.progress", 1);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    if (val0 == 0)
    {
        if (p == 7)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.pael_and_her_mom.progress", 0);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    if (val0 == 0)
    {
        if (p == 8)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.pael_and_her_mom.progress", 1);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    if (val0 == 0)
    {
        if (p == 9)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.pael_and_her_mom.progress", 0);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    if (val0 == 0)
    {
        if (p == 10)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.pael_and_her_mom.progress", 1);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    p = 0;
    if (story_quest_progress("core.wife_collector") != 0)
    {
        s = i18n::s.get("core.quest.journal.sub.wife_collector.title");
        p = story_quest_progress("core.wife_collector");
        if (val0 == 1)
        {
            if (p >= 1000)
            {
                noteadd(
                    "[" + i18n::s.get("core.quest.journal.sub.done") + "]" + s);
            }
        }
    }
    if (val0 == 0)
    {
        if (p == 1)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.wife_collector.progress", 0);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    p = 0;
    if (story_quest_progress("core.cat_house") != 0)
    {
        s = i18n::s.get("core.quest.journal.sub.cat_house.title");
        p = story_quest_progress("core.cat_house");
        if (val0 == 1)
        {
            if (p >= 1000)
            {
                noteadd(
                    "[" + i18n::s.get("core.quest.journal.sub.done") + "]" + s);
            }
        }
    }
    if (val0 == 0)
    {
        if (p == 1)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.cat_house.progress", 0);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    if (val0 == 0)
    {
        if (p == 2)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.cat_house.progress", 1);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    p = 0;
    if (story_quest_progress("core.defense_line") != 0)
    {
        s = i18n::s.get("core.quest.journal.sub.defense_line.title");
        p = story_quest_progress("core.defense_line");
        if (val0 == 1)
        {
            if (p >= 1000)
            {
                noteadd(
                    "[" + i18n::s.get("core.quest.journal.sub.done") + "]" + s);
            }
        }
    }
    if (val0 == 0)
    {
        if (p == 1)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.defense_line.progress", 0);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    if (val0 == 0)
    {
        if (p == 2)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.defense_line.progress", 1);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    if (val0 == 0)
    {
        if (p == 3)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.defense_line.progress", 2);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    p = 0;
    if (story_quest_progress("core.novice_knight") != 0)
    {
        s = i18n::s.get("core.quest.journal.sub.novice_knight.title");
        p = story_quest_progress("core.novice_knight");
        if (val0 == 1)
        {
            if (p >= 1000)
            {
                noteadd(
                    "[" + i18n::s.get("core.quest.journal.sub.done") + "]" + s);
            }
        }
    }
    if (val0 == 0)
    {
        if (p == 1)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.novice_knight.progress", 0);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    if (val0 == 0)
    {
        if (p == 2)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.novice_knight.progress", 1);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    p = 0;
    if (story_quest_progress("core.kamikaze_attack") != 0)
    {
        s = i18n::s.get("core.quest.journal.sub.kamikaze_attack.title");
        p = story_quest_progress("core.kamikaze_attack");
        if (val0 == 1)
        {
            if (p >= 1000)
            {
                noteadd(
                    "[" + i18n::s.get("core.quest.journal.sub.done") + "]" + s);
            }
        }
    }
    if (val0 == 0)
    {
        if (p == 1)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.kamikaze_attack.progress", 0);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    if (val0 == 0)
    {
        if (p == 2)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.kamikaze_attack.progress", 1);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    if (val0 == 0)
    {
        if (p == 3)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.kamikaze_attack.progress", 2);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    p = 0;
    if (story_quest_progress("core.mias_dream") != 0)
    {
        s = i18n::s.get("core.quest.journal.sub.mias_dream.title");
        p = story_quest_progress("core.mias_dream");
        if (val0 == 1)
        {
            if (p >= 1000)
            {
                noteadd(
                    "[" + i18n::s.get("core.quest.journal.sub.done") + "]" + s);
            }
        }
    }
    if (val0 == 0)
    {
        if (p == 1)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.mias_dream.progress", 0);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    p = 0;
    if (story_quest_progress("core.rare_books") != 0)
    {
        s = i18n::s.get("core.quest.journal.sub.rare_books.title");
        p = story_quest_progress("core.rare_books");
        if (val0 == 1)
        {
            if (p >= 1000)
            {
                noteadd(
                    "[" + i18n::s.get("core.quest.journal.sub.done") + "]" + s);
            }
        }
    }
    if (val0 == 0)
    {
        if (p == 1)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.rare_books.progress", 0);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    p = 0;
    if (story_quest_progress("core.pyramid_trial") != 0)
    {
        s = i18n::s.get("core.quest.journal.sub.pyramid_trial.title");
        p = story_quest_progress("core.pyramid_trial");
        if (val0 == 1)
        {
            if (p >= 1000)
            {
                noteadd(
                    "[" + i18n::s.get("core.quest.journal.sub.done") + "]" + s);
            }
        }
    }
    if (val0 == 0)
    {
        if (p == 1)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.pyramid_trial.progress", 0);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    p = 0;
    if (story_quest_progress("core.red_blossom_in_palmia") != 0)
    {
        s = i18n::s.get("core.quest.journal.sub.red_blossom_in_palmia.title");
        p = story_quest_progress("core.red_blossom_in_palmia");
        if (val0 == 1)
        {
            if (p >= 1000)
            {
                noteadd(
                    "[" + i18n::s.get("core.quest.journal.sub.done") + "]" + s);
            }
        }
    }
    if (val0 == 0)
    {
        if (p == 1)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.red_blossom_in_palmia.progress", 0);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    if (val0 == 0)
    {
        if (p == 2)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.red_blossom_in_palmia.progress", 1);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    p = 0;
    if (story_quest_progress("core.ambitious_scientist") != 0)
    {
        s = i18n::s.get("core.quest.journal.sub.ambitious_scientist.title");
        p = story_quest_progress("core.ambitious_scientist");
        if (val0 == 1)
        {
            if (p >= 1000)
            {
                noteadd(
                    "[" + i18n::s.get("core.quest.journal.sub.done") + "]" + s);
            }
        }
    }
    if (val0 == 0)
    {
        if (p ==
            story_quest_progress("core.ambitious_scientist") *
                    (story_quest_progress("core.ambitious_scientist") < 6) +
                (story_quest_progress("core.ambitious_scientist") == 0))
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.ambitious_scientist.progress",
                0,
                (6 - story_quest_progress("core.ambitious_scientist")));
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    p = 0;
    if (story_quest_progress("core.sewer_sweeping") != 0)
    {
        s = i18n::s.get("core.quest.journal.sub.sewer_sweeping.title");
        p = story_quest_progress("core.sewer_sweeping");
        if (val0 == 1)
        {
            if (p >= 1000)
            {
                noteadd(
                    "[" + i18n::s.get("core.quest.journal.sub.done") + "]" + s);
            }
        }
    }
    if (val0 == 0)
    {
        if (p == 1)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.sewer_sweeping.progress", 0);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    if (val0 == 0)
    {
        if (p == 2)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.sewer_sweeping.progress", 1);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    p = 0;
    if (game()->guild.joining_mages_guild != 0)
    {
        s = i18n::s.get("core.quest.journal.sub.joining_mages_guild.title");
        p = game()->guild.joining_mages_guild;
        if (val0 == 1)
        {
            if (p >= 1000)
            {
                noteadd(
                    "[" + i18n::s.get("core.quest.journal.sub.done") + "]" + s);
            }
        }
    }
    if (val0 == 0)
    {
        if (p == 1)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.joining_mages_guild.progress",
                0,
                game()->guild.mages_guild_quota);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    p = 0;
    if (game()->guild.joining_thieves_guild != 0)
    {
        s = i18n::s.get("core.quest.journal.sub.joining_thieves_guild.title");
        p = game()->guild.joining_thieves_guild;
        if (val0 == 1)
        {
            if (p >= 1000)
            {
                noteadd(
                    "[" + i18n::s.get("core.quest.journal.sub.done") + "]" + s);
            }
        }
    }
    if (val0 == 0)
    {
        if (p == 1)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.joining_thieves_guild.progress", 0);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    p = 0;
    if (game()->guild.joining_fighters_guild != 0)
    {
        s = i18n::s.get("core.quest.journal.sub.joining_fighters_guild.title");
        p = game()->guild.joining_fighters_guild;
        if (val0 == 1)
        {
            if (p >= 1000)
            {
                noteadd(
                    "[" + i18n::s.get("core.quest.journal.sub.done") + "]" + s);
            }
        }
    }
    if (val0 == 0)
    {
        if (p == 1)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.joining_fighters_guild.progress",
                0,
                game()->guild.fighters_guild_quota,
                chara_db_get_name(*the_character_db.get_id_from_integer(
                    game()->guild.fighters_guild_target)));
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    p = 0;
    if (game()->guild.mages_guild_quota_recurring != 0)
    {
        s = i18n::s.get("core.quest.journal.sub.quota_mages_guild.title");
        p = game()->guild.mages_guild_quota_recurring;
        if (val0 == 1)
        {
            if (p >= 1000)
            {
                noteadd(
                    "[" + i18n::s.get("core.quest.journal.sub.done") + "]" + s);
            }
        }
    }
    if (val0 == 0)
    {
        if (p == 1)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.quota_mages_guild.progress",
                0,
                game()->guild.mages_guild_quota);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    p = 0;
    if (game()->guild.fighters_guild_quota_recurring != 0)
    {
        s = i18n::s.get("core.quest.journal.sub.quota_fighters_guild.title");
        p = game()->guild.fighters_guild_quota_recurring;
        if (val0 == 1)
        {
            if (p >= 1000)
            {
                noteadd(
                    "[" + i18n::s.get("core.quest.journal.sub.done") + "]" + s);
            }
        }
    }
    if (val0 == 0)
    {
        if (p == 1)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.quota_fighters_guild.progress",
                0,
                game()->guild.fighters_guild_quota,
                chara_db_get_name(*the_character_db.get_id_from_integer(
                    game()->guild.fighters_guild_target)));
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    p = 0;
    if (game()->guild.thieves_guild_quota_recurring != 0)
    {
        s = i18n::s.get("core.quest.journal.sub.quota_thieves_guild.title");
        p = game()->guild.thieves_guild_quota_recurring;
        if (val0 == 1)
        {
            if (p >= 1000)
            {
                noteadd(
                    "[" + i18n::s.get("core.quest.journal.sub.done") + "]" + s);
            }
        }
    }
    if (val0 == 0)
    {
        if (p == 1)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.quota_thieves_guild.progress",
                0,
                game()->guild.thieves_guild_quota);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    p = 0;
    if (story_quest_progress("core.minotaur_king") != 0)
    {
        s = i18n::s.get("core.quest.journal.sub.minotaur_king.title");
        p = story_quest_progress("core.minotaur_king");
        if (val0 == 1)
        {
            if (p >= 1000)
            {
                noteadd(
                    "[" + i18n::s.get("core.quest.journal.sub.done") + "]" + s);
            }
        }
    }
    if (val0 == 0)
    {
        if (p == 1)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.minotaur_king.progress", 0);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    if (val0 == 0)
    {
        if (p == 2)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.minotaur_king.progress", 1);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
    p = 0;
    if (story_quest_progress("core.little_sister") != 0)
    {
        s = i18n::s.get("core.quest.journal.sub.little_sister.title");
        p = story_quest_progress("core.little_sister");
        if (val0 == 1)
        {
            if (p >= 1000)
            {
                noteadd(
                    "[" + i18n::s.get("core.quest.journal.sub.done") + "]" + s);
            }
        }
    }
    if (val0 == 0)
    {
        if (p == 1)
        {
            s1 = i18n::s.get_enum(
                "core.quest.journal.sub.little_sister.progress", 0);
            talk_conv(s1, 40 - en * 4);
            buff += "("s + s + ")\n"s + s1;
            noteadd(""s);
        }
    }
}



void append_quest_item_journal()
{
    noteadd("[" + i18n::s.get("core.quest.journal.item.old_talisman") + "]");
    if (story_quest_progress("core.elona") >= 30)
    {
        noteadd(
            "[" + i18n::s.get("core.quest.journal.item.letter_to_the_king") +
            "]");
    }
    if (story_quest_progress("core.magic_stone_of_fool") != 0)
    {
        noteadd(
            "[" + i18n::s.get("core.quest.journal.item.fools_magic_stone") +
            "]");
    }
    if (story_quest_progress("core.magic_stone_of_king") != 0)
    {
        noteadd(
            "[" + i18n::s.get("core.quest.journal.item.kings_magic_stone") +
            "]");
    }
    if (story_quest_progress("core.magic_stone_of_sage") != 0)
    {
        noteadd(
            "[" + i18n::s.get("core.quest.journal.item.sages_magic_stone") +
            "]");
    }
}



void parse_quest_board_text(int val0)
{
    elona_vector1<std::string> buff2;
    notesel(buffboard);
    SDIM1(buff2);
    p = instr(buffboard, 0, s + ","s + i18n::s.get("core.meta.tag"));
    buff2 = strmid(buffboard, p, instr(buffboard, p, "%END"s));
    notesel(buff2);
    if (noteinfo() <= 1)
    {
        buff2 = "no txt"s;
        return;
    }
    p = rnd(noteinfo() - 1) + 1;
    noteget(buff2, p);
    p = instr(buff2, 0, ":"s);
    s(3) = strmid(buff2, 0, p);
    if (val0 == 2)
    {
        notesel(buff);
        return;
    }
    buff2 = strmid(buff2, p + 1, buff2(0).size() - p - 1);
    if (val0 != 2)
    {
        buff = buff2;
    }
}



void load_random_name_table()
{
    std::vector<std::string> lines;
    range::copy(
        fileutil::read_by_line(lua::resolve_path_for_mod(
            "<core>/locale/<LANGUAGE>/lazy/name.csv")),
        std::back_inserter(lines));

    const auto rows = lines.size();
    _random_name_table.first.resize(rows);
    _random_name_table.second.resize(rows);

    for (size_t i = 0; i < rows; ++i)
    {
        if (lines[i].empty())
            continue;
        const auto pair = strutil::split_on_string(lines[i], ",");
        _random_name_table.first[i] = pair.first;
        _random_name_table.second[i] = pair.second;
    }
}



std::string random_name()
{
    while (true)
    {
        if (const auto name = _random_name_internal())
        {
            return cnven(*name);
        }
    }
}



void load_random_title_table()
{
    std::vector<std::string> lines;
    range::copy(
        fileutil::read_by_line(lua::resolve_path_for_mod(
            "<core>/locale/<LANGUAGE>/lazy/ndata.csv")),
        std::back_inserter(lines));

    for (size_t i = 0; i < lines.size(); ++i)
    {
        const auto columns = strutil::split(lines[i], ',');
        for (size_t j = 0; j < 15; ++j)
        {
            if (j < columns.size())
            {
                _rnlist(j, i) = columns[j];
            }
            else
            {
                _rnlist(j, i) = "";
            }
        }
    }
}



std::string random_title_base(RandomTitleType type)
{
    while (true)
    {
        int row;
        int column;
        while (true)
        {
            row = rnd(_rnlist.j_size());
            column = rnd(14);
            if (_rnlist(column, row) != "")
            {
                break;
            }
        }

        const auto category = _rnlist(14, row);
        if (type == RandomTitleType::weapon ||
            type == RandomTitleType::living_weapon)
        {
            if (category == "具"s)
            {
                continue;
            }
        }

        auto title = _rnlist(column, row);
        if (jp)
        {
            if (column == 10 || column == 11)
            {
                if (rnd(5) == 0)
                {
                    column = 0;
                    if (rnd(2) == 0)
                    {
                        title += "の"s;
                    }
                }
                else
                {
                    const auto p = rnd(5);
                    if (p == 0)
                    {
                        title += "・オブ・"s;
                    }
                    if (p == 1)
                    {
                        return "ザ・"s + title;
                    }
                    if (p == 2)
                    {
                        title += "・"s;
                    }
                }
            }
            else if (column == 0 || column == 1)
            {
                title += "の"s;
                if (rnd(10) == 0)
                {
                    column = 10;
                }
            }
        }
        else
        {
            if (column == 0 || column == 1)
            {
                if (rnd(6) == 0)
                {
                    title += " of"s;
                }
                else if (rnd(6) == 0)
                {
                    return "the "s + title;
                }
            }
            title = cnven(title + " ");
        }

        bool success = false;
        int new_row;
        for (int _i = 0; _i < 100; ++_i)
        {
            new_row = rnd(_rnlist.j_size());
            if (new_row == row)
            {
                continue;
            }
            if (_rnlist(14, new_row) == category && category != "万能"s)
            {
                continue;
            }
            if (column < 10)
            {
                column = rnd(2);
            }
            else
            {
                column = rnd(2);
                column += 10;
            }
            if (_rnlist(column, new_row) == "")
            {
                continue;
            }
            success = true;
            break;
        }

        if (!success)
        {
            continue;
        }

        if (en)
        {
            _rnlist(column, new_row) = cnven(_rnlist(column, new_row));
        }

        title += _rnlist(column, new_row);
        if (strlen_u(title) >= 28)
        {
            continue;
        }

        return title;
    }
}



std::string random_title(RandomTitleType type)
{
    auto ret = random_title_base(type);

    if (type == RandomTitleType::party)
    {
        if (jp)
        {
            if (rnd(5))
            {
                ret += choice(std::initializer_list<const char*>{
                    "団",
                    "チーム",
                    "パーティー",
                    "の集い",
                    "の軍",
                    "アーミー",
                    "隊",
                    "の一家",
                    "軍",
                    "の隊",
                    "の団",
                });
            }
        }
        else
        {
            if (rnd(2))
            {
                ret = choice(std::initializer_list<const char*>{
                          "The army of ",
                          "The party of ",
                          "The house of ",
                          "Clan ",
                      }) +
                    ret;
            }
            else
            {
                ret += choice(std::initializer_list<const char*>{
                    " Clan",
                    " Party",
                    " Band",
                    " Gangs",
                    " Gathering",
                    " House",
                    " Army",
                });
            }
        }
    }

    return ret;
}



std::string cheer_up_message(int hours)
{
    auto message = i18n::s.get_enum_optional("core.ui.cheer_up_message", hours);

    if (message)
    {
        return *message;
    }

    return "";
}



void text_replace_tags_in_quest_board(optional_ref<const Character> client)
{
    while (1)
    {
        const int p0 = instr(buff, 0, "{"s);
        const int p1 = instr(buff, p0, "}"s);
        const int p2 = buff(0).size();
        if (p0 == -1)
        {
            break;
        }
        const auto tag = strmid(buff, p0 + 1, p1 - 1);
        const auto head = strmid(buff, 0, p0);
        const auto tail = strmid(buff, p0 + p1 + 1, p2 - p1 - p0);
        buff = head + replace_tag(tag, client) + tail;
    }
}



void text_replace_tags_in_quest_text(optional_ref<const Character> client)
{
    for (int cnt = 0; cnt < 20; ++cnt)
    {
        p(0) = instr(buff, 0, "{"s);
        p(1) = instr(buff, p, "}"s);
        p(2) = buff(0).size();
        if (p == -1)
        {
            break;
        }
        s(0) = strmid(buff, p + 1, p(1) - 1);
        s(1) = strmid(buff, 0, p);
        s(2) = strmid(buff, p + p(1) + 1, p(2) - p(1) - p);
        for (int cnt = 0; cnt < 1; ++cnt)
        {
            if (s == "client"s)
            {
                s = s(12);
                break;
            }
            if (s == "map"s)
            {
                s = s(11);
                break;
            }
            if (s == "ref"s)
            {
                s = s(10);
                break;
            }
            if (s == "you"s)
            {
                assert(client);
                s = _kimi(*client, 3);
                break;
            }
            if (s == "me"s)
            {
                assert(client);
                s = _ore(*client, 3);
                break;
            }
            if (s == "reward"s)
            {
                s = s(5);
                break;
            }
            if (s == "objective"s)
            {
                s = s(4);
                break;
            }
            if (s == "deadline"s)
            {
                s = nquestdate;
                break;
            }
            if (s == "player"s)
            {
                s = cdata.player().name;
                break;
            }
            if (s == "aka"s)
            {
                s = cdata.player().alias;
                break;
            }
            if (s == "npc"s)
            {
                assert(client);
                s = client->name;
                break;
            }
            if (s == "ある"s)
            {
                assert(client);
                s = _aru(*client, 3);
                break;
            }
            if (s == "う"s)
            {
                assert(client);
                s = _u(*client, 3);
                break;
            }
            if (s == "か"s)
            {
                assert(client);
                s = _ka(*client, 3);
                break;
            }
            if (s == "が"s)
            {
                assert(client);
                s = _ga(*client, 3);
                break;
            }
            if (s == "かな"s)
            {
                assert(client);
                s = _kana(*client, 3);
                break;
            }
            if (s == "だ"s)
            {
                assert(client);
                s = _da(*client, 3);
                break;
            }
            if (s == "よ"s)
            {
                assert(client);
                s = _yo(*client, 3);
                break;
            }
            if (s == "た"s)
            {
                assert(client);
                s = _ta(*client, 3);
                break;
            }
            if (s == "だな"s)
            {
                assert(client);
                s = _dana(*client, 3);
                break;
            }
            if (s == "だろ"s)
            {
                assert(client);
                s = _daro(*client, 3);
                break;
            }
            if (s == "たのむ"s)
            {
                assert(client);
                s = _tanomu(*client, 3);
                break;
            }
            if (s == "る"s)
            {
                assert(client);
                s = _ru(*client, 3);
                break;
            }
            if (s == "のだ"s)
            {
                assert(client);
                s = _nda(*client, 3);
                break;
            }
            if (s == "な"s)
            {
                assert(client);
                s = _na(*client, 3);
                break;
            }
            if (s == "くれ"s)
            {
                assert(client);
                s = _kure(*client, 3);
                break;
            }
            s = "Unknown Code"s;
        }
        buff = s(1) + s + s(2);
    }
}



std::string name(int chara_index)
{
    if (chara_index == 0)
    {
        return i18n::s.get("core.chara.you");
    }
    if (is_in_fov(cdata[chara_index]) == 0)
    {
        return i18n::s.get("core.chara.something");
    }
    if (cdata.player().blind != 0 ||
        (cdata[chara_index].is_invisible() == 1 &&
         cdata.player().can_see_invisible() == 0 &&
         cdata[chara_index].wet == 0))
    {
        return i18n::s.get("core.chara.something");
    }
    if (en)
    {
        const char first = cdata[chara_index].name[0];
        if (first == '\"' || first == '<')
        {
            return cdata[chara_index].name;
        }
        if (cdata[chara_index].has_own_name() == 0)
        {
            return "the "s + cdata[chara_index].name;
        }
    }
    return cdata[chara_index].name;
}



std::string txtitemoncell(int x, int y)
{
    const auto& item_info_memory = cell_data.at(x, y).item_info_memory;
    if (item_info_memory.is_empty())
    {
        return "";
    }

    const auto stack_count = item_info_memory.stack_count();
    if (stack_count < 0)
    {
        return i18n::s.get(
            "core.action.move.item_on_cell.many",
            cell_count_exact_item_stacks({x, y}));
    }

    const auto item_indice = item_info_memory.item_indice();
    std::string items_text;
    bool first = true;
    auto own_state = OwnState::none;
    for (const auto& item_index : item_indice)
    {
        if (item_index == 0)
            break;

        const auto item = item_index < 0
            ? inv_map()->at(
                  static_cast<InventorySlot>(0)) /* TODO phantom ref */
            : inv_map()->at(static_cast<InventorySlot>(item_index - 1));
        if (first)
        {
            first = false;
            own_state = item->own_state;
        }
        else
        {
            items_text += i18n::s.get("core.misc.and");
        }
        items_text += itemname(item.unwrap());
    }
    if (own_state <= OwnState::none)
    {
        return i18n::s.get("core.action.move.item_on_cell.item", items_text);
    }
    else if (own_state == OwnState::shelter)
    {
        return i18n::s.get(
            "core.action.move.item_on_cell.building", items_text);
    }
    else
    {
        return i18n::s.get(
            "core.action.move.item_on_cell.not_owned", items_text);
    }
}



void cnvbonus(int ability_id, int bonus)
{
    // TODO: i18n
    if (ability_id >= 50 && ability_id < 61)
    {
        if (bonus > 0)
        {
            buff += "　　"s + the_skill_db.get_text(ability_id, "name") +
                "耐性に <green>クラス"s + bonus / 50 + "<col>("s + bonus +
                ") のボーナス\n"s;
        }
        if (bonus < 0)
        {
            buff += "　　"s + the_skill_db.get_text(ability_id, "name") +
                "耐性に <red>クラス"s + bonus / 50 + "<col>("s + bonus +
                ") のマイナス修正\n"s;
        }
    }
    else
    {
        if (bonus > 0)
        {
            buff += "　　"s + the_skill_db.get_text(ability_id, "name") +
                "に <green>+"s + bonus + "<col> のボーナス\n"s;
        }
        if (bonus < 0)
        {
            buff += "　　"s + the_skill_db.get_text(ability_id, "name") +
                "に <red>"s + bonus + "<col> のマイナス修正\n"s;
        }
    }
}



std::string get_armor_class_name(const Character& chara)
{
    int id = chara_armor_class(chara);
    if (id == 169)
    {
        return i18n::s.get("core.item.armor_class.heavy");
    }
    else if (id == 170)
    {
        return i18n::s.get("core.item.armor_class.medium");
    }
    else
    {
        return i18n::s.get("core.item.armor_class.light");
    }
}



void csvsort(
    elona_vector1<std::string>& result,
    std::string line,
    int separator)
{
    elona_vector1<int> p_at_m40;
    p_at_m40(0) = 0;
    for (int cnt = 0; cnt < 40; ++cnt)
    {
        result(cnt) = "";
        getstr(result(cnt), line, p_at_m40(0), separator);
        if (strsize == 0)
        {
            break;
        }
        p_at_m40(0) += strsize;
    }
}



void lenfix(std::string& str, int length)
{
    int p_at_m89 = 0;
    p_at_m89 = length - strlen_u(str);
    if (p_at_m89 < 1)
    {
        p_at_m89 = 1;
    }
    for (int cnt = 0, cnt_end = (p_at_m89); cnt < cnt_end; ++cnt)
    {
        str += " "s;
    }
}



std::string fixtxt(const std::string& str, int length)
{
    std::string m_at_m104;
    m_at_m104 = ""s + str;
    if (strlen_u(str) < size_t(length))
    {
        while (1)
        {
            if (strlen_u(m_at_m104) >= size_t(length))
            {
                break;
            }
            m_at_m104 += " "s;
        }
    }
    else
    {
        m_at_m104 = ""s + strmid(str, 0, length);
    }
    return ""s + m_at_m104;
}



std::string getnpctxt(const std::string& tag, const std::string& default_text)
{
    int p_at_m189 = 0;
    p_at_m189 = instr(txtbuff, 0, tag);
    if (p_at_m189 == -1)
    {
        return default_text;
    }
    p_at_m189 += instr(txtbuff, p_at_m189, "\""s);
    if (p_at_m189 == -1)
    {
        return default_text;
    }
    return strmid(
        txtbuff,
        p_at_m189 + 1,
        clamp(instr(txtbuff, p_at_m189 + 1, "\""s), 0, 70));
}



std::string guildname()
{
    if (game()->guild.belongs_to_mages_guild)
    {
        return i18n::s.get("core.guild.mages.name");
    }
    else if (game()->guild.belongs_to_fighters_guild)
    {
        return i18n::s.get("core.guild.fighters.name");
    }
    else if (game()->guild.belongs_to_thieves_guild)
    {
        return i18n::s.get("core.guild.thieves.name");
    }
    else
    {
        return i18n::s.get("core.guild.none.name");
    }
}



void initialize_rankn()
{
    SDIM4(rankn, 30, 11, 9);

    for (int category = 0; category < 9; category++)
    {
        if (category == 7)
        {
            // Skips the 7th row because there are no defined locale resources.
            continue;
        }
        for (int rank = 0; rank < 11; rank++)
        {
            rankn(rank, category) =
                i18n::s.get_enum("core.rank._"s + category, rank);
        }
    }

    DIM2(ranknorma, 9);
    ranknorma(0) = 20;
    ranknorma(1) = 60;
    ranknorma(2) = 45;
    ranknorma(6) = 30;
}



std::string ranktitle(int rank_id)
{
    int rank_value = game()->ranks.at(rank_id) / 100;
    if (rank_value == 1)
    {
        return rankn(0, rank_id);
    }
    if (rank_value <= 5)
    {
        return rankn(1, rank_id);
    }
    if (rank_value <= 10)
    {
        return rankn(2, rank_id);
    }
    if (rank_value <= 80)
    {
        return rankn(rank_value / 15 + 3, rank_id);
    }
    return rankn(9, rank_id);
}



std::string txttargetlevel(
    const Character& base_chara,
    const Character& target_chara)
{
    const int x = base_chara.level;
    const int y = target_chara.level;

    int danger;
    if (x * 20 < y)
        danger = 10;
    else if (x * 10 < y)
        danger = 9;
    else if (x * 5 < y)
        danger = 8;
    else if (x * 3 < y)
        danger = 7;
    else if (x * 2 < y)
        danger = 6;
    else if (x * 3 / 2 < y)
        danger = 5;
    else if (x < y)
        danger = 4;
    else if (x / 3 * 2 < y)
        danger = 3;
    else if (x / 2 < y)
        danger = 2;
    else if (x / 3 < y)
        danger = 1;
    else
        danger = 0;

    return i18n::s.get_enum("core.action.target.level", danger, target_chara);
}



void txttargetnpc(int x, int y)
{
    int dy_ = 0;
    int i_ = 0;
    int p_ = 0;
    dy_ = 0;
    font(14 - en * 2);
    if (!fov_los(cdata.player().position, {x, y}) ||
        dist(cdata.player().position, x, y) > cdata.player().fov_range / 2)
    {
        bmes(
            i18n::s.get("core.action.target.out_of_sight"),
            100,
            windowh - inf_verh - 45 - dy_ * 20);
        ++dy_;
        cansee = 0;
        return;
    }
    if (cell_data.at(x, y).chara_index_plus_one != 0)
    {
        i_ = cell_data.at(x, y).chara_index_plus_one - 1;
        if (cdata[i_].is_invisible() == 0 ||
            cdata.player().can_see_invisible() || cdata[i_].wet)
        {
            s = txttargetlevel(cdata.player(), cdata[i_]);
            bmes(s, 100, windowh - inf_verh - 45 - dy_ * 20);
            ++dy_;
            bmes(
                i18n::s.get(
                    "core.action.target.you_are_targeting",
                    cdata[i_],
                    dist(cdata.player().position, cdata[i_].position)),
                100,
                windowh - inf_verh - 45 - dy_ * 20);
            ++dy_;
        }
    }
    if (!cell_data.at(x, y).item_info_memory.is_empty())
    {
        bmes(txtitemoncell(x, y), 100, windowh - inf_verh - 45 - dy_ * 20);
        ++dy_;
    }
    if (cell_data.at(x, y).feats != 0)
    {
        if (map_data.type == mdata_t::MapType::world_map)
        {
            if (cell_data.at(x, y).feats / 1000 % 100 == 15)
            {
                p_ = cell_data.at(x, y).feats / 100000 % 100 +
                    cell_data.at(x, y).feats / 10000000 * 100;
                bmes(
                    mapname(p_, true), 100, windowh - inf_verh - 45 - dy_ * 20);
                ++dy_;
            }
            if (cell_data.at(x, y).feats / 1000 % 100 == 34)
            {
                bmes(
                    txtbuilding(
                        cell_data.at(x, y).feats / 100000 % 100,
                        cell_data.at(x, y).feats / 10000000),
                    100,
                    windowh - inf_verh - 45 - dy_ * 20);
                ++dy_;
            }
        }
    }
    cansee = 1;
}

} // namespace elona
