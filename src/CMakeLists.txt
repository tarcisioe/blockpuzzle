add_subdirectory(geom)
add_subdirectory(blockpuzzle)
add_subdirectory(cursespp)

add_executable(main)

target_sources(
    main
        PRIVATE
            main.cpp
)

target_link_libraries(
    main
        PRIVATE
            project_options
            blockpuzzle
            geom
            cursespp
)
