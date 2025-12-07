// Session implementation
#include "session.h"
#include "storage.h"
#include "../utils/logger.h"
#include "../utils/file_utils.h"

#include <fstream>
#include <sstream>

namespace llmchat {

Session::Session(const Config& config, const std::string& name)
    : config_(config), name_(name) {
    
    // Try to load existing session
    std::string path = get_session_path();
    if (file_exists(path)) {
        load(path);
    }
}

Session::~Session() {
    if (config_.save_sessions && modified_) {
        save();
    }
}

std::string Session::get_session_path() const {
    return config_.sessions_dir + "/" + name_ + ".json";
}

void Session::add_message(const Message& msg) {
    messages_.push_back(msg);
    modified_ = true;
}

void Session::clear() {
    messages_.clear();
    token_count_ = 0;
    modified_ = true;
}

bool Session::save() {
    return save(get_session_path());
}

bool Session::save(const std::string& path) {
    try {
        // Ensure directory exists
        std::string dir = path.substr(0, path.find_last_of("/\\"));
        create_directories(dir);
        
        // Save using storage module
        SessionStorage storage;
        if (storage.save(path, messages_)) {
            modified_ = false;
            Logger::debug("Session saved: {}", path);
            return true;
        }
        
        return false;
        
    } catch (const std::exception& e) {
        Logger::error("Failed to save session: {}", e.what());
        return false;
    }
}

bool Session::load(const std::string& path) {
    try {
        SessionStorage storage;
        if (storage.load(path, messages_)) {
            modified_ = false;
            Logger::debug("Session loaded: {}", path);
            return true;
        }
        
        return false;
        
    } catch (const std::exception& e) {
        Logger::error("Failed to load session: {}", e.what());
        return false;
    }
}

void Session::update_token_count(InferenceEngine& engine) {
    token_count_ = 0;
    for (const auto& msg : messages_) {
        token_count_ += engine.get_n_tokens(msg.content);
    }
}

void Session::compress_if_needed(InferenceEngine& engine) {
    update_token_count(engine);
    
    if (token_count_ < config_.compress_threshold) {
        return;
    }
    
    Logger::info("Session token count ({}) exceeds threshold ({}), compressing...", 
                 token_count_, config_.compress_threshold);
    
    // Keep recent messages, summarize older ones
    const int keep_recent = 10;
    
    if (messages_.size() <= keep_recent) {
        return;
    }
    
    std::vector<Message> messages_to_summarize(
        messages_.begin(), 
        messages_.end() - keep_recent
    );
    
    // Generate summary
    std::ostringstream oss;
    for (const auto& msg : messages_to_summarize) {
        oss << msg.role << ": " << msg.content << "\n\n";
    }
    
    std::string summary_prompt = config_.summarize_prompt + "\n\n" + oss.str();
    std::string summary = engine.generate(summary_prompt);
    
    // Replace old messages with summary
    messages_.erase(messages_.begin(), messages_.end() - keep_recent);
    messages_.insert(messages_.begin(), Message("system", config_.summary_prompt + summary));
    
    modified_ = true;
    update_token_count(engine);
    
    Logger::info("Session compressed to {} tokens", token_count_);
}

} // namespace llmchat

