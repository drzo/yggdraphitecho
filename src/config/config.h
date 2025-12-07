// Configuration management
#pragma once

#include <string>
#include <vector>
#include <map>

namespace llmchat {

struct Role {
    std::string name;
    std::string description;
    std::string system_prompt;
};

struct Config {
    // Model settings
    std::string model_path;
    std::string model_type = "llama";
    int context_size = 8192;
    int threads = -1;
    int batch_size = 512;
    int gpu_layers = 0;

    // Generation parameters
    float temperature = 0.7f;
    float top_p = 0.9f;
    int top_k = 40;
    float repeat_penalty = 1.1f;
    int max_tokens = 2048;
    int seed = -1;

    // Behavior
    bool stream = true;
    bool save_history = true;
    bool save_sessions = true;
    int auto_save_interval = 300;
    int max_history_size = 1000;

    // REPL settings
    std::string repl_prompt = ">>> ";
    bool repl_multiline = true;
    bool repl_syntax_highlight = true;
    bool repl_autocomplete = true;
    std::string keybindings = "emacs";

    // Function calling
    bool function_calling = true;
    std::string tools_dir;
    std::string agents_dir;
    bool mcp_enabled = false;
    int max_tool_iterations = 10;
    std::vector<std::string> default_tools;

    // RAG
    bool rag_enabled = true;
    std::string rag_db_path;
    std::string embedding_model;
    int embedding_dimension = 384;
    int chunk_size = 512;
    int chunk_overlap = 50;
    int top_k_retrieval = 5;
    float similarity_threshold = 0.7f;

    // Session management
    std::string sessions_dir;
    std::string default_session = "default";
    int compress_threshold = 4000;
    std::string summarize_prompt;
    std::string summary_prompt;

    // Rendering
    bool markdown_rendering = true;
    bool syntax_highlighting = true;
    std::string theme = "auto";
    std::string highlight_theme = "monokai";
    int wrap_width = 100;

    // Logging
    std::string log_level = "info";
    std::string log_file;
    bool log_to_console = false;

    // Roles
    std::vector<Role> roles;

    // Prelude
    std::string repl_prelude;
    std::string cmd_prelude;
    std::string agent_prelude;

    // Advanced
    std::string user_prompt_template;
    std::string assistant_prompt_template;
    std::string system_prompt_template;

    // Performance
    bool mlock = false;
    bool mmap = true;
    bool numa = false;
    bool low_vram = false;

    // Safety
    int max_command_length = 10000;
    std::vector<std::string> allowed_paths;
    std::vector<std::string> denied_paths;

    // Network
    std::string http_proxy;
    std::string https_proxy;
    int timeout = 30;

    // Document loaders
    std::map<std::string, std::string> document_loaders;

    // Helper methods
    void print_info() const;
    Role* find_role(const std::string& name);
    std::string expand_path(const std::string& path) const;
};

bool load_config(const std::string& path, Config& config);
bool save_config(const std::string& path, const Config& config);
std::string get_default_config_path();

} // namespace llmchat

