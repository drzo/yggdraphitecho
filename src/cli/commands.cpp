// Command mode implementation
#include "commands.h"
#include "../utils/logger.h"
#include "../render/terminal.h"

#include <iostream>
#include <fstream>
#include <sstream>

namespace llmchat {

CommandExecutor::CommandExecutor(const Config& config, InferenceEngine& engine)
    : config_(config), engine_(engine) {
}

int CommandExecutor::execute(const Args& args) {
    // Build prompt from args
    std::string prompt = args.prompt;
    
    // Check for stdin input
    if (!isatty(fileno(stdin)) || args.use_stdin) {
        std::string line;
        std::ostringstream oss;
        
        while (std::getline(std::cin, line)) {
            oss << line << "\n";
        }
        
        std::string stdin_content = oss.str();
        if (!stdin_content.empty()) {
            if (!prompt.empty()) {
                prompt = stdin_content + "\n\n" + prompt;
            } else {
                prompt = stdin_content;
            }
        }
    }
    
    // Add file contents if specified
    for (const auto& file_path : args.files) {
        // TODO: Load file content
        Logger::debug("Would load file: {}", file_path);
    }
    
    if (prompt.empty()) {
        std::cerr << "Error: No input provided\n";
        std::cerr << "Use 'llmchat --help' for usage information\n";
        return 1;
    }
    
    // Generate response
    try {
        if (config_.stream) {
            // Streaming output
            engine_.generate_stream(prompt, [](const std::string& token) {
                std::cout << token << std::flush;
            });
            std::cout << "\n";
        } else {
            // Non-streaming output
            std::string response = engine_.generate(prompt);
            std::cout << response << "\n";
        }
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}

} // namespace llmchat

