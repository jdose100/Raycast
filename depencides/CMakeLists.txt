cmake_minimum_required(VERSION 3.31)
project(compiled-depencides LANGUAGES C VERSION 0.0.1)

# Библиотека с зависимостями.
add_library(depencides STATIC ${PROJECT_SOURCE_DIR}/compiled_deps.c)
target_include_directories(depencides
    PRIVATE
        ${PROJECT_SOURCE_DIR}/private
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

# Настройка флагов линковки в зависимости от доступной платформы.
if (WIN32)
elseif (LINUX)
    target_link_libraries(depencides PUBLIC pthread X11 Xi Xcursor vulkan)
endif ()
