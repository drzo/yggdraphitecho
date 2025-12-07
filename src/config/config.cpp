// Configuration implementation
#include "config.h"
#include "yaml_parser.h"
#include "../utils/logger.h"
#include "../utils/file_utils.h"

#include <iostream>
#include <fstream>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#else
#include <unistd.h>
#include <pwd.h>
#endif

namespace llmchat {

std::string get_home_dir() {
#ifdef _WIN32
    char path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, path))) {
        return std::string(path);
    }
    const char* home = getenv("USERPROFILE");
    if (home) return std::string(home);
    return "C:\\";
#else
    const char* home = getenv("HOME");
    if (home) return std::string(home);
    
    struct passwd* pw = getpwuid(getuid());
    if (pw && pw->pw_dir) return std::string(pw->pw_dir);
    
    return "/tmp";
#endif
}

std::string get_default_config_path() {
#ifdef _WIN32
    return get_home_dir() + "\\.config\\llmchat\\config.yaml";
#else
    const char* xdg_config = getenv("XDG_CONFIG_HOME");
    if (xdg_config) {
        return std::string(xdg_config) + "/llmchat/config.yaml";
    }
    return get_home_dir() + "/.config/llmchat/config.yaml";
#endif
}

std::string Config::expand_path(const std::string& path) const {
    if (path.empty()) return path;
    
    std::string result = path;
    
    // Expand ~
    if (result[0] == '~') {
        result = get_home_dir() + result.substr(1);
    }
    
    // Expand environment variables
    size_t pos = 0;
    while ((pos = result.find('$', pos)) != std::string::npos) {
        size_t end = result.find_first_of("/\\", pos);
        if (end == std::string::npos) end = result.length();
        
        std::string var_name = result.substr(pos + 1, end - pos - 1);
        const char* var_value = getenv(var_name.c_str());
        
        if (var_value) {
            result = result.substr(0, pos) + var_value + result.substr(end);
            pos += strlen(var_value);
        } else {
            pos = end;
        }
    }
    
    return result;
}

void Config::print_info() const {
    std::cout << "LLMChat Configuration:" << std::endl;
    std::cout << "  Model: " << model_path << std::endl;
    std::cout << "  Model Type: " << model_type << std::endl;
    std::cout << "  Context Size: " << context_size << std::endl;
    std::cout << "  GPU Layers: " << gpu_layers << std::endl;
    std::cout << "  Threads: " << (threads < 0 ? "auto" : std::to_string(threads)) << std::endl;
    std::cout << "  Sessions Dir: " << sessions_dir << std::endl;
    std::cout << "  Tools Dir: " << tools_dir << std::endl;
    std::cout << "  Agents Dir: " << agents_dir << std::endl;
    if (rag_enabled) {
        std::cout << "  RAG Enabled: yes" << std::endl;
        std::cout << "  RAG DB: " << rag_db_path << std::endl;
        std::cout << "  Embedding Model: " << embedding_model << std::endl;
    }
    std::cout << "  Function Calling: " << (function_calling ? "enabled" : "disabled") << std::endl;
}

Role* Config::find_role(const std::string& name) {
    for (auto& role : roles) {
        if (role.name == name) {
            return &role;
        }
    }
    return nullptr;
}

bool load_config(const std::string& path, Config& config) {
    std::string config_path = path.empty() ? get_default_config_path() : path;
    
    // Set defaults for directories
    std::string home = get_home_dir();
    config.sessions_dir = home + "/.llmchat/sessions";
    config.tools_dir = home + "/.config/llmchat/functions/tools";
    config.agents_dir = home + "/.config/llmchat/functions/agents";
    config.rag_db_path = home + "/.llmchat/rag/vectordb";
    config.log_file = home + "/.llmchat/llmchat.log";
    
    // Try to load from file
    if (!file_exists(config_path)) {
        Logger::warn("Config file not found: {}", config_path);
        Logger::info("Using default configuration");
        
        // Set a default model path
        config.model_path = home + "/.llmchat/models/model.gguf";
        
        return true; // Not an error, just use defaults
    }
    
    try {
        YamlParser parser(config_path);
        
        // Parse config file
        parser.get("model_path", config.model_path);
        parser.get("model_type", config.model_type);
        parser.get("context_size", config.context_size);
        parser.get("threads", config.threads);
        parser.get("batch_size", config.batch_size);
        parser.get("gpu_layers", config.gpu_layers);
        
        parser.get("temperature", config.temperature);
        parser.get("top_p", config.top_p);
        parser.get("top_k", config.top_k);
        parser.get("repeat_penalty", config.repeat_penalty);
        parser.get("max_tokens", config.max_tokens);
        parser.get("seed", config.seed);
        
        parser.get("stream", config.stream);
        parser.get("save_history", config.save_history);
        parser.get("save_sessions", config.save_sessions);
        
        parser.get("function_calling", config.function_calling);
        parser.get("tools_dir", config.tools_dir);
        parser.get("agents_dir", config.agents_dir);
        
        parser.get("rag_enabled", config.rag_enabled);
        parser.get("rag_db_path", config.rag_db_path);
        parser.get("embedding_model", config.embedding_model);
        parser.get("chunk_size", config.chunk_size);
        
        parser.get("sessions_dir", config.sessions_dir);
        parser.get("log_level", config.log_level);
        parser.get("log_file", config.log_file);
        
        // Expand paths
        config.model_path = config.expand_path(config.model_path);
        config.embedding_model = config.expand_path(config.embedding_model);
        config.sessions_dir = config.expand_path(config.sessions_dir);
        config.tools_dir = config.expand_path(config.tools_dir);
        config.agents_dir = config.expand_path(config.agents_dir);
        config.rag_db_path = config.expand_path(config.rag_db_path);
        config.log_file = config.expand_path(config.log_file);
        
        Logger::info("Configuration loaded from: {}", config_path);
        return true;
        
    } catch (const std::exception& e) {
        Logger::error("Failed to parse config: {}", e.what());
        return false;
    }
}

bool save_config(const std::string& path, const Config& config) {
    // TODO: Implement YAML serialization
    Logger::warn("Config saving not yet implemented");
    return false;
}

} // namespace llmchat

