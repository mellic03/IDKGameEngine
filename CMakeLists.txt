cmake_minimum_required(VERSION 3.9)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_BUILD_PARALLEL_LEVEL 8)
# add_compile_options( -pedantic -Wextra -Ofast )
# -fsanitize=undefined
set (CMAKE_CXX_FLAGS "-lstdc++fs -std=c++17 -fsanitize=undefined -g -fdiagnostics-color=always")
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)


project(IDKGame)

add_subdirectory(src)

add_executable(
    ${PROJECT_NAME} "src/main.cpp"
)

include_directories(src)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    IDKengine
    # IDKtools
)




