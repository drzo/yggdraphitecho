// Tool/Function management
#pragma once

#include "../config/config.h"
#include <string>
#include <vector>
#include <map>
#include <functional>

namespace llmchat {

struct ToolDefinition {
    std::string name;
    std::string description;
    std::string script_path;
    std::string type; // bash, python, javascript
    std::map<std::string, std::string> parameters;
};

struct ToolCall {
    std::string name;
    std::map<std::string, std::string> arguments;
};

struct ToolResult {
    bool success;
    std::string output;
    std::string error;
};

class ToolManager {
public:
    explicit ToolManager(const Config& config);
    
    // Tool discovery and loading
    void load_tools();
    void load_tool(const std::string& path);
    bool has_tool(const std::string& name) const;
    const ToolDefinition* get_tool(const std::string& name) const;
    std::vector<std::string> get_tool_names() const;
    
    // Tool execution
    ToolResult execute_tool(const ToolCall& call);
    
    // Function calling support
    std::string generate_function_declarations() const;
    ToolCall parse_function_call(const std::string& json) const;
    
private:
    ToolDefinition parse_tool_script(const std::string& path);
    ToolResult execute_bash(const ToolDefinition& tool, const std::map<std::string, std::string>& args);
    ToolResult execute_python(const ToolDefinition& tool, const std::map<std::string, std::string>& args);
    ToolResult execute_js(const ToolDefinition& tool, const std::map<std::string, std::string>& args);
    
    const Config& config_;
    std::map<std::string, ToolDefinition> tools_;
};

} // namespace llmchat

