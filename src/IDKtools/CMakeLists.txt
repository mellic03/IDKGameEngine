cmake_minimum_required(VERSION 3.9)

project(IDKtools)

add_definitions(${IDKcommon_DEFINITIONS})
include_directories(${IDKcommon_INCLUDE_DIRS})

add_definitions(${IDKgraphics_DEFINITIONS})
include_directories(${IDKgraphics_INCLUDE_DIRS})


add_library(${PROJECT_NAME}
            "idk_tools.cpp"
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    IDKcommon
    IDKgraphics
)

