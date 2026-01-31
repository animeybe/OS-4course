cmake_minimum_required(VERSION 3.16)
project(temp_logger)

set(CMAKE_CXX_STANDARD 17)

# Создаем исполняемый файл
add_executable(temp_logger 
    src/main.cpp 
    src/temp_simulator.cpp 
    src/temp_logger.cpp 
    src/data_types.cpp
)

# Заголовочные файлы
target_include_directories(temp_logger PRIVATE include)

# Кроссплатформенность
if(WIN32)
    target_link_libraries(temp_logger wsock32 ws2_32)
endif()
