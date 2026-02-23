cmake_minimum_required(VERSION 3.31)
project(raycasting-game LANGUAGES C VERSION 0.0.1)

if (NOT CMAKE_C_COMPILER_ID STREQUAL "GNU" AND NOT CMAKE_C_COMPILER_ID STREQUAL "Clang")
    message(FATAL_ERROR "This project support only GCC 15 (or never) or Clang 22 (or never). You use" ${CMAKE_C_COMPILER_ID})
endif ()

### -- SDL3 -- ###
find_package(SDL3)
if (NOT SDL3_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL
        GIT_TAG        a962f40bbba175e9716557a25d5d7965f134a3d3
        GIT_PROGRESS   TRUE
    )
    FetchContent_MakeAvailable(SDL3)
endif ()

### -- SETTINGS -- ###
# set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
# set(CMAKE_C_STANDARD_REQUIRED ON)
# set(CMAKE_C_EXTENSIONS ON)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    set(
        CMAKE_C_STANDARD_INCLUDE_DIRECTORIES
        ${CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES}
    )
endif ()

### -- EXECUTABLE -- ###
set(MATH_LIB m)

add_executable(main ${PROJECT_SOURCE_DIR}/main.c)
target_link_libraries(main PRIVATE SDL3::SDL3 ${MATH_LIB})
target_compile_options(main PRIVATE -std=gnu2y -Wall -Wextra -Werror)
target_include_directories(main PRIVATE
    ${PROJECT_SOURCE_DIR}/ext_include
    ${PROJECT_SOURCE_DIR}/include
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT WIN32)
    target_compile_options(main PRIVATE -g
        -fsanitize=address -fsanitize=undefined
        -fno-sanitize-recover=all -fno-sanitize=null -fno-sanitize=alignment
        # -fsanitize-blacklist=${PROJECT_SOURCE_DIR}/ignorelist.txt
    )
    target_link_options(main PRIVATE -fsanitize=address -fsanitize=undefined)
else ()
    target_compile_options(main PRIVATE -O3 -ffast-math)
endif ()

### -- DOCUMENTATION -- ###
find_package(Doxygen)
if (Doxygen_FOUND)
    set(DOXYGEN_FILE_PATTERNS *.c *.h)
    set(DOXYGEN_OUTPUT_LANGUAGE Russian)
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_QUIET YES)

    set(DOXYGEN_SKIP_FUNCTION_MACROS NO)
    set(DOXYGEN_MACRO_EXPANSION YES)
    set(DOXYGEN_OPTIMIZE_FOR_C YES)
    set(DOXYGEN_EXTRACT_STATIC YES)
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_RECURSIVE YES)

    doxygen_add_docs(
        docs
        ${PROJECT_SOURCE_DIR}/ext_include/defer.h
        ${PROJECT_SOURCE_DIR}/include/
        ${PROJECT_SOURCE_DIR}/main.c
        ALL
    )
endif ()
