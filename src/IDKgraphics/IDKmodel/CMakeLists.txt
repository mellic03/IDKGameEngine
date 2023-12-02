cmake_minimum_required(VERSION 3.9)

project(IDKmodel)

file(GLOB SOURCES
    "./*.cpp"
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/${idkengine_source_dir})

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    libidk
)
