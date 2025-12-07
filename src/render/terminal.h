// Terminal rendering utilities
#pragma once

#include <string>

namespace llmchat {

class Terminal {
public:
    enum class Color {
        BLACK,
        RED,
        GREEN,
        YELLOW,
        BLUE,
        MAGENTA,
        CYAN,
        WHITE,
        RESET
    };
    
    static void print_colored(Color color, const std::string& text);
    static void clear_screen();
    static void move_cursor(int row, int col);
    static int get_terminal_width();
    static int get_terminal_height();
    static bool is_terminal();
    static std::string colorize(const std::string& text, Color color);
    
private:
    static std::string get_color_code(Color color);
};

} // namespace llmchat

