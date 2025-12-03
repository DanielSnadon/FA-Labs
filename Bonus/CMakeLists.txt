cmake_minimum_required(VERSION 3.10)
project(Lab1Task1 C)

set(CMAKE_C_STANDARD 99) 
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(main 
    src/main.c
    src/fheap.c
)

target_include_directories(main PRIVATE include)