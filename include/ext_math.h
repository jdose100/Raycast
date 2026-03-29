#pragma once
 
#include <math.h>
#include <stdint.h>

//! Превращает градусы в радианты.
[[gnu::const]] static inline double degree_to_radians(const double degree)
{ return degree * (M_PI / 180); }

//! Двухмерный вектор.
typedef struct {
    double x;  //< coord x
    double y;  //< coord y
} vec2_t;

//! Двухмерный целочисленный вектор.
typedef struct {
    int64_t x;  //< coord x
    int64_t y;  //< coord y
} ivec2_t;

//! \public \memberof vec2_t \brief Умножение вектора на число.
static inline vec2_t vec2_mul_val(vec2_t other, double val)
{ return (vec2_t){.x = other.x * val, .y = other.y * val}; }
