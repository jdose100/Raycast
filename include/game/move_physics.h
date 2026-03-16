#include "game/config.h"

constexpr double min_dist_from_wall = -0.025;
constexpr double distance_push_from_wall = 1.025;

constexpr double move_speed = 0.1 / 2.0;
constexpr double side_move_speed = move_speed * 1.3;

static void move_forward(void)
{
    const unsigned int map_x = (unsigned int)(player.pos.x + cos(player.dir) * move_speed);
    const unsigned int map_y = (unsigned int)(player.pos.y + sin(player.dir) * move_speed);

    /* Проверка, что нет выхода за карту. */
    if (!point_on_map(map_x, map_y)) return;

    // Координата X.
    if (map[(unsigned int)player.pos.y][map_x] == 0) {
        player.pos.x += cos(player.dir) * move_speed;
    } else if (cos(player.dir) > 0) {
        player.pos.x = floor(player.pos.x + cos(player.dir) * move_speed) + min_dist_from_wall;
    } else {
        player.pos.x = floor(player.pos.x + cos(player.dir) * move_speed) + distance_push_from_wall;
    }

    // Координата Y.
    if (map[map_y][(unsigned int)player.pos.x] == 0) {
        player.pos.y += sin(player.dir) * move_speed;
    } else if (sin(player.dir) > 0) {
        player.pos.y = floor(player.pos.y + sin(player.dir) * move_speed) + min_dist_from_wall;
    } else {
        player.pos.y = floor(player.pos.y + sin(player.dir) * move_speed) + distance_push_from_wall;
    }
}

static void move_back(void)
{
    const unsigned int map_x = (unsigned int)(player.pos.x - cos(player.dir) * move_speed);
    const unsigned int map_y = (unsigned int)(player.pos.y - sin(player.dir) * move_speed);

    /* Проверка, что нет выхода за карту. */
    if (!point_on_map(map_x, map_y)) return;

    // Координата X.
    if (map[(unsigned int)player.pos.y][map_x] == 0) {
        player.pos.x -= cos(player.dir) * move_speed;
    } else if (cos(player.dir) > 0) {
        player.pos.x = floor(player.pos.x - cos(player.dir) * move_speed) + distance_push_from_wall;
    } else {
        player.pos.x = floor(player.pos.x - cos(player.dir) * move_speed) + min_dist_from_wall;
    }

    // Координата Y.
    if (map[map_y][(unsigned int)player.pos.x] == 0) {
        player.pos.y -= sin(player.dir) * move_speed;
    } else if (sin(player.dir) > 0) {
        player.pos.y = floor(player.pos.y - sin(player.dir) * move_speed) + distance_push_from_wall;
    } else {
        player.pos.y = floor(player.pos.y - sin(player.dir) * move_speed) + min_dist_from_wall;
    }
}

static void move_right(void)
{
    const unsigned int map_x =
        (unsigned int)(player.pos.x - cos(player.dir - M_PI_2) * side_move_speed);

    const unsigned int map_y =
        (unsigned int)(player.pos.y - sin(player.dir - M_PI_2) * side_move_speed);

    /* Проверка, что нет выхода за карту. */
    if (!point_on_map(map_x, map_y)) return;

    // Координата X.
    if (map[(unsigned int)player.pos.y][map_x] == 0) {
        player.pos.x -= cos(player.dir - M_PI_2) * side_move_speed;
    }

    // Координата Y.
    if (map[map_y][(unsigned int)player.pos.x] == 0) {
        player.pos.y -= sin(player.dir - M_PI_2) * side_move_speed;
    } else if (sin(player.dir - M_PI_2) > 0) {
        player.pos.y = floor(player.pos.y - sin(player.dir - M_PI_2) * side_move_speed) +
                       distance_push_from_wall;
    } else {
        player.pos.y =
            floor(player.pos.y - sin(player.dir - M_PI_2) * side_move_speed) + min_dist_from_wall;
    }
}

static void move_left(void)
{
    const unsigned int map_x =
        (unsigned int)(player.pos.x - cos(player.dir + M_PI_2) * side_move_speed);

    const unsigned int map_y =
        (unsigned int)(player.pos.y - sin(player.dir + M_PI_2) * side_move_speed);

    /* Проверка, что нет выхода за карту. */
    if (!point_on_map(map_x, map_y)) return;

    // Координата X.
    if (map[(unsigned int)player.pos.y][map_x] == 0) {
        player.pos.x -= cos(player.dir + M_PI_2) * side_move_speed;
    }

    // Координата Y.
    if (map[map_y][(unsigned int)player.pos.x] == 0) {
        player.pos.y -= sin(player.dir + M_PI_2) * side_move_speed;
    } else if (sin(player.dir + M_PI_2) > 0) {
        player.pos.y = floor(player.pos.y - sin(player.dir + M_PI_2) * side_move_speed) +
                       distance_push_from_wall;
    } else {
        player.pos.y =
            floor(player.pos.y - sin(player.dir + M_PI_2) * side_move_speed) + min_dist_from_wall;
    }
}
