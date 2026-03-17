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

#include <sokol_gfx.h>
#include <sokol_gl.h>

#include "ext_math.h"
#include "game/config.h"
#include "graphics/graphics.h"

[[gnu::nonnull(2, 3, 4)]] [[nodiscard]] static inline bool _raycast(const int,
                                                                    double *,
                                                                    double *,
                                                                    unsigned int *);

bool foo = false;
int bar = 0;

//! Рисует мир.
static void drawing()
{
    sgl_enable_texture();
    defer { sgl_disable_texture(); }

    if (foo && bar <= screen.half_height) {
        bar += 5;
        foo = (bar <= screen.half_height);
    } else if (bar >= -screen.half_height) {
        bar -= 5;
        foo = (bar <= -screen.half_height);
    }

    for (auto x = 0; x < screen.width; x++) {
        double distance, u;
        unsigned int map_cell;
        if (!_raycast(x, &distance, &u, &map_cell)) continue;
        map_cell = map_cell - 1;  // Приводим к индексам текстур.

        /* Высота стенки на экране */
        const int line_height = (int)((screen.height / distance) / 2);
        const int start_y = screen.half_height - line_height + (int)main_camera.rot_y;
        const int end_y = screen.half_height + line_height + (int)main_camera.rot_y;

        const uint8_t calc_shade = (uint8_t)(255 / (1.0 + distance * distance * 0.1));
        const uint8_t shade = calc_shade < 180 ? calc_shade : 180;

        /* Нормализация координат. */
        const float start_y_normalized = (2.f * (float)start_y / screen.height) - 1.f;
        const float end_y_normalized = (2.f * (float)end_y / screen.height) - 1.f;
        const float x_normalized = (2.f * (float)x / screen.width) - 1.f;

        /* Отрисовка */
        if (map_cell < _textures_countof)
            sgl_texture(_textures[map_cell].view, _textures[map_cell].sampler);

        sgl_begin_lines();  // clang-format off
            sgl_v2f_t2f(x_normalized, start_y_normalized, (float)u, 1.0);
            sgl_v2f_t2f(x_normalized, end_y_normalized, (float)u, 0.0);
            sgl_c3b(shade, shade, shade);
        sgl_end();  // clang-format on
    }
}

/*!
    \protected \brief Рассчитывает расстояние до близлежащей стены.
    \details Ищет близлежащую стену относительно main_camera.
    \important Угол альфа должен быть в градусах.

    \param[in] x смещение.
    \param[out] out_distance возвращаемая дистанция.
    \param[out] out_u координата x для текстур.
    \param[out] finded_map_cell найденная клетка.
    \return true если стена была найдена, иначе false.
*/
bool _raycast(const int x, double *out_distance, double *out_u, unsigned int *finded_map_cell)
{
    constexpr auto max_raycast_distance = 20.0;
    constexpr auto delta = 0.005;

    /* Угол луча для текущего столбца. */
    double const camera_x = 2.0 * x / screen.width - 1.0;  // от -1 до 1
    auto const ray_direction = main_camera.dir_x + atan(tan(half_fov) * camera_x);
    // printf("%f\n", ray_direction);

    /* DDA‑алгоритм (Digital Differential Analyzer) */
    const double step_x = cos(ray_direction) * delta;
    const double step_y = sin(ray_direction) * delta;
    double ray_x = main_camera.pos.x;
    double ray_y = main_camera.pos.y;
    double distance = 0.0;

    while (distance < max_raycast_distance) {
        // получаем координаты стены пересеченной лучем для проверок
        auto const map_x = (unsigned int)ray_x;
        auto const map_y = (unsigned int)ray_y;

        /* Проверка на на отсутствия выхода за область массива. */
        if (point_on_map(map_x, map_y)) {
            /* Проверка на наличие стены (map[map_x][map_y] != 0). */
            if (map[map_y][map_x]) {
                *finded_map_cell = map[map_y][map_x];

                /* Получаем финальную дистанцию без fish-eye эффекта. */
                *out_distance = distance * cos(ray_direction - main_camera.dir_x);

                /*
                    Приводим глобальные координаты луча в локальные координаты стенки.
                    Это диапазон [0.0; 1.0].
                */
                double x = fmod(ray_x, 1.0);
                double y = fmod(ray_y, 1.0);

                /* Если координата x/y > 0.5, то приводим в диапазон [0; 0.5] */
                if (x > 0.5) x = 1.0 - x;
                if (y > 0.5) y = 1.0 - y;

                /* Та точка, что дальше, содержит нужную координату текстуры по иксу. */
                *out_u = x > y ? ray_x : ray_y;

                return true;
            }
        }

        distance += delta;
        ray_x += step_x;
        ray_y += step_y;
    }

    return false;
}
