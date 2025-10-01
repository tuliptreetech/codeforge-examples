cmake_minimum_required(VERSION 3.22)

project(CODEFORGE_EXAMPLES)

if (CMAKE_QPCPP_EXAMPLE)
    add_subdirectory(qpcpp)
endif ()

if (CMAKE_CPP_EXAMPLE)
    add_subdirectory(cpp)
endif ()