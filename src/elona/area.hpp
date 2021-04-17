#pragma once

#include <vector>

#include "optional.hpp"
#include "position.hpp"
#include "time.hpp"



namespace elona
{


template <typename T>
struct elona_vector2;

struct MapDefData;



struct Area
{
    int type{};
    Position position;
    int entrance{};
    int tile_set{};
    int dungeon_prefix{};
    int visited_deepest_level{};
    int turn_cost_base{};
    int deepest_level{};
    bool is_generated_every_time = true;
    int default_ai_calm{};
    int appearance{};
    int id{};
    int danger_level{};
    int tile_type{};
    int has_been_conquered{};
    bool is_indoor{};
    int winning_streak_in_arena{};
    int winning_streak_in_pet_arena{};
    int arena_random_seed{};
    time::Instant time_of_next_update_of_arena_random_seed{};
    time::Instant time_of_next_arena{};
    time::Instant time_of_next_rumble{};
    int quest_town_id{};
    bool christmas_festival{};
    int outer_map{};


    bool can_return_to();
    bool can_return_to_if_wizard();
    bool is_museum_or_shop();
    bool is_hidden_in_world_map();

    /**
     * Clear this Area.
     */
    void clear();
};



struct AreaData
{
    static constexpr size_t area_size = 500;


    AreaData()
        : areas(area_size)
    {
    }


    Area& operator[](int i)
    {
        return areas.at(static_cast<size_t>(i));
    }



    /**
     * Clear the whole of areas.
     */
    void clear();


    Area& current();


    optional_ref<MapDefData> current_mapdef();


private:
    std::vector<Area> areas;
};


extern AreaData area_data;


void initialize_adata();

void area_generate_from_mapdef(
    Area& area,
    const MapDefData& map,
    int outer_map,
    int x,
    int y);


} // namespace elona
