cmake_minimum_required(VERSION 3.9)
project(IDKGameEngine)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/imgui-docking/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/ImGuizmo/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/imnodes-0.5)
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/luajit)


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/IDKAudio/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/IDKEvents/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/IDKThreading/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/IDKBuiltinCS/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/IDKBuiltinUI/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/IDKECS/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/IDKGameEngine/)



add_library(${PROJECT_NAME} SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/dummy.cpp)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)


# if(MSVC)
#     target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
# else()
#     target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
# endif()
find_package(Lua51 REQUIRED)
include_directories(${LUA_INCLUDE_DIR})


target_include_directories(
    ${PROJECT_NAME} PUBLIC
)


target_link_libraries(
    ${PROJECT_NAME} PUBLIC

    IDKAudio
    IDKEvents
    IDKBuiltinCS
    IDKThreading
    IDKECS
    IDKGameEngine_internal

    ImGui
    ImGuizmo
    ImNodes

    ${LUA_LIBRARIES}
)


FILE(
    COPY ${CMAKE_CURRENT_SOURCE_DIR}/IDKGE/
    DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/IDKGE/
)

