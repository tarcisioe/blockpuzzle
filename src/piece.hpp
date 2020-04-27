#ifndef GAME_PIECE_HPP
#define GAME_PIECE_HPP

#include <array>

#include "matrix.hpp"


namespace game {

enum class BlockType {
    Empty,
    I,
    O,
    J,
    L,
    S,
    Z,
    T,
    Line,
};

class Piece {
public:
    using Shape = geom::Matrix<bool, 4, 4>;

    constexpr Piece(Shape const& shape, BlockType type):
        type_{type},
        shape_{shape}
    {}

    constexpr Shape const& shape() const
    {
        return shape_;
    }

    constexpr BlockType type() const
    {
        return type_;
    }

private:
    BlockType type_;
    Shape shape_;
};

constexpr auto pieces = std::array<Piece, 7> {
    Piece{
        {{
            0, 0, 1, 0,
            0, 0, 1, 0,
            0, 0, 1, 0,
            0, 0, 1, 0,
         }},
        BlockType::I,
    },
    {
        {{
            0, 0, 0, 0,
            0, 1, 1, 0,
            0, 1, 1, 0,
            0, 0, 0, 0,
         }},
        BlockType::O,
    },
    {
        {{
            0, 0, 1, 0,
            0, 0, 1, 0,
            0, 1, 1, 0,
            0, 0, 0, 0,
         }},
        BlockType::J,
    },
    {
        {{
            0, 1, 0, 0,
            0, 1, 0, 0,
            0, 1, 1, 0,
            0, 0, 0, 0,
         }},
        BlockType::L,
    },
    {
        {{
            0, 1, 0, 0,
            0, 1, 1, 0,
            0, 0, 1, 0,
            0, 0, 0, 0,
         }},
        BlockType::S,
    },
    {
        {{
            0, 0, 1, 0,
            0, 1, 1, 0,
            0, 1, 0, 0,
            0, 0, 0, 0,
         }},
        BlockType::Z,
    },
    {
        {{
            0, 0, 1, 0,
            0, 1, 1, 0,
            0, 0, 1, 0,
            0, 0, 0, 0,
         }},
        BlockType::T,
    },
};

}


#endif
