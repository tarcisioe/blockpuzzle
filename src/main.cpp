#include <chrono>
#include <iostream>
#include <thread>

#include "blockpuzzle/game.hpp"
#include "geom/matrix.hpp"

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
    blockpuzzle::Piece const& piece,
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

char block_character(blockpuzzle::BlockType type)
{
    using blockpuzzle::BlockType;

    switch (type) {
        case BlockType::Line: {
            return '=';
        }
        case BlockType::Empty: {
            return ' ';
        }
        default: {
            return '#';
        }
    }
}

void draw_board(cursespp::Window& window, blockpuzzle::Board const& board)
{
    for (auto r = 0; r < board.rows; ++r) {
        auto window_row = r + 1;

        for (auto c = 0; c < board.columns; ++c) {
            auto window_column = 2 * c + 1;

            auto character = block_character(board[{r, c}]);

            window.move_waddch(window_row, window_column, character);
            window.move_waddch(window_row, window_column + 1, character);
        }
    }
}

constexpr auto KEY_DOWN = 0402;
constexpr auto KEY_UP = 0403;
constexpr auto KEY_LEFT = 0404;
constexpr auto KEY_RIGHT = 0405;

blockpuzzle::RNG create_rng()
{
    auto device = std::random_device{};

    return blockpuzzle::RNG{std::default_random_engine{device()}};
}

int main()
try {
    auto curses = cursespp::Curses();

    curses.cbreak();
    curses.set_noecho();
    curses.curs_set(0);

    auto main_window = curses.main_window();

    main_window.keypad(true);
    main_window.set_timeout(0);

    main_window.wrefresh();

    auto board = curses.newwin(20 + 2, 10 * 2 + 2, 0, 0);

    board.add_box();
    board.wrefresh();

    auto cleared = curses.newwin(1 + 2, 5 + 2, 0, 22);

    cleared.add_box();
    cleared.wrefresh();

    auto level = curses.newwin(1 + 2, 5 + 2, 3, 22);

    level.add_box();
    level.wrefresh();

    auto game = blockpuzzle::BlockPuzzle{create_rng()};

    while (not game.is_game_over()) {
        using namespace std::chrono;
        using namespace std::chrono_literals;

        auto frame_start = high_resolution_clock::now();

        auto c = main_window.wgetch();

        if (c == 'q') {
            break;
        }

        auto input = [&]() -> blockpuzzle::Input
        {
            using blockpuzzle::Input;

            switch (c) {
                case KEY_LEFT: {
                    return Input::Left;
                }
                case KEY_RIGHT: {
                    return Input::Right;
                }
                case KEY_DOWN: {
                    return Input::Drop;
                }
                case KEY_UP: {
                    return Input::Rotate;
                }
                case ' ': {
                    return Input::FullDrop;
                }
                default: {
                    return Input::Nothing;
                }
            }
        }();

        game.advance(input);

        auto const& falling = game.current_piece();
        draw_board(board, game.board());
        draw_piece(board, falling.piece, falling.position, falling.rotation);
        cleared.move_print_int(1, 1, game.cleared_count());
        level.move_print_int(1, 1, game.level());

        board.wrefresh();
        cleared.wrefresh();
        level.wrefresh();

        auto done = high_resolution_clock::now();
        auto remaining_time = 16666us - (done - frame_start);

        std::this_thread::sleep_for(remaining_time);
    }
} catch (std::exception const& e) {
    std::clog << e.what() << '\n';
    return 1;
}
