// Simple YAML parser for configuration
#pragma once

#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>

namespace llmchat {

// Simple YAML parser (basic key-value support)
// For production, consider using yaml-cpp library
class YamlParser {
public:
    explicit YamlParser(const std::string& filepath);
    
    bool get(const std::string& key, std::string& value);
    bool get(const std::string& key, int& value);
    bool get(const std::string& key, float& value);
    bool get(const std::string& key, bool& value);
    bool get(const std::string& key, std::vector<std::string>& value);
    
private:
    void parse_file(const std::string& filepath);
    std::string trim(const std::string& str);
    
    std::map<std::string, std::string> data_;
};

} // namespace llmchat

