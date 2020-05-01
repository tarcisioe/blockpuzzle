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

class PieceBag {
public:
    PieceBag(RNG& rng):
        indices{rng.random_piece_sequence()}
    {}

    Piece const& random_piece()
    {
        return pieces[indices[current_index++]];
    }

    bool empty() {
        return current_index >= indices.size();
    }

private:
    std::array<std::size_t, 7> indices;
    std::size_t current_index{0};
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
    FullDrop,
    Nothing,
};

class BlockPuzzle {
public:
    BlockPuzzle(RNG rng):
        rng_{std::move(rng)},
        pieces{rng_},
        state{FallingPiece{pick_new_piece()}}
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
    Piece const& pick_new_piece();
    void mark_cleared_lines();
    void clear_lines();
    bool try_drop();

    Board board_;
    RNG rng_;
    PieceBag pieces;
    GameState state;
    bool game_over{false};
};

}

#endif
