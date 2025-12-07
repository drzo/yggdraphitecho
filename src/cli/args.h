// CLI argument parsing
#pragma once

#include <string>
#include <vector>

namespace llmchat {

struct Args {
    // Mode flags
    bool repl_mode = false;
    bool serve_mode = false;
    bool show_help = false;
    bool show_version = false;
    bool show_info = false;
    bool edit_config = false;

    // Input
    std::string prompt;
    std::vector<std::string> files;
    bool use_stdin = false;

    // Model & session
    std::string model_path;
    std::string session_name;
    std::string role_name;
    std::string agent_name;

    // Generation parameters
    float temperature = -1.0f;
    float top_p = -1.0f;
    int top_k = -1;
    int max_tokens = -1;
    bool no_stream = false;

    // Features
    bool enable_rag = false;
    std::string index_dir;
    std::vector<std::string> tools;

    // Configuration
    std::string config_path;
    bool verbose = false;
    bool debug = false;

    // Server mode
    std::string serve_addr;
};

bool parse_args(int argc, char* argv[], Args& args);
void apply_args_to_config(const Args& args, struct Config& config);

} // namespace llmchat

