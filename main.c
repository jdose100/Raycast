// clang-format off
#include <sokol_app.h>
#include <sokol_gfx.h>
#include <sokol_gl.h>
#include <sokol_glue.h>
#include <sokol_log.h>
#include <sokol_debugtext.h>
// clang-format on

#include "game/logic.h"
// #include "game/move_physics.h"
#include "graphics/raycast/raycast.h"

static struct {
    sg_pass_action action;

    unsigned int frame_count;
    double time_acc;
    double fps;
} state;

void frame(void)
{
    // FPS
    state.frame_count++;
    state.time_acc += sapp_frame_duration();

    if (state.time_acc > 0.5) {
        state.fps = (double)state.frame_count / state.time_acc;
        state.frame_count = 0;
        state.time_acc = 0;
    }
    sdtx_printf("FPS: %.2f\n", state.fps);
    sdtx_printf("Camera position(x: %f\ty: %f)\n", main_camera.pos.x, main_camera.pos.y);

    // Drawing.
    sg_begin_pass(&(sg_pass){.action = state.action, .swapchain = sglue_swapchain()});
    sgl_defaults();

    // drawing();
    raycast_draw();

    sgl_draw();
    sdtx_draw();

    sg_end_pass();
    sg_commit();

    // Game logic.
    game_update();
}

static void event(const sapp_event *event)
{
    if (event->type == SAPP_EVENTTYPE_KEY_DOWN && event->key_code == SAPP_KEYCODE_ESCAPE)
        sapp_lock_mouse(!sapp_mouse_locked());

    if (!sapp_mouse_locked()) return;

    switch (event->type) {
        case SAPP_EVENTTYPE_RESIZED: {
            screen.height = (uint16_t)event->window_height;
            screen.width = (uint16_t)event->window_width;

            screen.half_height = screen.height / 2;
            screen.half_width = screen.width / 2;

            sdtx_canvas(screen.half_width, screen.half_height);
            sdtx_origin(1.f, 2.f);
        } break;

        case SAPP_EVENTTYPE_MOUSE_MOVE: /* Обработка движений мыши. */ {
            const float max_y_offset = screen.half_height * 1.25f;
            constexpr double rotate_sensitivity_x = 0.02;
            constexpr float rotate_sensitivity_y = 2.1f;

            // Врашение игрока по координате X.
            player.dir_x += event->mouse_dx * rotate_sensitivity_x;
            if (player.dir_x > M_PI) player.dir_x -= 2 * M_PI;
            if (player.dir_x < -M_PI) player.dir_x += 2 * M_PI;

            // Врашение игрока по координате Y.
            player.rot_y += event->mouse_dy * rotate_sensitivity_y;
            if (player.rot_y > max_y_offset) player.rot_y = max_y_offset;
            if (player.rot_y < -max_y_offset) player.rot_y = -max_y_offset;

            // Применение вращения к камере.
            main_camera.dir_x = player.dir_x;
            main_camera.rot_y = player.rot_y;
        } break;

            /* ######################### */
            /* ### KEYBOARD HANDLING ### */
            /* ######################### */

        case SAPP_EVENTTYPE_KEY_DOWN:
        case SAPP_EVENTTYPE_KEY_UP: /* Обработка нажатий клавиатуры. */ {
            switch (event->key_code) {
                // case SAPP_KEYCODE_W: move_forward(); break;
                // case SAPP_KEYCODE_S: move_back(); break;
                // case SAPP_KEYCODE_D: move_right(); break;
                // case SAPP_KEYCODE_A: move_left(); break;

                case SAPP_KEYCODE_W:
                    player.pos.x += cos(main_camera.dir_x) * 0.1;
                    player.pos.y += sin(main_camera.dir_x) * 0.1;
                    break;

                case SAPP_KEYCODE_S:
                    player.pos.x -= cos(main_camera.dir_x) * 0.1;
                    player.pos.y -= sin(main_camera.dir_x) * 0.1;
                    break;

                case SAPP_KEYCODE_D:
                    player.pos.x -= cos(main_camera.dir_x - M_PI_2) * 0.1 * 1.3;
                    player.pos.y -= sin(main_camera.dir_x - M_PI_2) * 0.1 * 1.3;
                    break;

                case SAPP_KEYCODE_A:
                    player.pos.x -= cos(main_camera.dir_x + M_PI_2) * 0.1 * 1.3;
                    player.pos.y -= sin(main_camera.dir_x + M_PI_2) * 0.1 * 1.3;
                    break;

                default: break;
            }

            main_camera.pos = player.pos;
        } break;

        default: break;
    }
}

static void init(void)
{
    // Инициализация sokol_gfx.
    sg_setup(&(sg_desc){
        .environment = sglue_environment(),
        .logger.func = slog_func,
    });

    // Инициализация sokol_debugtext.
    sdtx_setup(&(sdtx_desc_t){
        .fonts[0] = sdtx_font_c64(),
        .logger.func = slog_func,
    });
    sdtx_canvas(screen.half_width, screen.half_height);
    sdtx_origin(1.f, 2.f);

    // Инициализация sokol_gl.
    sgl_setup(&(sgl_desc_t){.logger.func = slog_func});

    // Инициализация state.
    state.action = (sg_pass_action){
        .colors[0] =  /* clang-format off */ {
            .load_action = SG_LOADACTION_CLEAR,
            .clear_value = { 0.3f, 0.3f, 0.3f, 0.3f},
        },  /* clang-format on */
    };

    graphics_init();
    sapp_lock_mouse(true);

    // Game init.
    game_init();

    player.pos = (vec2_t){1.5, 1.5};

    game_add_entity((entity_t){
        .position = (vec2_t){5, 5},
        .id = 0,
    });

    game_add_entity((entity_t){
        .position = (vec2_t){29, 30},
        .id = 1,
    });

    game_add_entity((entity_t){
        .position = (vec2_t){100, 100},
        .id = 2,
    });
    
    game_add_entity((entity_t){
        .position = (vec2_t){15, 5},
        .id = 3,
    });
}

static void cleanup(void)
{
    sdtx_shutdown();
    sgl_shutdown();
    sg_shutdown();

    game_deinit();
}

sapp_desc sokol_main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    return (sapp_desc){
        /* CALLBACKS */
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .event_cb = event,

        /* WINDOW */
        .width = screen.width,
        .height = screen.height,
        .window_title = title,
        .icon.sokol_default = true,

        /* LOGGER */
        .logger.func = slog_func,
    };
}

/*!
    \mainpage Main Page
*/
