cmake_minimum_required(VERSION 3.9)

set(CMAKE_BUILD_PARALLEL_LEVEL 8)
set(CMAKE_CXX_FLAGS "-std=c++17 -Wuninitialized -Winit-self -Wmaybe-uninitialized -Werror -fsanitize=undefined -fdiagnostics-color=always -lstdc++fs")
add_compile_options("-g" "-fvisibility=hidden" "-fPIC" "-fno-ident" "-fno-asynchronous-unwind-tables")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/output)


add_definitions(-DIDK_DEBUG)

project(IDKengine)


include_directories(${CMAKE_SOURCE_DIR}/external/include/)
include_directories(${CMAKE_SOURCE_DIR}/IDKGameEngine/src/)

add_subdirectory(${CMAKE_SOURCE_DIR}/IDKGameEngine/src/IDKaudio)
add_subdirectory(${CMAKE_SOURCE_DIR}/IDKGameEngine/src/IDKevents)
add_subdirectory(${CMAKE_SOURCE_DIR}/IDKGameEngine/src/IDKgraphics)


file(GLOB SOURCES
    "IDKGameEngine/src/*.cpp"
    "IDKGameEngine/src/gameobject/*.cpp"
    "IDKGameEngine/src/IDKthreading/*.cpp"
    "IDKGameEngine/src/IDKmodules/*.cpp"
)


add_library(${PROJECT_NAME} SHARED ${SOURCES})


target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    IDKaudio
    IDKevents
    IDKgraphics
)
