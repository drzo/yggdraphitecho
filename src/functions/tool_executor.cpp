// Tool executor implementation
#include "tool_executor.h"
#include "../utils/logger.h"

#include <cstdio>
#include <array>
#include <memory>

namespace llmchat {

ToolExecutor::ToolExecutor(const Config& config)
    : config_(config) {
}

std::string ToolExecutor::build_command_line(const std::string& script, const std::map<std::string, std::string>& args) {
    std::string cmd = script;
    
    for (const auto& arg : args) {
        cmd += " --" + arg.first + " \"" + arg.second + "\"";
    }
    
    return cmd;
}

ToolResult ToolExecutor::run_command(const std::string& command) {
    Logger::debug("Running command: {}", command);
    
    ToolResult result;
    result.success = true;
    
    // Execute command and capture output
    std::array<char, 128> buffer;
    std::string output;
    
#ifdef _WIN32
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(command.c_str(), "r"), _pclose);
#else
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
#endif
    
    if (!pipe) {
        result.success = false;
        result.error = "Failed to execute command";
        return result;
    }
    
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        output += buffer.data();
    }
    
    result.output = output;
    
    return result;
}

ToolResult ToolExecutor::execute_bash(const std::string& script_path, const std::map<std::string, std::string>& args) {
    std::string cmd = "bash " + build_command_line(script_path, args);
    return run_command(cmd);
}

ToolResult ToolExecutor::execute_python(const std::string& script_path, const std::map<std::string, std::string>& args) {
    std::string cmd = "python3 " + build_command_line(script_path, args);
    return run_command(cmd);
}

ToolResult ToolExecutor::execute_javascript(const std::string& script_path, const std::map<std::string, std::string>& args) {
    std::string cmd = "node " + build_command_line(script_path, args);
    return run_command(cmd);
}

} // namespace llmchat

