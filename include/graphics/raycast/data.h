#pragma once

#include "game/data.h"

enum _raycast_zbufer_data_type {
    _ZBUFFER_DATA_EMPTY = 0,

    _ZBUFFER_DATA_TYPE_WALL,
    _ZBUFFER_DATA_TYPE_ENTITY,
};

struct _raycast_zbuffer_data {
    double depth;
    unsigned int x;

    union {
        struct {
            unsigned int texture_idx;
            double u;
        } wall;

        entity_t *entity;
    } data; //< Зависит от type.

    enum _raycast_zbufer_data_type type;
};
