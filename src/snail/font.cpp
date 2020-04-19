#include "font.hpp"

#include <tuple>
#include <unordered_map>

#include "application.hpp"



namespace elona
{
namespace snail
{

Font::Font(const fs::path& filepath, int size, Style style)
    : _filepath(filepath)
    , _size(size)
    , _style(style)
    , _ptr(::FC_CreateFont(), ::FC_FreeFont)
{
    ::FC_LoadFont(
        _ptr.get(),
        Application::instance().get_renderer().ptr(),
        filepathutil::to_utf8_path(filepath).c_str(),
        size,
        ::FC_MakeColor(0, 0, 0, 255),
        static_cast<int>(style));
}

} // namespace snail
} // namespace elona
