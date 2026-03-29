/*
    Данный файл существует лишь только для компиляции разного рода
    HEADER-ONLY зависимостей, а именно их реализации для ускорения
    компиляции основного проекта.
*/

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define SOKOL_IMPL
#include "sokol_app.h"
#include "sokol_audio.h"
#include "sokol_gfx.h"
#include "sokol_gl.h"
#include "sokol_glue.h"
#include "sokol_log.h"
#include "sokol_debugtext.h"
