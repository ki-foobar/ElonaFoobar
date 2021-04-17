#include "profile.hpp"



namespace elona
{
namespace profile
{

bool is_valid_id(const ProfileId& profile_id)
{
    const std::regex pattern{"[0-9A-Za-z_]+"};
    return std::regex_match(profile_id, pattern) &&
        fs::is_portable_path(fs::u8path(profile_id));
}



bool is_reserved_id(const ProfileId& profile_id)
{
    return profile_id == default_profile_id;
}



bool exists(const ProfileId& profile_id)
{
    const auto dir = get_dir_for(profile_id);
    return fs::exists(dir) && fs::is_directory(dir);
}



fs::path get_dir_for(const ProfileId& profile_id)
{
    assert(is_valid_id(profile_id));
    return filesystem::dirs::profile_root() / fs::u8path(profile_id);
}

} // namespace profile
} // namespace elona
