// Document chunking for RAG
#pragma once

#include "../config/config.h"
#include <string>
#include <vector>

namespace llmchat {

struct Chunk {
    std::string text;
    int start_pos;
    int end_pos;
};

class Chunker {
public:
    explicit Chunker(const Config& config);
    
    std::vector<Chunk> chunk(const std::string& text);
    
private:
    const Config& config_;
};

} // namespace llmchat

