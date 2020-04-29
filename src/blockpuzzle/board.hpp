#ifndef BLOCKPUZZLE_BOARD_HPP
#define BLOCKPUZZLE_BOARD_HPP

#include "geom/matrix.hpp"
#include "piece.hpp"

namespace blockpuzzle {

class Board {
public:
    constexpr static auto rows = 20;
    constexpr static auto columns = 10;

    using Blocks = geom::Matrix<BlockType, rows, columns>;

    Board()
    {
        blocks.fill(BlockType::Empty);
    }

    BlockType operator[](geom::Position2D position) const
    {
        return blocks[{position, geom::Rotation::R0}];
    }

    BlockType& operator[](geom::Position2D position)
    {
        return blocks[{position, geom::Rotation::R0}];
    }

    bool in_bounds(geom::Position2D position) const
    {
        return (position.row >= 0 and position.row < rows) and
               (position.column >= 0 and position.column < columns);
    }

    bool piece_fits(
        Piece const& piece,
        geom::Position2D top_left,
        geom::Rotation rotation) const;

private:
    Blocks blocks;
};

}

#endif
