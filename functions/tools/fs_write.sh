#!/usr/bin/env bash
# @describe Write content to a file
# @option --path! The path to the file to write
# @option --content! The content to write

set -e

main() {
    if [[ -z "$argc_path" ]]; then
        echo "Error: No path provided" >&2
        exit 1
    fi
    
    if [[ -z "$argc_content" ]]; then
        echo "Error: No content provided" >&2
        exit 1
    fi
    
    echo "$argc_content" > "$argc_path"
    echo "File written: $argc_path"
}

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --path)
            argc_path="$2"
            shift 2
            ;;
        --content)
            argc_content="$2"
            shift 2
            ;;
        *)
            shift
            ;;
    esac
done

main "$@"

