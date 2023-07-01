cmake_minimum_required(VERSION 3.9)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_BUILD_PARALLEL_LEVEL 8)
# add_compile_options( -pedantic -Wextra -Ofast )
# -fsanitize=undefined
set (CMAKE_CXX_FLAGS "-lstdc++fs -std=c++17 -fsanitize=undefined -g -fdiagnostics-color=always")
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

project(IDKGame)

add_subdirectory (src/IDKengine)

file(
    GLOB SOURCES
    "src/*.cpp"
)

add_executable(
    ${PROJECT_NAME} "src/main.cpp"
    ${SOURCES}
)
include_directories(./)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    IDKengine
)




project(objconvert)


file(
    GLOB SOURCES
    "src/IDKtools/*.cpp"
)

add_executable(
    ${PROJECT_NAME} "src/IDKtools/objconvert.cpp"
    ${SOURCES}
)
include_directories(./)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    IDKengine
)



# objconvert ------------------------------------------------------------------
# file(
#     GLOB SOURCES
#     "src/IDKtools/*.cpp"
#     "src/IDKcore/common/datastructures/*.cpp"
#     "src/IDKcore/common/util/*.cpp"

#     "src/IDKgraphics/IDKgraphics_common/*.cpp"
#     "src/IDKgraphics/IDKui/*.cpp"
#     "src/IDKgraphics/*.cpp"
# )


# add_executable(
#     objconvert "src/IDKtools/objconvert.cpp"
#     ${SOURCES}
#     ${SDL2_LIBRARY}
# )


# target_include_directories(objconvert PUBLIC ${OPENGL_INCLUDE_DIRS})
# target_include_directories(objconvert PUBLIC ${SDL2_INCLUDE_DIRS})
# target_include_directories(objconvert PUBLIC ${SDL2_image_INCLUDE_DIRS})
# target_include_directories(objconvert PUBLIC ${OPENGL_INCLUDE_DIRS})


# target_link_libraries(
#     objconvert PUBLIC
#     GLEW
#     SDL2
#     SDL2_image
#     ${OPENGL_LIBRARIES}
# )
# -----------------------------------------------------------------------------
