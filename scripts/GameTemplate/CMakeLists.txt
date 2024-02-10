cmake_minimum_required(VERSION 3.9)

set(CMAKE_BUILD_PARALLEL_LEVEL 8)
set(CMAKE_CXX_FLAGS "-std=c++17 -Wuninitialized -Winit-self -Wmaybe-uninitialized -Werror -fsanitize=undefined -fdiagnostics-color=always -lstdc++fs")

add_definitions(-DIDK_DEBUG)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/)


project(game)
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)

include_directories(${CMAKE_SOURCE_DIR}/external/include/)

file(GLOB SOURCES
    "src/*.cpp"
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

# set_target_properties(${PROJECT_NAME} PROPERTIES
#     VERSION        "${PROJECT_VERSION}"
#     SOVERSION      "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
# )

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/external/lib/libIDKengine.so
)

