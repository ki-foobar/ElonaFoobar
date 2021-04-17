#pragma once

#include "eobject/forward.hpp"
#include "optional.hpp"



namespace elona
{

struct Character;
struct Item;



bool magic(
    Character& subject,
    Character& target,
    const OptionalItemRef& item = nullptr);

} // namespace elona
