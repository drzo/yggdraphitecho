// Embedding generation
#pragma once

#include "../config/config.h"
#include "../inference/engine.h"
#include <string>
#include <vector>

namespace llmchat {

class Embedder {
public:
    Embedder(const Config& config, InferenceEngine& engine);
    
    std::vector<float> embed(const std::string& text);
    std::vector<std::vector<float>> embed_batch(const std::vector<std::string>& texts);
    
private:
    const Config& config_;
    InferenceEngine& engine_;
};

} // namespace llmchat

