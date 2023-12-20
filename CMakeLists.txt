cmake_minimum_required(VERSION 3.9)

set(CMAKE_BUILD_PARALLEL_LEVEL 8)
set(CMAKE_CXX_FLAGS "-std=c++17 -Wuninitialized -Winit-self -Wmaybe-uninitialized -Werror -fsanitize=undefined -fdiagnostics-color=always -lstdc++fs")
add_compile_options("-O3" "-fPIC" "-fno-ident" "-fno-asynchronous-unwind-tables")

add_definitions(-DIDK_DEBUG)



project(IDKengine)
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)


include_directories(${CMAKE_SOURCE_DIR}/libidk)
include_directories(${CMAKE_SOURCE_DIR}/IDKGameEngine/src)

add_subdirectory(${CMAKE_SOURCE_DIR}/libidk/libidk/IDKcore)
add_subdirectory(${CMAKE_SOURCE_DIR}/libidk/libidk/IDKgl)
add_subdirectory(${CMAKE_SOURCE_DIR}/libidk/libidk/IDKcommon)
add_subdirectory(${CMAKE_SOURCE_DIR}/IDKGameEngine/src/IDKaudio)
add_subdirectory(${CMAKE_SOURCE_DIR}/IDKGameEngine/src/IDKevents)
add_subdirectory(${CMAKE_SOURCE_DIR}/IDKGameEngine/src/IDKgraphics)


file(GLOB IDKENGINE_SOURCES
    "IDKGameEngine/src/*.cpp"
)
    
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${IDKENGINE_SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION        "${PROJECT_VERSION}"
    SOVERSION      "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    IDKcore
    IDKgl
    IDKcommon
    IDKaudio
    IDKevents
    IDKgraphics
)
