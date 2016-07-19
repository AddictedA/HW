cmake_minimum_required(VERSION 3.3)
project(PEX3_Chiriac_Adrian)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

set(SOURCE_FILES
    byutr.h
    LRU.c LRUstack.c LRUstack.h)

add_executable(PEX3 ${SOURCE_FILES})