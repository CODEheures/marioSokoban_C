cmake_minimum_required(VERSION 3.9)
project(sokoban C)

include_directories("./include")
link_directories("./libs")

set(CMAKE_C_STANDARD 99)

add_executable(sokoban src/main.c src/game.c src/game.h src/utils.c src/utils.h)