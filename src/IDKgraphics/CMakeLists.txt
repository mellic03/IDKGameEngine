cmake_minimum_required(VERSION 3.9)

project(IDKgraphics)

file(GLOB SOURCES
    "./*.cpp"
    "./lighting/*.cpp"
    "./model/*.cpp"
)

add_library(${PROJECT_NAME} OBJECT ${SOURCES})

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
)

