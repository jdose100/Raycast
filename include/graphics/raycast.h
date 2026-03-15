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
#include <stdio.h>

#include "ext_math.h"
#include "game/config.h"

[[gnu::nonnull(2)]] [[nodiscard]] static inline bool _raycast(const int, double *, double *);

//! Рисует мир.
static void drawing(sg_view view, sg_sampler sampler)
{
    sgl_texture(view, sampler);
    sgl_enable_texture();
    defer { sgl_disable_texture(); }

    sgl_begin_lines();
    defer { sgl_end(); }

    for (auto x = 0; x < screen.width; x++) {
        double distance, u;
        if (!_raycast(x, &distance, &u)) continue;

        /* Высота стенки на экране */
        const int line_height = (int)((screen.height / distance) / 2);
        const int start_y = screen.half_height - line_height;
        const int end_y = screen.half_height + line_height;

        const uint8_t calc_shade = (uint8_t)(255 / (1.0 + distance * distance * 0.1));
        const uint8_t shade = calc_shade < 180 ? calc_shade : 180;

        /* Нормализация координат. */
        const float start_y_normalized = (2.f * (float)start_y / screen.height) - 1.f;
        const float end_y_normalized = (2.f * (float)end_y / screen.height) - 1.f;
        const float x_normalized = (2.f * (float)x / screen.width) - 1.f;

        /* Отрисовка */
        sgl_v2f_t2f(x_normalized, start_y_normalized, (float)u, 1.0);
        sgl_v2f_t2f(x_normalized, end_y_normalized, (float)u, 0.0);
        sgl_c3b(shade, shade, shade);
    }
}

/*!
    \protected \brief Рассчитывает расстояние до близлежащей стены.
    \details Ищет близлежащею стену относительно main_camera.
    \important Угол альфа должен быть в градусах.

    \param[in] x смещение.
    \param[out] out_distance возвращаемая дистанция.
    \param[out] out_u координата x для текстур.
    \return true если стена была найдена, иначе false.
*/
bool _raycast(const int x, double *out_distance, double *out_u)
{
    constexpr auto delta = 0.005;

    /* Угол луча для текущего столбца. */
    const double camera_x = 2.0 * x / screen.width - 1.0;  // от -1 до 1
    auto const ray_direction = main_camera.direction + atan(tan(half_fov) * camera_x);
    // printf("%f\n", ray_direction);

    /* DDA‑алгоритм (Digital Differential Analyzer) */
    const double step_x = cos(ray_direction) * delta;
    const double step_y = sin(ray_direction) * delta;
    double ray_x = main_camera.pos.x;
    double ray_y = main_camera.pos.y;
    double distance = 0.0;

    while (distance < max_raycast_distance) {
        //получаем координаты стены пересеченной лучем для проверок
        const int map_x = (int)ray_x;
        const int map_y = (int)ray_y;
        //проверка на выход за область массива
        if (map_x >= 0 && map_x < map_size.x && map_y >= 0 && map_y < map_size.y) {
            // проверка на наличие стены (map[map_x][map_y] != 0)
            if (map[map_x][map_y]) {
                /* Получаем финальную дистанцию без fish-eye эффекта. */
                *out_distance = distance * cos(ray_direction - main_camera.direction);

                /* Получаем точные координаты пересечения луча с стеной на самой стене где 0 и 1 края стены*/
                double x = fmod(ray_x, 1.0);
                double y = fmod(ray_y, 1.0);
                /* если точка находится на координатах x/y > нуля отзеркаливаем эту координату так чтобы расстояние до
                 ближайшего целого числа осталось равным
                 перед иф:0------------|-------*----1 
                 после иф:0----*-------|------------1   */
                if (y > 0.5) y = (y - 1.0) * -1.0;
                if (x > 0.5) x = (x - 1.0) * -1.0;
                // после выше описанной подготовки к сравнению с целым числом идет сравнение кто ближе к 0
                /* если x < y => x ≈ 0 => fmod(X:ray_x, Y:1.0) ≈ 0||1 если fmod(X:ray_x, Y:1.0) ≈ 0||1 то луч попал на угл и тогда нет разницы
                 какую текстуру рисовать или он попал на стену пролегающею вдоль оси Y и мы рисуем текстуру для стены пролегающей вдоль оси Y */
                if (x < y)
                    *out_u = ray_y;
                /* else => x >= y => y ≈ 0 => fmod(X:ray_y, Y:1.0) ≈ 0||1 если fmod(X:ray_y, Y:1.0) ≈ 0||1 то луч попал на угл и тогда нет разницы
                 какую текстуру рисовать или он попал на стену пролегающею вдоль оси X и мы рисуем текстуру для стены пролегающей вдоль оси X */
                else
                    *out_u = ray_x;

                return true;
            }
        }

        distance += delta;
        ray_x += step_x;
        ray_y += step_y;
    }

    return false;
}
