#pragma once

#include <sokol_gfx.h>
#include <stb_image.h>

enum LoadTextureError {
    LOAD_TEXTURE_OK,
    LOAD_TEXTURE_STB_ERROR,
};

[[nodiscard]] [[gnu::nonnull(1)]] static enum LoadTextureError load_texture(
    sg_view *out, const char *path)
{
    int width, height, channels;
    auto image = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);

    if (!image)
        return LOAD_TEXTURE_STB_ERROR;

    defer { stbi_image_free(image); }

    // clang-format off
    *out = sg_make_view(&(sg_view_desc){
        .texture.image = sg_make_image(&(sg_image_desc){
            .pixel_format = SG_PIXELFORMAT_RGBA8,
            .width = width,
            .height = height,
            .data.mip_levels[0] = {
                .size = (size_t)(width * height * 4),
                .ptr = image,
            },
        }),
    });
    // clang-format on

    return LOAD_TEXTURE_OK;
}
