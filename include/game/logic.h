#pragma once

#include "data.h"

#ifdef __cplusplus
extern "C" {
#endif

static bool game_add_entity(struct entity_t entity)
{
    if (entity.vtable.init == NULL) {
        entity.vtable.init = entity_default_init;
    }

    if (entity.vtable.destroy == NULL) {
        entity.vtable.destroy = entity_default_destroy;
    }

    if (entity.vtable.update == NULL) {
        entity.vtable.update = entity_default_update;
    }

    return cc_push(&game_data.entities, entity) != NULL;
}

static void game_init(void)
{ game_data.entities = cc_initialized(&game_data.entities); }

static void game_deinit(void)
{
    cc_for_each(&game_data.entities, entity) entity->vtable.destroy(entity);
    cc_cleanup(&game_data.entities);
}

static void game_update(void)
{
    for (size_t i = 0; i < cc_size(&game_data.entities); i++) {
        entity_t *entity = cc_get(&game_data.entities, i);

        auto const status = entity->vtable.update(entity);
        if (status == ENTITY_STATUS_ERROR ||
            status == ENTITY_STATUS_WANT_DESTROY) {
            entity->vtable.destroy(entity);
            cc_erase(&game_data.entities, i);
        }
    }
}

#ifdef __cplusplus
}  // extern "C"
#endif
