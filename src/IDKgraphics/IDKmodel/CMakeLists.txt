cmake_minimum_required(VERSION 3.9)

project(IDKmodel)

file(GLOB SOURCES
    "./*.cpp"
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/${idkengine_source_dir})

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    IDKcommon
)


set(${PROJECT_NAME}_DEFINITIONS ${C_DEFINITIONS}
    CACHE INTERNAL "${PROJECT_NAME}: Definitions" FORCE)

set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include
                                 ${Boost_INCLUDE_DIRS}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
