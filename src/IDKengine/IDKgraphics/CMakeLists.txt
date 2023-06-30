cmake_minimum_required(VERSION 3.9)

project(IDKgraphics)

add_subdirectory(IDKcore)

file(
    GLOB SOURCES
    "./*.cpp"
)
add_library(${PROJECT_NAME} ${SOURCES})
include_directories(./)

target_include_directories( ${PROJECT_NAME} PUBLIC IDKcore)


target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    IDKcore
)

