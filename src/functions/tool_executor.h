// Tool executor
#pragma once

#include "../config/config.h"
#include "tool_manager.h"
#include <string>
#include <map>

namespace llmchat {

class ToolExecutor {
public:
    explicit ToolExecutor(const Config& config);
    
    ToolResult execute_bash(const std::string& script_path, const std::map<std::string, std::string>& args);
    ToolResult execute_python(const std::string& script_path, const std::map<std::string, std::string>& args);
    ToolResult execute_javascript(const std::string& script_path, const std::map<std::string, std::string>& args);
    
private:
    std::string build_command_line(const std::string& script, const std::map<std::string, std::string>& args);
    ToolResult run_command(const std::string& command);
    
    const Config& config_;
};

} // namespace llmchat

