// REPL implementation
#include "repl.h"
#include "../utils/logger.h"
#include "../render/terminal.h"
#include "../render/highlighter.h"

#include <iostream>
#include <sstream>
#include <algorithm>

#ifdef _WIN32
#include <io.h>
#define isatty _isatty
#define fileno _fileno
#else
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#endif

namespace llmchat {

REPL::REPL(const Config& config, InferenceEngine& engine)
    : config_(config), engine_(engine) {
    
    // Create default session
    session_ = std::make_unique<Session>(config, config_.default_session);
}

REPL::~REPL() {
    // Save session if configured
    if (config_.save_sessions && session_) {
        session_->save();
    }
}

void REPL::print_welcome() {
    Terminal::print_colored(Terminal::Color::CYAN, "╔═══════════════════════════════════════════════════════╗\n");
    Terminal::print_colored(Terminal::Color::CYAN, "║           LLMChat Interactive REPL v1.0.0           ║\n");
    Terminal::print_colored(Terminal::Color::CYAN, "╚═══════════════════════════════════════════════════════╝\n");
    std::cout << "\n";
    std::cout << "Model: " << engine_.get_model_name() << "\n";
    std::cout << "Type .help for commands, .exit to quit\n";
    std::cout << "\n";
}

void REPL::print_help() {
    std::cout << R"(
REPL Commands:
  .help                 Show this help
  .exit, .quit          Exit REPL
  .clear                Clear conversation history
  .save <path>          Save conversation to file
  .load <path>          Load conversation from file
  .session <name>       Create/switch session
  .role <name>          Use a role
  .model <path>         Load different model
  .agent <name>         Start an agent
  .file <path>          Add file(s) to context
  .info                 Show current settings
  .history              Show conversation history
  .tools                List available tools

Navigation:
  Ctrl+C                Cancel current input
  Ctrl+D                Exit REPL
  Up/Down               Navigate history (if readline available)
)";
}

std::string REPL::read_line(const std::string& prompt) {
#ifndef _WIN32
    if (isatty(fileno(stdin))) {
        char* line = readline(prompt.c_str());
        if (line == nullptr) {
            return "";
        }
        
        std::string result(line);
        if (!result.empty()) {
            add_history(line);
        }
        
        free(line);
        return result;
    }
#endif
    
    // Fallback for Windows or non-tty
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}

std::string REPL::read_multiline() {
    std::string result;
    std::string line;
    
    std::cout << "Enter message (Ctrl+D or empty line to finish):\n";
    
    while (true) {
        line = read_line("... ");
        
        if (line.empty() || std::cin.eof()) {
            break;
        }
        
        result += line + "\n";
    }
    
    return result;
}

bool REPL::process_command(const std::string& input) {
    if (input.empty()) return true;
    
    // Check for REPL commands
    if (input[0] == '.') {
        std::istringstream iss(input.substr(1));
        std::string cmd;
        iss >> cmd;
        
        if (cmd == "help" || cmd == "h") {
            cmd_help();
        } else if (cmd == "exit" || cmd == "quit" || cmd == "q") {
            cmd_exit();
        } else if (cmd == "clear" || cmd == "c") {
            cmd_clear();
        } else if (cmd == "save") {
            std::string path;
            iss >> path;
            cmd_save(path);
        } else if (cmd == "load") {
            std::string path;
            iss >> path;
            cmd_load(path);
        } else if (cmd == "session" || cmd == "s") {
            std::string name;
            iss >> name;
            cmd_session(name);
        } else if (cmd == "role" || cmd == "r") {
            std::string name;
            iss >> name;
            cmd_role(name);
        } else if (cmd == "model" || cmd == "m") {
            std::string path;
            iss >> path;
            cmd_model(path);
        } else if (cmd == "agent" || cmd == "a") {
            std::string name;
            iss >> name;
            cmd_agent(name);
        } else if (cmd == "file" || cmd == "f") {
            std::vector<std::string> paths;
            std::string path;
            while (iss >> path) {
                paths.push_back(path);
            }
            cmd_file(paths);
        } else if (cmd == "info" || cmd == "i") {
            cmd_info();
        } else if (cmd == "history") {
            cmd_history();
        } else if (cmd == "tools" || cmd == "t") {
            cmd_tools();
        } else {
            std::cout << "Unknown command: " << cmd << "\n";
            std::cout << "Type .help for available commands\n";
        }
        
        return true;
    }
    
    // Regular chat message
    return false;
}

