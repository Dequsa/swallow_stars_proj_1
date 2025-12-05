cmake_minimum_required(VERSION 4.0)
project(swallow_game_proj_1)

find_package(Curses REQUIRED)

#target_include_directories(swallow_game_proj_1 PRIVATE ${CURSES_INCLUDE_DIR}

set(CMAKE_CXX_STANDARD 20)

add_executable(swallow_game_proj_1 main.cpp
        player.h
        player.cpp
        board.h
        board.cpp
        stars.h
        stars.cpp
        hunters.h
        hunters.cpp
        taxi.h
        taxi.cpp
        wind.cpp
)

target_link_libraries(swallow_game_proj_1 ${CURSES_LIBRARIES})

file(COPY CONFIGS DESTINATION ${CMAKE_BINARY_DIR})