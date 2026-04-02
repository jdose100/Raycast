#pragma once

#include <sokol_gfx.h>
#include <sokol_gl.h>

#include "game/config.h"

#include "data.h"
#include "dda.h"

// #include "graphics/graphics.h"
#include "../graphics.h"
#include "graphics/raycast/entity.h"

static void raycast_draw()
{
    const int zbuffer_len = screen.width;
    struct _raycast_zbuffer_data zbuffer[zbuffer_len];

    /* Заполняем буфер глубины */

    // Стены.
    for (int i = 0; i < zbuffer_len; i++) {
        double distance, u;  // u - это координата x в текстуре.
        unsigned int map_cell;

        if (!_raycast_dda(i, &distance, &u, &map_cell)) {
            zbuffer[i].type = _ZBUFFER_DATA_EMPTY;
            continue;
        }

        zbuffer[i] = (struct _raycast_zbuffer_data){
            .depth = distance,
            .type = _ZBUFFER_DATA_TYPE_WALL,
            .data.wall = {.texture_idx = map_cell - 1, .u = u},
        };
    }

    // Сущности.
    _raycast_entities((size_t)zbuffer_len, zbuffer);

    // TODO: Сущности.

    /* Отрисовка. */
    sgl_enable_texture();
    defer { sgl_disable_texture(); }

    for (int i = 0; i < zbuffer_len; i++) {
        switch (zbuffer[i].type) {
            case _ZBUFFER_DATA_TYPE_WALL: {
                const unsigned int texture = zbuffer[i].data.wall.texture_idx;
                const double u = zbuffer[i].data.wall.u;
                const double distance = zbuffer[i].depth;

                /* Высота стенки на экране */
                const int line_height = (int)((screen.height / distance) / 2);

                const int start_y =
                    screen.half_height - line_height + (int)main_camera.rot_y;

                const int end_y =
                    screen.half_height + line_height + (int)main_camera.rot_y;

                /* Затемнение */
                const uint8_t calc_shade =
                    (uint8_t)(255 / (1.0 + distance * distance * 0.1));

                const uint8_t shade = calc_shade < 180 ? calc_shade : 180;

                /* Нормализация координат. */
                const float start_y_normalized =
                    (2.f * (float)start_y / screen.height) - 1.f;

                const float end_y_normalized =
                    (2.f * (float)end_y / screen.height) - 1.f;

                const float x_normalized =
                    (2.f * (float)i / screen.width) - 1.f;

                /* Отрисовка */
                if (texture < _textures_countof)
                    sgl_texture(_textures[texture].view,
                                _textures[texture].sampler);

                sgl_begin_lines();  // clang-format off
                    sgl_v2f_t2f(x_normalized, start_y_normalized, (float)u, 1.0);
                    sgl_v2f_t2f(x_normalized, end_y_normalized, (float)u, 0.0);
                    sgl_c3b(shade, shade, shade);
                sgl_end();  // clang-format on
            } break;

            case _ZBUFFER_DATA_TYPE_ENTITY: {
                const double dist = zbuffer[i].data.entity->_distance;
                const int line_height = (int)((screen.height / dist) / 2);

                const int start_y =
                    screen.half_height - line_height + (int)main_camera.rot_y;

                const int end_y =
                    screen.half_height + line_height + (int)main_camera.rot_y;

                /* Нормализация координат. */
                const float start_y_normalized =
                    (2.f * (float)start_y / screen.height) - 1.f;

                const float end_y_normalized =
                    (2.f * (float)end_y / screen.height) - 1.f;

                /* Отрисовка. */
                for (int j = -40; j < 0; j++) {
                    const float x_normalized =
                        (2.f * (float)(i + j) / screen.width) - 1.f;

                    sgl_begin_lines();  // clang-format off
                        sgl_v2f(x_normalized, start_y_normalized);
                        sgl_v2f(x_normalized, end_y_normalized);
                        sgl_c3b(0, 0, 0);
                    sgl_end();  // clang-format on
                }
            } break;
            default: break;
        }
    }
}
