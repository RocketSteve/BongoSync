#!/bin/bash

OS="$(uname)"
echo "Operating System detected: $OS"

# Detect which OS is running and install libpq and libpqxx
case "$OS" in
    "Darwin")
        echo "Checking and installing libpq for macOS..."
        if ! brew list libpq &>/dev/null; then
            echo "Installing libpq..."
            brew install libpq
        else
            echo "libpq is already installed."
        fi

        echo "Checking and installing libpqxx for macOS..."
        if ! brew list libpqxx &>/dev/null; then
            echo "Installing libpqxx..."
            brew install libpqxx
        else
            echo "libpqxx is already installed."
        fi
        ;;

    "Linux")
        sudo apt-get update
        echo "Checking and installing libpq-dev for Linux..."
        if ! dpkg -s libpq-dev &>/dev/null; then
            echo "Installing libpq-dev..."
            sudo apt-get install libpq-dev
        else
            echo "libpq-dev is already installed."
        fi

        echo "Checking and installing libpqxx for Linux..."
        if ! dpkg -s libpqxx-dev &>/dev/null; then
            echo "Installing libpqxx-dev..."
            sudo apt-get install libpqxx-dev
        else
            echo "libpqxx-dev is already installed."
        fi
        ;;

    *)
        echo "Unsupported operating system."
        exit 1
        ;;
esac
