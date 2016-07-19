cmake_minimum_required(VERSION 3.3)
project(PEX2)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

set(SOURCE_FILES
    CPUs.c
    CPUs.h
    main.c
    processQueue.c
    processQueue.h)

add_executable(PEX2 ${SOURCE_FILES})
target_link_libraries(PEX2 pthread)