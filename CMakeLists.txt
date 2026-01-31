cmake_minimum_required(VERSION 3.16)
project(temp_logger)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(temp_logger 
    src/main.cpp
    src/temp_simulator.cpp
    src/temp_logger.cpp
)

target_include_directories(temp_logger PRIVATE include)
