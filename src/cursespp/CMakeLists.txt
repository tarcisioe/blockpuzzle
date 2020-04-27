add_library(cursespp)

target_sources(
    cursespp
        PUBLIC
            cursespp.hpp
        PRIVATE
            cursespp.cpp
)

target_link_libraries(
    cursespp
        PRIVATE
            project_options
            CONAN_PKG::ncurses
)
