// Syntax highlighter
#pragma once

#include <string>

namespace llmchat {

class Highlighter {
public:
    static std::string highlight_code(const std::string& code, const std::string& language);
    static std::string detect_language(const std::string& code);
    
private:
    static std::string highlight_cpp(const std::string& code);
    static std::string highlight_python(const std::string& code);
    static std::string highlight_bash(const std::string& code);
};

} // namespace llmchat

