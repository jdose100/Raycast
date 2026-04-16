

#include "game/config.h"
bool is_out_of_bounse(entity_t *seva)
{
    if (map[(int)seva->position.x][(int)seva->position.y] != 0)
        return true;
    return false;
}
entity_status_t entity_update(struct entity_t *self)
{
    if (is_out_of_bounse(self))
        return ENTITY_STATUS_WANT_DESTROY;  // это вызовет self->vtable.destroy
                                            // когда надо
    return ENTITY_STATUS_OK;
}