cmake_minimum_required(VERSION 3.9)
add_compile_options("-fPIC")

project(IDKengine)

file(GLOB SOURCES
    "./src/*.cpp"
)


add_library(${PROJECT_NAME} SHARED ${SOURCES})

include_directories(${CMAKE_SOURCE_DIR}/external/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

add_subdirectory(src/IDKaudio)
add_subdirectory(src/IDKevents)
add_subdirectory(src/IDKgraphics)

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    IDKaudio
    IDKevents
    IDKgraphics
)

