#!/usr/bin/env bash

# Define color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
CYAN='\033[0;36m'
WHITE='\033[0;37m'
BOLD='\033[1m'
RESET='\033[0m'

# Check if the ISO exists
if [ ! -f "build/os.iso" ]; then
    echo -e "${RED}[ERROR]${RESET} ISO file not found! Please build the OS first with ${CYAN}./build.sh${RESET}."
    ./build.sh
fi

echo -e "${CYAN}[INFO]${RESET} Starting the OS in QEMU..."

# Run QEMU with the ISO
qemu-system-x86_64 -cdrom build/os.iso -m 512M -serial stdio

# Check if the QEMU command ran successfully
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[SUCCESS]${RESET} OS booted successfully in QEMU!"
else
    echo -e "${RED}[ERROR]${RESET} Something went wrong while booting the OS!"
fi
