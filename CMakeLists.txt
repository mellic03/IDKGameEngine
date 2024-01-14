cmake_minimum_required(VERSION 3.9)
project(IDKGameEngine)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/imgui-docking/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/ImGuizmo/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/imnodes-0.5)


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/IDKaudio/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/IDKevents/)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/IDKBuiltinCS/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/IDKBuiltinUI/)


file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IDKGameEngine/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IDKGameEngine/gameobject/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IDKGameEngine/IDKthreading/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IDKGameEngine/IDKmodules/*.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CXX_STANDARD 17
)


# if(MSVC)
#     target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
# else()
#     target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
# endif()



target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    IDKaudio
    IDKevents
    IDKBuiltinCS
    IDKBuiltinUI
)


FILE(
    COPY ${CMAKE_CURRENT_SOURCE_DIR}/IDKGE/
    DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/IDKGE/
)

