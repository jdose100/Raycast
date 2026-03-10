#include <sokol_app.h>
#include <sokol_gfx.h>
#include <sokol_gl.h>
#include <sokol_glue.h>
#include <sokol_help.h>
#include <sokol_log.h>
#include <stb_image.h>

#include <shaders/raycast.glsl.h>

#include "graphics/raycast.h"
#include "graphics/textures.h"

enum KEYS : unsigned char {
    KEY_W = 1,
    KEY_S,
    KEY_D,
    KEY_A,

    KEYS_SIZE,
};

static struct {
    sg_pipeline pip;
    sg_bindings bind;
    sg_pass_action action;

    sg_sampler sampler;
    sg_view view;

    bool keys_buffer[KEYS_SIZE];  //< Буфер нажатых клавиш с индексом KEYS.
} state;

static void init(void)
{
    // Инициализация sokol_gfx.
    sg_setup(&(sg_desc){
        .environment = sglue_environment(),
        .logger.func = slog_func,
    });

    // Инициализация sokol_gl.
    sgl_setup(&(sgl_desc_t){.logger.func = slog_func});

    // Инициализация state.

    /* Инициализация view и sampler. */
    state.sampler = sg_make_sampler(&(sg_sampler_desc){});

    // Загрузка изображения.
    const char image_path[] = "assets/textures/wall_2.png";
    if (load_texture(&state.view, image_path) != LOAD_TEXTURE_OK) {
        slog("start, load image", SLOG_PANIC, 0, "Error while loading image");
    }

    /* Инициализация action. */
    state.action = (sg_pass_action){
        .colors[0] =  /* clang-format off */ {
            .load_action = SG_LOADACTION_CLEAR,
            .clear_value = { 0.3f, 0.3f, 0.3f, 0.3f},
        },  /* clang-format on */
    };
}

static void cleanup(void)
{
    sgl_shutdown();
    sg_shutdown();
}

void frame(void)
{
    sg_begin_pass(&(sg_pass){.action = state.action, .swapchain = sglue_swapchain()});
    sgl_defaults();

    drawing(state.view, state.sampler);

    sgl_draw();
    sg_end_pass();
    sg_commit();
}

static void event(const sapp_event *event)
{
#ifdef __GNUC__
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wswitch-enum"
#elifdef __clang__
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wswitch-enum"
#endif

    switch (event->type) {
        case SAPP_EVENTTYPE_MOUSE_MOVE: /* Обработка движений мыши. */ {
            constexpr double rotate_speed = 0.05;

            main_camera.direction += event->mouse_dx * rotate_speed;
            if (main_camera.direction > M_PI) main_camera.direction -= 2 * M_PI;
            if (main_camera.direction < -M_PI) main_camera.direction += 2 * M_PI;
        } break;

        case SAPP_EVENTTYPE_KEY_DOWN:
        case SAPP_EVENTTYPE_KEY_UP: /* Обработка нажатий клавиатуры. */ {
            const bool is_active = event->type == SAPP_EVENTTYPE_KEY_DOWN;
            switch (event->key_code) {
                case SAPP_KEYCODE_W: state.keys_buffer[KEY_W] = is_active; break;
                case SAPP_KEYCODE_S: state.keys_buffer[KEY_S] = is_active; break;
                case SAPP_KEYCODE_D: state.keys_buffer[KEY_D] = is_active; break;
                case SAPP_KEYCODE_A: state.keys_buffer[KEY_A] = is_active; break;
                default: break;
            }
        } break;

        default: break;
    }

    /* ######################### */
    /* ### KEYBOARD HANDLING ### */
    /* ######################### */

    if (state.keys_buffer[KEY_W]) /* forward */ {
        main_camera.pos.x += cos(main_camera.direction) * camera_speed;
        main_camera.pos.y += sin(main_camera.direction) * camera_speed;
    }

    if (state.keys_buffer[KEY_S]) /* back */ {
        main_camera.pos.x -= cos(main_camera.direction) * camera_speed;
        main_camera.pos.y -= sin(main_camera.direction) * camera_speed;
    }

    if (state.keys_buffer[KEY_D]) /* right */ {
        main_camera.pos.x -= cos(main_camera.direction - M_PI_2) * camera_side_speed;
        main_camera.pos.y -= sin(main_camera.direction - M_PI_2) * camera_side_speed;
    }

    if (state.keys_buffer[KEY_A]) /* left */ {
        main_camera.pos.x -= cos(main_camera.direction + M_PI_2) * camera_side_speed;
        main_camera.pos.y -= sin(main_camera.direction + M_PI_2) * camera_side_speed;
    }

#ifdef __GNUC__
    #pragma GCC diagnostic pop
#elifdef __clang__
    #pragma clang diagnostic pop
#endif
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
