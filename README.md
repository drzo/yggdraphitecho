# LLMChat: Unified LLM CLI Tool with Integrated Inference

LLMChat is a complete C/C++ implementation combining the functionality of **aichat** and **llm-functions** with an integrated **llama.cpp/ggml** inference engine. It provides advanced CLI capabilities for local LLM interaction without external dependencies.

## Features

### Core Capabilities
- 🚀 **Integrated Local Inference**: Built-in llama.cpp/ggml engine for GGUF model support
- 💬 **Interactive REPL Mode**: Full-featured chat with history, autocomplete, and syntax highlighting
- 🛠️ **Function Calling**: Execute tools and scripts from LLM conversations
- 🤖 **AI Agents**: CLI-based agents with tools and RAG
- 📚 **RAG Support**: Document embedding, retrieval, and context-aware responses
- 🎯 **Session Management**: Persistent conversation contexts
- 🎨 **Rich Terminal UI**: Markdown rendering and syntax highlighting
- ⚡ **Multi-platform**: Windows, Linux, macOS support

### Advanced Features
- Multi-modal input (files, directories, URLs, stdin)
- Role-based prompting
- Session compression and summarization
- Custom tool creation (Bash, Python, JavaScript via system calls)
- Vector similarity search
- Streaming responses
- Command mode for one-shot queries

## Building from Source

### Prerequisites

```bash
# Install build tools
# Ubuntu/Debian:
sudo apt-get install build-essential cmake git

# macOS:
brew install cmake

# Windows: Install Visual Studio 2019+ with C++ tools
```

### Clone and Build

```bash
# Clone with submodules
git clone --recursive https://github.com/yourusername/llmchat.git
cd llmchat

# Or if already cloned:
git submodule update --init --recursive

# Build
mkdir build && cd build
cmake ..
cmake --build . --config Release

# Install (optional)
sudo cmake --install .
```

### Build Options

```bash
# Enable CUDA support
cmake -DLLMCHAT_CUDA=ON ..

# Enable Metal (macOS)
cmake -DLLMCHAT_METAL=ON ..

# Build with tests
cmake -DLLMCHAT_BUILD_TESTS=ON ..
```

## Quick Start

### 1. Download a Model

```bash
# Download a GGUF model (example: Llama 3.1 8B)
mkdir -p ~/.llmchat/models
wget https://huggingface.co/model.gguf -O ~/.llmchat/models/llama-3.1-8b-q4.gguf
```

### 2. Configure

```bash
# Create config directory
mkdir -p ~/.config/llmchat

# Copy example config
cp config.example.yaml ~/.config/llmchat/config.yaml

# Edit config to point to your model
# model_path: ~/.llmchat/models/llama-3.1-8b-q4.gguf
```

### 3. Run

```bash
# Start REPL mode
llmchat

# One-shot command
llmchat "What is the capital of France?"

# With file input
llmchat -f document.txt "Summarize this"

# Execute shell commands
llmchat -e "list all PDF files in current directory"
```

## Usage

### REPL Mode

```bash
llmchat
```

**REPL Commands:**
- `.help` - Show help
- `.model <path>` - Load a different model
- `.session <name>` - Start/switch session
- `.role <name>` - Use a role
- `.agent <name>` - Start agent
- `.file <path>` - Add file to context
- `.clear` - Clear conversation
- `.save <path>` - Save conversation
- `.exit` - Exit REPL

### Command Mode

```bash
# Simple query
llmchat "Hello, how are you?"

# With files
llmchat -f code.cpp "Explain this code"

# With directory
llmchat -f ./src/ "Analyze this codebase"

# Pipe input
cat file.txt | llmchat "Summarize"

# Execute mode (shell assistant)
llmchat -e "find all TODO comments in C++ files"
```

### Function Calling & Tools

```bash
# Enable tools in config.yaml
function_calling: true

# Use built-in tools
llmchat --tools "What's the weather in Paris?"

# Create custom tool (functions/tools/my_tool.sh)
./llmchat-tool create my_tool
```

### Agents

```bash
# Start an agent
llmchat --agent coder "Write a sorting algorithm"

# Create custom agent
./llmchat-agent create my_agent
```

