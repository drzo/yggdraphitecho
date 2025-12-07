cmake_minimum_required(VERSION 3.15)
project(llmchat VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Options
option(LLMCHAT_BUILD_TESTS "Build tests" OFF)
option(LLMCHAT_CUDA "Enable CUDA support" OFF)
option(LLMCHAT_METAL "Enable Metal support (macOS)" OFF)

# Find dependencies
find_package(Threads REQUIRED)

# Add llama.cpp as subdirectory or find it
set(LLAMA_STATIC ON CACHE BOOL "Build llama.cpp as static library")
set(LLAMA_BUILD_TESTS OFF CACHE BOOL "Build llama.cpp tests")
set(LLAMA_BUILD_EXAMPLES OFF CACHE BOOL "Build llama.cpp examples")

# Check if llama.cpp is available, if not provide instructions
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/llama.cpp/CMakeLists.txt")
    message(FATAL_ERROR 
        "llama.cpp not found. Please run:\n"
        "  git submodule update --init --recursive\n"
        "Or clone llama.cpp manually:\n"
        "  git clone https://github.com/ggerganov/llama.cpp.git")
endif()

add_subdirectory(llama.cpp)

# Source files
set(LLMCHAT_SOURCES
    src/main.cpp
    src/cli/args.cpp
    src/cli/repl.cpp
    src/cli/commands.cpp
    src/config/config.cpp
    src/config/yaml_parser.cpp
    src/inference/engine.cpp
    src/inference/chat.cpp
    src/inference/embeddings.cpp
    src/session/session.cpp
    src/session/storage.cpp
    src/rag/vector_store.cpp
    src/rag/embedder.cpp
    src/rag/chunker.cpp
    src/functions/tool_manager.cpp
    src/functions/tool_executor.cpp
    src/functions/json_schema.cpp
    src/agent/agent.cpp
    src/agent/agent_executor.cpp
    src/utils/logger.cpp
    src/utils/markdown.cpp
    src/utils/json.cpp
    src/utils/file_utils.cpp
    src/utils/string_utils.cpp
    src/render/highlighter.cpp
    src/render/terminal.cpp
)

set(LLMCHAT_HEADERS
    src/cli/args.h
    src/cli/repl.h
    src/cli/commands.h
    src/config/config.h
    src/config/yaml_parser.h
    src/inference/engine.h
    src/inference/chat.h
    src/inference/embeddings.h
    src/session/session.h
    src/session/storage.h
    src/rag/vector_store.h
    src/rag/embedder.h
    src/rag/chunker.h
    src/functions/tool_manager.h
    src/functions/tool_executor.h
    src/functions/json_schema.h
    src/agent/agent.h
    src/agent/agent_executor.h
    src/utils/logger.h
    src/utils/markdown.h
    src/utils/json.h
    src/utils/file_utils.h
    src/utils/string_utils.h
    src/render/highlighter.h
    src/render/terminal.h
)

# Create executable
add_executable(llmchat ${LLMCHAT_SOURCES} ${LLMCHAT_HEADERS})

# Include directories
target_include_directories(llmchat PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/llama.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/yaml-cpp/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/json/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/cxxopts/include
)

# Link libraries
target_link_libraries(llmchat PRIVATE 
    llama
    ggml
    Threads::Threads
)

# Platform-specific settings
if(UNIX AND NOT APPLE)
    target_link_libraries(llmchat PRIVATE dl m)
elseif(APPLE)
    target_link_libraries(llmchat PRIVATE "-framework Foundation" "-framework Accelerate")
    if(LLMCHAT_METAL)
        target_link_libraries(llmchat PRIVATE "-framework Metal" "-framework MetalKit")
    endif()
elseif(WIN32)
    target_compile_definitions(llmchat PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

# Compiler warnings
if(MSVC)
    target_compile_options(llmchat PRIVATE /W4)
else()
    target_compile_options(llmchat PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Install targets
install(TARGETS llmchat RUNTIME DESTINATION bin)
install(DIRECTORY functions/ DESTINATION share/llmchat/functions)
install(FILES config.example.yaml DESTINATION share/llmchat)

# Tests
if(LLMCHAT_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
