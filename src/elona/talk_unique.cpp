#include "character.hpp"
#include "dialog.hpp"
#include "talk.hpp"
#include "variables.hpp"



namespace elona
{

TalkResult talk_unique(Character& speaker)
{
    listmax = 0;

    const auto& dialog_id = the_character_db[speaker.id]->dialog_id;

    if (dialog_id)
    {
        dialog_start(speaker, *dialog_id);
        return TalkResult::talk_end;
    }

    return TalkResult::talk_ignored;
}

} // namespace elona
