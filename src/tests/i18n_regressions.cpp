#include <sstream>

#include "../elona/i18n.hpp"
#include "../elona/item.hpp"
#include "../elona/itemgen.hpp"
#include "../elona/testing.hpp"
#include "../elona/ui.hpp"
#include "../elona/variables.hpp"
#include "../thirdparty/catch2/catch.hpp"
#include "tests.hpp"

using namespace std::literals::string_literals;
using namespace elona;

TEST_CASE("test foodname", "[I18N: Regressions]")
{
    testing::start_in_debug_map();
    testing::set_japanese();

    {
        const auto item = testing::create_item(260);
        update_slight();

        item->param2 = 1;
        REQUIRE(i18n::s.format("{itemname($1)}", item) == "恐怖のパン");
        item->param2 = 8;
        REQUIRE(i18n::s.format("{itemname($1)}", item) == "カレーパン");
    }
    {
        const auto item = testing::create_item(573);
        update_slight();

        item->param2 = 1;
        REQUIRE(i18n::s.format("{itemname($1)}", item) == "グロテスクな鳥の卵");
        item->param2 = 8;
        REQUIRE(
            i18n::s.format("{itemname($1)}", item) == "鳥のレアチーズケーキ");
        item->subname = 3;
        REQUIRE(
            i18n::s.format("{itemname($1)}", item) == "プチのレアチーズケーキ");
    }
}
