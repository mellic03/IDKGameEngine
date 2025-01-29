cmake_minimum_required(VERSION 3.9)

project(IDKBuiltinCS)

file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ecs/*.cpp
)

add_library(${PROJECT_NAME} OBJECT ${SOURCES})


set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../external/include/
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
)


FILE(
    COPY ${CMAKE_CURRENT_SOURCE_DIR}/../${PROJECT_NAME}
    DESTINATION ${IDK_OUTPUT_DIRECTORY}/include/.
)
