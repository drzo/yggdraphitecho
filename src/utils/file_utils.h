// File utilities
#pragma once

#include <string>
#include <vector>

namespace llmchat {

bool file_exists(const std::string& path);
bool is_directory(const std::string& path);
bool create_directories(const std::string& path);
std::string read_file(const std::string& path);
bool write_file(const std::string& path, const std::string& content);
std::vector<std::string> list_directory(const std::string& path);
std::string get_file_extension(const std::string& path);
std::string get_filename(const std::string& path);
std::string get_directory(const std::string& path);
std::string join_paths(const std::string& a, const std::string& b);

} // namespace llmchat

