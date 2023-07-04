cmake_minimum_required(VERSION 3.9)

project(IDKengine)


file(GLOB SOURCES
    "idk_builtin_CS/*.cpp"
    "idk_builtin_CS/IDK_physics/*.cpp"
)

add_library(${PROJECT_NAME} SHARED ${SOURCES} "idk_engine.cpp")
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src/)

find_package(Bullet REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${BULLET_INCLUDE_DIR})
target_compile_definitions(${PROJECT_NAME} PUBLIC ${BULLET_DEFINITIONS})
target_link_directories(${PROJECT_NAME} PUBLIC ${Bullet_DIR}/${BULLET_ROOT_DIR}/${BULLET_LIBRARY_DIRS})

add_subdirectory(IDKcommon)
add_subdirectory(IDKgraphics)


target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    IDKcommon
    IDKgraphics
    BulletDynamics BulletCollision LinearMath
)

