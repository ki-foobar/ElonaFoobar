#pragma once

#include <string>
#include <vector>

#include "data/types/type_class.hpp"


namespace elona
{

struct Character;



void class_init_chara(Character& chara, data::InstanceId class_id);
std::string class_get_name(data::InstanceId class_id);
int class_get_equipment_type(data::InstanceId class_id);
int class_get_item_type(data::InstanceId class_id);
std::vector<std::reference_wrapper<const ClassData>> class_get_available(
    bool is_extra_class);

} // namespace elona
