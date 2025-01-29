cmake_minimum_required(VERSION 3.9)

project(IDKEditorUI)

file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/menubar/*.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})


set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${IDK_OUTPUT_DIRECTORY}/IDKGE/modules/
)


target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../external/include/
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    ImGui
    ImGuizmo
    ImNodes

    libidk
    IDKGraphics
    IDKAudio
    IDKEvents
    IDKECS
)


file(
    GLOB PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/menubar/*.hpp
)

install(
    FILES ${PUBLIC_HEADER}
    DESTINATION ${CMAKE_SOURCE_DIR}/build/include/IDKEditorUI/
)