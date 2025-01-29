cmake_minimum_required(VERSION 3.9)
project(IDKGameEngine-Complete)

cmake_policy(SET CMP0048 NEW)

set(CMAKE_BUILD_PARALLEL_LEVEL 24)
set(CMAKE_INSTALL_MESSAGE LAZY)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_COLOR_DIAGNOSTICS ON)

set(IDK_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${IDK_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${IDK_OUTPUT_DIRECTORY}/IDKGE/runtime/)


if(MSVC)
else()
    add_compile_options(
        # -Wextra
        # -Wpedantic
        # -Werror
        # -Wuninitialized -Wmaybe-uninitialized
        # -Winit-self
        # -fsanitize=undefined -fno-sanitize=vptr -lstdc++fs
    )
endif()

string(ASCII 27 Esc)
set(Green "${Esc}[32m")


if (IDK_DEBUG)
    message("${Green}------ Debug build ------")
    add_definitions(-DIDK_DEBUG)
    set(CMAKE_CXX_FLAGS "-Werror -fsanitize=undefined -fsanitize-undefined-trap-on-error -lstdc++fs")
    # add_compile_options("-g" "-O1")
    add_compile_options("-g")

else ()
    message("${Green}------ Release build ------")
    add_compile_options("-O3")

endif ()


add_definitions(
    -DTW_STATIC
    -DTW_NO_LIB_PRAGMA
    -DTW_NO_DIRECT3D
    -DGLEW_STATIC
)


include_directories(${CMAKE_SOURCE_DIR}/external/glew-2.1.0/include/)
include_directories(${CMAKE_SOURCE_DIR}/external/libidk/src/)
include_directories(${CMAKE_SOURCE_DIR}/external/IDKPhysics/src/)
include_directories(${CMAKE_SOURCE_DIR}/external/IDKGraphics/src/)
include_directories(${CMAKE_SOURCE_DIR}/external/IDKGameEngine/src/)
include_directories(${CMAKE_SOURCE_DIR}/src/)


add_subdirectory(${CMAKE_SOURCE_DIR}/external/glm/glm/)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/libidk/)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/IDKPhysics/)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/IDKGraphics/)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/IDKGameEngine/)

# add_subdirectory(${CMAKE_SOURCE_DIR}/src/bossfight/)
# add_subdirectory(${CMAKE_SOURCE_DIR}/src/flightsim/)
# add_subdirectory(${CMAKE_SOURCE_DIR}/src/flightsim-editor/)
# add_subdirectory(${CMAKE_SOURCE_DIR}/src/watercraft/)
# add_subdirectory(${CMAKE_SOURCE_DIR}/src/planets/)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/spacegame/)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/evo/)


FILE(
    COPY ${CMAKE_SOURCE_DIR}/src/assets/
    DESTINATION ${IDK_OUTPUT_DIRECTORY}/assets/
)
