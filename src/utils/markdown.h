// Markdown rendering
#pragma once

#include <string>

namespace llmchat {

class Markdown {
public:
    static std::string render(const std::string& text);
    static std::string strip_formatting(const std::string& text);
    
private:
    static std::string render_code_blocks(const std::string& text);
    static std::string render_inline_code(const std::string& text);
    static std::string render_bold(const std::string& text);
    static std::string render_italic(const std::string& text);
    static std::string render_headers(const std::string& text);
};

} // namespace llmchat

