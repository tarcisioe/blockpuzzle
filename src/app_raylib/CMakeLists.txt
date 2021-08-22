project(tetris_raylib C CXX)

add_executable(tetris_raylib)

target_sources(
    tetris_raylib
        PRIVATE
            main.cpp
)

target_include_directories(
    tetris_raylib
        PUBLIC
            "${CMAKE_SOURCE_DIR}/src"
)

target_link_libraries(
    tetris_raylib
        PRIVATE
            project_options
            blockpuzzle
            CONAN_PKG::raylib
)
