cmake_minimum_required(VERSION 3.10)
project(TimerLab)

set(CMAKE_CXX_STANDARD 17)

# Создаем директорию для логов
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/logs)

add_executable(timer_lab 
    src/main.cpp 
    src/timer.cpp
)

target_include_directories(timer_lab PRIVATE include)

if(WIN32)
    target_link_libraries(timer_lab ws2_32)
endif()
