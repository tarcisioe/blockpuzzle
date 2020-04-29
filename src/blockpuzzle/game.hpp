#ifndef GAME_GAME_HPP
#define GAME_GAME_HPP

#include <functional>

#include "board.hpp"

namespace blockpuzzle {

struct FallingPiece {
    explicit FallingPiece(Piece const& p):
        piece{p}
    {}

    std::reference_wrapper<Piece const> piece;
    geom::Position2D position{0, 0};
    geom::Rotation rotation{geom::Rotation::R0};
};

struct GameState {
    explicit GameState(FallingPiece p):
        piece{std::move(p)}
    {}

    FallingPiece piece;
    int ticks{0};
};

enum class Input {
    Rotate,
    Drop,
    Right,
    Left,
    Nothing,
};

class BlockPuzzle {
public:
    BlockPuzzle():
        state{FallingPiece{pieces[0]}}
    {}

    bool is_game_over() const
    {
        return game_over;
    }

    FallingPiece const& current_piece() const
    {
        return state.piece;
    }

    Board const& board() const
    {
        return board_;
    }

    void advance(Input user_input);

private:
    void apply_input(Input user_input);
    void lock_piece();
    void pick_new_piece();
    bool try_drop();

    Board board_;
    GameState state;
    bool game_over{false};
};

}

#endif