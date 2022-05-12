cmake_minimum_required(VERSION 3.2)

project(oscLab LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(deps)

file(GLOB ROOT_SOURCE 
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable (oscLab 
    ${ROOT_SOURCE}
)

include_directories(deps/libopz/deps)
include_directories(deps/libopz/include)
include_directories(deps/liblo)
target_include_directories(oscLab PRIVATE
    # "${PROJECT_SOURCE_DIR}/deps/libopz/deps"
    # "${PROJECT_SOURCE_DIR}/deps/libopz/include"
    "${PROJECT_SOURCE_DIR}/src"
)

target_link_libraries(oscLab PRIVATE libopz lo_static ncurses)
