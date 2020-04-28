#include <iostream>

#include "board.hpp"
#include "matrix.hpp"
#include "piece.hpp"

#include "cursespp/cursespp.hpp"

template <typename T>
void print_matrix(geom::Matrix<T, 4, 4> const& m, geom::Rotation r)
{
    for (auto i = 0; i < 4; ++i) {
        for (auto j = 0; j < 4; ++j) {
            std::cout << m[{{i, j}, r}] << ' ';
        }
        std::cout << '\n';
    }
}

void draw_piece(
    cursespp::Window& window,
    game::Piece const& piece,
    geom::Position2D top_left,
    geom::Rotation rotation)
{
    for (auto r = 0; r < 4; ++r) {
        auto window_row = top_left.row + r + 1;

        for (auto c = 0; c < 4; ++c) {
            auto window_column = 2 * (top_left.column + c) + 1;

            auto solid = piece.shape()[{{r, c}, rotation}];

            if (solid) {
                window.move_waddch(window_row, window_column, '#');
                window.move_waddch(window_row, window_column + 1, '#');
            }
        }
    }
}

constexpr auto KEY_DOWN = 66;
constexpr auto KEY_UP = 65;
constexpr auto KEY_LEFT = 68;
constexpr auto KEY_RIGHT = 67;

geom::Rotation next(geom::Rotation rotation)
{
    auto value = static_cast<int>(rotation);
    return static_cast<geom::Rotation>((value + 1) % 4);
}

int main()
try {
    auto curses = cursespp::Curses();

    curses.cbreak();
    curses.set_noecho();
    curses.curs_set(0);

    auto main_window = curses.main_window();

    main_window.wrefresh();

    auto board = curses.newwin(20 + 2, 10 * 2 + 2, 0, 0);

    board.add_box();
    board.wrefresh();

    auto const& piece = game::pieces[0];

    auto position = geom::Position2D{0, 0};
    auto rotation = geom::Rotation::R0;

    draw_piece(board, piece, position, geom::Rotation::R0);

    board.wrefresh();

    auto clear = [&board]()
    {
        for (auto row = 1; row < 21; ++row) {
            for (auto column = 1; column < 21; ++column) {
                board.move_waddch(row, column, ' ');
            }
        }
    };

    auto game_board = game::Board{};

    while (true) {
        auto c = main_window.wgetch();
        board.move_print_int(0, 0, c);

        if (c == 'q') {
            break;
        }

        auto delta = [&]() -> geom::Position2D
        {
            switch (c) {
                case KEY_LEFT: {
                    return {0, -1};
                }
                case KEY_RIGHT: {
                    return {0, 1};
                }
                case KEY_DOWN: {
                    return {1, 0};
                }
            }

            return {0, 0};
        }();

        auto new_rotation = [&]() -> geom::Rotation
        {
            switch (c) {
                case KEY_UP: {
                    return next(rotation);
                }
            }

            return rotation;
        }();

        if (game_board.piece_fits(piece, position + delta, new_rotation))
        {
            position += delta;
            rotation = new_rotation;
        }

        clear();
        draw_piece(board, piece, position, rotation);

        board.wrefresh();
    }
} catch (std::exception const& e) {
    std::clog << e.what() << '\n';
    return 1;
}
