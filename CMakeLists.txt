cmake_minimum_required(VERSION 3.16)
project(PointEngine)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

set(SDLTTF_VENDORED ON)

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_image EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_ttf EXCLUDE_FROM_ALL)

# add_compile_options(-fsanitize=address -g)
# add_link_options(-fsanitize=address)

add_executable(PointEngine)
add_library(PElib source/PointEngine.c)

target_sources(PointEngine
    PRIVATE
        source/main.c

)

target_link_libraries(PointEngine PRIVATE SDL3_image::SDL3_image SDL3::SDL3 SDL3_ttf::SDL3_ttf PElib m)
target_link_libraries(PElib       PRIVATE SDL3_image::SDL3_image SDL3::SDL3 SDL3_ttf::SDL3_ttf m)