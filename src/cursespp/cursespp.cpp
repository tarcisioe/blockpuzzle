#include "cursespp.hpp"

#include <curses.h>

namespace cursespp {

namespace {
void check_error(int error_code, char const* message)
{
    if (error_code == ERR) {
        throw CursesError{message};
    }
}
}

struct Window::Impl {
    Impl(WINDOW* window): window_{window} {}

    ~Impl()
    {
        if (window_ != stdscr) {
            delwin(window_);
        }
    }

    WINDOW* window_;
};

Window::Window(std::unique_ptr<Impl> impl): impl_{std::move(impl)} {}

Window::~Window() = default;

void Window::move_waddch(int row, int column, char c)
{
    check_error(
        mvwaddch(impl_->window_, row, column, static_cast<chtype>(c)),
        "Error on call to mvwaddch().");
}

void Window::add_box()
{
    check_error(::box(impl_->window_, 0, 0), "Error on call to box().");
}

void Window::wrefresh()
{
    check_error(::wrefresh(impl_->window_), "Error on call to wrefresh().");
}

int Window::wgetch()
{
    return ::wgetch(impl_->window_);
}

void Window::move_print_int(int row, int column, int value)
{
    check_error(::mvwprintw(impl_->window_, row, column, "%d", value), "Error on call to mvwprintw.");
}

void Window::keypad(bool enabled)
{
    check_error(::keypad(impl_->window_, enabled), "Error on call to keypad.");
}

void Window::set_timeout(int delay)
{
    ::wtimeout(impl_->window_, delay);
}

class Curses::Impl {
public:
    Impl()
    {
        initscr();
    }

    ~Impl()
    {
        endwin();
    }
};

Curses::Curses(): impl{std::make_unique<Impl>()} {}

Curses::~Curses() = default;

void Curses::cbreak()
{
    check_error(::cbreak(), "Error on call to cbreak().");
}

void Curses::set_noecho()
{
    check_error(::noecho(), "Error on call to noecho().");
}

void Curses::curs_set(int visibility)
{
    check_error(::curs_set(visibility), "Error on call to curs_set().");
}

Window Curses::main_window()
{
    return Window{std::make_unique<Window::Impl>(stdscr)};
}

Window Curses::newwin(int rows, int columns, int begin_y, int begin_x)
{
    return Window{std::make_unique<Window::Impl>(
        ::newwin(rows, columns, begin_y, begin_x))};
}

}
