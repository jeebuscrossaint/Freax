#!/usr/bin/env bash

# Get dependencies
if [ ! -d "limine" ]; then
    git clone https://github.com/limine-bootloader/limine.git --branch=v8.x-binary --depth=1
    make -C limine
fi

# Create build directory
mkdir -p build/

# Compiler and linker settings
CC=$(which clang)
LD=$(which ld.lld)
CFLAGS="-ffreestanding -fno-stack-protector -fno-pic -mno-red-zone -Wall -Wextra -Iinclude -std=gnu99 -O2"
LDFLAGS="-T linker.ld -nostdlib -zmax-page-size=0x1000 --no-dynamic-linker"
OBJS=""

# Compile each C file in kernel/ to an object file in build/
for file in $(find kernel/ -name "*.c"); do
    obj="build/$(basename ${file%.c}.o)"
    OBJS="$OBJS $obj"
    echo "Compiling $file -> $obj"
    $CC $CFLAGS -c "$file" -o "$obj"
done

# Link all object files into the final kernel ELF
echo "Linking kernel.elf"
$LD $LDFLAGS -o build/kernel.elf $OBJS

echo "Build complete: build/kernel.elf"
