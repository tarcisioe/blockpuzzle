#include <chrono>
#include <iostream>
#include <optional>
#include <random>
#include <thread>
#include <variant>

#include <raylib.h>

#include "blockpuzzle/board.hpp"
#include "blockpuzzle/game.hpp"
#include "blockpuzzle/piece.hpp"
#include "geom/matrix.hpp"

// Map block types to curses characters.
//
// Args:
//     type: The type of the block to be rendered.
// Returns:
//     A character representing such block.
Color block_color(blockpuzzle::BlockType type)
{
    static const Color colors[]
        {BLACK, BLUE, BROWN, RED, GREEN, ORANGE, PURPLE, YELLOW, WHITE};
    return colors[static_cast<std::size_t>(type)];
}

void draw_board(blockpuzzle::Board const& board)
{
    for (auto r = 0; r < blockpuzzle::Board::rows; ++r) {
        for (auto c = 0; c < blockpuzzle::Board::columns; ++c) {
            DrawRectangle(
                24 + c * 24,
                24 + r * 24,
                24,
                24,
                block_color(board[{r, c}]));
        }
    }
}

void draw_piece(blockpuzzle::FallingPiece const& falling)
{
    auto& piece = falling.piece.get();
    auto type = piece.type();
    auto color = block_color(type);

    for (auto r = 0; r < 4; ++r) {
        auto window_row = falling.position.row + r;
        auto first_column = falling.position.column;

        for (auto c = 0; c < 4; ++c) {
            auto window_column = first_column + c;
            auto solid = piece.shape()[{{r, c}, falling.rotation}];
            if (solid) {
                DrawRectangle(
                    24 + window_column * 24,
                    24 + window_row * 24,
                    24,
                    24,
                    color);
            }
        }
    }
}

struct GameWindow {
    blockpuzzle::BlockPuzzle game;

    GameWindow(): game{new_game()}
    {
        InitWindow(
            24 * (game.board().columns + 2) + 150,
            24 * (game.board().rows + 2),
            "BLOCKPUZZLE");
        SetTargetFPS(60);
    }

    blockpuzzle::BlockPuzzle new_game()
    {
        std::random_device rd{};
        std::default_random_engine engine{rd()};
        return {std::move(engine)};
    }

    ~GameWindow()
    {
        CloseWindow();
    }

    void run()
    {
        using namespace std::chrono;
        using namespace std::chrono_literals;

        const auto INPUT_RATE = 120ms;

        auto tick_start = high_resolution_clock::now();
        while (not (WindowShouldClose() or IsKeyPressed(KEY_Q))) {
            auto elapsed = high_resolution_clock::now() - tick_start;

            if (IsKeyPressed(KEY_Q)) {
                break;
            }

            if (game.is_game_over()) {
                if (IsKeyPressed(KEY_R)) {
                    game = new_game();
                }
            } else {
                auto input = [&]
                {
                    if (IsKeyPressed(KEY_UP)) {
                        return blockpuzzle::Input::Rotate;
                    }
                    if (elapsed > INPUT_RATE) {
                        if (IsKeyDown(KEY_SPACE)) {
                            return blockpuzzle::Input::FullDrop;
                        }
                        if (IsKeyDown(KEY_DOWN)) {
                            return blockpuzzle::Input::Drop;
                        }
                        if (IsKeyDown(KEY_LEFT)) {
                            return blockpuzzle::Input::Left;
                        }
                        if (IsKeyDown(KEY_RIGHT)) {
                            return blockpuzzle::Input::Right;
                        }
                    }
                    return blockpuzzle::Input::Nothing;
                }();
                if (input != blockpuzzle::Input::Nothing) {
                    tick_start = high_resolution_clock::now();
                }
                game.advance(input);
            }

            // Draw
            auto const board_width = game.board().columns * 24;
            auto const board_height = game.board().rows * 24;
            BeginDrawing();
            ClearBackground(BLACK);
            DrawRectangleLines(
                23,
                23,
                board_width + 2,
                board_height + 2,
                WHITE);

            DrawText(
                FormatText("Level: %3i", game.level()),
                board_width + 48,
                24,
                20,
                YELLOW);
            DrawText(
                FormatText("Cleared: %3i", game.cleared_count()),
                board_width + 48,
                66,
                20,
                YELLOW);

            if (game.is_game_over()) {
                DrawText("GAME OVER", 48, 48, 32, YELLOW);
                DrawText("Press 'R' to try again", 48, 96, 16, WHITE);
            } else {
                draw_board(game.board());
                draw_piece(game.current_piece());
            }

            EndDrawing();
        }
    }
};

int main()
try {
    auto game_window = GameWindow{};
    game_window.run();
} catch (std::exception const& e) {
    std::clog << e.what() << '\n';
    return 1;
} catch (...) {
    std::clog << "Aborted with unknown error.";
    return 1;
}
