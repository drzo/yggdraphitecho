// Simple JSON utilities
#pragma once

#include <string>

namespace llmchat {

class Json {
public:
    static std::string escape(const std::string& str);
    static std::string unescape(const std::string& str);
};

} // namespace llmchat

