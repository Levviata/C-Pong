cmake_minimum_required(VERSION 3.16) # Version 3.10 seems to be the bare minimum that is being supported, but use a newer version as minimum because why not

project(cpong C) # Project name

set(SDL_STATIC ON)
add_subdirectory(libs/SDL3)	# Add SDL3 library

set(CMAKE_BUILD_TYPE Debug)

add_executable(cpong src/main.c) # Make an executable based on these files or/and paths

target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE SDL3-static)

