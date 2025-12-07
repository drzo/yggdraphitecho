# Getting Started with LLMChat

This guide will help you get LLMChat up and running quickly.

## Prerequisites

### System Requirements

- **Operating System**: Linux, macOS, or Windows (with WSL recommended)
- **RAM**: 8GB minimum, 16GB+ recommended
- **Storage**: 10GB+ for models and build artifacts
- **Compiler**: GCC 8+, Clang 10+, or MSVC 2019+
- **CMake**: 3.15 or higher

### Dependencies

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake git libreadline-dev
```

**macOS:**
```bash
brew install cmake
```

**Arch Linux:**
```bash
sudo pacman -S base-devel cmake git
```

**Windows:**
- Install Visual Studio 2019+ with C++ tools
- Install CMake from https://cmake.org/

## Installation

### Method 1: From Source (Recommended)

1. **Clone the repository:**
```bash
git clone --recursive https://github.com/yourusername/llmchat.git
cd llmchat
```

If you already cloned without `--recursive`:
```bash
git submodule update --init --recursive
```

2. **Build:**
```bash
chmod +x build.sh
./build.sh

# With CUDA support:
./build.sh -DLLMCHAT_CUDA=ON

# With Metal support (macOS):
./build.sh -DLLMCHAT_METAL=ON
```

3. **Install (optional):**
```bash
cd build
sudo cmake --install .
```

### Method 2: Manual Build

```bash
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

## Configuration

### 1. Create Configuration Directory

```bash
mkdir -p ~/.config/llmchat
```

### 2. Copy Example Configuration

```bash
cp config.example.yaml ~/.config/llmchat/config.yaml
```

### 3. Edit Configuration

```bash
# Use your favorite editor
vim ~/.config/llmchat/config.yaml
# or
nano ~/.config/llmchat/config.yaml
```

**Key settings to configure:**

```yaml
# Set your model path
model_path: ~/.llmchat/models/llama-3.1-8b-q4.gguf

# Adjust for your hardware
threads: 8              # CPU threads
gpu_layers: 0           # GPU layers (0 = CPU only)
context_size: 8192      # Context window

# Generation parameters
temperature: 0.7
max_tokens: 2048
```

## Download a Model

LLMChat uses GGUF format models (llama.cpp compatible).

### Option 1: From Hugging Face

```bash
# Create models directory
mkdir -p ~/.llmchat/models

# Download a model (example: Llama 3.1 8B Q4)
# You can use wget, curl, or Hugging Face CLI
huggingface-cli download \
  TheBloke/Llama-3.1-8B-GGUF \
  llama-3.1-8b.Q4_K_M.gguf \
  --local-dir ~/.llmchat/models
```

### Option 2: Manual Download

Visit https://huggingface.co/models and search for GGUF models.

Popular options:
- **Small (2-7B)**: Good for laptops, 4-8GB RAM
  - Phi-2, TinyLlama, Mistral-7B
- **Medium (8-13B)**: Good performance, 8-16GB RAM
  - Llama-3.1-8B, Mistral-8x7B
- **Large (30B+)**: Best quality, 32GB+ RAM
  - Llama-3.1-70B, DeepSeek-Coder

## First Run

### 1. Start REPL Mode

```bash
./build/llmchat
```

You should see:
```
╔═══════════════════════════════════════════════════════╗
║           LLMChat Interactive REPL v1.0.0           ║
╚═══════════════════════════════════════════════════════╝

Model: Llama-3.1-8B
Type .help for commands, .exit to quit

>>> 
```

### 2. Try Your First Query

```
>>> Hello! Can you introduce yourself?
```

### 3. Explore REPL Commands

```
>>> .help
```

Available commands:
- `.help` - Show help
- `.model <path>` - Load different model
- `.session <name>` - Start/switch session
- `.role <name>` - Use a role
- `.clear` - Clear conversation
- `.exit` - Exit

## Basic Usage Examples

### One-Shot Queries

```bash
# Simple question
./build/llmchat "What is the capital of France?"

# With file input
./build/llmchat -f document.txt "Summarize this"

# From stdin
cat file.txt | ./build/llmchat "Analyze this"
```

### Using Sessions

