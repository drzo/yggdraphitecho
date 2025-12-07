#!/bin/bash
# Build script for LLMChat

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}Building LLMChat...${NC}"

# Check for llama.cpp
if [ ! -d "llama.cpp" ]; then
    echo -e "${YELLOW}llama.cpp not found. Cloning...${NC}"
    git clone https://github.com/ggerganov/llama.cpp.git
fi

# Create build directory
mkdir -p build
cd build

# Configure
echo -e "${GREEN}Configuring CMake...${NC}"
cmake .. "$@"

# Build
echo -e "${GREEN}Compiling...${NC}"
cmake --build . --config Release -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

echo -e "${GREEN}Build complete!${NC}"
echo -e "Executable: ${YELLOW}$(pwd)/llmchat${NC}"
echo ""
echo "To install:"
echo "  sudo cmake --install ."
echo ""
echo "To run:"
echo "  ./llmchat --help"

