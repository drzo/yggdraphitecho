// Inference engine - llama.cpp/ggml integration
#pragma once

#include "../config/config.h"
#include <string>
#include <vector>
#include <memory>
#include <functional>

// Forward declarations for llama.cpp types
struct llama_model;
struct llama_context;
struct llama_sampler;

namespace llmchat {

struct Message {
    std::string role;      // system, user, assistant, tool
    std::string content;
    std::string name;      // For tool calls
    
    Message() = default;
    Message(const std::string& r, const std::string& c) : role(r), content(c) {}
};

struct GenerationOptions {
    float temperature = 0.7f;
    float top_p = 0.9f;
    int top_k = 40;
    float repeat_penalty = 1.1f;
    int max_tokens = 2048;
    bool stream = true;
    std::vector<std::string> stop_sequences;
    
    // Function calling
    bool enable_functions = false;
    std::vector<std::string> available_functions;
};

using StreamCallback = std::function<void(const std::string& token)>;

class InferenceEngine {
public:
    explicit InferenceEngine(const Config& config);
    ~InferenceEngine();
    
    // Model management
    bool load_model();
    bool load_model(const std::string& path);
    void unload_model();
    bool is_loaded() const { return model_ != nullptr; }
    
    // Text generation
    std::string generate(const std::string& prompt, const GenerationOptions& opts = {});
    std::string generate(const std::vector<Message>& messages, const GenerationOptions& opts = {});
    
    // Streaming generation
    void generate_stream(const std::string& prompt, StreamCallback callback, const GenerationOptions& opts = {});
    void generate_stream(const std::vector<Message>& messages, StreamCallback callback, const GenerationOptions& opts = {});
    
    // Chat interface
    std::string chat(const std::vector<Message>& history, const std::string& user_message, const GenerationOptions& opts = {});
    
    // Embeddings
    std::vector<float> embed(const std::string& text);
    
    // Model info
    int get_context_size() const;
    int get_n_tokens(const std::string& text) const;
    std::string get_model_name() const;
    
    // Stop generation
    void stop();
    
private:
    std::string format_messages(const std::vector<Message>& messages) const;
    std::string apply_chat_template(const std::vector<Message>& messages) const;
    std::vector<int> tokenize(const std::string& text, bool add_bos = true) const;
    std::string detokenize(const std::vector<int>& tokens) const;
    
    const Config& config_;
    llama_model* model_ = nullptr;
    llama_context* ctx_ = nullptr;
    llama_sampler* sampler_ = nullptr;
    
    bool should_stop_ = false;
    std::string model_path_;
};

} // namespace llmchat

