#!/usr/bin/env bash
# @describe Get the current date and time

set -e

main() {
    date "+%Y-%m-%d %H:%M:%S %Z"
}

main "$@"

