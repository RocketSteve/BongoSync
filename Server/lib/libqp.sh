#!/bin/bash

OS="$(uname)"
echo "Operating System detected: $OS"

# Detect wchich OS is running and install libpq-dev
case "$OS" in
    "Darwin")
        if brew list libpq &>/dev/null; then
            echo "libpq is already installed."
        else
            echo "Installing libpq..."
            brew install libpq
        fi
        ;;
    "Linux")
        sudo apt-get update

        if dpkg -s libpq-dev &>/dev/null; then
            echo "libpq-dev is already installed."
        else
            echo "Installing libpq-dev..."
            sudo apt-get install libpq-dev
        fi
        ;;
    *)
        echo "Unsupported operating system."
        exit 1
        ;;
esac