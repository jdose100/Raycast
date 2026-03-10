cmake_minimum_required(VERSION 3.31)
project(raycasting-game LANGUAGES C VERSION 0.0.1)
add_subdirectory(depencides)
add_subdirectory(shaders)

if (NOT CMAKE_C_COMPILER_ID STREQUAL "GNU" AND NOT CMAKE_C_COMPILER_ID STREQUAL "Clang")
    message(FATAL_ERROR "This project support only GCC 15 (or never) or Clang 22 (or never). You use" ${CMAKE_C_COMPILER_ID})
endif ()

######################
### -- SETTINGS -- ###
######################
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    set(
        CMAKE_C_STANDARD_INCLUDE_DIRECTORIES
        ${CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES}
    )

    if (WIN32)
        set(CMAKE_C_USE_RESPONSE_FILE_FOR_INCLUDES OFF)
    endif ()
endif ()

########################
### -- EXECUTABLE -- ###
########################
set(MATH_LIB m)

add_executable(main ${PROJECT_SOURCE_DIR}/main.c)
target_link_libraries(main PRIVATE ${MATH_LIB} depencides shaders)
target_include_directories(main PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_compile_options(main PRIVATE
    -std=gnu2y -fstack-protector-strong --include defer.h
    # Warnings
    -Werror -Wall -Wextra -Wunreachable-code -Wwrite-strings
    -Wswitch -Wswitch-enum -Wswitch-default -Wconversion
    -Wformat=2 -Wmissing-field-initializers -Wcast-align -Wcast-qual
    -Wundef 
)

# -- Specific flags
if (CMAKE_C_COMPILER_ID STREQUAL "GNU") # gcc specific flags
    target_compile_options(main PRIVATE -fanalyzer)
else () # clang specific flags
    target_compile_options(main PRIVATE -Wnull-dereference -fdefer-ts)
endif ()

#########################
### -- DEBUG SETUP -- ###
#########################

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(main PRIVATE -g -Og -fno-omit-frame-pointer)

    if (LINUX)
        target_compile_options(main PRIVATE
            -fsanitize=address -fsanitize=undefined
            -fno-sanitize-recover=all -fno-sanitize=null -fno-sanitize=alignment
            # -fsanitize-blacklist=${PROJECT_SOURCE_DIR}/ignorelist.txt
        )
        target_link_options(main PRIVATE -fsanitize=address -fsanitize=undefined)
    endif ()
else ()
    target_compile_options(main PRIVATE -O3 -ffast-math)

    if (WIN32)
        target_compile_options(main PRIVATE -mwin32 -mwindows)
    endif ()
endif ()

###########################
### -- DOCUMENTATION -- ###
###########################

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
        ${PROJECT_SOURCE_DIR}/include/
        ${PROJECT_SOURCE_DIR}/main.c
        ALL
    )
endif ()
