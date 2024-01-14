cmake_minimum_required(VERSION 3.9)

project(IDKBuiltinCS)

file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(${PROJECT_NAME} OBJECT ${SOURCES})


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

    ImGui
    ImGuizmo
    ImNodes
)


# target_sources(
#     ${PROJECT_NAME} PUBLIC
#     ${CMAKE_CURRENT_SOURCE_DIR}/idk_camera_cs.cpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/idk_icon_cs.cpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/idk_model_cs.cpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/idk_transform_cs.cpp
# )


# file(
#     GLOB PUBLIC_HEADER
#     # ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp

#     ${CMAKE_CURRENT_SOURCE_DIR}/idk_camera_cs.hpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/idk_icon_cs.hpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/idk_model_cs.hpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/idk_transform_cs.hpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/IDKBuiltinCS.hpp
# )

# install(
#     FILES ${PUBLIC_HEADER}
#     DESTINATION ${CMAKE_SOURCE_DIR}/build/include/IDKBuiltinCS/
# )