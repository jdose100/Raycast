#include "game/config.h"

void move_forward()
{
    const int map_x = (int)(main_camera.pos.x + cos(main_camera.direction) * camera_speed);
    const int map_y = (int)(main_camera.pos.y + sin(main_camera.direction) * camera_speed);
    if (map_x < 0 || map_x >= map_size.x || map_y < 0 || map_y >= map_size.y) return;

    // Координата X.
    if (map[map_x][(int)main_camera.pos.y] == 0) {
        main_camera.pos.x += cos(main_camera.direction) * camera_speed;
    } else {
        if (cos(main_camera.direction) > 0) {
            main_camera.pos.x =
                floor(main_camera.pos.x + cos(main_camera.direction) * camera_speed) - 0.02;
        } else {
            main_camera.pos.x =
                floor(main_camera.pos.x + cos(main_camera.direction) * camera_speed) + 1.02;
        }
    }

    // Координата Y.
    if (map[(int)main_camera.pos.x][map_y] == 0) {
        main_camera.pos.y += sin(main_camera.direction) * camera_speed;
    } else if (sin(main_camera.direction) > 0) {
        main_camera.pos.y =
            floor(main_camera.pos.y + sin(main_camera.direction) * camera_speed) - 0.02;
    } else {
        main_camera.pos.y =
            floor(main_camera.pos.y + sin(main_camera.direction) * camera_speed) + 1.02;
    }
}

void move_back()
{
    const int map_x = (int)(main_camera.pos.x - cos(main_camera.direction) * camera_speed);
    const int map_y = (int)(main_camera.pos.y - sin(main_camera.direction) * camera_speed);
    if (map_x < 0 || map_x >= map_size.x || map_y < 0 || map_y >= map_size.y) return;

    // Координата X.
    if (map[map_x][(int)main_camera.pos.y] == 0) {
        main_camera.pos.x -= cos(main_camera.direction) * camera_speed;
    } else if (cos(main_camera.direction) > 0) {
        main_camera.pos.x =
            floor(main_camera.pos.x - cos(main_camera.direction) * camera_speed) + 1.02;
    } else {
        main_camera.pos.x =
            floor(main_camera.pos.x - cos(main_camera.direction) * camera_speed) - 0.02;
    }

    // Координата Y.
    if (map[(int)main_camera.pos.x][map_y] == 0) {
        main_camera.pos.y -= sin(main_camera.direction) * camera_speed;
    } else if (sin(main_camera.direction) > 0) {
        main_camera.pos.y =
            floor(main_camera.pos.y - sin(main_camera.direction) * camera_speed) + 1.02;
    } else {
        main_camera.pos.y =
            floor(main_camera.pos.y - sin(main_camera.direction) * camera_speed) - 0.02;
    }
}

void move_right()
{  //--------------ispravit------------
    const int map_x =
        (int)(main_camera.pos.x - cos(main_camera.direction - M_PI_2) * camera_side_speed);
    const int map_y =
        (int)(main_camera.pos.y - sin(main_camera.direction - M_PI_2) * camera_side_speed);
    if (map_x < 0 || map_x >= map_size.x || map_y < 0 || map_y >= map_size.y) return;

    // Координата X.
    if (map[map_x][(int)main_camera.pos.y] == 0) {
        main_camera.pos.x -= cos(main_camera.direction - M_PI_2) * camera_side_speed;
    } else {
        //            if (cos(main_camera.direction - M_PI_2) > 0)
        //            {
        //                main_camera.pos.x = floor(main_camera.pos.x -
        //                cos(main_camera.direction)* camera_side_speed) + 1.02;
        //           }
        //            else
        //            {
        //                main_camera.pos.x = floor(main_camera.pos.x -
        //                cos(main_camera.direction)* camera_side_speed) - 0.02;
        //            }
    }

    // Координата Y.
    if (map[(int)main_camera.pos.x][map_y] == 0) {
        main_camera.pos.y -= sin(main_camera.direction - M_PI_2) * camera_side_speed;
    } else if (sin(main_camera.direction - M_PI_2) > 0) {
        main_camera.pos.y =
            floor(main_camera.pos.y - sin(main_camera.direction - M_PI_2) * camera_side_speed) +
            1.02;
    } else {
        main_camera.pos.y =
            floor(main_camera.pos.y - sin(main_camera.direction - M_PI_2) * camera_side_speed) -
            0.02;
    }
}

void move_left()
{
    const int map_x =
        (int)(main_camera.pos.x - cos(main_camera.direction + M_PI_2) * camera_side_speed);
    const int map_y =
        (int)(main_camera.pos.y - sin(main_camera.direction + M_PI_2) * camera_side_speed);

    if (map_x < 0 || map_x >= map_size.x || map_y < 0 || map_y >= map_size.y) return;

    // Координата X.
    if (map[map_x][(int)main_camera.pos.y] == 0) {
        main_camera.pos.x -= cos(main_camera.direction + M_PI_2) * camera_side_speed;
    } else {
        //            if (cos(main_camera.direction + M_PI_2) > 0)
        //            {
        //                main_camera.pos.x = floor(main_camera.pos.x -
        //                cos(main_camera.direction)* camera_side_speed) + 1.02;
        //           }
        //            else
        //            {
        //                main_camera.pos.x = floor(main_camera.pos.x -
        //                cos(main_camera.direction)* camera_side_speed) - 0.02;
        //            }
    }

    // Координата Y.
    if (map[(int)main_camera.pos.x][map_y] == 0) {
        main_camera.pos.y -= sin(main_camera.direction + M_PI_2) * camera_side_speed;
    } else if (sin(main_camera.direction + M_PI_2) > 0) {
        main_camera.pos.y =
            floor(main_camera.pos.y - sin(main_camera.direction + M_PI_2) * camera_side_speed) +
            1.02;
    } else {
        main_camera.pos.y =
            floor(main_camera.pos.y - sin(main_camera.direction + M_PI_2) * camera_side_speed) -
            0.02;
    }
}
