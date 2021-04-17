#pragma once

namespace elona
{
namespace mdata_t
{

enum class MapType
{
    world_map = 1,
    guild = 2,
    town = 3,
    field = 4,
    player_owned = 5,
    shelter = 6,
    temporary = 7,
    dungeon = 20,
    dungeon_tower = 21,
    dungeon_forest = 22,
    dungeon_castle = 23,
};

// FIXME: Delete them for secure comparison.
inline bool operator==(int lhs, MapType rhs)
{
    return lhs == static_cast<int>(rhs);
}

inline bool operator==(MapType lhs, int rhs)
{
    return rhs == lhs;
}

inline bool operator!=(int lhs, MapType rhs)
{
    return !(lhs == rhs);
}

inline bool operator!=(MapType lhs, int rhs)
{
    return !(lhs == rhs);
}



inline bool is_nefia(MapType type)
{
    switch (type)
    {
    case MapType::dungeon:
    case MapType::dungeon_tower:
    case MapType::dungeon_forest:
    case MapType::dungeon_castle: return true;
    default: return false;
    }
}



// TODO: Delete it!
inline bool is_nefia(int type)
{
    return is_nefia(static_cast<MapType>(type));
}



enum class MapId
{
    none = 0,
    fields = 2,
    lesimas = 3,
    north_tyris = 4,
    vernis = 5,
    arena = 6,
    your_home = 7,
    random_dungeon = 8,
    test_site = 9,
    lumiest_graveyard = 10,
    port_kapul = 11,
    yowyn = 12,
    quest = 13,
    derphy = 14,
    palmia = 15,
    tower_of_fire = 16,
    crypt_of_the_damned = 17,
    ancient_castle = 18,
    dragons_nest = 19,
    truce_ground = 20,
    cyber_dome = 21,
    fort_of_chaos_beast = 22,
    fort_of_chaos_machine = 23,
    fort_of_chaos_collapsed = 24,
    larna = 25,
    mountain_pass = 26,
    puppy_cave = 27,
    yeeks_nest = 28,
    mansion_of_younger_sister = 29,
    shelter_ = 30,
    ranch = 31,
    embassy = 32,
    noyel = 33,
    miral_and_garoks_workshop = 34,
    show_house = 35,
    lumiest = 36,
    pyramid = 37,
    minotaurs_nest = 38,
    your_dungeon = 39,
    pet_arena = 40,
    jail = 41,
    the_void = 42,
    north_tyris_south_border = 43,
    south_tyris = 44,
    south_tyris_north_border = 45,
    the_smoke_and_pipe = 46,
    test_world = 47,
    test_world_north_border = 48,

    museum = 101,
    shop = 102,
    crop = 103,
    storage_house = 104,

    debug_map = 499,
};

// FIXME: Delete them for secure comparison.
inline bool operator==(int lhs, MapId rhs)
{
    return lhs == static_cast<int>(rhs);
}

inline bool operator==(MapId lhs, int rhs)
{
    return rhs == lhs;
}

inline bool operator!=(int lhs, MapId rhs)
{
    return !(lhs == rhs);
}

inline bool operator!=(MapId lhs, int rhs)
{
    return !(lhs == rhs);
}

} // namespace mdata_t

} // namespace elona
