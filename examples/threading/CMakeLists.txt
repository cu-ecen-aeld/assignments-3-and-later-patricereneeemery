cmake_minimum_required(VERSION 3.10)

add_library(threading
    threading.c
)

target_include_directories(threading PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
