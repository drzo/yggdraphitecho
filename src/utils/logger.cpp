// Logger implementation
#include "logger.h"
#include <iostream>
#include <algorithm>
#include <ctime>

namespace llmchat {

Logger::Level Logger::min_level_ = Logger::Level::INFO;
bool Logger::log_to_console_ = false;
std::unique_ptr<std::ofstream> Logger::log_file_;
std::mutex Logger::mutex_;

void Logger::init(const std::string& level_str, const std::string& log_file, bool console) {
    // Set log level
    std::string level_lower = level_str;
    std::transform(level_lower.begin(), level_lower.end(), level_lower.begin(), ::tolower);
    
    if (level_lower == "debug") {
        min_level_ = Level::DEBUG;
    } else if (level_lower == "info") {
        min_level_ = Level::INFO;
    } else if (level_lower == "warn" || level_lower == "warning") {
        min_level_ = Level::WARN;
    } else if (level_lower == "error") {
        min_level_ = Level::ERROR;
    }
    
    log_to_console_ = console;
    
    // Open log file
    if (!log_file.empty()) {
        log_file_ = std::make_unique<std::ofstream>(log_file, std::ios::app);
        if (!log_file_->is_open()) {
            std::cerr << "Warning: Failed to open log file: " << log_file << std::endl;
            log_file_.reset();
        }
    }
}

std::string Logger::format_string(const std::string& format) {
    return format;
}

} // namespace llmchat

