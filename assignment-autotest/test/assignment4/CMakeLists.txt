cmake_minimum_required(VERSION 3.10)

add_executable(assignment-autotest
    Test_threading.c
    ${CMAKE_SOURCE_DIR}/assignment-autotest/Unity/src/unity.c
)

target_include_directories(assignment-autotest PRIVATE
    ${CMAKE_SOURCE_DIR}/examples/threading
    ${CMAKE_SOURCE_DIR}/assignment-autotest/Unity/src
)

target_sources(assignment-autotest PRIVATE
    ${CMAKE_SOURCE_DIR}/examples/threading/threading.c
)
