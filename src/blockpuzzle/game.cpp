#include "game.hpp"

#include <algorithm>


namespace blockpuzzle {

namespace {

geom::Rotation next(geom::Rotation rotation)
{
    auto value = static_cast<int>(rotation);
    return static_cast<geom::Rotation>((value + 1) % 4);
}

}


void BlockPuzzle::apply_input(Input input)
{
    if (input == Input::FullDrop) {
        while (try_drop());
        return;
    }

    auto delta = [&]() -> geom::Position2D
    {
        switch (input) {
            case Input::Left: {
                return {0, -1};
            }
            case Input::Right: {
                return {0, 1};
            }
            case Input::Drop: {
                return {1, 0};
            }
            default: {
                return {0, 0};
            }
        }
    }();

    auto new_position = state.piece.position + delta;

    auto new_rotation = [&]() -> geom::Rotation
    {
        switch (input) {
            case Input::Rotate: {
                return next(state.piece.rotation);
            }
            default: {
                return state.piece.rotation;
            }
        }
    }();

    if (board_.piece_fits(
            state.piece.piece,
            new_position,
            new_rotation)) {
        state.piece.position = new_position;
        state.piece.rotation = new_rotation;
    }
}

bool BlockPuzzle::try_drop()
{
    auto dropped_position = state.piece.position + geom::Position2D{1, 0};

    if (board_.piece_fits(
            state.piece.piece,
            dropped_position,
            state.piece.rotation)) {
        state.piece.position = dropped_position;
        return true;
    }

    return false;
}

void BlockPuzzle::lock_piece()
{
    auto const& piece = state.piece.piece.get();
    auto const& position = state.piece.position;
    auto const& rotation = state.piece.rotation;

    for (auto r = 0; r < 4; ++r) {
        for (auto c = 0; c < 4; ++c) {
            if (piece.shape()[{{r, c}, rotation}]) {
                board_[position + geom::Position2D{r, c}] = piece.type();
            }
        }
    }
}

Piece const& BlockPuzzle::pick_new_piece()
{
    if (pieces.empty()) {
        pieces = PieceBag{rng_};
    }

    return pieces.random_piece();
}

void BlockPuzzle::mark_cleared_lines()
{
    auto start = state.piece.position.row;
    auto end = state.piece.position.row + 4;

    for (auto r = start; r < end ; ++r) {
        [&]()
        {
            for (auto c = 0; c < board_.columns; ++c) {
                if (board_[{r, c}] == BlockType::Empty) {
                    return;
                }
            }

            state.cleared_lines.push_back(r);
        }();
    }

    for (auto cleared: state.cleared_lines) {
        for (auto c = 0; c < board_.columns; ++c) {
            board_[{cleared, c}] = BlockType::Line;
        }
    }

    state.clearing_ticks = 20;
}

namespace {
    bool contains(std::vector<int> const& v, int value)
    {
        auto end = std::end(v);

        return std::find(begin(v), end, value) != end;
    }
}

void BlockPuzzle::clear_lines()
{
    auto writing_row = board_.rows - 1;

    for (auto row = writing_row; row >= 0; --row) {
        if (contains(state.cleared_lines, row)) {
            continue;
        }

        for (auto c = 0; c < board_.columns; ++c) {
            board_[{writing_row, c}] = board_[{row, c}];
        }

        --writing_row;
    }

    state.cleared_lines.clear();
}

void BlockPuzzle::advance(Input input)
{
    if (state.clearing_ticks > 0) {
        --state.clearing_ticks;
        return;
    }

    if (not state.cleared_lines.empty()) {
        clear_lines();
    }

    apply_input(input);

    if (state.ticks == 20) {
        state.ticks = 0;

        auto dropped = try_drop();

        if (not dropped) {
            lock_piece();

            mark_cleared_lines();

            state.piece = FallingPiece{pick_new_piece()};
        }
    }

    state.ticks += 1;
}

}
