cmake_minimum_required(VERSION 3.10)
project(NinGEn)

file(GLOB_RECURSE SOURCE_FILES 
    ${CMAKE_SOURCE_DIR}/src/*.c
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/include/glad/glad.c
    ${CMAKE_SOURCE_DIR}/include/nhntxtcol.cpp
    ${CMAKE_SOURCE_DIR}/include/stb/*.cpp
)

set(LIBS
    ${CMAKE_SOURCE_DIR}/include/GLFW/libglfw.so
    GL
    dl
    m
    stdc++
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include/glm)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME} ${LIBS})

# TODO:
# - list of files as SOURCE_FILES
# - OS dependent linking