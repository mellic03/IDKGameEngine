cmake_minimum_required(VERSION 3.9)

project(IDKengine)

file(GLOB SOURCES
    "./src/*.cpp"
)


include_directories(${CMAKE_SOURCE_DIR}/external/include/)
include_directories(${CMAKE_SOURCE_DIR}/src/)

add_subdirectory(src/IDKaudio)
add_subdirectory(src/IDKevents)
add_subdirectory(src/IDKgraphics)

add_library(${PROJECT_NAME} STATIC ${SOURCES})


target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    IDKaudio
    IDKevents
    IDKgraphics
)
