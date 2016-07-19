cmake_minimum_required(VERSION 3.3)
project(PEX1_Chiriac)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

set(SOURCE_FILES
    afsh.c
    afsh.h
    list.c
    list.h
    main.c)

add_executable(afsh ${SOURCE_FILES})