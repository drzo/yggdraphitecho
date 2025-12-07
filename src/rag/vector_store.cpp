// Vector store implementation
#include "vector_store.h"
#include "../utils/logger.h"

#include <algorithm>
#include <cmath>

namespace llmchat {

VectorStore::VectorStore(const Config& config)
    : config_(config) {
}

void VectorStore::add_document(const Document& doc) {
    documents_.push_back(doc);
}

void VectorStore::remove_document(const std::string& id) {
    documents_.erase(
        std::remove_if(documents_.begin(), documents_.end(),
            [&id](const Document& doc) { return doc.id == id; }),
        documents_.end()
    );
}

void VectorStore::clear() {
    documents_.clear();
}

float VectorStore::cosine_similarity(const std::vector<float>& a, const std::vector<float>& b) {
    if (a.size() != b.size() || a.empty()) {
        return 0.0f;
    }
    
    float dot = 0.0f, norm_a = 0.0f, norm_b = 0.0f;
    
    for (size_t i = 0; i < a.size(); i++) {
        dot += a[i] * b[i];
        norm_a += a[i] * a[i];
        norm_b += b[i] * b[i];
    }
    
    if (norm_a == 0.0f || norm_b == 0.0f) {
        return 0.0f;
    }
    
    return dot / (std::sqrt(norm_a) * std::sqrt(norm_b));
}

std::vector<SearchResult> VectorStore::search(const std::vector<float>& query_embedding, int top_k) {
    std::vector<SearchResult> results;
    
    // Calculate similarity for all documents
    for (const auto& doc : documents_) {
        if (doc.embedding.empty()) continue;
        
        float score = cosine_similarity(query_embedding, doc.embedding);
        
        if (score >= config_.similarity_threshold) {
            results.push_back({doc, score});
        }
    }
    
    // Sort by score descending
    std::sort(results.begin(), results.end(),
        [](const SearchResult& a, const SearchResult& b) {
            return a.score > b.score;
        });
    
    // Return top-k results
    if (results.size() > static_cast<size_t>(top_k)) {
        results.resize(top_k);
    }
    
    return results;
}

std::vector<SearchResult> VectorStore::search_by_text(const std::string& query, int top_k) {
    // TODO: Generate embedding for query text
    // This requires an embedding model
    Logger::warn("Text search not yet implemented");
    return {};
}

bool VectorStore::save(const std::string& path) {
    // TODO: Implement persistence
    Logger::warn("Vector store save not yet implemented");
    return false;
}

bool VectorStore::load(const std::string& path) {
    // TODO: Implement loading
    Logger::warn("Vector store load not yet implemented");
    return false;
}

} // namespace llmchat

