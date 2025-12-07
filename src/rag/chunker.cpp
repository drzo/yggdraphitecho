// Chunker implementation
#include "chunker.h"

namespace llmchat {

Chunker::Chunker(const Config& config)
    : config_(config) {
}

std::vector<Chunk> Chunker::chunk(const std::string& text) {
    std::vector<Chunk> chunks;
    
    int chunk_size = config_.chunk_size;
    int overlap = config_.chunk_overlap;
    
    for (size_t i = 0; i < text.length(); i += chunk_size - overlap) {
        Chunk chunk;
        chunk.start_pos = i;
        chunk.end_pos = std::min(i + chunk_size, text.length());
        chunk.text = text.substr(chunk.start_pos, chunk.end_pos - chunk.start_pos);
        
        chunks.push_back(chunk);
        
        if (chunk.end_pos >= text.length()) {
            break;
        }
    }
    
    return chunks;
}

} // namespace llmchat

