#include "../elona/i18n.hpp"

#include <sstream>

#include "../elona/character.hpp"
#include "../elona/item.hpp"
#include "../elona/itemgen.hpp"
#include "../elona/lua_env/mod_manager.hpp"
#include "../elona/testing.hpp"
#include "../elona/ui.hpp"
#include "../elona/variables.hpp"
#include "../thirdparty/catch2/catch.hpp"
#include "tests.hpp"

using namespace std::literals::string_literals;
using namespace elona;



namespace
{

i18n::Store load(const std::string& str)
{
    i18n::Store store;

    auto& mod_mgr = lua::lua->get_mod_manager();
    if (!mod_mgr.get_mod("test"))
    {
        REQUIRE_NOTHROW(mod_mgr.load_testing_mod_from_script("test", ""));
    }
    REQUIRE_NOTHROW(store.load_from_string(str, "test"));
    return store;
}

} // namespace



TEST_CASE("test formats", "[I18N: Formatting]")
{
    REQUIRE(i18n::s.format("{$1}", 1) == "1"s);
    REQUIRE(i18n::s.format("{$1}", "foo"s) == "foo"s);
    REQUIRE(i18n::s.format("{$1} {$2}", "foo"s, 2) == "foo 2"s);
    REQUIRE(i18n::s.format("{$1} {$1}", "foo"s, 2) == "foo foo"s);
    REQUIRE(
        i18n::s.format("{$1} {$2}", "foo"s) == "foo <missing argument #2>"s);
    REQUIRE(i18n::s.format("You see {$1}.", "Palmia") == "You see Palmia."s);
    REQUIRE(
        i18n::s.format("You see {$1} the {$2}.", "Adam", "rock thrower") ==
        "You see Adam the rock thrower."s);
}



TEST_CASE("test format chara", "[I18N: Formatting]")
{
    testing::start_in_debug_map();
    Character& chara = testing::create_chara(charaid2int(PUTIT_PROTO_ID), 4, 8);

    REQUIRE(
        i18n::s.format("{$1}", chara) ==
        "LuaCharacter("s + chara.obj_id.to_string() + ")"s);

    REQUIRE(i18n::s.format("{name($1)}", chara) == "何か"s);
    REQUIRE(i18n::s.format("{basename($1)}", chara) == "プチ"s);
    REQUIRE(i18n::s.format("{basename($1)}: {$2}", chara, 42) == "プチ: 42");
}



TEST_CASE("test format item", "[I18N: Formatting]")
{
    // This test is disabled until bug #1631 is fixed.
#if 0
    testing::start_in_debug_map();
    Item& i = testing::create_item(PUTITORO_PROTO_ID, 3);

    REQUIRE(
        i18n::s.format("{$1}", i) ==
        "LuaItem("s + i.obj_id.to_string() + ")"s);

    REQUIRE(i18n::s.format("{itemname($1)}", i) == "3個のプチトロ"s);
    REQUIRE(i18n::s.format("{itembasename($1)}", i) == "プチトロ"s);
#endif
}



TEST_CASE("test i18n store literal", "[I18N: Store]")
{
    i18n::Store store = load(R"(
local I18N = require("core.I18N")

I18N.add {
   foo = "bar"
}
)");

    REQUIRE(store.get("test.foo") == "bar");
    REQUIRE(store.get("test.baz") == "<Unknown ID: test.baz>");
}



TEST_CASE("test i18n store nested literal", "[I18N: Store]")
{
    i18n::Store store = load(R"(
local I18N = require("core.I18N")

I18N.add {
   foo = {
      bar = "baz"
   }
}
)");

    REQUIRE(store.get("test.foo.bar") == "baz");
}



TEST_CASE("test i18n store multiple nested literals", "[I18N: Store]")
{
    i18n::Store store = load(R"(
local I18N = require("core.I18N")

I18N.add {
   foo = {
      bar = "baz"
   },
   hoge = {
      fuga = "piyo"
   }
}
)");

    REQUIRE(store.get("test.foo.bar") == "baz");
    REQUIRE(store.get("test.hoge.fuga") == "piyo");
}



