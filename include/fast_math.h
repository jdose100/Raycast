#pragma once

/*!
    \defgroup fast_math Fast math functions.
    \brief Группа, содержащая функции оптимизированной математики.
*/

#include "math.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef RAYC_NO_SHORT_NAMES
    /*!
        \brief Быстрый синус.
        \details Доступен если отсутствует RAYC_NO_SHORT_NAMES.
        \ingroup fast_math
    */
    #define fsin rayc_fsin

    /*!
        \brief Быстрый косинус.
        \details Доступен если отсутствует RAYC_NO_SHORT_NAMES.
        \ingroup fast_math
    */
    #define fcos rayc_fcos
#endif

//! \private
static constexpr double __raycast_sin_cos_table_step = 0.001;

//! \private \brief Размер таблицы синусов и косинусов.
static constexpr int __raycast_sin_cos_table_size =
    6284;  // (int)((2 * M_PI / __raycast_sin_cos_table_step) + 1);

//! \private \brief Таблица синусов.
static double __raycast_sin_table[__raycast_sin_cos_table_size] = {0};

//! \private \brief Таблица косинусов.
static double __raycast_cos_table[__raycast_sin_cos_table_size] = {0};

//! \private \brief Инициализация таблицы синусов и косинусов.
[[maybe_unused]] static void __raycast_sin_cost_table_init(void)
{
    for (auto i = 0; i < __raycast_sin_cos_table_size; i++) {
        const double angle = i * __raycast_sin_cos_table_step;
        __raycast_cos_table[i] = cos(angle);
        __raycast_sin_table[i] = sin(angle);
    }
}

//! \brief Быстрый синус (fast sin). Имеется потеря точности.
//! \ingroup fast_math
inline static double rayc_fsin(double a)
{
    // Приводим к диапазону [0, 2P].
    while (a < 0) a += 2 * M_PI;
    while (a >= 2 * M_PI) a -= 2 * M_PI;

    int idx = (int)(a / __raycast_sin_cos_table_step);
    double frac = (a - idx * __raycast_sin_cos_table_step) /
                  __raycast_sin_cos_table_step;  // Линейная интерполяция.

    // Защита от выхода за границу.
    int idx_next = (idx + 1) % __raycast_sin_cos_table_size;

    return __raycast_sin_table[idx] * (1 - frac) +
           __raycast_sin_table[idx_next] * frac;
}

//! \brief Быстрый косинус (fast cos). Имеется потеря точности.
//! \ingroup fast_math
inline static double rayc_fcos(double a)
{
    // Приводим к диапазону [0, 2P].
    while (a < 0) a += 2 * M_PI;
    while (a >= 2 * M_PI) a -= 2 * M_PI;

    int idx = (int)(a / __raycast_sin_cos_table_step);
    double frac = (a - idx * __raycast_sin_cos_table_step) /
                  __raycast_sin_cos_table_step;  // Линейная интерполяция.

    // Защита от выхода за границу.
    int idx_next = (idx + 1) % __raycast_sin_cos_table_size;

    return __raycast_cos_table[idx] * (1 - frac) +
           __raycast_cos_table[idx_next] * frac;
}

#ifdef __cplusplus
}
#endif
