#include "game.hpp"

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

void BlockPuzzle::pick_new_piece()
{
    state.piece = FallingPiece{pieces[0]};
}

void BlockPuzzle::advance(Input input)
{
    apply_input(input);

    if (state.ticks == 20) {
        state.ticks = 0;

        auto dropped = try_drop();

        if (not dropped) {
            lock_piece();
            pick_new_piece();
        }
    }

    state.ticks += 1;
}

}
