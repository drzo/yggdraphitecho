// Agent executor
#pragma once

#include "../config/config.h"
#include "agent.h"
#include <string>
#include <map>

namespace llmchat {

class AgentExecutor {
public:
    explicit AgentExecutor(const Config& config);
    
    void load_agents();
    bool has_agent(const std::string& name) const;
    Agent* get_agent(const std::string& name);
    
private:
    AgentConfig load_agent_config(const std::string& path);
    
    const Config& config_;
    std::map<std::string, Agent> agents_;
};

} // namespace llmchat

