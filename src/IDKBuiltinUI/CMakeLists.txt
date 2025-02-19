cmake_minimum_required(VERSION 3.9)

project(IDKBuiltinUI)
# add_compile_options("-shared" "-fPIC" "-fno-ident" "-fno-asynchronous-unwind-tables")

file(GLOB SOURCES
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/IDKBuiltinCS/*.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/menubar/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tabs/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/*.cpp
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


FILE(
    COPY ${CMAKE_CURRENT_SOURCE_DIR}/../${PROJECT_NAME}
    DESTINATION ${IDK_OUTPUT_DIRECTORY}/include/.
)
