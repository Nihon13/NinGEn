cmake_minimum_required(VERSION 3.10)
project(NinGEn)

file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/src/*.c
    ${CMAKE_SOURCE_DIR}/src/*.cpp)

set(LIBS
    ${CMAKE_SOURCE_DIR}/include/GLFW/libglfw.so)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} ${LIBS})