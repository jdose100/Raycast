#pragma once

/*!
    \page raycast page title
*/

#include <SDL3/SDL.h>

#include "fast_math.h"
#include "game/config.h"

#ifdef __cplusplus
extern "C" {
#endif

//! Данные для отрисовки.
typedef struct renderer_data_s {
    SDL_Renderer *renderer;  //< sdl renderer ptr
    SDL_Window *window;      //< sdl window ptr
} renderer_data_t;

/*!
    \public \memberof renderer_data_t
    \brief Создаёт новый конфиг отрисовки.
    \details
    Автоматически создаёт всё необходимое через SDL
    самостоятельно.

    \important Вызывать только после `SDL_Init(SDL_INIT_VIDEO | ...)`;
*/
[[gnu::nonnull(1)]] /**/ [[nodiscard]] static bool renderer_data_new(renderer_data_t *self)
{
    if (!SDL_CreateWindowAndRenderer(title, screen.width, screen.height, 0, &self->window,
                                     &self->renderer))
        return false;

    SDL_SetRenderLogicalPresentation(self->renderer, screen.width, screen.height,
                                     SDL_LOGICAL_PRESENTATION_LETTERBOX);

    return true;
}

// --- FUNCTIONS --- //

//! Инициализация движка графики.
static void graphics_init(void) { __raycast_sin_cost_table_init(); }

#ifdef __cplusplus
}
#endif
