// CLI argument parsing implementation
#include "args.h"
#include "../config/config.h"
#include "../utils/logger.h"

#include <iostream>
#include <cstring>
#include <algorithm>

namespace llmchat {

static bool has_flag(int argc, char* argv[], const char* flag, const char* alias = nullptr) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], flag) == 0 || (alias && strcmp(argv[i], alias) == 0)) {
            return true;
        }
    }
    return false;
}

static const char* get_option(int argc, char* argv[], const char* flag, const char* alias = nullptr) {
    for (int i = 1; i < argc - 1; i++) {
        if (strcmp(argv[i], flag) == 0 || (alias && strcmp(argv[i], alias) == 0)) {
            return argv[i + 1];
        }
    }
    return nullptr;
}

static std::vector<std::string> get_multi_option(int argc, char* argv[], const char* flag, const char* alias = nullptr) {
    std::vector<std::string> results;
    for (int i = 1; i < argc - 1; i++) {
        if (strcmp(argv[i], flag) == 0 || (alias && strcmp(argv[i], alias) == 0)) {
            results.push_back(argv[i + 1]);
        }
    }
    return results;
}

bool parse_args(int argc, char* argv[], Args& args) {
    if (argc < 2) {
        args.repl_mode = true;
        return true;
    }

    // Mode flags
    args.repl_mode = has_flag(argc, argv, "--repl", "-r");
    args.serve_mode = has_flag(argc, argv, "--serve");
    args.show_help = has_flag(argc, argv, "--help", "-h");
    args.show_version = has_flag(argc, argv, "--version", "-v");
    args.show_info = has_flag(argc, argv, "--info");
    args.edit_config = has_flag(argc, argv, "--edit-config");

    // Configuration
    if (auto val = get_option(argc, argv, "--config", "-c")) {
        args.config_path = val;
    }

    args.verbose = has_flag(argc, argv, "--verbose");
    args.debug = has_flag(argc, argv, "--debug");

    // Model & session
    if (auto val = get_option(argc, argv, "--model", "-m")) {
        args.model_path = val;
    }
    if (auto val = get_option(argc, argv, "--session", "-s")) {
        args.session_name = val;
    }
    if (auto val = get_option(argc, argv, "--role")) {
        args.role_name = val;
    }
    if (auto val = get_option(argc, argv, "--agent")) {
        args.agent_name = val;
    }

    // Files
    args.files = get_multi_option(argc, argv, "--file", "-f");

    // Generation parameters
    if (auto val = get_option(argc, argv, "--temperature", "-t")) {
        args.temperature = std::stof(val);
    }
    if (auto val = get_option(argc, argv, "--top-p")) {
        args.top_p = std::stof(val);
    }
    if (auto val = get_option(argc, argv, "--top-k")) {
        args.top_k = std::stoi(val);
    }
    if (auto val = get_option(argc, argv, "--max-tokens")) {
        args.max_tokens = std::stoi(val);
    }
    args.no_stream = has_flag(argc, argv, "--no-stream");

    // Features
    args.enable_rag = has_flag(argc, argv, "--rag");
    if (auto val = get_option(argc, argv, "--index")) {
        args.index_dir = val;
    }
    if (auto val = get_option(argc, argv, "--tools")) {
        // Parse comma-separated tool list
        std::string tools_str = val;
        size_t pos = 0;
        while ((pos = tools_str.find(',')) != std::string::npos) {
            args.tools.push_back(tools_str.substr(0, pos));
            tools_str.erase(0, pos + 1);
        }
        if (!tools_str.empty()) {
            args.tools.push_back(tools_str);
        }
    }

    // Server mode
    if (auto val = get_option(argc, argv, "--serve")) {
        args.serve_addr = val;
    } else if (args.serve_mode) {
        args.serve_addr = "127.0.0.1:8000";
    }

    // Collect prompt from remaining args
    std::vector<std::string> prompt_parts;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        // Skip flags and their values
        if (arg[0] == '-') {
            // Check if this flag takes a value
            if (arg == "--file" || arg == "-f" || arg == "--model" || arg == "-m" ||
                arg == "--session" || arg == "-s" || arg == "--role" || arg == "--agent" ||
                arg == "--temperature" || arg == "-t" || arg == "--top-p" || arg == "--top-k" ||
                arg == "--max-tokens" || arg == "--tools" || arg == "--index" ||
                arg == "--config" || arg == "-c" || arg == "--serve") {
                i++; // Skip the value too
            }
            continue;
        }

        prompt_parts.push_back(arg);
    }

    // Join prompt parts
    for (size_t i = 0; i < prompt_parts.size(); i++) {
        if (i > 0) args.prompt += " ";
        args.prompt += prompt_parts[i];
    }

    return true;
}

void apply_args_to_config(const Args& args, Config& config) {
    // Override config with CLI arguments
    if (!args.model_path.empty()) {
        config.model_path = args.model_path;
    }

    if (args.temperature >= 0) {
        config.temperature = args.temperature;
    }

    if (args.top_p >= 0) {
        config.top_p = args.top_p;
    }

    if (args.top_k >= 0) {
        config.top_k = args.top_k;
    }

    if (args.max_tokens > 0) {
        config.max_tokens = args.max_tokens;
    }

    if (args.no_stream) {
        config.stream = false;
    }

    if (args.enable_rag) {
        config.rag_enabled = true;
    }

    if (args.verbose) {
        config.log_level = "debug";
    }

    if (args.debug) {
        config.log_level = "debug";
        config.log_to_console = true;
    }
}

} // namespace llmchat

