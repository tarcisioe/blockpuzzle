#include "board.hpp"

namespace game {
bool Board::piece_fits(
    Piece const& piece,
    geom::Position2D top_left,
    geom::Rotation rotation) const
{
    for (auto row = 0; row < 4; ++row) {
        for (auto column = 0; column < 4; ++column) {
            auto solid = piece.shape()[{{row, column}, rotation}];

            if (not solid) {
                continue;
            }

            auto board_position = top_left + geom::Position2D{row, column};

            if (not in_bounds(board_position) or
                (*this)[board_position] != BlockType::Empty) {
                return false;
            }
        }
    }

    return true;
}
}
