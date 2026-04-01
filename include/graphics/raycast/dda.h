#pragma once

#include "game/config.h"

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
[[gnu::nonnull(2, 3, 4)]] [[nodiscard]] static inline bool _raycast_dda(
    const int x, double *out_distance, double *out_u, unsigned int *finded_map_cell)
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
unsigned int angle_to_screen_x(const double angle)
{
    /*
    ray_direction == angle
    ray_direction = main_camera.dir_x + atan(tan(half_fov) * camera_x)
    ray_direction - main_camera.dir_x = atan(tan(half_fov) * camera_x) =>
    tan(ray_direction - main_camera.dir_x) = tan(half_fov) * camera_x
    tan(ray_direction - main_camera.dir_x) / tan(half_fov) = camera_x
    */
    // double const camera_x = 2.0 * x / screen.width - 1.0;  // от -1 до 1
    const double x = ((tan(angle - main_camera.dir_x) / tan(half_fov)) + 1) * screen.width / 2.0;
    if (x >= 0 && x <= screen.width) {
        if (fmod(x, 1.0) > 0.5) return (unsigned int)x + 1;
        return (unsigned int)x;
    }
    return 9999999;
}
