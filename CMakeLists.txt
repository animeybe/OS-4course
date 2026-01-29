cmake_minimum_required(VERSION 3.10)
project(process_lib VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

# Библиотека
add_library(process_lib src/process.cpp)
target_include_directories(process_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Тестовая программа  
add_executable(process_test tests/main.cpp)
target_link_libraries(process_test PRIVATE process_lib)
target_include_directories(process_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
