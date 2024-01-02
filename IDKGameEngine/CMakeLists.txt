cmake_minimum_required(VERSION 3.9)
project(IDKGameEngine)

set(CMAKE_BUILD_PARALLEL_LEVEL 8)
set(CMAKE_CXX_FLAGS "-std=c++17 -Wuninitialized -Winit-self -Wmaybe-uninitialized -Werror -fsanitize=undefined -fdiagnostics-color=always -lstdc++fs")
add_compile_options("-g" "-fvisibility=hidden" "-fPIC" "-fno-ident" "-fno-asynchronous-unwind-tables")
add_definitions(-DIDK_DEBUG)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/IDKGameEngine/src/)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/IDKGameEngine/src/IDKaudio)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/IDKGameEngine/src/IDKevents)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/IDKGameEngine/src/IDKgraphics)


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

    idk
)
