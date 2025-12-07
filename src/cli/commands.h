// Command mode executor
#pragma once

#include "../config/config.h"
#include "../inference/engine.h"
#include "args.h"

namespace llmchat {

class CommandExecutor {
public:
    CommandExecutor(const Config& config, InferenceEngine& engine);
    
    int execute(const Args& args);
    
private:
    const Config& config_;
    InferenceEngine& engine_;
};

} // namespace llmchat

