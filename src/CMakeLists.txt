add_subdirectory(cursespp)

add_executable(main)

target_sources(
    main
        PRIVATE
            main.cpp
            matrix.cpp
            matrix.hpp
            piece.cpp
            piece.hpp
)

target_link_libraries(
    main
        PRIVATE
            project_options
            cursespp
)