### RAG (Document Q&A)

```bash
# Index documents
llmchat --index-dir ./documents

# Query with RAG
llmchat --rag "What does the contract say about termination?"
```

## Configuration

Edit `~/.config/llmchat/config.yaml`:

```yaml
# Model settings
model_path: ~/.llmchat/models/llama-3.1-8b-q4.gguf
model_type: llama
context_size: 8192
threads: 8
gpu_layers: 32  # For CUDA/Metal

# Generation parameters
temperature: 0.7
top_p: 0.9
top_k: 40
repeat_penalty: 1.1

# Behavior
stream: true
save_history: true
max_history: 1000

# Function calling
function_calling: true
tools_dir: ~/.config/llmchat/functions/tools
agents_dir: ~/.config/llmchat/functions/agents

# RAG
rag_enabled: true
embedding_model: ~/.llmchat/models/bge-small-en-v1.5-q8.gguf
chunk_size: 512
chunk_overlap: 50
top_k_retrieval: 5

# UI
syntax_highlighting: true
markdown_rendering: true
theme: auto  # auto, dark, light
```

## Function System

### Built-in Tools

- `execute_command` - Run shell commands
- `fs_read` - Read files
- `fs_write` - Write files
- `fs_list` - List directory
- `web_search` - Search the web (requires API)
- `get_weather` - Get weather info (requires API)
- `calculator` - Basic calculations

### Creating Custom Tools

**Bash Tool** (`functions/tools/my_tool.sh`):
```bash
#!/bin/bash
# @describe Does something useful
# @option --input! The input text

main() {
    echo "Processing: $argc_input"
    # Your logic here
}
```

**Python Tool** (`functions/tools/my_tool.py`):
```python
#!/usr/bin/env python3
def run(input_text: str) -> str:
    """Does something useful
    Args:
        input_text: The input to process
    """
    return f"Processed: {input_text}"
```

### Creating Agents

Create `functions/agents/my_agent/index.yaml`:
```yaml
name: MyAgent
description: Does specialized tasks
instructions: |
  You are a helpful agent that specializes in...
tools:
  - execute_command
  - fs_read
documents:
  - knowledge.txt
```

## Architecture

```
llmchat/
├── src/
│   ├── main.cpp              # Entry point
│   ├── cli/                  # CLI argument parsing & REPL
│   ├── config/               # Configuration management
│   ├── inference/            # llama.cpp/ggml integration
│   ├── session/              # Session persistence
│   ├── rag/                  # RAG implementation
│   ├── functions/            # Tool system
│   ├── agent/                # Agent execution
│   ├── render/               # Terminal rendering
│   └── utils/                # Utilities
├── functions/
│   ├── tools/                # Tool scripts
│   └── agents/               # Agent definitions
├── llama.cpp/                # Submodule
└── third_party/              # Dependencies
```

## Performance

- **Startup time**: ~100ms (without model load)
- **Model loading**: 1-5s depending on size
- **Inference**: Depends on hardware and model
  - CPU: 10-50 tokens/sec (8B model)
  - CUDA: 50-200 tokens/sec
  - Metal: 40-150 tokens/sec
- **Memory**: Model size + 2-4GB overhead

## Limitations

- Local models only (no API providers)
- Function calling requires system shell access
- Web search tools need external APIs
- RAG requires embedding model

## Contributing

Contributions welcome! Please see CONTRIBUTING.md

## License

MIT OR Apache-2.0 (dual licensed, same as aichat and llama.cpp)

## Acknowledgments

- [aichat](https://github.com/sigoden/aichat) - Original inspiration
- [llm-functions](https://github.com/sigoden/llm-functions) - Function system design
- [llama.cpp](https://github.com/ggerganov/llama.cpp) - Inference engine
- [ggml](https://github.com/ggerganov/ggml) - ML primitives

## Roadmap

- [ ] Multi-modal support (vision)
- [ ] Server mode (OpenAI-compatible API)
- [ ] Web UI
- [ ] Model quantization tools
- [ ] Performance optimization
- [ ] Extended platform support (Android, iOS)
