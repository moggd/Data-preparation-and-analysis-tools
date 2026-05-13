#!/bin/bash
echo "Building the project..."
cd build
cmake ..
make
echo "Build completed!"