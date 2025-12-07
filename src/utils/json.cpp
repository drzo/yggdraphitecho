// JSON utilities implementation
#include "json.h"
#include <sstream>

namespace llmchat {

std::string Json::escape(const std::string& str) {
    std::ostringstream oss;
    
    for (char c : str) {
        switch (c) {
            case '"':  oss << "\\\""; break;
            case '\\': oss << "\\\\"; break;
            case '\b': oss << "\\b";  break;
            case '\f': oss << "\\f";  break;
            case '\n': oss << "\\n";  break;
            case '\r': oss << "\\r";  break;
            case '\t': oss << "\\t";  break;
            default:
                if (c < 0x20) {
                    oss << "\\u" << std::hex << std::setw(4) << std::setfill('0') << (int)c;
                } else {
                    oss << c;
                }
                break;
        }
    }
    
    return oss.str();
}

std::string Json::unescape(const std::string& str) {
    std::ostringstream oss;
    
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == '\\' && i + 1 < str.length()) {
            switch (str[i + 1]) {
                case '"':  oss << '"';  i++; break;
                case '\\': oss << '\\'; i++; break;
                case 'b':  oss << '\b'; i++; break;
                case 'f':  oss << '\f'; i++; break;
                case 'n':  oss << '\n'; i++; break;
                case 'r':  oss << '\r'; i++; break;
                case 't':  oss << '\t'; i++; break;
                default: oss << str[i]; break;
            }
        } else {
            oss << str[i];
        }
    }
    
    return oss.str();
}

} // namespace llmchat

