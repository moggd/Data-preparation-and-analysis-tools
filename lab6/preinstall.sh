#!/bin/bash
echo "Installing dependencies..."
if [[ "$OSTYPE" == "darwin"* ]]; then
    brew install opencv cmake gcc
else
    sudo apt update
    sudo apt install -y libopencv-dev cmake g++ gcc
fi
echo "Dependencies installed successfully!"