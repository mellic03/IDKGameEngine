cmake_minimum_required(VERSION 3.9)
project(IDKGameEngine)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/imgui-docking/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/ImGuizmo/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/imnodes-0.5)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/IDKAudio/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/IDKEvents/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/IDKThreading/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/IDKBuiltinCS/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/IDKBuiltinUI/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/IDKGameEngine/)



add_library(${PROJECT_NAME} SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/dummy.cpp)

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
    IDKAudio
    IDKEvents
    IDKThreading
    IDKBuiltinCS
    IDKBuiltinUI
    IDKGameEngine_internal
)


FILE(
    COPY ${CMAKE_CURRENT_SOURCE_DIR}/IDKGE/
    DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/IDKGE/
)

