cmake_minimum_required(VERSION 3.9)

project(IDKengine)

add_subdirectory(IDKgraphics)
find_package(Bullet REQUIRED)


file(GLOB SOURCES
    "./*.cpp"
    "./IDK_builtin_CS/*.cpp"
    "./IDK_builtin_CS/IDK_physics/*.cpp"
)
add_library(${PROJECT_NAME} ${SOURCES})
include_directories(./)

target_include_directories(${PROJECT_NAME} PUBLIC ${BULLET_INCLUDE_DIRS})
target_compile_definitions(${PROJECT_NAME} PUBLIC ${BULLET_DEFINITIONS})
target_link_directories(${PROJECT_NAME} PUBLIC ${Bullet_DIR}/${BULLET_ROOT_DIR}/${BULLET_LIBRARY_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC IDKgraphics)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    IDKgraphics
    BulletCollision BulletDynamics LinearMath
)
