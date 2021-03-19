cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(fr)

## If you want to link SFML statically
# set(SFML_STATIC_LIBRARIES TRUE)

## In most cases better set in the CMake cache
set(SFML_DIR "lib/cmake/SFML")
include_directories(${SFML_DIR})

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

file(GLOB SRC "src/*.hpp" "src/*.cpp")

set(CMAKE_CXX_FLAGS "-g")

add_executable(fr ${SRC})

target_link_libraries(fr sfml-graphics sfml-audio)
