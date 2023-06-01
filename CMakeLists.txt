cmake_minimum_required(VERSION 3.24)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_BUILD_PARALLEL_LEVEL 8)
add_compile_options( -pedantic -Wextra -Ofast )
set (CMAKE_CXX_FLAGS "-lstdc++fs -std=c++17 -fdiagnostics-color=always")

project(IDKEngine)

add_subdirectory (external)
include_directories(
    external/glew-2.1.0/include/
)

set(ALL_LIBS
  ${OPENGL_LIBRARY}
  GLEW_210
)

add_definitions(
  -DTW_STATIC
  -DTW_NO_LIB_PRAGMA
  -DTW_NO_DIRECT3D
  -DGLEW_STATIC
)

file(
    GLOB SOURCES
    "src/*.cpp"
    "src/gameobject/*.cpp"
)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(
    ${PROJECT_NAME} "src/main.cpp"
    ${SOURCES}
    ${SDL2_LIBRARY}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIRS})


add_library(libIDKgraphics STATIC IMPORTED)
set_target_properties(
    libIDKgraphics PROPERTIES
    IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/src/IDKgraphics/libIDKgraphics.a
)


target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIRS})

target_link_libraries(
    ${PROJECT_NAME} PUBLIC libIDKgraphics stdc++fs
    GLEW
    SDL2
    ${OPENGL_LIBRARIES}
)


