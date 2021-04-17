#include <string>
#include <vector>

#include "data/id.hpp"



namespace elona
{

void proc_random_event();

/**
 * Show random event window. If g_config.skiprandevents() is enabled and
 * choices has only one choice, doesn't show a window and returns -1
 * immediately.
 * @param[in] title
 * @param[in] text
 * @param[in] choices The list of choices whose size is not 0.
 * @param[in] background_image_id
 * @return The index of the selected choice in choices. If the event is
 * skipped, returns -1.
 */
int show_random_event_window(
    const std::string& title,
    const std::string& text,
    const std::vector<std::string> choices,
    data::InstanceId background_image_id);

} // namespace elona
