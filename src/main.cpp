#include <iostream>

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
            auto window_column = 2*(top_left.column + c) + 1;

            auto solid = piece.shape()[{{r, c}, rotation}];

            if (solid) {
                window.move_waddch(window_row, window_column, '#');
                window.move_waddch(window_row, window_column+1, '#');
            }
        }
    }

}

constexpr auto KEY_DOWN	= 0402;
constexpr auto KEY_UP = 0403;
constexpr auto KEY_LEFT = 0404;
constexpr auto KEY_RIGHT =  0405;

int main()
try {
    auto curses = cursespp::Curses();

    curses.cbreak();
    curses.set_noecho();
    curses.curs_set(0);

    auto main_window = curses.main_window();

    main_window.wrefresh();

    auto board = curses.newwin(
        20 + 2,
        10*2 + 2,
        0,
        0
    );

    board.add_box();
    board.wrefresh();

    auto const& piece = game::pieces[0];

    auto position = geom::Position2D{0, 0};

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

    while (true) {
        auto c = main_window.wgetch();

        if (c == 'q') {
            break;
        }

        switch (c) {
            case 'a': {
                position += {0, -1};
                break;
            }
            case 'e': {
                position += {0, 1};
                break;
            }
        }

        clear();
        draw_piece(board, piece, position, geom::Rotation::R0);

        board.wrefresh();
    }
} catch (...) {
    return 1;
}

