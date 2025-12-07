// LLMChat - Unified LLM CLI Tool with Integrated Inference
// Copyright (c) 2025 LLMChat Developers
// Licensed under MIT OR Apache-2.0

#include <iostream>
#include <string>
#include <memory>
#include <cstdlib>

#include "cli/args.h"
#include "cli/repl.h"
#include "cli/commands.h"
#include "config/config.h"
#include "inference/engine.h"
#include "utils/logger.h"

using namespace llmchat;

static void print_version() {
    std::cout << "LLMChat v1.0.0" << std::endl;
    std::cout << "Unified LLM CLI Tool with Integrated llama.cpp/ggml Inference" << std::endl;
    std::cout << "License: MIT OR Apache-2.0" << std::endl;
}

static void print_usage() {
    std::cout << R"(
LLMChat - All-in-one LLM CLI Tool

USAGE:
    llmchat [OPTIONS] [PROMPT]
    llmchat [OPTIONS] --repl
    llmchat [OPTIONS] --agent <NAME>

MODES:
    (default)          Command mode - one-shot query
    --repl, -r         Interactive REPL mode
    --agent <NAME>     Start an agent
    --serve [ADDR]     Start HTTP server mode

OPTIONS:
    -f, --file <PATH>          Add file/directory to context
    -m, --model <PATH>         Model file path
    -s, --session <NAME>       Use/create session
    --role <NAME>              Use a role
    --tools <LIST>             Enable specific tools
    --rag                      Enable RAG for this query
    --index <DIR>              Index directory for RAG
    
GENERATION:
    -t, --temperature <NUM>    Temperature (0.0-2.0)
    --top-p <NUM>              Top-p sampling
    --top-k <NUM>              Top-k sampling
    --max-tokens <NUM>         Maximum tokens to generate
    --no-stream                Disable streaming
    
CONFIGURATION:
    -c, --config <PATH>        Config file path
    --info                     Show config info
    --edit-config              Edit config file
    
OTHER:
    -h, --help                 Show this help
    -v, --version              Show version
    --verbose                  Verbose logging
    --debug                    Debug mode

EXAMPLES:
    # Interactive mode
    llmchat --repl
    
    # One-shot query
    llmchat "What is the capital of France?"
    
    # With file input
    llmchat -f code.cpp "Explain this code"
    
    # Use session
    llmchat --session coding "Help me debug this"
    
    # Use agent
    llmchat --agent coder "Write a sorting algorithm"
    
    # RAG query
    llmchat --rag -f docs/ "What does the documentation say about X?"
    
    # Shell assistant
    llmchat --role shell "list all PDF files recursively"

For more information, visit: https://github.com/yourusername/llmchat
)";
}

static int run_command_mode(const Config& config, const Args& args, InferenceEngine& engine) {
    try {
        CommandExecutor executor(config, engine);
        return executor.execute(args);
    } catch (const std::exception& e) {
        Logger::error("Command execution failed: {}", e.what());
        return 1;
    }
}

static int run_repl_mode(const Config& config, InferenceEngine& engine) {
    try {
        REPL repl(config, engine);
        return repl.run();
    } catch (const std::exception& e) {
        Logger::error("REPL failed: {}", e.what());
        return 1;
    }
}

static int run_agent_mode(const Config& config, const std::string& agent_name, InferenceEngine& engine) {
    try {
        // Agent execution handled by command executor
        Logger::info("Starting agent: {}", agent_name);
        // Implementation in agent module
        return 0;
    } catch (const std::exception& e) {
        Logger::error("Agent execution failed: {}", e.what());
        return 1;
    }
}

int main(int argc, char* argv[]) {
    try {
        // Parse arguments
        Args args;
        if (!parse_args(argc, argv, args)) {
            return 1;
        }

        // Handle special flags
        if (args.show_help) {
            print_usage();
            return 0;
        }

        if (args.show_version) {
            print_version();
            return 0;
        }

        // Load configuration
        Config config;
        if (!load_config(args.config_path, config)) {
            Logger::error("Failed to load configuration");
            return 1;
        }

        // Override config with CLI args
        apply_args_to_config(args, config);

        // Initialize logger
        Logger::init(config.log_level, config.log_file, config.log_to_console);

        if (args.show_info) {
            config.print_info();
            return 0;
        }

        // Initialize inference engine
        Logger::info("Loading model: {}", config.model_path);
        InferenceEngine engine(config);
        
        if (!engine.load_model()) {
            Logger::error("Failed to load model");
            return 1;
        }

        Logger::info("Model loaded successfully");

        // Determine mode and execute
        if (args.repl_mode) {
            return run_repl_mode(config, engine);
        } else if (!args.agent_name.empty()) {
            return run_agent_mode(config, args.agent_name, engine);
        } else if (args.serve_mode) {
            Logger::error("Server mode not yet implemented");
            return 1;
        } else {
            return run_command_mode(config, args, engine);
        }

    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

