#pragma once

#include "data.h"
#include "game/config.h"
#include "game/data.h"

[[gnu::nonnull(1)]] static void _raycast_quick_sort(cc_vec(entity_t) * entities);

[[gnu::nonnull(2)]] static void _raycast_entities(size_t zbuffer_len,
                                                  struct _raycast_zbuffer_data zbuffer[zbuffer_len])
{
    // Сортируем массив сущностей.
    _raycast_quick_sort(&game_data.entities);

    // Перебираем сущности.
    cc_for_each(&game_data.entities, entity) {
        const double screen_x = (entity->position.x - -100) / (100 - -100) * screen.width;
        if (screen_x < 0 || (size_t)screen_x >= zbuffer_len) continue;
        if (zbuffer[(size_t)screen_x].depth > entity->_distance) continue;

        zbuffer[(size_t)screen_x].type = _ZBUFFER_DATA_TYPE_ENTITY;
        zbuffer[(size_t)screen_x].data.entity = entity;
    }
}

static entity_t *__raycast_quick_sort2(const size_t len, entity_t array[len]);

[[gnu::nonnull(1)]] static void _raycast_quick_sort(cc_vec(entity_t) * entities)
{
    // Создаём массив который и будем сортировать.
    const size_t len = cc_size(entities);
    entity_t *array = malloc(len * sizeof(entity_t));
    defer { free(array); }

    // Заполняем массив элементами.
    entity_t *element = cc_first(entities);
    for (size_t i = 0; i < len; i++) {
        array[i] = *element;
        element = cc_next(entities, element);
    }

    // Сортируем массив.
    const entity_t *sorted = __raycast_quick_sort2(len, array);

    // Заменяем все элементы в старом массиве.
    cc_clear(entities);
    cc_insert_n(entities, 0, sorted, len);
}

static entity_t *__raycast_quick_sort2(const size_t len, entity_t array[len])
{
    if (len <= 1) return array;

    // Выбираем середину в качестве опорного элемента.
    const entity_t pivot = array[len / 2];
    const double pivot_dst =
        pow(pivot.position.x - main_camera.pos.x, 2) + pow(pivot.position.y - main_camera.pos.y, 2);

    entity_t left[len];  // Массив элементов меньше опорного.
    size_t left_idx = 0;

    entity_t middle[len];  // Массив элементов равных опорному.
    size_t middle_idx = 0;

    entity_t right[len];  // Массив элементов больше опорного.
    size_t right_idx = 0;

    // Разбираем массив на под массивы.
    for (size_t i = 0; i < len; i++) {
        entity_t current = array[i];

        // Считаем дистанцию.
        const double dst = pow(current.position.x - main_camera.pos.x, 2) +
                           pow(current.position.y - main_camera.pos.y, 2);
        current._distance = sqrt(dst);

        // Сравниваем.
        if (dst < pivot_dst)
            left[left_idx++] = current;
        else if (dst == pivot_dst)
            middle[middle_idx++] = current;
        else
            right[right_idx++] = current;
    }

    // Сортируем под массивы.
    const entity_t *left_sorted = __raycast_quick_sort2(left_idx, left);
    const entity_t *right_sorted = __raycast_quick_sort2(right_idx, right);

    // Склеиваем массивы.
    if (left_idx != 0) memmove(array, left_sorted, left_idx * sizeof(entity_t));
    if (middle_idx != 0) memmove(array + left_idx, middle, middle_idx * sizeof(entity_t));
    if (right_idx != 0)
        memmove(array + left_idx + middle_idx, right_sorted, right_idx * sizeof(entity_t));

    return array;
}
