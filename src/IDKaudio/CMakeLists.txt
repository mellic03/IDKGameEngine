cmake_minimum_required(VERSION 3.9)

project(IDKaudio)

file(GLOB SOURCES
    "./*.cpp"
)

add_library(${PROJECT_NAME} OBJECT ${SOURCES})

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
)

