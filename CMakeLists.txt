cmake_minimum_required(VERSION 3.24)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_BUILD_PARALLEL_LEVEL 8)
# add_compile_options( -pedantic -Wextra -Ofast )
# -fsanitize=undefined
set (CMAKE_CXX_FLAGS "-lstdc++fs -std=c++17 -fsanitize=undefined -Ofast -fdiagnostics-color=always")

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

    "src/IDKcore/common/datastructures/*.cpp"

    "src/IDKgraphics/IDKgraphics_common/*.cpp"
    "src/IDKgraphics/IDKui/*.cpp"
    "src/IDKgraphics/*.cpp"
    
    "src/IDKGameEngine/IDKGameEngine_common/*.cpp"
    "src/IDKGameEngine/gameobject/*.cpp"
    "src/IDKGameEngine/*.cpp"
    "src/IDKGameEngine/IDK_builtin_modules/*.cpp"
    "src/IDKGameEngine/IDK_builtin_modules/IDK_ui/*.cpp"

)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

find_package(OpenGL REQUIRED)

add_executable(
    ${PROJECT_NAME} "src/main.cpp"
    ${SOURCES}
    ${SDL2_LIBRARY}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_image_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIRS})

target_link_libraries(
    ${PROJECT_NAME}
    GLEW
    SDL2
    SDL2_image
    ${OPENGL_LIBRARIES}
)





project(objconvert)
add_executable(
    ${PROJECT_NAME} "src/IDKtools/objconvert.cpp"
)

