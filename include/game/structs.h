#pragma once

#include <stddef.h>
#include <stdint.h>

#include "ext_math.h"

//! Камера.
typedef struct {
    double dir_x;  //< Угол поворота в радиантах.
    vec2_t pos;    //< Позиция.

    //! Смещение по y для просмотра вверх, вниз.
    //! Не является углом, поэтому не direction.
    float rot_y;
} camera_t;

//! Представление игрока.
typedef struct {
    vec2_t pos;    //< Позиция.
    double dir_x;  //< Угол поворота в радиантах.

    //! Смещение по y для просмотра вверх, вниз.
    //! Не является углом, поэтому не direction.
    float rot_y;
} player_t;

//! \private \brief Данные об экране.
struct __screen_config_data {
    uint16_t width;        //< width
    uint16_t height;       //< height
    uint16_t half_width;   //< half width
    uint16_t half_height;  //< half height
};

typedef enum entity_status_t entity_status_t;

//! Структура сущности.
typedef struct entity_t {
    vec2_t position;  //< Position.
    double rotation;  //< Rotation in radians.
    void *user_data;  //< User data.
    size_t id;

    double _distance;  //< Дистанция от камеры до сущности.

    struct {
        [[gnu::nonnull(1)]] entity_status_t (*init)(struct entity_t *self);
        [[gnu::nonnull(1)]] void (*destroy)(struct entity_t *self);
        [[gnu::nonnull(1)]] entity_status_t (*update)(struct entity_t *self);
    } vtable;  // Entity's functions.
} entity_t;

//! Состояние сущности.
enum entity_status_t {
#define X(name) ENTITY_STATUS_##name
    X(OK),            //< Entity is ok.
    X(ERROR) = 2,     //< Entity have an error.
    X(WANT_DESTROY),  //< Entity want's destroy.
#undef X
};

/* --- DEFAULT ENTITY FUNCTIONS --- */
#ifdef __cplusplus
extern "C" {
#endif

[[maybe_unused]] static entity_status_t entity_default_init(entity_t *)
{ return ENTITY_STATUS_OK; }
[[maybe_unused]] static void entity_default_destroy(entity_t *) {}
[[maybe_unused]] static entity_status_t entity_default_update(entity_t *)
{ return ENTITY_STATUS_OK; }

#define ENTITY_DEFAULT_VTABLE                                                  \
    {entity_default_init, entity_default_, destroy, entity_default_update}

#ifdef __cplusplus
}  // extern "C"
#endif
