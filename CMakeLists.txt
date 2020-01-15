cmake_minimum_required(VERSION 3.15)
project(SneC C)


set(CMAKE_C_STANDARD 11)

add_executable(SneC main.c snake.h)
target_link_libraries(SneC SDL2 SDL2_ttf)