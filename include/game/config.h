#pragma once

#include "structs.h"

#ifdef __cplusplus
extern "C" {
#endif

// --- Save GCC and clang diagnostic --- //
#ifdef __GNUC__
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wold-style-declaration"
#elifdef __clang__
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wgnu-folding-constant"
#endif

static constexpr ivec2_t map_size = {.x = 12, .y = 10};
static int map[map_size.x][map_size.y] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

static constexpr int title_size = 64;
static constexpr int wall_height = 64;

static constexpr double fov = 60;
static constexpr double half_fov = fov / 2;

static auto const title = "Raycast game";

static constexpr struct __screen_config_data screen = {
    .width = 800,
    .height = 600,
    .half_width = 800 / 2,
    .half_height = 600 / 2,
};

//! \important Не менять!
static double projection_plane_direction;

[[maybe_unused]] static player_t player = {
    .pos = {.x = 2, .y = 2},
};

static camera_t main_camera = {
    .pos = {.x = 4 * title_size, .y = 4 * title_size},
    .angle = 0,
};
constexpr double camera_speed = 2;

// --- Recovery GCC and clang diagnostics --- //
#ifdef __GNUC__
    #pragma GCC diagnostic pop
#elif __clang__
    #pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}
#endif
