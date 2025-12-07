// String utilities
#pragma once

#include <string>
#include <vector>

namespace llmchat {

std::string trim(const std::string& str);
std::string ltrim(const std::string& str);
std::string rtrim(const std::string& str);
std::vector<std::string> split(const std::string& str, char delimiter);
std::string join(const std::vector<std::string>& parts, const std::string& separator);
bool starts_with(const std::string& str, const std::string& prefix);
bool ends_with(const std::string& str, const std::string& suffix);
std::string to_lower(const std::string& str);
std::string to_upper(const std::string& str);
std::string replace_all(const std::string& str, const std::string& from, const std::string& to);

} // namespace llmchat

