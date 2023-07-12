cmake_minimum_required(VERSION 3.9)

project(IDKcomponentsystems)


file(GLOB SOURCES
    "./*.cpp"
    "./idk_lightsource_CS/*.cpp"
    "./idk_character_controller_CS/*.cpp"
)


add_subdirectory(idk_physics_CS)

add_library(
    ${PROJECT_NAME} SHARED
    ${SOURCES}
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    idk_physics_CS
)

