// Tool manager implementation
#include "tool_manager.h"
#include "tool_executor.h"
#include "../utils/logger.h"
#include "../utils/file_utils.h"
#include <fstream>
#include <sstream>

namespace llmchat {

ToolManager::ToolManager(const Config& config)
    : config_(config) {
}

void ToolManager::load_tools() {
    if (!is_directory(config_.tools_dir)) {
        Logger::warn("Tools directory not found: {}", config_.tools_dir);
        return;
    }
    
    auto files = list_directory(config_.tools_dir);
    
    for (const auto& file : files) {
        std::string path = join_paths(config_.tools_dir, file);
        
        std::string ext = get_file_extension(file);
        if (ext == "sh" || ext == "py" || ext == "js") {
            try {
                load_tool(path);
            } catch (const std::exception& e) {
                Logger::error("Failed to load tool {}: {}", file, e.what());
            }
        }
    }
    
    Logger::info("Loaded {} tools", tools_.size());
}

void ToolManager::load_tool(const std::string& path) {
    ToolDefinition tool = parse_tool_script(path);
    
    if (tool.name.empty()) {
        Logger::warn("Tool has no name: {}", path);
        return;
    }
    
    tools_[tool.name] = tool;
    Logger::debug("Loaded tool: {}", tool.name);
}

ToolDefinition ToolManager::parse_tool_script(const std::string& path) {
    ToolDefinition tool;
    tool.script_path = path;
    
    std::string ext = get_file_extension(path);
    if (ext == "sh") {
        tool.type = "bash";
    } else if (ext == "py") {
        tool.type = "python";
    } else if (ext == "js") {
        tool.type = "javascript";
    }
    
    // Parse script for metadata comments
    std::ifstream file(path);
    if (!file.is_open()) {
        return tool;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        // Look for special comments
        size_t pos;
        
        // @describe - tool description
        if ((pos = line.find("@describe")) != std::string::npos) {
            tool.description = trim(line.substr(pos + 9));
        }
        // @option - parameter
        else if ((pos = line.find("@option")) != std::string::npos) {
            std::string param_def = trim(line.substr(pos + 7));
            // Parse parameter name and description
            // e.g., "@option --input! The input text"
            size_t space_pos = param_def.find(' ');
            if (space_pos != std::string::npos) {
                std::string param_name = param_def.substr(0, space_pos);
                std::string param_desc = param_def.substr(space_pos + 1);
                
                // Remove -- prefix
                if (starts_with(param_name, "--")) {
                    param_name = param_name.substr(2);
                }
                // Remove ! suffix (required marker)
                if (ends_with(param_name, "!")) {
                    param_name = param_name.substr(0, param_name.length() - 1);
                }
                
                tool.parameters[param_name] = param_desc;
            }
        }
    }
    
    // Use filename as tool name if not specified
    if (tool.name.empty()) {
        tool.name = get_filename(path);
        size_t dot_pos = tool.name.find_last_of('.');
        if (dot_pos != std::string::npos) {
            tool.name = tool.name.substr(0, dot_pos);
        }
    }
    
    return tool;
}

bool ToolManager::has_tool(const std::string& name) const {
    return tools_.find(name) != tools_.end();
}

const ToolDefinition* ToolManager::get_tool(const std::string& name) const {
    auto it = tools_.find(name);
    if (it != tools_.end()) {
        return &it->second;
    }
    return nullptr;
}

std::vector<std::string> ToolManager::get_tool_names() const {
    std::vector<std::string> names;
    for (const auto& pair : tools_) {
        names.push_back(pair.first);
    }
    return names;
}

ToolResult ToolManager::execute_tool(const ToolCall& call) {
    auto tool = get_tool(call.name);
    if (!tool) {
        return ToolResult{false, "", "Tool not found: " + call.name};
    }
    
    Logger::info("Executing tool: {}", call.name);
    
    if (tool->type == "bash") {
        return execute_bash(*tool, call.arguments);
    } else if (tool->type == "python") {
        return execute_python(*tool, call.arguments);
    } else if (tool->type == "javascript") {
        return execute_js(*tool, call.arguments);
    }
    
    return ToolResult{false, "", "Unsupported tool type: " + tool->type};
}

ToolResult ToolManager::execute_bash(const ToolDefinition& tool, const std::map<std::string, std::string>& args) {
    ToolExecutor executor(config_);
    return executor.execute_bash(tool.script_path, args);
}

ToolResult ToolManager::execute_python(const ToolDefinition& tool, const std::map<std::string, std::string>& args) {
    ToolExecutor executor(config_);
    return executor.execute_python(tool.script_path, args);
}

ToolResult ToolManager::execute_js(const ToolDefinition& tool, const std::map<std::string, std::string>& args) {
    ToolExecutor executor(config_);
    return executor.execute_javascript(tool.script_path, args);
}

std::string ToolManager::generate_function_declarations() const {
    // Generate JSON schema for function calling
    std::ostringstream oss;
    oss << "[\n";
    
    bool first = true;
    for (const auto& pair : tools_) {
        if (!first) oss << ",\n";
        first = false;
        
        const auto& tool = pair.second;
        
        oss << "  {\n";
        oss << "    \"name\": \"" << tool.name << "\",\n";
        oss << "    \"description\": \"" << tool.description << "\",\n";
        oss << "    \"parameters\": {\n";
        oss << "      \"type\": \"object\",\n";
        oss << "      \"properties\": {\n";
        
        bool first_param = true;
        for (const auto& param : tool.parameters) {
            if (!first_param) oss << ",\n";
            first_param = false;
            
            oss << "        \"" << param.first << "\": {\n";
            oss << "          \"type\": \"string\",\n";
            oss << "          \"description\": \"" << param.second << "\"\n";
            oss << "        }";
        }
        
        oss << "\n      }\n";
        oss << "    }\n";
        oss << "  }";
    }
    
    oss << "\n]\n";
    
    return oss.str();
}

ToolCall ToolManager::parse_function_call(const std::string& json) const {
    // TODO: Implement proper JSON parsing
    ToolCall call;
    return call;
}

} // namespace llmchat

