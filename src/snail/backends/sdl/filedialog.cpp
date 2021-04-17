#include "../../filedialog.hpp"

#include <cstdlib>

#include <type_traits>

#include "../../../thirdparty/nfd/nfd.h"
#include "../../../util/scope_guard.hpp"



namespace elona
{
namespace snail
{

static_assert(
    std::is_same_v<u8string::value_type, nfdchar_t>,
    "NFD only accepts UTF-8 strings.");



FileDialogResult OpenFileDialog::show()
{
    // `out` is dynamically allocated by `NFD_OpenDialogMultiple()`. Free it at
    // the end if a user successfully selected something.
    nfdchar_t* out = nullptr;
    const auto result_type = ::NFD_OpenDialog(
        filter_list.c_str(), default_path.to_u8string().c_str(), &out);

    lib::scope_guard to_free([=]() {
        if (result_type == NFD_OKAY)
        {
            std::free(out);
        };
    });

    // TODO: throws exception if result type is error?
    return {static_cast<FileDialogResultType>(result_type), fs::u8path(out)};
}



FileDialogResultMultiple OpenMultipleFileDialog::show()
{
    // `out` is dynamically allocated by `NFD_OpenDialogMultiple()`. Free it at
    // the end if a user successfully selected something.
    nfdpathset_t out;
    nfdpathset_t* outp = &out;
    const auto result_type = ::NFD_OpenDialogMultiple(
        filter_list.c_str(), default_path.to_u8string().c_str(), outp);

    lib::scope_guard to_free([=]() {
        if (result_type == NFD_OKAY)
        {
            ::NFD_PathSet_Free(outp);
        };
    });

    std::vector<fs::path> list;
    const auto count = ::NFD_PathSet_GetCount(outp);
    for (size_t i = 0; i < count; ++i)
    {
        const auto each_path = ::NFD_PathSet_GetPath(outp, i);
        list.push_back(fs::u8path(each_path));
    }

    // TODO: throws exception if result type is error?
    return {static_cast<FileDialogResultType>(result_type), list};
}



FileDialogResult SaveFileDialog::show()
{
    // `out` is dynamically allocated by `NFD_OpenDialogMultiple()`. Free it at
    // the end if a user successfully selected something.
    nfdchar_t* out = nullptr;
    const auto result_type = ::NFD_SaveDialog(
        filter_list.c_str(), default_path.to_u8string().c_str(), &out);

    lib::scope_guard to_free([=]() {
        if (result_type == NFD_OKAY)
        {
            std::free(out);
        };
    });

    // TODO: throws exception if result type is error?
    return {static_cast<FileDialogResultType>(result_type), fs::u8path(out)};
}



FileDialogResult OpenFolderDialog::show()
{
    // `out` is dynamically allocated by `NFD_OpenDialogMultiple()`. Free it at
    // the end if a user successfully selected something.
    nfdchar_t* out = nullptr;
    const auto result_type =
        ::NFD_PickFolder(default_path.to_u8string().c_str(), &out);

    lib::scope_guard to_free([=]() {
        if (result_type == NFD_OKAY)
        {
            std::free(out);
        };
    });

    // TODO: throws exception if result type is error?
    return {static_cast<FileDialogResultType>(result_type), fs::u8path(out)};
}

} // namespace snail
} // namespace elona
