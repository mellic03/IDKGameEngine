cmake_minimum_required(VERSION 3.9)

project(IDKBuiltinUI)

file(GLOB SOURCES
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/IDKBuiltinCS/*.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/menubar/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tabs/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/*.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CXX_STANDARD 17
)

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../external/include/
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC

    IDKGraphics

    ImGui
    ImGuizmo
    ImNodes
)




file(
    GLOB PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/menubar/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tabs/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/*.hpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/idk_camera_cs.hpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/idk_icon_cs.hpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/idk_model_cs.hpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/idk_transform_cs.hpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/IDKBuiltinCS.hpp
)

install(
    FILES ${PUBLIC_HEADER}
    DESTINATION ${CMAKE_SOURCE_DIR}/build/include/IDKBuiltinUI/
)