// Embedder implementation
#include "embedder.h"
#include "../utils/logger.h"

namespace llmchat {

Embedder::Embedder(const Config& config, InferenceEngine& engine)
    : config_(config), engine_(engine) {
}

std::vector<float> Embedder::embed(const std::string& text) {
    // Use inference engine to generate embeddings
    return engine_.embed(text);
}

std::vector<std::vector<float>> Embedder::embed_batch(const std::vector<std::string>& texts) {
    std::vector<std::vector<float>> embeddings;
    
    for (const auto& text : texts) {
        embeddings.push_back(embed(text));
    }
    
    return embeddings;
}

} // namespace llmchat

