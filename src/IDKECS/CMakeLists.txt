cmake_minimum_required(VERSION 3.9)

project(IDKECS)

file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ecs2/*.cpp
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



file(
    GLOB PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
)

install(
    FILES ${PUBLIC_HEADER}
    DESTINATION ${CMAKE_SOURCE_DIR}/build/include/IDKECS/
)