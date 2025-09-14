cmake_minimum_required(VERSION 3.10)
project(FA_Lab1 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "-Wall -Wextra -Werror -pedantic")

add_executable(main src/main.c)

target_include_directories(main PRIVATE include)