TEST_CASE("test i18n store enum", "[I18N: Store]")
{
    i18n::Store store = load(R"(
local I18N = require("core.I18N")

I18N.add {
   foo = {
      _1 = "bar",
      _2 = "baz",
   }
}
)");

    REQUIRE(store.get_enum("test.foo", 1) == "bar");
    REQUIRE(store.get_enum("test.foo", 2) == "baz");
}



TEST_CASE("test i18n store complex enum", "[I18N: Store]")
{
    i18n::Store store = load(R"(
local I18N = require("core.I18N")

I18N.add {
   foo = {
      _1 = {
         name = "bar"
      },
      _2 = {
         name = "baz"
      }
   }
}
)");

    REQUIRE(store.get_enum_property("test.foo", "name", 1) == "bar");
    REQUIRE(store.get_enum_property("test.foo", "name", 2) == "baz");
}



TEST_CASE("test i18n store interpolation", "[I18N: Store]")
{
    i18n::Store store = load(R"(
local I18N = require("core.I18N")

I18N.add {
   foo = "bar: {$1}"
}
)");

    REQUIRE(store.get("test.foo") == "bar: <missing argument #1>");
    REQUIRE(store.get("test.foo", 12) == "bar: 12");
    REQUIRE(store.get("test.foo", "baz") == "bar: baz");
    REQUIRE(store.get("test.foo", "baz", "hoge") == "bar: baz");
}



TEST_CASE("test i18n store multiple interpolation", "[I18N: Store]")
{
    i18n::Store store = load(R"(
local I18N = require("core.I18N")

I18N.add {
   foo = "{$2}: {$1}"
}
)");

    REQUIRE(
        store.get("test.foo") ==
        "<missing argument #2>: <missing argument #1>");
    REQUIRE(store.get("test.foo", 42) == "<missing argument #2>: 42");
    REQUIRE(store.get("test.foo", 12, "bar") == "bar: 12");
    REQUIRE(store.get("test.foo", "bar", "baz") == "baz: bar");
    REQUIRE(store.get("test.foo", "bar", "baz", "hoge") == "baz: bar");
}



TEST_CASE("test i18n store enum interpolation", "[I18N: Store]")
{
    i18n::Store store = load(R"(
local I18N = require("core.I18N")

I18N.add {
   foo = {
      _1 = {
         name = "bar: {$1}"
      },
      _2 = {
         name = "baz: {$1}"
      }
   }
}
)");

    REQUIRE(
        store.get_enum_property("test.foo", "name", 1, "dood") == "bar: dood");
    REQUIRE(
        store.get_enum_property("test.foo", "name", 2, "dood") == "baz: dood");
}



TEST_CASE("test i18n lists", "[I18N: Store]")
{
    i18n::Store store = load(R"(
local I18N = require("core.I18N")

I18N.add {
   foo = {"baz: {$1}"}
}
)");

    REQUIRE(store.get("test.foo", "dood") == "baz: dood");
}



TEST_CASE("test i18n halfwidth katakana", "[I18N: Store]")
{
    i18n::Store store = load(
        u8R"(
local I18N = require("core.I18N")

I18N.add {
   ether_disease = "ｴｰﾃﾙ病"
}
)");

    REQUIRE(store.get("test.ether_disease") == "ｴｰﾃﾙ病"s);
}



TEST_CASE("test loading i18n data from multiple sources", "[I18N: Store]")
{
    i18n::Store store;
    auto& mod_mgr = lua::lua->get_mod_manager();
    mod_mgr.load_testing_mod_from_file(
        testing::get_mods_path() / "test_i18n_a");
    mod_mgr.load_testing_mod_from_file(
        testing::get_mods_path() / "test_i18n_b");
    store.init();

    REQUIRE(store.get("test_i18n_a.test") == "こんばんは"s);
    REQUIRE(store.get("test_i18n_b.test") == "こんにちは"s);
}
