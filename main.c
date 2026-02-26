#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "graphics/graphics.h"
#include "graphics/raycast.h"

static renderer_data_t renderer_data;

SDL_AppResult SDL_AppInit([[maybe_unused]] void **app_state,
                          [[maybe_unused]] int argc,
                          [[maybe_unused]] char *argv[])
{
    // --- BASIC INIT --- //
    graphics_init();

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't init SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // --- DATA INIT --- //
    if (!renderer_data_new(&renderer_data)) {
        SDL_Log("Couldn't create renderer data. SDL err: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit([[maybe_unused]] void *appstate, [[maybe_unused]] SDL_AppResult result) {}

SDL_AppResult SDL_AppEvent([[maybe_unused]] void *appstate, SDL_Event *event)
{    
    switch (event->type) {
        case SDL_EVENT_QUIT: return SDL_APP_SUCCESS;

        case SDL_EVENT_MOUSE_MOTION: {
            constexpr double rotate_speed = 0.05;

            main_camera.direction += event->motion.xrel * rotate_speed;
            if (main_camera.direction > M_PI) main_camera.direction -= 2 * M_PI;
            if (main_camera.direction < -M_PI) main_camera.direction += 2 * M_PI;
        } break;

        case SDL_EVENT_KEY_DOWN: {
            const SDL_Keycode key = event->key.key;

            if (key == SDLK_W) /* forward */ {
                main_camera.pos.x += cos(main_camera.direction) * camera_speed;
                main_camera.pos.y += sin(main_camera.direction) * camera_speed;
            }

            if (key == SDLK_S) /* back */ {
                main_camera.pos.x -= cos(main_camera.direction) * camera_speed;
                main_camera.pos.y -= sin(main_camera.direction) * camera_speed;
            }

            if (key == SDLK_D) /* right */ {
                main_camera.pos.x -= cos(main_camera.direction - M_PI_2) * camera_side_speed;
                main_camera.pos.y -= sin(main_camera.direction - M_PI_2) * camera_side_speed;
            }

            if (key == SDLK_A) /* left */ {
                main_camera.pos.x -= cos(main_camera.direction + M_PI_2) * camera_side_speed;
                main_camera.pos.y -= sin(main_camera.direction + M_PI_2) * camera_side_speed;
            }
        } break;

        default: break;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate([[maybe_unused]] void *appstate)
{
    SDL_SetRenderDrawColor(renderer_data.renderer, 30, 30, 30, 30);
    SDL_RenderClear(renderer_data.renderer);

    SDL_SetRenderDrawColor(renderer_data.renderer, 240, 240, 240, 30);
    drawing(&renderer_data);

    SDL_RenderPresent(renderer_data.renderer);
    return SDL_APP_CONTINUE;
}

/*!
    \mainpage Main Page
*/
