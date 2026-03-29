cmake_minimum_required(VERSION 3.31)
project(raycasting-shaders)

# Библиотека, хранящая массив шейдеров.
add_library(shaders INTERFACE)
target_include_directories(shaders INTERFACE ${PROJECT_BINARY_DIR}/compiled)
make_directory(${PROJECT_BINARY_DIR}/compiled/shaders)

# Функция, компилирующая шейдеры.
function(compile_shader shader)
    # Находил sokol-shdc для компилируемой платфомы.
    find_program(sokol_shdc NAMES sokol-shdc PATHS ${PROJECT_SOURCE_DIR}/bin REQUIRED)

    # Выбираем формат между Vulkan и OpenGL 4.1
    find_package(Vulkan OPTIONAL)
    if (Vulkan_FOUND OR LINUX)
        set(SHADER_LANG spirv_vk)
    else ()
        set(SHADER_LANG glsl410)
    endif ()

    # Компилируем шейдер и добавляем его в зависимость к остальным шейдерам.
    set(OUT ${PROJECT_BINARY_DIR}/compiled/shaders/${shader}.h)
    add_custom_command(
        COMMAND ${sokol_shdc} -i ${PROJECT_SOURCE_DIR}/${shader} -l ${SHADER_LANG} -o ${OUT}
        OUTPUT ${OUT}
        DEPENDS ${PROJECT_SOURCE_DIR}/${shader}
    )
    add_custom_target(${shader} ALL DEPENDS ${OUT})
    add_dependencies(shaders ${shader})
endfunction()

# Все шейдеры, которые надо скомпилировать.
compile_shader(raycast.glsl)
