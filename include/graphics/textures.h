#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <stb_image.h>

#ifdef __GNUC__ /* Enable warnings for GCC  */
    #pragma GCC diagnostic pop
#else /* Enable warnings for clang */
    #pragma clang diagnostic pop
#endif

[[gnu::nonnull(1)]] static SDL_Texture *load_texture(const char *path)
{
    int x, y, comp, req_comp;
    unsigned char *raw_image = stbi_load(path, &x, &y, &comp, req_comp);

    return nullptr;
}
