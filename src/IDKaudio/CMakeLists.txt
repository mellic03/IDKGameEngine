cmake_minimum_required(VERSION 3.9)

project(IDKaudio)

file(GLOB SOURCES
    "./*.cpp"
)

add_library(${PROJECT_NAME} OBJECT ${SOURCES})

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CXX_STANDARD 17
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
)

