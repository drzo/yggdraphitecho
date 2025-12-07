// AI Agent
#pragma once

#include "../config/config.h"
#include "../inference/engine.h"
#include "../functions/tool_manager.h"
#include <string>
#include <vector>
#include <map>

namespace llmchat {

struct AgentConfig {
    std::string name;
    std::string description;
    std::string instructions;
    std::vector<std::string> tools;
    std::vector<std::string> documents;
    std::map<std::string, std::string> variables;
};

class Agent {
public:
    Agent(const Config& config, const AgentConfig& agent_config);
    
    std::string execute(const std::string& query, InferenceEngine& engine, ToolManager& tools);
    
    const std::string& name() const { return config_.name; }
    const std::string& description() const { return config_.description; }
    
private:
    const Config& config_;
    AgentConfig agent_config_;
};

} // namespace llmchat

