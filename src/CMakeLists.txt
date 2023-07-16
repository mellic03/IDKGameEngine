cmake_minimum_required(VERSION 3.9)

project(IDKengine)


file(GLOB SOURCES
    "./*.cpp"
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/${idkengine_source_dir})

add_subdirectory(IDKcommon)
add_subdirectory(IDKgraphics)
add_subdirectory(IDKaudio)
add_subdirectory(IDKevents)


target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    IDKcommon
    IDKgraphics
    IDKaudio
    IDKevents
)
