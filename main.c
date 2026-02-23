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
            // Шаг вращения угла в пределах 360 градусов.
            constexpr const double step = 360.0 / screen.width;

            // Получаем коодинату x мыши.
            SDL_GetMouseState(&main_camera.mice_x, nullptr);

            // Вращаем угол (если необходимо).
            const float difference = main_camera.mice_x - main_camera.last_x;
            if (difference == 0) break;
            main_camera.angle += step * difference;

            // Нормализует угл для сохранения предела [0; 360].
            while (main_camera.angle > 360.0) main_camera.angle -= 360.0;
            while (main_camera.angle < 0.0) main_camera.angle += 360.0;

            // Сохраняем прошлое значение x.
            main_camera.last_x = main_camera.mice_x;
        } break;

        case SDL_EVENT_KEY_DOWN: {
            if (event->key.key == SDLK_W) main_camera.pos.x += camera_speed;  // forward
            if (event->key.key == SDLK_S) main_camera.pos.x -= camera_speed;  // back
            if (event->key.key == SDLK_D) main_camera.pos.y += camera_speed;  // right
            if (event->key.key == SDLK_A) main_camera.pos.y -= camera_speed;  // left

            printf("cam pos: x(%f), y(%f)\n", main_camera.pos.x / title_size,
                   main_camera.pos.y / title_size);
        } break;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate([[maybe_unused]] void *appstate)
{
    SDL_SetRenderDrawColor(renderer_data.renderer, 30, 30, 30, 30);
    SDL_RenderClear(renderer_data.renderer);

    SDL_SetRenderDrawColor(renderer_data.renderer, 240, 240, 240, 30);
    raycastring(&renderer_data);

    SDL_RenderPresent(renderer_data.renderer);
    return SDL_APP_CONTINUE;
}

/*!
    \mainpage Main Page
*/
