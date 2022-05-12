#!/usr/bin/env bash

set -e

cd "$(dirname "$0")"

# Compile
cc -o server server.c

# Run server and replace the shell
exec ./server "$1" "$2"
