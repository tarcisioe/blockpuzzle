cmake_minimum_required(VERSION 3.15)
project(blockpuzzle CXX)

include(cmake/base.cmake)
include(cmake/project_options.cmake)
include(cmake/conan.cmake)

conan(
    # PACKAGES
)

add_subdirectory(src)
