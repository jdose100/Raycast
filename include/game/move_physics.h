#include "game/config.h"

constexpr double min_dist_from_wall = -0.025;
constexpr double distance_push_from_wall = 1.025;

constexpr double move_speed = 0.1 / 1.0;
constexpr double side_move_speed = move_speed * 1.3;

static void move_forward_P(player_t *player, double movement_dir)
{
    if (movement_dir > 2 * M_PI)
        movement_dir -= 4 * M_PI;
    if (movement_dir < -2 * M_PI)
        movement_dir += 4 * M_PI;
    const unsigned int map_x =
        (unsigned int)(player->pos.x + cos(movement_dir) * move_speed);

    const unsigned int map_y =
        (unsigned int)(player->pos.y + sin(movement_dir) * move_speed);

    /* Проверка, что нет выхода за карту. */
    if (!point_on_map(map_x, map_y))
        return;

    // clang-format off
    // Координата X.
    if (map[(unsigned int)player->pos.y][map_x] == 0) {
        player->pos.x += cos(movement_dir) * move_speed;
    } else if (cos(movement_dir) > 0) {
        player->pos.x = 
            floor(player->pos.x + cos(movement_dir) * move_speed) + min_dist_from_wall;
    } else {
        player->pos.x = 
            floor(player->pos.x + cos(movement_dir) * move_speed) + distance_push_from_wall;
    }

    // Координата Y.
    if (map[map_y][(unsigned int)player->pos.x] == 0) {
        player->pos.y += sin(movement_dir) * move_speed;
    } else if (sin(movement_dir) > 0) {
        player->pos.y =
            floor(player->pos.y + sin(movement_dir) * move_speed) + min_dist_from_wall;
    } else {
        player->pos.y =
            floor(player->pos.y + sin(movement_dir) * move_speed) + distance_push_from_wall;
    }
    // clang-format on
}
static unsigned int move_forward_Е(entity_t *target,
                          double movement_dir,
                          double e_move_speed)
{
    if (movement_dir > 2 * M_PI)
        movement_dir -= 4 * M_PI;
    if (movement_dir < -2 * M_PI)
        movement_dir += 4 * M_PI;
    const unsigned int map_x =
        (unsigned int)(target->position.x + cos(movement_dir) * e_move_speed);

    const unsigned int map_y =
        (unsigned int)(target->position.y + sin(movement_dir) * e_move_speed);

    /* Проверка, что нет выхода за карту. */
    if (!point_on_map(map_x, map_y)) {
        return 0;
    }
    bool punched_to_a_wall = false;
    // clang-format off
    // Координата X.
    if (map[(unsigned int)target->position.y][map_x] == 0) {
        target->position.x += cos(movement_dir) * e_move_speed;
    } else { punched_to_a_wall = true; 
        if (cos(movement_dir) > 0) {
        target->position.x = 
            floor(target->position.x + cos(movement_dir) * e_move_speed) + min_dist_from_wall;
    }   else {
        target->position.x = 
            floor(target->position.x + cos(movement_dir) * e_move_speed) + distance_push_from_wall;
    }
    }

    // Координата Y.
    if (map[map_y][(unsigned int)target->position.x] == 0) {
        target->position.y += sin(movement_dir) * e_move_speed;
    } else {    punched_to_a_wall = true;
        if (sin(movement_dir) > 0) {
        target->position.y =
            floor(target->position.y + sin(movement_dir) * e_move_speed) + min_dist_from_wall;
    }   else {
        target->position.y =
            floor(target->position.y + sin(movement_dir) * e_move_speed) + distance_push_from_wall;
    }
    }
    if (punched_to_a_wall && target->forced_movement_timer != 0){
        unsigned int dmg = target->forced_movement_timer - 5;
        target->forced_movement_timer = 0;
        target->forced_movement_dir = 9;
        return dmg;
    }
    return 0;
    // clang-format on
}
/*static void move_back(void)
{
    const unsigned int map_x =
        (unsigned int)(player.pos.x - cos(player.dir_x) * move_speed);

    const unsigned int map_y =
        (unsigned int)(player.pos.y - sin(player.dir_x) * move_speed);

    // Проверка, что нет выхода за карту.
    if (!point_on_map(map_x, map_y))
        return;

    // clang-format off
    // Координата X.
    if (map[(unsigned int)player.pos.y][map_x] == 0) {
        player.pos.x -= cos(player.dir_x) * move_speed;
    } else if (cos(player.dir_x) > 0) {
        player.pos.x =
            floor(player.pos.x - cos(player.dir_x) * move_speed) +
distance_push_from_wall; } else { player.pos.x = floor(player.pos.x -
cos(player.dir_x) * move_speed) + min_dist_from_wall;
    }

    // Координата Y.
    if (map[map_y][(unsigned int)player.pos.x] == 0) {
        player.pos.y -= sin(player.dir_x) * move_speed;
    } else if (sin(player.dir_x) > 0) {
        player.pos.y =
            floor(player.pos.y - sin(player.dir_x) * move_speed) +
distance_push_from_wall; } else { player.pos.y = floor(player.pos.y -
sin(player.dir_x) * move_speed) + min_dist_from_wall;
    }
    // clang-format on
}

static void move_right(void)
{
    const unsigned int map_x =
        (unsigned int)(player.pos.x -
                       cos(player.dir_x - M_PI_2) * side_move_speed);

    const unsigned int map_y =
        (unsigned int)(player.pos.y -
                       sin(player.dir_x - M_PI_2) * side_move_speed);

    // Проверка, что нет выхода за карту.
    if (!point_on_map(map_x, map_y))
        return;

    // Координата X.
    if (map[(unsigned int)player.pos.y][map_x] == 0) {
        player.pos.x -= cos(player.dir_x - M_PI_2) * side_move_speed;
    }

    // clang-format off
    // Координата Y.
    if (map[map_y][(unsigned int)player.pos.x] == 0) {
        player.pos.y -= sin(player.dir_x - M_PI_2) * side_move_speed;
    } else if (sin(player.dir_x - M_PI_2) > 0) {
        player.pos.y =
            floor(player.pos.y - sin(player.dir_x - M_PI_2)
                    * side_move_speed) + distance_push_from_wall;
    } else {
        player.pos.y =
            floor(player.pos.y - sin(player.dir_x - M_PI_2)
                    * side_move_speed) + min_dist_from_wall;
    }
    // clang-format on
}

static void move_left(void)
{
    const unsigned int map_x =
        (unsigned int)(player.pos.x -
                       cos(player.dir_x + M_PI_2) * side_move_speed);

    const unsigned int map_y =
        (unsigned int)(player.pos.y -
                       sin(player.dir_x + M_PI_2) * side_move_speed);

    // Проверка, что нет выхода за карту.
    if (!point_on_map(map_x, map_y))
        return;

    // Координата X.
    if (map[(unsigned int)player.pos.y][map_x] == 0) {
        player.pos.x -= cos(player.dir_x + M_PI_2) * side_move_speed;
    }

    // clang-format off
    // Координата Y.
    if (map[map_y][(unsigned int)player.pos.x] == 0) {
        player.pos.y -= sin(player.dir_x + M_PI_2) * side_move_speed;
    } else if (sin(player.dir_x + M_PI_2) > 0) {
        player.pos.y =
            floor(player.pos.y - sin(player.dir_x + M_PI_2) * side_move_speed) +
            distance_push_from_wall;
    } else {
        player.pos.y =
            floor(player.pos.y - sin(player.dir_x + M_PI_2) * side_move_speed) +
            min_dist_from_wall;
    }
    // clang-format on
}
*/