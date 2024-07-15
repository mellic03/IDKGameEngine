cmake_minimum_required(VERSION 3.9)

project(IDKIO)

file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)


add_library(${PROJECT_NAME} OBJECT ${SOURCES})

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    libidk
)

