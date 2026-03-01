cmake_minimum_required(VERSION 3.31)
project(compiled-depencides LANGUAGES C VERSION 0.0.1)

# Библиотека с зависимостями.
add_library(depencides STATIC ${PROJECT_SOURCE_DIR}/compiled_deps.c)
target_include_directories(depencides
    PRIVATE
        ${PROJECT_SOURCE_DIR}/private
        ${PROJECT_SOURCE_DIR}/../include
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include 
)

# Настройка флагов линковки в зависимости от доступной платформы.
if (WIN32)
    target_compile_options(depencides PRIVATE -mwin32 -mwindows)
    target_link_libraries(depencides PUBLIC kernel32 user32 shell32)

    find_package(Vulkan)
    if (Vulkan_FOUND)
        target_link_libraries(depencides PUBLIC Vulkan::Vulkan)
        target_compile_definitions(depencides PRIVATE SOKOL_VULKAN)
        message("Used Vulkan")
    else ()
        target_compile_definitions(depencides PRIVATE SOKOL_GLCORE)
        message("Used OpenGL")
    endif () 
elseif (LINUX)
    target_link_libraries(depencides PUBLIC pthread X11 Xi Xcursor vulkan)
endif ()
