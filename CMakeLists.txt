cmake_minimum_required(VERSION 3.30)

project(chip VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED)

file(GLOB files CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/**.cpp")

add_executable(chip ${files})
target_link_libraries(chip PRIVATE SDL3::SDL3)
target_include_directories(chip PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

