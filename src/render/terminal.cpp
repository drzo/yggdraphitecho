// Terminal rendering implementation
#include "terminal.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#define isatty _isatty
#define fileno _fileno
#else
#include <unistd.h>
#include <sys/ioctl.h>
#endif

namespace llmchat {

std::string Terminal::get_color_code(Color color) {
    switch (color) {
        case Color::BLACK:   return "\033[30m";
        case Color::RED:     return "\033[31m";
        case Color::GREEN:   return "\033[32m";
        case Color::YELLOW:  return "\033[33m";
        case Color::BLUE:    return "\033[34m";
        case Color::MAGENTA: return "\033[35m";
        case Color::CYAN:    return "\033[36m";
        case Color::WHITE:   return "\033[37m";
        case Color::RESET:   return "\033[0m";
        default:             return "";
    }
}

void Terminal::print_colored(Color color, const std::string& text) {
    if (is_terminal()) {
        std::cout << get_color_code(color) << text << get_color_code(Color::RESET);
    } else {
        std::cout << text;
    }
}

std::string Terminal::colorize(const std::string& text, Color color) {
    if (is_terminal()) {
        return get_color_code(color) + text + get_color_code(Color::RESET);
    }
    return text;
}

void Terminal::clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    std::cout << "\033[2J\033[H";
#endif
}

void Terminal::move_cursor(int row, int col) {
    std::cout << "\033[" << row << ";" << col << "H";
}

int Terminal::get_terminal_width() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
#endif
}

int Terminal::get_terminal_height() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_row;
#endif
}

bool Terminal::is_terminal() {
    return isatty(fileno(stdout)) != 0;
}

} // namespace llmchat

