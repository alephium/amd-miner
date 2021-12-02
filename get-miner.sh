#!/bin/bash

set -eu

echo "Installing build-essential cmake"
sudo apt install -y build-essential cmake

echo "Git cloning amd-miner"
git clone https://github.com/alephium/amd-miner.git

echo "Installing conan"
temp_file=$(mktemp --suffix=.deb)
curl -L https://github.com/conan-io/conan/releases/latest/download/conan-ubuntu-64.deb -o $temp_file
sudo apt install "$temp_file"
rm -f "$temp_file"

echo "Building the amd miner"
cd amd-miner && ./make.sh

echo "Your miner is built, you could run it with: amd-miner/run-miner.sh"