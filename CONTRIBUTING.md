# Contributing to LLMChat

Thank you for your interest in contributing to LLMChat!

## Development Setup

1. Clone the repository with submodules:
```bash
git clone --recursive https://github.com/yourusername/llmchat.git
cd llmchat
```

2. Install dependencies:
```bash
# Ubuntu/Debian
sudo apt-get install build-essential cmake git

# macOS
brew install cmake

# Arch Linux
sudo pacman -S base-devel cmake git
```

3. Build:
```bash
./build.sh
```

## Code Style

- Follow the existing code style
- Use meaningful variable names
- Add comments for complex logic
- Keep functions focused and small

## Testing

- Test your changes before submitting
- Add tests for new features
- Ensure existing tests pass

## Pull Request Process

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## Reporting Issues

Please include:
- OS and version
- LLMChat version
- Steps to reproduce
- Expected vs actual behavior
- Error messages/logs

## Adding Tools

1. Create a script in `functions/tools/`
2. Use special comments for metadata:
   - `@describe` - Tool description
   - `@option` - Parameter definition
3. Test the tool standalone
4. Submit a PR

## Adding Agents

1. Create a directory in `functions/agents/`
2. Add `index.yaml` with configuration
3. Specify tools and instructions
4. Test the agent
5. Submit a PR

## License

By contributing, you agree that your contributions will be licensed under the same license as the project (MIT OR Apache-2.0).
