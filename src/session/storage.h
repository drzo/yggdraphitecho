// Session storage
#pragma once

#include "../inference/engine.h"
#include <string>
#include <vector>

namespace llmchat {

class SessionStorage {
public:
    bool save(const std::string& path, const std::vector<Message>& messages);
    bool load(const std::string& path, std::vector<Message>& messages);
    
private:
    std::string serialize(const std::vector<Message>& messages);
    std::vector<Message> deserialize(const std::string& data);
};

} // namespace llmchat

