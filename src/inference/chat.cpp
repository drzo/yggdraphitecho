// Chat interface implementation
#include "chat.h"
#include <sstream>

namespace llmchat {

std::string ChatInterface::format_chat_ml(const std::vector<Message>& messages) {
    std::ostringstream oss;
    for (const auto& msg : messages) {
        oss << "<|im_start|>" << msg.role << "\n" << msg.content << "<|im_end|>\n";
    }
    oss << "<|im_start|>assistant\n";
    return oss.str();
}

std::string ChatInterface::format_llama2(const std::vector<Message>& messages) {
    std::ostringstream oss;
    
    for (const auto& msg : messages) {
        if (msg.role == "system") {
            oss << "[INST] <<SYS>>\n" << msg.content << "\n<</SYS>>\n\n";
        } else if (msg.role == "user") {
            oss << msg.content << " [/INST] ";
        } else if (msg.role == "assistant") {
            oss << msg.content << " ";
        }
    }
    
    return oss.str();
}

std::string ChatInterface::format_alpaca(const std::vector<Message>& messages) {
    std::ostringstream oss;
    
    for (const auto& msg : messages) {
        if (msg.role == "system") {
            oss << msg.content << "\n\n";
        } else if (msg.role == "user") {
            oss << "### Instruction:\n" << msg.content << "\n\n### Response:\n";
        } else if (msg.role == "assistant") {
            oss << msg.content << "\n\n";
        }
    }
    
    return oss.str();
}

std::string ChatInterface::detect_format(const std::string& model_name) {
    if (model_name.find("llama-2") != std::string::npos) {
        return "llama2";
    }
    if (model_name.find("alpaca") != std::string::npos) {
        return "alpaca";
    }
    return "chatml";
}

} // namespace llmchat

