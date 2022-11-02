cmake_minimum_required(VERSION 3.12)

project(gaggit C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)
if (${GAGGIT_WITH_SUBMODULES})
    add_subdirectory(lib)
endif()
