#ifndef GAME_GAME_HPP
#define GAME_GAME_HPP

#include <functional>
#include <vector>

#include "rng.hpp"
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
    std::vector<int> cleared_lines;
    int clearing_ticks{0};
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
    BlockPuzzle(RNG rng):
        rng_{std::move(rng)},
        state{FallingPiece{random_piece()}}
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

    Piece const& random_piece()
    {
        auto index = rng_.get_int();

        return pieces[index];
    }

    void advance(Input user_input);

private:
    void apply_input(Input user_input);
    void lock_piece();
    void pick_new_piece();
    void mark_cleared_lines();
    void clear_lines();
    bool try_drop();

    RNG rng_;
    Board board_;
    GameState state;
    bool game_over{false};
};

}

#endif
