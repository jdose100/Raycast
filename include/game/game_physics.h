#include <sokol_debugtext.h>
#include "game/config.h"
#include "game/move_physics.h"

bool is_out_of_bounse(entity_t *seva)
{
    if (map[(int)seva->position.x][(int)seva->position.y] != 0)
        return true;
    return false;
}
entity_status_t entity_update(struct entity_t *self)
{
    sdtx_printf("t_hp: %i\n", self->hp);
    if (self->forced_movement_timer != 0) {
        self->hp -= (int)move_forward_Е(
            self, self->forced_movement_dir,
            (double)self->forced_movement_timer * self->light);
        if (self->forced_movement_timer != 0)
            self->forced_movement_timer--;
    }
    if (self->hp <= 0)
        return ENTITY_STATUS_WANT_DESTROY;
    if (is_out_of_bounse(self))
        return ENTITY_STATUS_WANT_DESTROY;  // это вызовет self->vtable.destroy
                                            // // когда надо
    return ENTITY_STATUS_OK;
}

