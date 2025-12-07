// Chat interface helpers
#pragma once

#include "engine.h"
#include <string>

namespace llmchat {

class ChatInterface {
public:
    static std::string format_chat_ml(const std::vector<Message>& messages);
    static std::string format_llama2(const std::vector<Message>& messages);
    static std::string format_alpaca(const std::vector<Message>& messages);
    static std::string detect_format(const std::string& model_name);
};

} // namespace llmchat

