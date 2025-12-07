// JSON Schema for function calling
#pragma once

#include <string>

namespace llmchat {

class JsonSchema {
public:
    static std::string generate_openai_function_schema(const struct ToolDefinition& tool);
};

} // namespace llmchat

