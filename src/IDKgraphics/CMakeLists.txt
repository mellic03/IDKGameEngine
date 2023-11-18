cmake_minimum_required(VERSION 3.9)

project(IDKgraphics)


file(GLOB SOURCES
    "./*.cpp"
    "./IDKlightsource/*.cpp"
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/${idkengine_source_dir})

add_subdirectory(IDKmodel)
add_subdirectory(IDKgl)


target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    IDKmodel
    IDKgl
)

