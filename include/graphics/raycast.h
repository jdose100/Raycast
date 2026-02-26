/*
В данном файле реализуется алгоритм raycasting. Предпологается что используемая
система координат соответствует следующей:
                0°                                  y
                |                                   |
                |                                   |
                |                                   |
270° -----------+----------- 90°      -x -----------+----------- x
                |                                   |
                |                                   |
                |                                   |
               180°                                -y
*/

#pragma once

#include "ext_math.h"
#include "game/config.h"
#include "graphics/graphics.h"

[[gnu::nonnull(2)]] [[nodiscard]] static inline bool _raycast(const int, double *);

//! Рисует мир.
static void drawing(renderer_data_t *render_data)
{
    for (auto x = 0; x < screen.width; x++) {
        double distance;
        if (!_raycast(x, &distance)) continue;

        /* Высота стенки на экране */
        const int lineHeight = (int)(screen.height / distance) / 2;
        const int start_y = screen.half_height - lineHeight;
        const int end_y = screen.half_height + lineHeight;

        Uint8 shade = (Uint8)(255 / (1.0 + distance * distance * 0.1));
        SDL_SetRenderDrawColor(render_data->renderer, shade, shade, shade, 255);
        SDL_RenderLine(render_data->renderer, (float)x, (float)start_y, (float)x, (float)end_y);
    }
}

/*!
    \protected \brief Рассчитывает растояние до близжайшей стены.
    \details Исчет близжайшую стену относительно main_camera.
    \important Угол альфа должен быть в градусах.

    \param[in] x смещение.
    \param[out] out_distance возвращаемая дистанция.
    \return true если стена была найдена, иначе false.
*/
bool _raycast(const int x, double *out_distance)
{
    constexpr auto delta = 0.005;

    /* Угол луча для текущего столбца. */
    const double camera_x = 2.0 * x / screen.width - 1.0;  // от -1 до 1
    auto const ray_direction = main_camera.direction + atan(tan(half_fov) * camera_x);

    /* DDA‑алгоритм (Digital Differential Analyzer) */
    const double step_x = cos(ray_direction) * delta;
    const double step_y = sin(ray_direction) * delta;
    double ray_x = main_camera.pos.x;
    double ray_y = main_camera.pos.y;
    double distance = 0.0;

    while (distance < max_raycast_distance) {
        const int map_x = (int)ray_x;
        const int map_y = (int)ray_y;

        if (map_x >= 0 && map_x < map_size.x && map_y >= 0 && map_y < map_size.y) {
            if (map[map_x][map_y]) {
                /* Получаем финальную дистанцию без fish-eye эффекта. */
                *out_distance = distance * cos(ray_direction - main_camera.direction);
                return true;
            }
        }

        distance += delta;
        ray_x += step_x;
        ray_y += step_y;
    }

    return false;
}