int REPL::run() {
    print_welcome();
    
    running_ = true;
    
    while (running_) {
        std::string prompt = config_.repl_prompt;
        std::string input = read_line(prompt);
        
        if (std::cin.eof()) {
            break;
        }
        
        // Skip empty lines
        if (input.empty()) {
            continue;
        }
        
        // Store in history
        history_.push_back(input);
        
        // Process command or chat
        if (process_command(input)) {
            continue;
        }
        
        // Send to LLM
        try {
            std::cout << "\n";
            
            if (config_.stream) {
                // Streaming response
                engine_.generate_stream(input, [](const std::string& token) {
                    std::cout << token << std::flush;
                });
                std::cout << "\n\n";
            } else {
                // Non-streaming response
                std::string response = engine_.generate(input);
                
                if (config_.markdown_rendering) {
                    // TODO: Render markdown
                    std::cout << response << "\n\n";
                } else {
                    std::cout << response << "\n\n";
                }
            }
            
        } catch (const std::exception& e) {
            Terminal::print_colored(Terminal::Color::RED, "Error: " + std::string(e.what()) + "\n");
        }
    }
    
    std::cout << "\nGoodbye!\n";
    return 0;
}

void REPL::cmd_help() {
    print_help();
}

void REPL::cmd_exit() {
    running_ = false;
}

void REPL::cmd_clear() {
    if (session_) {
        session_->clear();
    }
    std::cout << "Conversation cleared\n";
}

void REPL::cmd_save(const std::string& path) {
    if (path.empty()) {
        std::cout << "Usage: .save <path>\n";
        return;
    }
    
    if (session_ && session_->save(path)) {
        std::cout << "Conversation saved to: " << path << "\n";
    } else {
        std::cout << "Failed to save conversation\n";
    }
}

void REPL::cmd_load(const std::string& path) {
    std::cout << "Load command not yet implemented\n";
}

void REPL::cmd_session(const std::string& name) {
    if (name.empty()) {
        std::cout << "Current session: " << (session_ ? session_->name() : "none") << "\n";
        return;
    }
    
    session_ = std::make_unique<Session>(config_, name);
    std::cout << "Switched to session: " << name << "\n";
}

void REPL::cmd_role(const std::string& name) {
    if (name.empty()) {
        std::cout << "Current role: " << (current_role_.empty() ? "none" : current_role_) << "\n";
        return;
    }
    
    auto role = const_cast<Config&>(config_).find_role(name);
    if (role) {
        current_role_ = name;
        std::cout << "Using role: " << name << "\n";
        std::cout << role->description << "\n";
    } else {
        std::cout << "Role not found: " << name << "\n";
    }
}

void REPL::cmd_model(const std::string& path) {
    if (path.empty()) {
        std::cout << "Current model: " << engine_.get_model_name() << "\n";
        return;
    }
    
    std::cout << "Loading model: " << path << "\n";
    if (engine_.load_model(path)) {
        std::cout << "Model loaded successfully\n";
    } else {
        std::cout << "Failed to load model\n";
    }
}

void REPL::cmd_agent(const std::string& name) {
    std::cout << "Agent support not yet implemented\n";
}

void REPL::cmd_file(const std::vector<std::string>& paths) {
    std::cout << "File input not yet implemented\n";
}

void REPL::cmd_info() {
    std::cout << "\nCurrent Settings:\n";
    std::cout << "  Model: " << engine_.get_model_name() << "\n";
    std::cout << "  Context: " << engine_.get_context_size() << " tokens\n";
    std::cout << "  Session: " << (session_ ? session_->name() : "none") << "\n";
    std::cout << "  Role: " << (current_role_.empty() ? "none" : current_role_) << "\n";
    std::cout << "  Temperature: " << config_.temperature << "\n";
    std::cout << "  Max tokens: " << config_.max_tokens << "\n";
    std::cout << "\n";
}

void REPL::cmd_history() {
    if (history_.empty()) {
        std::cout << "No history\n";
        return;
    }
    
    std::cout << "\nHistory:\n";
    for (size_t i = 0; i < history_.size(); i++) {
        std::cout << "  " << (i + 1) << ": " << history_[i] << "\n";
    }
    std::cout << "\n";
}

void REPL::cmd_tools() {
    std::cout << "Available tools:\n";
    std::cout << "  (Tool listing not yet implemented)\n";
}

} // namespace llmchat

