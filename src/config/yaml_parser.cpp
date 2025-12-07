// Simple YAML parser implementation
#include "yaml_parser.h"
#include <algorithm>
#include <cctype>

namespace llmchat {

YamlParser::YamlParser(const std::string& filepath) {
    parse_file(filepath);
}

std::string YamlParser::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

void YamlParser::parse_file(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filepath);
    }
    
    std::string line;
    std::string current_section;
    
    while (std::getline(file, line)) {
        // Skip comments and empty lines
        size_t comment_pos = line.find('#');
        if (comment_pos != std::string::npos) {
            line = line.substr(0, comment_pos);
        }
        
        line = trim(line);
        if (line.empty()) continue;
        
        // Parse key-value pairs
        size_t colon_pos = line.find(':');
        if (colon_pos != std::string::npos) {
            std::string key = trim(line.substr(0, colon_pos));
            std::string value = trim(line.substr(colon_pos + 1));
            
            // Remove quotes
            if (!value.empty() && (value.front() == '"' || value.front() == '\'')) {
                value = value.substr(1, value.length() - 2);
            }
            
            // Handle sections
            if (!current_section.empty()) {
                key = current_section + "." + key;
            }
            
            data_[key] = value;
        }
    }
}

bool YamlParser::get(const std::string& key, std::string& value) {
    auto it = data_.find(key);
    if (it != data_.end()) {
        value = it->second;
        return true;
    }
    return false;
}

bool YamlParser::get(const std::string& key, int& value) {
    std::string str_value;
    if (get(key, str_value)) {
        try {
            value = std::stoi(str_value);
            return true;
        } catch (...) {
            return false;
        }
    }
    return false;
}

bool YamlParser::get(const std::string& key, float& value) {
    std::string str_value;
    if (get(key, str_value)) {
        try {
            value = std::stof(str_value);
            return true;
        } catch (...) {
            return false;
        }
    }
    return false;
}

bool YamlParser::get(const std::string& key, bool& value) {
    std::string str_value;
    if (get(key, str_value)) {
        std::transform(str_value.begin(), str_value.end(), str_value.begin(), ::tolower);
        value = (str_value == "true" || str_value == "yes" || str_value == "1");
        return true;
    }
    return false;
}

bool YamlParser::get(const std::string& key, std::vector<std::string>& value) {
    // Simple implementation - look for array-like values
    // For full YAML support, use yaml-cpp
    value.clear();
    return false;
}

} // namespace llmchat