```bash
# Start named session
./build/llmchat -s coding

# Continue session
./build/llmchat -s coding "Remember our previous discussion?"
```

### Using Roles

```bash
# Use predefined role
./build/llmchat --role coder "Write a sorting algorithm"

# Shell assistant
./build/llmchat --role shell "find all Python files modified today"
```

### Using Tools (Function Calling)

```bash
# Enable tools
./build/llmchat --tools "What time is it?"

# The model will use the get_current_time tool
```

## Setting Up Functions

### 1. Create Functions Directory

```bash
mkdir -p ~/.config/llmchat/functions/tools
mkdir -p ~/.config/llmchat/functions/agents
```

### 2. Copy Example Tools

```bash
cp -r functions/tools/* ~/.config/llmchat/functions/tools/
cp -r functions/agents/* ~/.config/llmchat/functions/agents/
```

### 3. Make Tools Executable

```bash
chmod +x ~/.config/llmchat/functions/tools/*.sh
```

### 4. Enable Function Calling

Edit `~/.config/llmchat/config.yaml`:
```yaml
function_calling: true
tools_dir: ~/.config/llmchat/functions/tools
agents_dir: ~/.config/llmchat/functions/agents
```

## Using Agents

```bash
# Start an agent
./build/llmchat --agent coder "Write a Python web scraper"

# Demo agent
./build/llmchat --agent demo "Show me what you can do"
```

## Setting Up RAG

### 1. Download Embedding Model

```bash
# Download a small embedding model
mkdir -p ~/.llmchat/models
# Download BGE-small or similar GGUF embedding model
```

### 2. Configure RAG

Edit `~/.config/llmchat/config.yaml`:
```yaml
rag_enabled: true
embedding_model: ~/.llmchat/models/bge-small-en-v1.5-q8.gguf
rag_db_path: ~/.llmchat/rag/vectordb
```

### 3. Index Documents

```bash
./build/llmchat --index ./documents
```

### 4. Query with RAG

```bash
./build/llmchat --rag "What does the documentation say about X?"
```

## Troubleshooting

### Model Won't Load

**Error**: `Failed to load model`

**Solutions**:
1. Check file path in config
2. Verify model is GGUF format
3. Check file permissions
4. Try reducing context_size

### Out of Memory

**Error**: System runs out of RAM

**Solutions**:
1. Use a smaller model (Q4 instead of Q8)
2. Reduce context_size
3. Lower gpu_layers
4. Close other applications

### Slow Performance

**Solutions**:
1. Increase threads count
2. Enable GPU layers (if you have GPU)
3. Use quantized model (Q4 is faster)
4. Reduce batch_size for lower RAM usage

### Tool Execution Fails

**Error**: `Failed to execute tool`

**Solutions**:
1. Check tool script has execute permission
2. Verify bash/python/node is in PATH
3. Check tool script for errors
4. Enable debug logging

## Next Steps

- **Customize roles**: Edit config.yaml to add your own roles
- **Create tools**: Write custom tools in functions/tools/
- **Create agents**: Define specialized agents
- **Explore RAG**: Index your documents for Q&A
- **Tune performance**: Adjust threads, GPU layers for your hardware

## Additional Resources

- **README.md** - Project overview
- **CONTRIBUTING.md** - Contribution guidelines
- **config.example.yaml** - Full configuration reference
- **llama.cpp docs** - https://github.com/ggerganov/llama.cpp

## Getting Help

- Check existing issues: https://github.com/yourusername/llmchat/issues
- Join discussions: https://github.com/yourusername/llmchat/discussions
- Read the wiki: https://github.com/yourusername/llmchat/wiki

## Common Commands Cheat Sheet

```bash
# Start interactive mode
llmchat

# One-shot query
llmchat "your question"

# With file
llmchat -f file.txt "analyze this"

# Use session
llmchat -s myproject "continue our work"

# Use role
llmchat --role coder "write code"

# Use agent
llmchat --agent coder "complex task"

# With tools enabled
llmchat --tools "task requiring tools"

# Load different model
llmchat -m path/to/model.gguf

# Show info
llmchat --info

# Stream disabled
llmchat --no-stream "question"
```

Happy chatting!

