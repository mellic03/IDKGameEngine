cmake_minimum_required(VERSION 3.9)
project(TemplateGameName)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)


file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)


add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output/
)


if (IDK_DEBUG)
    target_compile_options(${PROJECT_NAME} PUBLIC -Werror -fsanitize=undefined -lstdc++fs -g)
else()
    target_compile_options(${PROJECT_NAME} PUBLIC -Ofast)
endif ()



target_link_libraries(
    ${PROJECT_NAME} PUBLIC

    libidk
    IDKGraphics

    IDKAudio
    IDKEvents
    IDKBuiltinCS
    IDKBuiltinUI
    IDKECS
    IDKPhysics
)

