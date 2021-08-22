cmake_minimum_required(VERSION 3.15)
project(blockpuzzle CXX)

include(cmake/base.cmake)
include(cmake/project_options.cmake)
include(cmake/conan.cmake)

option(RAYLIB_FRONTEND "Enable raylib frontend." FALSE)

set(DEPS ncurses/6.2)

if(RAYLIB_FRONTEND)
    set(DEPS
        ${DEPS}
        raylib/3.5.0
    )
endif()


conan(
    PACKAGES
        ${DEPS}
)

add_subdirectory(src)
