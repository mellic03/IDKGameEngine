cmake_minimum_required(VERSION 3.9)
project(IDKGameEngine)


set(CMAKE_BUILD_PARALLEL_LEVEL 8)
set(CMAKE_CXX_FLAGS "-std=c++17 -Wuninitialized -Winit-self -Wmaybe-uninitialized -Werror -fsanitize=undefined -fdiagnostics-color=always -lstdc++fs")
add_compile_options("-g" "-fvisibility=hidden" "-fPIC" "-fno-ident" "-fno-asynchronous-unwind-tables")
add_definitions(-DIDK_DEBUG)


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/IDKGameEngine/IDKaudio)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/IDKGameEngine/IDKevents)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/IDKGameEngine/IDKgraphics)


file(GLOB SOURCES
    "src/IDKGameEngine/*.cpp"
    "src/IDKGameEngine/gameobject/*.cpp"
    "src/IDKGameEngine/IDKthreading/*.cpp"
    "src/IDKGameEngine/IDKmodules/*.cpp"
)


add_library(${PROJECT_NAME} SHARED ${SOURCES})


target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    IDKaudio
    IDKevents
    IDKgraphics

    libidk
)
