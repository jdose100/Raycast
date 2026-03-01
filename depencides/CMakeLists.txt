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

    # Используется vulkan-1.lib из версии Vulkan SDK 1.3.275.0
    target_link_directories(depencides PUBLIC ${PROJECT_SOURCE_DIR})
    target_link_libraries(depencides PUBLIC kernel32 user32 shell32 vulkan-1)
elseif (LINUX)
    target_link_libraries(depencides PUBLIC pthread X11 Xi Xcursor vulkan)
endif ()
