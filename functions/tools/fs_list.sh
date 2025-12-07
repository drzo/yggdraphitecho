#!/usr/bin/env bash
# @describe List files in a directory
# @option --path The directory path (default: current directory)

set -e

main() {
    local path="${argc_path:-.}"
    
    if [[ ! -d "$path" ]]; then
        echo "Error: Directory not found: $path" >&2
        exit 1
    fi
    
    ls -lah "$path"
}

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --path)
            argc_path="$2"
            shift 2
            ;;
        *)
            shift
            ;;
    esac
done

main "$@"

