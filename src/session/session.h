// Session management
#pragma once

#include "../config/config.h"
#include "../inference/engine.h"
#include <string>
#include <vector>
#include <memory>

namespace llmchat {

class Session {
public:
    Session(const Config& config, const std::string& name);
    ~Session();
    
    // Session operations
    void add_message(const Message& msg);
    void clear();
    bool save();
    bool save(const std::string& path);
    bool load(const std::string& path);
    
    // Getters
    const std::string& name() const { return name_; }
    const std::vector<Message>& messages() const { return messages_; }
    int token_count() const { return token_count_; }
    
    // Compression
    void compress_if_needed(InferenceEngine& engine);
    
private:
    void update_token_count(InferenceEngine& engine);
    std::string get_session_path() const;
    
    const Config& config_;
    std::string name_;
    std::vector<Message> messages_;
    int token_count_ = 0;
    bool modified_ = false;
};

} // namespace llmchat

