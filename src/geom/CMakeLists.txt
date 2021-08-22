add_library(geom)

target_sources(
    geom
        PUBLIC
            matrix.hpp
        PRIVATE
            matrix.cpp
)

target_include_directories(
    geom
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    geom
        PRIVATE
            project_options
)
