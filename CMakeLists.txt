cmake_minimum_required(VERSION 3.9)
project(IDKGameEngine)


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/IDKGameEngine/IDKaudio)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/IDKGameEngine/IDKevents)


file(GLOB SOURCES
    "src/IDKGameEngine/*.cpp"
    "src/IDKGameEngine/gameobject/*.cpp"
    "src/IDKGameEngine/IDKthreading/*.cpp"
    "src/IDKGameEngine/IDKmodules/*.cpp"
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
)
