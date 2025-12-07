// Agent implementation
#include "agent.h"
#include "../utils/logger.h"

namespace llmchat {

Agent::Agent(const Config& config, const AgentConfig& agent_config)
    : config_(config), agent_config_(agent_config) {
}

std::string Agent::execute(const std::string& query, InferenceEngine& engine, ToolManager& tools) {
    Logger::info("Agent {} executing query", agent_config_.name);
    
    // Build agent prompt with instructions
    std::string prompt = agent_config_.instructions + "\n\n";
    prompt += "User: " + query + "\n\n";
    prompt += "Assistant: ";
    
    // Generate response
    std::string response = engine.generate(prompt);
    
    // TODO: Implement function calling loop
    // TODO: Integrate RAG if documents are specified
    
    return response;
}

} // namespace llmchat

