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
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/IDKECS/)



# add_library(${PROJECT_NAME} SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/dummy.cpp)


set(OpenGL_GL_PREFERENCE GLVND)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
# find_package(SDL2 REQUIRED)
# find_package(SDL2_image REQUIRED)
# find_package(SDL2_mixer REQUIRED)
# find_package(SDL2_ttf REQUIRED)

file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IDKGameEngine/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -rdynamic")

if (IDK_DEBUG)
    target_compile_options(${PROJECT_NAME} PUBLIC -O0 -g)
else()
    target_compile_options(${PROJECT_NAME} PUBLIC -Ofast)
endif ()




target_include_directories(
    ${PROJECT_NAME} PUBLIC
)

include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})
# include_directories(${SDL2_INCLUDE_DIRS})
# include_directories(${SDL2_image_INCLUDE_DIRS})
# include_directories(${SDL2_mixer_INCLUDE_DIRS})
# include_directories(${SDL2_ttf_INCLUDE_DIRS})


target_link_libraries(
    ${PROJECT_NAME} PUBLIC

    GLEW
    SDL2
    SDL2_image
    SDL2_mixer
    SDL2_ttf
    ${OPENGL_LIBRARIES}

    libidk
    IDKGraphics

    IDKAudio
    IDKEvents
    IDKBuiltinCS
    IDKThreading
    IDKECS

    ImGui
    ImGuizmo
    ImNodes

)


FILE(
    COPY ${CMAKE_CURRENT_SOURCE_DIR}/IDKGE/
    DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/IDKGE/
)


