#!/usr/bin/env bash

# Define colors
RED='\033[31m'
GREEN='\033[32m'
YELLOW='\033[33m'
BLUE='\033[34m'
MAGENTA='\033[35m'
CYAN='\033[36m'
WHITE='\033[37m'
RESET='\033[0m'

# Function to print success messages
print_success() {
    echo -e "${GREEN}[SUCCESS] ${RESET}$1"
}

# Function to print warning messages
print_warning() {
    echo -e "${YELLOW}[WARNING] ${RESET}$1"
}

# Function to print error messages
print_error() {
    echo -e "${RED}[ERROR] ${RESET}$1"
}

# Get dependencies
echo -e "${CYAN}Checking dependencies...${RESET}"
if [ ! -d "limine" ]; then
    print_warning "Cloning Limine repository..."
    git clone https://github.com/limine-bootloader/limine.git --branch=v8.x-binary --depth=1
    print_warning "Building Limine..."
    make -C limine
else
    print_success "Limine already exists!"
fi

# Delete old build
print_warning "Cleaning old build..."
rm -rf build/
rm -rf iso/

# Create build directory (ensure it exists)
print_success "Creating build directory."
mkdir -p build/ || { print_error "Failed to create build directory."; exit 1; }

# Ensure the build directory is writeable
if [ ! -w build/ ]; then
    print_error "Build directory is not writable!"
    exit 1
fi

# Compiler and linker settings
CC=$(which clang)
LD=$(which ld.lld)
CFLAGS="-ffreestanding -fno-stack-protector -fno-pic -mno-red-zone -Wall -Wextra -Iinclude -std=c11 -O2"
LDFLAGS="-T linker.ld -nostdlib -zmax-page-size=0x1000 --no-dynamic-linker"
OBJS=""

# Compile each C file in kernel/ to an object file in build/
echo -e "${BLUE}Compiling kernel and driver files...${RESET}"
for file in $(find kernel/ drivers/ -name "*.c"); do
    obj="build/$(basename ${file%.c}.o)"
    OBJS="$OBJS $obj"
    echo -e "${MAGENTA}Compiling $file -> $obj...${RESET}"
    $CC $CFLAGS -c "$file" -o "$obj" || { print_error "Compilation failed for $file"; exit 1; }
done

# Link all object files into the final kernel ELF
print_success "Linking kernel.elf"
$LD $LDFLAGS -o build/kernel.elf $OBJS || { print_error "Linking failed."; exit 1; }

print_success "Build complete: build/kernel.elf"

# Manufacturing ISO
echo -e "${CYAN}Manufacturing ISO...${RESET}"
mkdir -p iso/boot
cp -v build/kernel.elf iso/boot
cp -v limine/limine-bios.sys limine/limine.cfg limine/limine-uefi-cd.bin boot/limine.cfg iso/boot/

# Run xorriso to create the ISO
print_success "Running xorriso to create the ISO."
xorriso -as mkisofs -b boot/limine-bios.sys \
    -no-emul-boot -boot-load-size 4 -boot-info-table \
    --efi-boot boot/limine-uefi-cd.bin \
    -o build/os.iso iso || { print_error "ISO creation failed."; exit 1; }

if [ ! -f build/os.iso ]; then
    print_error "Error: ISO not created!"
    exit 1
else
    print_success "ISO Created: build/os.iso"
fi
