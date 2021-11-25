#!/bin/bash

set -eu

echo "Installing build-essential libuv"
sudo apt install -y build-essential libuv1-dev

echo "Git cloning gpu-miner"
git clone https://github.com/alephium/amd-miner.git

echo "Building the gpu miner"
cd amd-miner && make linux-gpu

echo "Your miner is built, you could run it with: amd-miner/run-miner.sh"