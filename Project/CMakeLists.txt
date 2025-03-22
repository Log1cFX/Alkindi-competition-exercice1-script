cmake_minimum_required(VERSION 3.10.0)
project(Project LANGUAGES C)

file(GLOB SOURCES "${Project_SOURCE_DIR}/Src/*.c")

add_executable(Project ${SOURCES})
target_include_directories(Project PRIVATE "${Project_SOURCE_DIR}/Inc")

if (WIN32)
    target_link_libraries(Project user32)
endif()
