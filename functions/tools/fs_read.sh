#!/usr/bin/env bash
# @describe Read the contents of a file
# @option --path! The path to the file to read

set -e

main() {
    if [[ -z "$argc_path" ]]; then
        echo "Error: No path provided" >&2
        exit 1
    fi
    
    if [[ ! -f "$argc_path" ]]; then
        echo "Error: File not found: $argc_path" >&2
        exit 1
    fi
    
    cat "$argc_path"
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

