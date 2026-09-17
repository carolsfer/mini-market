cmake_minimum_required(VERSION 3.16)
project(MiniMarket VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SQLite3 REQUIRED)

add_executable(minimarket
    src/main.cpp
    src/Database.cpp
)

target_include_directories(minimarket PRIVATE include)
target_link_libraries(minimarket PRIVATE SQLite::SQLite3)

if(MSVC)
    target_compile_options(minimarket PRIVATE /W4)
else()
    target_compile_options(minimarket PRIVATE -Wall -Wextra -Wpedantic)
endif()
