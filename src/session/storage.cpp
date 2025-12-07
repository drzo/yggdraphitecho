// Session storage implementation
#include "storage.h"
#include "../utils/json.h"
#include "../utils/file_utils.h"

#include <fstream>

namespace llmchat {

std::string SessionStorage::serialize(const std::vector<Message>& messages) {
    // Simple JSON serialization
    std::ostringstream oss;
    oss << "{\n";
    oss << "  \"messages\": [\n";
    
    for (size_t i = 0; i < messages.size(); i++) {
        const auto& msg = messages[i];
        
        oss << "    {\n";
        oss << "      \"role\": \"" << Json::escape(msg.role) << "\",\n";
        oss << "      \"content\": \"" << Json::escape(msg.content) << "\"";
        
        if (!msg.name.empty()) {
            oss << ",\n      \"name\": \"" << Json::escape(msg.name) << "\"";
        }
        
        oss << "\n    }";
        
        if (i < messages.size() - 1) {
            oss << ",";
        }
        
        oss << "\n";
    }
    
    oss << "  ]\n";
    oss << "}\n";
    
    return oss.str();
}

std::vector<Message> SessionStorage::deserialize(const std::string& data) {
    std::vector<Message> messages;
    
    // Simple JSON parsing - for production use a proper JSON library
    // This is a minimal implementation
    
    size_t pos = data.find("\"messages\"");
    if (pos == std::string::npos) {
        return messages;
    }
    
    // TODO: Implement proper JSON parsing
    // For now, return empty
    
    return messages;
}

bool SessionStorage::save(const std::string& path, const std::vector<Message>& messages) {
    try {
        std::ofstream file(path);
        if (!file.is_open()) {
            return false;
        }
        
        std::string json = serialize(messages);
        file << json;
        
        return true;
        
    } catch (...) {
        return false;
    }
}

bool SessionStorage::load(const std::string& path, std::vector<Message>& messages) {
    try {
        std::ifstream file(path);
        if (!file.is_open()) {
            return false;
        }
        
        std::string data((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
        
        messages = deserialize(data);
        
        return true;
        
    } catch (...) {
        return false;
    }
}

} // namespace llmchat

