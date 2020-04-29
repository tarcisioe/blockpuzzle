add_library(blockpuzzle)

target_sources(
    blockpuzzle
        PUBLIC
            board.hpp
            game.hpp
            piece.hpp
        PRIVATE
            board.cpp
            game.cpp
            piece.cpp
)

target_include_directories(
    blockpuzzle
        PUBLIC
            "${CMAKE_SOURCE_DIR}/src"
)

target_link_libraries(
    blockpuzzle
        PUBLIC
            geom
        PRIVATE
            project_options
)
