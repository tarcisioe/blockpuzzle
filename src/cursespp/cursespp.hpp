#ifndef CURSESPP_CURSESPP_HPP
#define CURSESPP_CURSESPP_HPP

#include <memory>
#include <stdexcept>

namespace cursespp {

struct CursesError: std::runtime_error {
    using std::runtime_error::runtime_error;
};

class Window {
public:
    friend class Curses;

    ~Window();

    void move_waddch(int row, int column, char c);
    void add_box();
    void wrefresh();
    int wgetch();
    void keypad(bool enabled);
    void set_timeout(int delay);

    void move_print_int(int row, int column, int value);

private:
    struct Impl;
    Window(std::unique_ptr<Impl> impl);

    std::unique_ptr<Impl> impl_;
};

class Curses {
public:
    Curses();
    ~Curses();

    void cbreak();
    void set_noecho();
    void curs_set(int visibility);

    Window main_window();
    Window newwin(int rows, int columns, int begin_y, int begin_x);

private:
    class Impl;

    std::unique_ptr<Impl> impl;
};

}

#endif
