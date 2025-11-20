cmake_minimum_required(VERSION 3.10)
project(MailServiceSim C)

set(CMAKE_C_STANDARD 99) 
set(CMAKE_C_STANDARD_REQUIRED ON)

set(SOURCE_FILES
    include/structures.h
    src/main.c
    src/queue.c
    src/manager.c
    src/simulation.c
    src/menu.c
)

add_executable(main ${SOURCE_FILES})

target_include_directories(main PRIVATE include)