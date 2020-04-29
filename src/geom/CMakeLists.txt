add_library(geom)

target_sources(
    geom
        PUBLIC
            matrix.hpp
        PRIVATE
            matrix.cpp
)

target_link_libraries(
    geom
        PRIVATE
            project_options
)
