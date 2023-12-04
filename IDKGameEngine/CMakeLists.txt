cmake_minimum_required(VERSION 3.9)

project(IDKengine_internal)

file(GLOB SOURCES
    "./src/*.cpp"
)

add_subdirectory(src/IDKaudio)
add_subdirectory(src/IDKevents)
add_subdirectory(src/IDKgraphics)

add_library(${PROJECT_NAME} OBJECT ${SOURCES})

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    IDKaudio
    IDKevents
    IDKgraphics
)

