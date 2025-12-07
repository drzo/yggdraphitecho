// Inference engine implementation
#include "engine.h"
#include "../utils/logger.h"

#include "llama.h"
#include "ggml.h"

#include <cstring>
#include <sstream>
#include <algorithm>

namespace llmchat {

InferenceEngine::InferenceEngine(const Config& config) 
    : config_(config) {
    // Initialize llama backend
    llama_backend_init();
}

InferenceEngine::~InferenceEngine() {
    unload_model();
    llama_backend_free();
}

bool InferenceEngine::load_model() {
    return load_model(config_.model_path);
}

bool InferenceEngine::load_model(const std::string& path) {
    if (is_loaded()) {
        Logger::warn("Model already loaded, unloading first");
        unload_model();
    }
    
    model_path_ = path;
    
    // Setup model parameters
    llama_model_params model_params = llama_model_default_params();
    model_params.n_gpu_layers = config_.gpu_layers;
    model_params.use_mlock = config_.mlock;
    model_params.use_mmap = config_.mmap;
    
    // Load model
    Logger::info("Loading model from: {}", path);
    model_ = llama_load_model_from_file(path.c_str(), model_params);
    
    if (!model_) {
        Logger::error("Failed to load model");
        return false;
    }
    
    // Setup context parameters
    llama_context_params ctx_params = llama_context_default_params();
    ctx_params.n_ctx = config_.context_size;
    ctx_params.n_batch = config_.batch_size;
    ctx_params.n_threads = config_.threads > 0 ? config_.threads : std::thread::hardware_concurrency();
    ctx_params.n_threads_batch = ctx_params.n_threads;
    
    // Create context
    ctx_ = llama_new_context_with_model(model_, ctx_params);
    
    if (!ctx_) {
        Logger::error("Failed to create context");
        llama_free_model(model_);
        model_ = nullptr;
        return false;
    }
    
    // Setup sampler
    llama_sampler_chain_params sampler_params = llama_sampler_chain_default_params();
    sampler_ = llama_sampler_chain_init(sampler_params);
    
    llama_sampler_chain_add(sampler_, llama_sampler_init_top_k(config_.top_k));
    llama_sampler_chain_add(sampler_, llama_sampler_init_top_p(config_.top_p, 1));
    llama_sampler_chain_add(sampler_, llama_sampler_init_temp(config_.temperature));
    llama_sampler_chain_add(sampler_, llama_sampler_init_dist(config_.seed));
    
    Logger::info("Model loaded successfully");
    Logger::debug("Context size: {}", config_.context_size);
    Logger::debug("Batch size: {}", config_.batch_size);
    Logger::debug("Threads: {}", ctx_params.n_threads);
    
    return true;
}

void InferenceEngine::unload_model() {
    if (sampler_) {
        llama_sampler_free(sampler_);
        sampler_ = nullptr;
    }
    
    if (ctx_) {
        llama_free(ctx_);
        ctx_ = nullptr;
    }
    
    if (model_) {
        llama_free_model(model_);
        model_ = nullptr;
    }
}

std::vector<int> InferenceEngine::tokenize(const std::string& text, bool add_bos) const {
    if (!model_) return {};
    
    const int n_tokens_max = text.length() + (add_bos ? 1 : 0);
    std::vector<llama_token> tokens(n_tokens_max);
    
    const int n_tokens = llama_tokenize(
        model_,
        text.c_str(),
        text.length(),
        tokens.data(),
        n_tokens_max,
        add_bos,
        false // parse_special
    );
    
    if (n_tokens < 0) {
        tokens.resize(-n_tokens);
        llama_tokenize(model_, text.c_str(), text.length(), tokens.data(), -n_tokens, add_bos, false);
    } else {
        tokens.resize(n_tokens);
    }
    
    return tokens;
}

std::string InferenceEngine::detokenize(const std::vector<int>& tokens) const {
    if (!model_ || tokens.empty()) return "";
    
    std::string result;
    result.reserve(tokens.size() * 4); // Rough estimate
    
    for (int token : tokens) {
        char buf[32];
        int len = llama_token_to_piece(model_, token, buf, sizeof(buf), false);
        if (len > 0) {
            result.append(buf, len);
        }
    }
    
    return result;
}

std::string InferenceEngine::apply_chat_template(const std::vector<Message>& messages) const {
    // Simple chat template - for production, use model-specific templates
    // Consider using llama_chat_apply_template or custom templates
    
    std::ostringstream oss;
    
    for (const auto& msg : messages) {
        if (msg.role == "system") {
            oss << "### System:\n" << msg.content << "\n\n";
        } else if (msg.role == "user") {
            oss << "### User:\n" << msg.content << "\n\n";
        } else if (msg.role == "assistant") {
            oss << "### Assistant:\n" << msg.content << "\n\n";
        }
    }
    
    oss << "### Assistant:\n";
    
    return oss.str();
}

std::string InferenceEngine::format_messages(const std::vector<Message>& messages) const {
    return apply_chat_template(messages);
}

std::string InferenceEngine::generate(const std::string& prompt, const GenerationOptions& opts) {
    if (!is_loaded()) {
        Logger::error("Model not loaded");
        return "";
    }
    
    should_stop_ = false;
    
    // Tokenize prompt
    auto tokens = tokenize(prompt, true);
    
    if (tokens.empty()) {
        Logger::error("Failed to tokenize prompt");
        return "";
    }
    
    Logger::debug("Prompt tokens: {}", tokens.size());
    
    // Evaluate prompt
    llama_batch batch = llama_batch_get_one(tokens.data(), tokens.size(), 0, 0);
    
    if (llama_decode(ctx_, batch) != 0) {
        Logger::error("Failed to evaluate prompt");
        return "";
    }
    
    // Generate tokens
    std::string result;
    int n_generated = 0;
    const int max_tokens = opts.max_tokens > 0 ? opts.max_tokens : config_.max_tokens;
    
    while (n_generated < max_tokens && !should_stop_) {
        // Sample next token
        llama_token new_token = llama_sampler_sample(sampler_, ctx_, -1);
        
        // Check for EOS
        if (llama_token_is_eog(model_, new_token)) {
            break;
        }
        
        // Convert token to text
        char buf[32];
        int len = llama_token_to_piece(model_, new_token, buf, sizeof(buf), false);
        if (len > 0) {
            result.append(buf, len);
        }
        
        // Evaluate token
        batch = llama_batch_get_one(&new_token, 1, tokens.size() + n_generated, 0);
        if (llama_decode(ctx_, batch) != 0) {
            Logger::error("Failed to evaluate token");
            break;
        }
        
        n_generated++;
    }
    
    Logger::debug("Generated tokens: {}", n_generated);
    
    return result;
}

std::string InferenceEngine::generate(const std::vector<Message>& messages, const GenerationOptions& opts) {
    std::string prompt = format_messages(messages);
    return generate(prompt, opts);
}

void InferenceEngine::generate_stream(const std::string& prompt, StreamCallback callback, const GenerationOptions& opts) {
    if (!is_loaded()) {
        Logger::error("Model not loaded");
        return;
    }
    
    should_stop_ = false;
    
    // Tokenize prompt
    auto tokens = tokenize(prompt, true);
    if (tokens.empty()) return;
    
    // Evaluate prompt
    llama_batch batch = llama_batch_get_one(tokens.data(), tokens.size(), 0, 0);
    if (llama_decode(ctx_, batch) != 0) return;
    
    // Generate tokens with streaming
    int n_generated = 0;
    const int max_tokens = opts.max_tokens > 0 ? opts.max_tokens : config_.max_tokens;
    
    while (n_generated < max_tokens && !should_stop_) {
        llama_token new_token = llama_sampler_sample(sampler_, ctx_, -1);
        
        if (llama_token_is_eog(model_, new_token)) break;
        
        // Convert and send token
        char buf[32];
        int len = llama_token_to_piece(model_, new_token, buf, sizeof(buf), false);
        if (len > 0) {
            callback(std::string(buf, len));
        }
        
        // Evaluate token
        batch = llama_batch_get_one(&new_token, 1, tokens.size() + n_generated, 0);
        if (llama_decode(ctx_, batch) != 0) break;
        
        n_generated++;
    }
}

void InferenceEngine::generate_stream(const std::vector<Message>& messages, StreamCallback callback, const GenerationOptions& opts) {
    std::string prompt = format_messages(messages);
    generate_stream(prompt, callback, opts);
}

std::string InferenceEngine::chat(const std::vector<Message>& history, const std::string& user_message, const GenerationOptions& opts) {
    std::vector<Message> messages = history;
    messages.push_back(Message("user", user_message));
    return generate(messages, opts);
}

std::vector<float> InferenceEngine::embed(const std::string& text) {
    // Embedding functionality - requires embedding model
    // TODO: Implement with separate embedding context
    Logger::warn("Embedding not yet implemented");
    return {};
}

int InferenceEngine::get_context_size() const {
    if (!ctx_) return 0;
    return llama_n_ctx(ctx_);
}

int InferenceEngine::get_n_tokens(const std::string& text) const {
    auto tokens = tokenize(text, false);
    return tokens.size();
}

std::string InferenceEngine::get_model_name() const {
    if (!model_) return "";
    
    char buf[256];
    llama_model_desc(model_, buf, sizeof(buf));
    return std::string(buf);
}

void InferenceEngine::stop() {
    should_stop_ = true;
}

} // namespace llmchat

