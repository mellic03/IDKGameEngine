cmake_minimum_required(VERSION 3.9)

project(IDKcommon)

add_subdirectory(glew-2.1.0)


find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(OpenGL REQUIRED)


file(
    SOURCES
)
add_library(${PROJECT_NAME} ${SOURCES})
include_directories(./)

target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_image_INCLUDE_DIRS})


target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    GLEW
    SDL2
    SDL2_image
    ${OPENGL_LIBRARIES}
    ${EXTRA_LIBS}
)

