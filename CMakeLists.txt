cmake_minimum_required(VERSION 3.16)

project(cpp_thread_pool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(thread_pool
    src/thread_pool.cpp
)

target_include_directories(thread_pool
    PUBLIC include
)

add_executable(basic_example
    examples/basic_example.cpp
)

target_link_libraries(basic_example
    thread_pool
)
