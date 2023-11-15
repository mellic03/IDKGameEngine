cmake_minimum_required(VERSION 3.9)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_BUILD_PARALLEL_LEVEL 8)
set(CMAKE_CXX_FLAGS "-std=c++17 -Wuninitialized -Winit-self -Wmaybe-uninitialized -Werror -fsanitize=undefined -fdiagnostics-color=always -lstdc++fs")
add_compile_options("-O3" "-s" "-fno-ident" "-fno-asynchronous-unwind-tables" "-I/usr/include/opencv4/")

add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)


# Debugging
# --------------------------------------------
add_definitions(-DIDK_DEBUG)
# --------------------------------------------

# IDKEngine source directory
# --------------------------------------------
set(idkengine_source_dir "IDKGameEngine/src/")
add_subdirectory(IDKGameEngine/src/)
# --------------------------------------------

# add_subdirectory(IDKGame/src/ComponentSystems/)
add_subdirectory(Game/Modules/)


project(game)

add_executable(
    ${PROJECT_NAME} "Game/main.cpp"
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    IDKengine
    # IDKcomponentsystems
    IDKmodules
)






# project(texconvert)

# add_executable(
#     ${PROJECT_NAME} "IDKGameEngine/src/IDKtools/texconvert.cpp"
# )

# target_link_libraries(
#     ${PROJECT_NAME} PUBLIC
#     IDKengine
#     IDKcomponentsystems
#     IDKmodules
# )
