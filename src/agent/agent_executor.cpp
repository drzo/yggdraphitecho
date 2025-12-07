// Agent executor implementation
#include "agent_executor.h"
#include "../utils/logger.h"
#include "../utils/file_utils.h"

namespace llmchat {

AgentExecutor::AgentExecutor(const Config& config)
    : config_(config) {
}

void AgentExecutor::load_agents() {
    if (!is_directory(config_.agents_dir)) {
        Logger::warn("Agents directory not found: {}", config_.agents_dir);
        return;
    }
    
    auto dirs = list_directory(config_.agents_dir);
    
    for (const auto& dir : dirs) {
        std::string agent_path = join_paths(config_.agents_dir, dir);
        
        if (!is_directory(agent_path)) {
            continue;
        }
        
        std::string config_path = join_paths(agent_path, "index.yaml");
        if (file_exists(config_path)) {
            try {
                AgentConfig agent_config = load_agent_config(config_path);
                agents_.emplace(agent_config.name, Agent(config_, agent_config));
                Logger::debug("Loaded agent: {}", agent_config.name);
            } catch (const std::exception& e) {
                Logger::error("Failed to load agent {}: {}", dir, e.what());
            }
        }
    }
    
    Logger::info("Loaded {} agents", agents_.size());
}

AgentConfig AgentExecutor::load_agent_config(const std::string& path) {
    AgentConfig config;
    
    // TODO: Parse YAML agent configuration
    // For now, return minimal config
    
    return config;
}

bool AgentExecutor::has_agent(const std::string& name) const {
    return agents_.find(name) != agents_.end();
}

Agent* AgentExecutor::get_agent(const std::string& name) {
    auto it = agents_.find(name);
    if (it != agents_.end()) {
        return &it->second;
    }
    return nullptr;
}

} // namespace llmchat

