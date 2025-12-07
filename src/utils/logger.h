// Logging utilities
#pragma once

#include <string>
#include <fstream>
#include <mutex>
#include <memory>

namespace llmchat {

class Logger {
public:
    enum class Level {
        DEBUG,
        INFO,
        WARN,
        ERROR
    };
    
    static void init(const std::string& level_str, const std::string& log_file = "", bool console = false);
    
    template<typename... Args>
    static void debug(const std::string& format, Args&&... args) {
        log(Level::DEBUG, format, std::forward<Args>(args)...);
    }
    
    template<typename... Args>
    static void info(const std::string& format, Args&&... args) {
        log(Level::INFO, format, std::forward<Args>(args)...);
    }
    
    template<typename... Args>
    static void warn(const std::string& format, Args&&... args) {
        log(Level::WARN, format, std::forward<Args>(args)...);
    }
    
    template<typename... Args>
    static void error(const std::string& format, Args&&... args) {
        log(Level::ERROR, format, std::forward<Args>(args)...);
    }
    
private:
    template<typename... Args>
    static void log(Level level, const std::string& format, Args&&... args);
    
    static std::string format_string(const std::string& format);
    
    template<typename T, typename... Args>
    static std::string format_string(const std::string& format, T&& value, Args&&... args);
    
    static Level min_level_;
    static bool log_to_console_;
    static std::unique_ptr<std::ofstream> log_file_;
    static std::mutex mutex_;
};

// Template implementations
template<typename T, typename... Args>
std::string Logger::format_string(const std::string& format, T&& value, Args&&... args) {
    size_t pos = format.find("{}");
    if (pos == std::string::npos) {
        return format;
    }
    
    std::string result = format.substr(0, pos);
    
    std::ostringstream oss;
    oss << value;
    result += oss.str();
    
    result += format.substr(pos + 2);
    
    return format_string(result, std::forward<Args>(args)...);
}

template<typename... Args>
void Logger::log(Level level, const std::string& format, Args&&... args) {
    if (level < min_level_) return;
    
    std::string message = format_string(format, std::forward<Args>(args)...);
    
    const char* level_str = "";
    switch (level) {
        case Level::DEBUG: level_str = "DEBUG"; break;
        case Level::INFO:  level_str = "INFO "; break;
        case Level::WARN:  level_str = "WARN "; break;
        case Level::ERROR: level_str = "ERROR"; break;
    }
    
    // Get timestamp
    time_t now = time(nullptr);
    char timestamp[32];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    std::string log_line = std::string("[") + timestamp + "] [" + level_str + "] " + message + "\n";
    
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (log_to_console_) {
        if (level >= Level::ERROR) {
            std::cerr << log_line;
        } else {
            std::cout << log_line;
        }
    }
    
    if (log_file_ && log_file_->is_open()) {
        *log_file_ << log_line;
        log_file_->flush();
    }
}

} // namespace llmchat

