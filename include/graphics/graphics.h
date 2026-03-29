#pragma once

#include <sokol_gfx.h>
#include <sokol_log.h>

#include "game/config.h"
// #include "graphics/raycast.h"
#include "graphics/textures.h"

/*!
    \page raycast page title
*/

#ifdef __cplusplus
extern "C" {
#endif

static struct {
    sg_view view;
    sg_sampler sampler;
} _textures[10] = {};

#define _textures_countof (sizeof(_textures) / sizeof(_textures[0]))

// --- FUNCTIONS --- //

//! Инициализация движка графики.
static void graphics_init(void)
{
    const char texture_wall1[] = "assets/textures/wall_1.png";
    const char texture_wall2[] = "assets/textures/wall_2.png";

#define X(idx, path)                                                                               \
    if (load_texture(&_textures[idx].view, path) != LOAD_TEXTURE_OK) {                             \
        slog("start, load image", SLOG_PANIC, 0, "Error while loading image");                     \
    }

    X(map_texture_brick_wall_idx, texture_wall2);
    X(map_texture_flower_brick_wall_idx, texture_wall1);
#undef X

    _textures[0].sampler = sg_make_sampler(&(sg_sampler_desc){});
    _textures[1].sampler = sg_make_sampler(&(sg_sampler_desc){});
}

[[maybe_unused]] static void drawing() {
    // _world_drawing();
} 

#ifdef __cplusplus
} // extern "C"
#endif
