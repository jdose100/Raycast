#pragma once

#include <sokol_gfx.h>
#include <sokol_log.h>

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
} _textures[2] = {};

// --- FUNCTIONS --- //

//! Инициализация движка графики.
static void graphics_init(void)
{
    const char *texture_wall1 = "assets/textures/wall_1.png";
    const char *texture_wall2 = "assets/textures/wall_2.png";

    if (load_texture(&_textures[0].view, texture_wall1) != LOAD_TEXTURE_OK) {
        slog("start, load image", SLOG_PANIC, 0, "Error while loading image");
    } else if (load_texture(&_textures[1].view, texture_wall2) != LOAD_TEXTURE_OK) {
        slog("start, load image", SLOG_PANIC, 0, "Error while loading image");
    }

    _textures[0].sampler = sg_make_sampler(&(sg_sampler_desc){});
    _textures[1].sampler = sg_make_sampler(&(sg_sampler_desc){});
}

#ifdef __cplusplus
}
#endif
