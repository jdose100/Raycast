#pragma once

#include <stdint.h>

#include "ext_math.h"

//! Камера.
typedef struct {
    double dir_x;  //< Угол поворота в радиантах.
    vec2_t pos;    //< Позиция.

    //! Смещение по y для просмотра вверх, вниз.
    //! Не является углом, поэтому не direction.
    float rot_y;
} camera_t;

//! Представление игрока.
typedef struct {
    vec2_t pos;    //< Позиция.
    double dir_x;  //< Угол поворота в радиантах.

    //! Смещение по y для просмотра вверх, вниз.
    //! Не является углом, поэтому не direction.
    float rot_y;
} player_t;

//! \private \brief Данные об экране.
struct __screen_config_data {
    uint16_t width;        //< width
    uint16_t height;       //< height
    uint16_t half_width;   //< half width
    uint16_t half_height;  //< half height
};
