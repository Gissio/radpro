cmake_minimum_required(VERSION 3.0.0)

project(sign-crc)

set(CMAKE_C_STANDARD 99)

add_executable(sign-crc sign-crc.cpp)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(fs2011pro PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_include_directories(fs2011pro PRIVATE platform.io/lib/U8g2 platform.io/lib/mcu-max)
