// REPL (Read-Eval-Print Loop) interface
#pragma once

#include "../config/config.h"
#include "../inference/engine.h"
#include "../session/session.h"
#include <string>
#include <vector>
#include <memory>

namespace llmchat {

class REPL {
public:
    REPL(const Config& config, InferenceEngine& engine);
    ~REPL();
    
    int run();
    
private:
    bool process_command(const std::string& input);
    void print_help();
    void print_welcome();
    std::string read_line(const std::string& prompt);
    std::string read_multiline();
    
    // REPL commands
    void cmd_help();
    void cmd_exit();
    void cmd_clear();
    void cmd_save(const std::string& path);
    void cmd_load(const std::string& path);
    void cmd_session(const std::string& name);
    void cmd_role(const std::string& name);
    void cmd_model(const std::string& path);
    void cmd_agent(const std::string& name);
    void cmd_file(const std::vector<std::string>& paths);
    void cmd_info();
    void cmd_history();
    void cmd_tools();
    
    const Config& config_;
    InferenceEngine& engine_;
    std::unique_ptr<Session> session_;
    std::vector<std::string> history_;
    std::string current_role_;
    bool running_ = true;
};

} // namespace llmchat

