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

constexpr ivec2_t map_size = {.x = 12, .y = 12};
constexpr unsigned int map[map_size.y][map_size.x] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1},
};

//! Проверяет, находится ли точка на карте.
static inline bool point_on_map(const unsigned int x, const unsigned int y)
{ return x < map_size.x && y < map_size.y; }

#define X(name, index_on_map) constexpr unsigned int map_texture_##name##_idx = index_on_map - 1;

X(brick_wall, 1);
X(flower_brick_wall, 2);

#undef X

// FOV = degree * (M_PI / 180) - это перевод градусов в радианты
// для угла degree в градусах.
constexpr double fov = 90 * (M_PI / 180);
constexpr double half_fov = fov / 2;

static auto const title = "Raycast game";

static struct __screen_config_data screen = {
    .width = 800,
    .height = 600,
    .half_width = 800 / 2,
    .half_height = 600 / 2,
};

static player_t player = {
    .pos = {.x = 10, .y = 10},
    .dir = 0,
};

static camera_t main_camera = {};

// --- Recovery GCC and clang diagnostics --- //
#ifdef __GNUC__
    #pragma GCC diagnostic pop
#elif __clang__
    #pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}  // extern "C"
#endif
