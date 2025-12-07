# LLMChat Quick Start Guide

Get up and running with LLMChat in 5 minutes!

## 🚀 Quick Installation

### Step 1: Clone with Submodules

```bash
# Clone the repository
git clone --recursive https://github.com/yourusername/llmchat.git
cd llmchat
```

**Note**: If llama.cpp submodule is missing, run:
```bash
git submodule update --init --recursive
```

### Step 2: Build

```bash
# Make build script executable
chmod +x build.sh

# Build the project
./build.sh

# For GPU support:
# ./build.sh -DLLMCHAT_CUDA=ON          # NVIDIA
# ./build.sh -DLLMCHAT_METAL=ON         # Apple Silicon
```

### Step 3: Download a Model

```bash
# Create models directory
mkdir -p ~/.llmchat/models

# Example: Download Llama 3.1 8B (4-bit quantized)
# Option 1: Using huggingface-cli
huggingface-cli download \
  TheBloke/Llama-3.1-8B-GGUF \
  llama-3.1-8b.Q4_K_M.gguf \
  --local-dir ~/.llmchat/models

# Option 2: Manual download
# Visit https://huggingface.co/models?library=gguf
# Download any GGUF model to ~/.llmchat/models/
```

### Step 4: Configure

```bash
# Create config directory
mkdir -p ~/.config/llmchat

# Copy example config
cp config.example.yaml ~/.config/llmchat/config.yaml

# Edit to set your model path
nano ~/.config/llmchat/config.yaml
# Change: model_path: ~/.llmchat/models/llama-3.1-8b.Q4_K_M.gguf
```

### Step 5: Run!

```bash
# Start interactive mode
./build/llmchat

# Or one-shot query
./build/llmchat "Hello! Can you introduce yourself?"
```

## 📋 Common Commands

```bash
# Interactive REPL
./build/llmchat

# One-shot query
./build/llmchat "Your question here"

# With file input
./build/llmchat -f document.txt "Summarize this"

# Use a session
./build/llmchat -s myproject "Continue our work"

# Use a role
./build/llmchat --role coder "Write a sorting algorithm"

# Enable tools
./build/llmchat --tools "What time is it?"

# Show help
./build/llmchat --help

# Show config info
./build/llmchat --info
```

## 🔧 Setup Functions (Optional)

```bash
# Copy example tools and agents
mkdir -p ~/.config/llmchat/functions
cp -r functions/* ~/.config/llmchat/functions/

# Make tools executable
chmod +x ~/.config/llmchat/functions/tools/*.sh

# Enable in config
# Edit ~/.config/llmchat/config.yaml:
# function_calling: true
# tools_dir: ~/.config/llmchat/functions/tools
# agents_dir: ~/.config/llmchat/functions/agents
```

## 💡 REPL Commands

Once in REPL mode:

```
.help           - Show help
.model <path>   - Load different model
.session <name> - Start/switch session
.role <name>    - Use a role
.clear          - Clear conversation
.info           - Show current settings
.exit           - Exit REPL
```

## 🎯 Example Workflows

### Code Assistant

```bash
# Start coding session
./build/llmchat -s coding --role coder

# In REPL:
> Write a Python function to merge two sorted lists
> Now write unit tests for it
> Explain the time complexity
```

### Document Q&A with RAG

```bash
# Index documents (future feature)
./build/llmchat --index ./my-docs

# Query with RAG
./build/llmchat --rag "What does the documentation say about X?"
```

### Shell Assistant

```bash
# Get shell commands
./build/llmchat --role shell "find all Python files modified in the last 24 hours"

# Execute safely
./build/llmchat --role shell "list all running Docker containers"
```

### Using Agents

```bash
# Use the coder agent
./build/llmchat --agent coder "Create a web scraper for news articles"

# Use the demo agent
./build/llmchat --agent demo "Show me what you can do"
```

## 🔍 Troubleshooting

### Build Fails

```bash
# Clean build
rm -rf build
mkdir build && cd build
cmake .. && cmake --build .
```

### Model Won't Load

1. Check file path: `ls -lh ~/.llmchat/models/`
2. Verify GGUF format
3. Try smaller model or reduce context_size in config

### Out of Memory

1. Use smaller/more quantized model (Q4 vs Q8)
2. Reduce `context_size` in config
3. Lower `gpu_layers` if using GPU

### Slow Performance

1. Increase `threads` in config to match CPU cores
2. Enable GPU layers if you have a GPU
3. Use Q4 quantization (faster than Q8)

## 📚 Next Steps

- **Read**: [GETTING_STARTED.md](GETTING_STARTED.md) for detailed setup
- **Explore**: [README.md](README.md) for features overview
- **Learn**: [IMPLEMENTATION.md](IMPLEMENTATION.md) for architecture details
- **Contribute**: [CONTRIBUTING.md](CONTRIBUTING.md) for development guide

## 🆘 Getting Help

- GitHub Issues: https://github.com/yourusername/llmchat/issues
- Documentation: https://github.com/yourusername/llmchat/wiki

## ⚡ Pro Tips

1. **Use sessions** for ongoing projects to maintain context
2. **Create custom roles** for specialized tasks
3. **Write custom tools** for your specific workflows
4. **Use quantized models** (Q4_K_M) for best speed/quality balance
5. **Enable GPU layers** if you have compatible hardware

---

**Ready to chat!** 🎉

For the full experience, check out the complete [GETTING_STARTED.md](GETTING_STARTED.md) guide.

