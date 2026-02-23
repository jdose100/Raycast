/*!
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

#include <stdio.h>
#include <stdlib.h>

#include "ext_math.h"
#include "game/config.h"
#include "graphics/graphics.h"

[[gnu::nonnull(2)]] [[nodiscard]] extern bool __raycast_find_wall(double, double *);

//! Raycasting logic.
static void raycastring(renderer_data_t *render_data)
{
    // Размер шага угла альфа.
    auto constexpr alpha_step = fov / screen.width;

    // Угол альфа. В начале равен углу поворота камеры - половина FOV (крайний левый столбец).
    // В конце должен быть равен углу повора камеры + половина FOV (крайний правый столбец).
    // Данный угол может находится в пределе [0; 360].
    double alpha = main_camera.angle - half_fov;
    if (alpha + fov > 360.0)
        alpha = 360 - fov;
    else if (alpha < 0.0)
        alpha = 0;

    for (auto x = 0; x < screen.width; x++) {
        double current_wall_height;
        if (!__raycast_find_wall(alpha, &current_wall_height)) continue;

        const double draw_wall_height =
            wall_height / current_wall_height * projection_plane_direction;

        const float start_y = (screen.height - draw_wall_height) / 2;
        const float end_y = start_y + draw_wall_height;

        // printf("wall height: %f\n", current_wall_height);
        SDL_RenderLine(render_data->renderer, x, start_y, x, end_y);

        alpha += alpha_step;
        if (alpha >= 360) alpha = 0;
    }
}

/*!
   \protected \brief Рассчитывает растояние до близжайшей стены.
   \details Исчет близжайшую стену относительно main_camera и угла alpha.
   \important Угол альфа должен быть в градусах.

   \param[in] alpha угол альфа в градусах.
   \param[out] result возвращаемая дистанция.
   \return true если стена была найдена, иначе false.
*/
bool __raycast_find_wall(const double alpha, double *result)
{
    // --- # ПОДГОТОВКА К ФУНКЦИИ # --- //
    constexpr size_t attempts_to_find_dist = 20;               // Кол-во попыток найти пересечение.
    const double alpha_in_radians = degree_to_radians(alpha);  // Угол альфа в радиантах.

    // ------------------------------------------- //
    // --- # ПОИСК ГОРИЗОНТАЛЬНОГО РАСТОЯНИЯ # --- //
    // ------------------------------------------- //

    // Горизонтальная точка, c помощью которой находится горизонтальное растояние.
    vec2_t horizontal_point;

    // ------------------------------------------------------
    // -- Вычисление координаты в горизонтальном пересечении.
    if ((270.0 <= alpha && alpha < 360.0) || alpha < 90.0)
        horizontal_point.y = /* floor(main_camera.pos.y / title_size) * */ title_size + title_size;
    else /* вниз */
        horizontal_point.y = /* floor(main_camera.pos.y / title_size) * */ title_size - 1;
    horizontal_point.x =
        main_camera.pos.x + (main_camera.pos.y - horizontal_point.y) / tan(alpha_in_radians);

    // -------------------------------------------------
    // -- Преобразование координаты в координаты клеток.
    horizontal_point.x /= title_size;
    horizontal_point.y /= title_size;

    printf("horiz x: %f\thoriz y: %f\n", horizontal_point.x, horizontal_point.y);

    // ----------------------------------------------------------------------------
    // -- Проверка что координата находится в допустимом диапазоне и есть на карте.
    if ((int)horizontal_point.x < 0.0 || (int)horizontal_point.y < 0.0 ||
        (int)horizontal_point.x >= map_size.x || (int)horizontal_point.y >= map_size.y) {
        goto fail_horizontal_search;
    } else if (map[(int)horizontal_point.x][(int)horizontal_point.y])
        // Мы уже нашли точку, так что пропускаем цикл, который пытается найти точку.
        goto final_horizontal_search;

    // ----------------------------------
    // -- Ещё пытаемся найти пересечения.

    // Дистанция до следующей клетки.
    vec2_t next_cell_dist = {
        .y = (270.0 <= alpha && alpha <= 360.0) || alpha <= 90.0 ? -title_size /* вверх */
                                                                 : title_size /* вниз */,
        .x = title_size * tan(alpha_in_radians),
    };

    // Цикл поиска.
    for (size_t _ = 0; _ < attempts_to_find_dist; _++) {
        // Увеличиваем координаты.
        horizontal_point.x = (horizontal_point.x + next_cell_dist.x) / title_size;
        horizontal_point.y = (horizontal_point.y + next_cell_dist.y) / title_size;
        printf("horiz x: %f\thoriz y: %f\n", horizontal_point.x, horizontal_point.y);

        // Проверка что нашли.
        if ((int)horizontal_point.x < 0.0 || (int)horizontal_point.y < 0.0 ||  // clang-format off
            (int)horizontal_point.x >= map_size.x || (int)horizontal_point.y >= map_size.y)
        {
            break;
        } /* clang-format on */
        else if (map[(int)horizontal_point.x][(int)horizontal_point.y])
            goto final_horizontal_search;  // Нашли, выходим.
    }

fail_horizontal_search:  // Мы не нашли, так что указываем это.
    horizontal_point.x = INT64_MAX;
    horizontal_point.y = INT64_MAX;

    // --------------------------------------------
    // -- Конец поиска горизонтального пересечения.
final_horizontal_search:
    // Горизонтальная дистанция, отрицательна если ничего не найденно.
    const double horizontal_dist =
        horizontal_point.x != INT64_MAX && horizontal_point.y != INT64_MAX
            ? abs((int)(main_camera.pos.x - horizontal_point.x)) / cos(alpha_in_radians)
            : INFINITY;

    // ------------------------------------------- //
    // --- # ПОИСК ВЕРТИКАЛЬНОГО ПЕРЕСЕЧЕНИЯ # --- //
    // ------------------------------------------- //
    vec2_t vertical_point;

    // --------------------------------------------------
    // -- Вычисление координаты вертикальном пересечении.
    if (alpha >= 0 && alpha < 180) /* справо */
        vertical_point.x = /* floor(main_camera.pos.x / title_size) * */ title_size + title_size;
    else /* слево */
        vertical_point.x = /* floor(main_camera.pos.x / title_size) * */ title_size - 1;
    vertical_point.y =
        main_camera.pos.y + (main_camera.pos.x - vertical_point.x) * tan(alpha_in_radians);

    // -------------------------------------------------
    // -- Преобразование координаты в координаты клеток.
    vertical_point.x /= title_size;
    vertical_point.y /= -title_size;

    // printf("vert x: %f\tvert y: %f\n", vertical_point.x, vertical_point.y);

    // ----------------------------------------------------------------------------
    // -- Проверка что координата находится в допустимом диапазоне и есть на карте.
    if ((int)vertical_point.x < 0 || (int)vertical_point.y < 0 ||  // clang-format off
        (int)vertical_point.x >= map_size.x || (int)vertical_point.y >= map_size.y)
    {
        goto fail_vertical_search;
    } // Мы уже нашли точку, так что пропускаем цикл, который пытается найти точку.
    else if (map[(int)vertical_point.x][(int)vertical_point.y])
        goto final_vertical_search;
    // clang-format on

    // ----------------------------------
    // -- Ещё пытаемся найти пересечения.

    // Дистанция до следующей клетки.
    next_cell_dist = (vec2_t){
        .x = alpha >= 0 && alpha < 180 ? title_size /* справо */ : -title_size /* слево */,
        .y = title_size / tan(alpha_in_radians),
    };

    // Цикл поиска.
    for (size_t _ = 0; _ < attempts_to_find_dist; _++) {
        // Увеличиваем координаты.
        vertical_point.x = (vertical_point.x + next_cell_dist.x) / title_size;
        vertical_point.y = (vertical_point.y + next_cell_dist.y) / title_size;
        // printf("alpha: %f\tvert x: %f\tvert y: %f\n", alpha, vertical_point.x, vertical_point.y);

        // Проверка что нашли.
        if ((int)vertical_point.x < 0 || (int)vertical_point.y < 0 ||  // clang-format off
            (int)vertical_point.x >= map_size.x || (int)vertical_point.y >= map_size.y)
        {
            break;
        } else if (map[(int)vertical_point.x][(int)vertical_point.y])
        {
            goto final_vertical_search;  // Нашли, выходим.
        }
        // clang-format on
    }

fail_vertical_search:  // Не нашли, указываем это.
    vertical_point.x = INT64_MAX;
    vertical_point.y = INT64_MAX;

    // ------------------------------------------
    // -- Конец поиска вертикального пересечения.
final_vertical_search:
    const double vertical_dist =
        vertical_point.x != INT64_MAX && vertical_point.y != INT64_MAX
            ? abs((int)(main_camera.pos.y - vertical_point.x)) / sin(alpha_in_radians)
            : INFINITY;

    // --------------------------------------- //
    // --- # ПОИСК БЛИЗЖАЙШЕГО РАСТОЯНИЯ # --- //
    // --------------------------------------- //
    // printf("distance: horizontal(%f), vertical(%f)\n", horizontal_dist, vertical_dist);
    if (horizontal_dist == INFINITY && vertical_dist == INFINITY)
        return false;
    else if (horizontal_dist == INFINITY)
        *result = vertical_dist;
    else if (vertical_dist == INFINITY)
        *result = horizontal_dist;
    else
        *result = horizontal_dist < vertical_dist ? horizontal_dist : vertical_dist;

    // Убираем эффект рыбьего глаза.
    *result = *result * cos(alpha_in_radians);
    return true;

    // Удаляем вспомогательный макрос DISTANCE.
#undef DISTANCE
}
