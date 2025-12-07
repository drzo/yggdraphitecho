// Vector store for RAG
#pragma once

#include "../config/config.h"
#include <string>
#include <vector>

namespace llmchat {

struct Document {
    std::string id;
    std::string content;
    std::vector<float> embedding;
    std::map<std::string, std::string> metadata;
};

struct SearchResult {
    Document document;
    float score;
};

class VectorStore {
public:
    explicit VectorStore(const Config& config);
    
    // Document management
    void add_document(const Document& doc);
    void remove_document(const std::string& id);
    void clear();
    
    // Search
    std::vector<SearchResult> search(const std::vector<float>& query_embedding, int top_k = 5);
    std::vector<SearchResult> search_by_text(const std::string& query, int top_k = 5);
    
    // Persistence
    bool save(const std::string& path);
    bool load(const std::string& path);
    
private:
    float cosine_similarity(const std::vector<float>& a, const std::vector<float>& b);
    
    const Config& config_;
    std::vector<Document> documents_;
};

} // namespace llmchat

