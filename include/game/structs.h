#pragma once

#include <stdint.h>

#include "ext_math.h"

//! Камера.
typedef struct {
    float last_x;  //< Прошлое значение mice_x.
    float mice_x;  //< Координата поворота мышки.
    double dir;    //< Угол поворота в радиантах.
    vec2_t pos;    //< Позиция.
} camera_t;

//! Представление игрока.
typedef struct {
    vec2_t pos;  //< Позиция.
    double dir;  //< Угол поворота в радиантах.
} player_t;

//! \private \brief Данные об экране.
struct __screen_config_data {
    uint16_t width;        //< width
    uint16_t height;       //< height
    uint16_t half_width;   //< half width
    uint16_t half_height;  //< half height
};
