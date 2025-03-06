cmake_minimum_required(VERSION 3.10)

project(SDLTest)

find_package(SDL3 REQUIRED)
link_libraries(SDL3)

find_package(SDL3_image REQUIRED)
link_libraries(SDL3_image)

find_package(box2d REQUIRED)
link_libraries(box2d)

include_directories(include)

add_executable(SDLTest  src/main.cpp)

