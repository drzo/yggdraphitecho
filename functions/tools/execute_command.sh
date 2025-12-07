#!/usr/bin/env bash
# @describe Execute a shell command
# @option --command! The shell command to execute

set -e

main() {
    # Safety check
    if [[ -z "$argc_command" ]]; then
        echo "Error: No command provided" >&2
        exit 1
    fi
    
    # Execute the command
    eval "$argc_command"
}

# Parse arguments (argc-compatible)
while [[ $# -gt 0 ]]; do
    case $1 in
        --command)
            argc_command="$2"
            shift 2
            ;;
        *)
            shift
            ;;
    esac
done

main "$@"

