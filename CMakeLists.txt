cmake_minimum_required(VERSION 3.9)
add_compile_options("-Ofast" "-fPIC")

project(IDKengine)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)


include_directories(${CMAKE_SOURCE_DIR}/libidk/src)
include_directories(${CMAKE_SOURCE_DIR}/IDKGameEngine/src)

add_subdirectory(${CMAKE_SOURCE_DIR}/libidk/src/libidk/IDKgl)
add_subdirectory(${CMAKE_SOURCE_DIR}/libidk/src/libidk/IDKcommon)
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
    IDKgl
    IDKcommon
    IDKaudio
    IDKevents
    IDKgraphics
)
