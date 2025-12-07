// String utilities implementation
#include "string_utils.h"
#include <algorithm>
#include <cctype>
#include <sstream>

namespace llmchat {

std::string trim(const std::string& str) {
    return ltrim(rtrim(str));
}

std::string ltrim(const std::string& str) {
    size_t start = 0;
    while (start < str.length() && std::isspace(str[start])) {
        start++;
    }
    return str.substr(start);
}

std::string rtrim(const std::string& str) {
    size_t end = str.length();
    while (end > 0 && std::isspace(str[end - 1])) {
        end--;
    }
    return str.substr(0, end);
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    std::istringstream iss(str);
    std::string token;
    
    while (std::getline(iss, token, delimiter)) {
        result.push_back(token);
    }
    
    return result;
}

std::string join(const std::vector<std::string>& parts, const std::string& separator) {
    if (parts.empty()) return "";
    
    std::ostringstream oss;
    oss << parts[0];
    
    for (size_t i = 1; i < parts.size(); i++) {
        oss << separator << parts[i];
    }
    
    return oss.str();
}

bool starts_with(const std::string& str, const std::string& prefix) {
    return str.length() >= prefix.length() &&
           str.compare(0, prefix.length(), prefix) == 0;
}

bool ends_with(const std::string& str, const std::string& suffix) {
    return str.length() >= suffix.length() &&
           str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
}

std::string to_lower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string to_upper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

std::string replace_all(const std::string& str, const std::string& from, const std::string& to) {
    std::string result = str;
    size_t pos = 0;
    
    while ((pos = result.find(from, pos)) != std::string::npos) {
        result.replace(pos, from.length(), to);
        pos += to.length();
    }
    
    return result;
}

} // namespace llmchat

