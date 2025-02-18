cmake_minimum_required(VERSION 3.31.5)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/toolchain.cmake CACHE STRING "Toolchain file")
project(Freax LANGUAGES CXX C ASM)

# Check if limine exists and clone/build if it doesn't
if(NOT EXISTS ${CMAKE_SOURCE_DIR}/limine)
    message(STATUS "Cloning and building Limine bootloader...")
    execute_process(
        COMMAND git clone https://github.com/limine-bootloader/limine.git --branch=v4.x-branch-binary --depth=1
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    execute_process(
        COMMAND make
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/limine
    )
endif()

# Create necessary directories
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/boot)

add_compile_options(
    -ffreestanding # No libc
    -fno-builtin # fuck no builtins?
    -nostdlib
    -mcmodel=kernel
    -mno-red-zone
    -fno-exceptions
    -fno-rtti
    -Wall -Wextra # Enable warnings with strictness
    --target=x86_64-elf # clang target
)

add_executable(freax.elf
  kernel/main.cc # add more sources when necessary
)

# Prevent linking with standard system startup files
target_link_options(freax.elf PRIVATE
    -nostdlib
    -static
    -z max-page-size=0x1000
    -T ${CMAKE_SOURCE_DIR}/linker.ld
)

set_target_properties(freax.elf PROPERTIES
    LINK_DEPENDS ${CMAKE_SOURCE_DIR}/linker.ld
)

# Copy Limine files to build directory
add_custom_target(copy_limine_files
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/limine/limine-bios.sys
        ${CMAKE_SOURCE_DIR}/limine/limine-bios-cd.bin
        ${CMAKE_SOURCE_DIR}/limine/limine-uefi-cd.bin
        ${CMAKE_BINARY_DIR}/boot/
    COMMENT "Copying Limine files"
)

add_dependencies(freax.elf copy_limine_files)

add_Custom_command(TARGET freax.elf POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/boot
    COMMAND ${CMAKE_OBJCOPY} --strip-unneeded -O elf64-x86-64 ${CMAKE_BINARY_DIR}/freax.elf ${CMAKE_BINARY_DIR}/boot/freax.elf
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/boot/limine.cfg ${CMAKE_BINARY_DIR}/boot/
    COMMAND xorriso -as mkisofs
        -b limine-bios.sys
        -no-emul-boot
        -boot-load-size 4
        -boot-info-table
        --efi-boot limine-uefi-cd.bin
        -efi-boot-part
        --efi-boot-image
        --protective-msdos-label
        -o ${CMAKE_BINARY_DIR}/Freax.iso
        ${CMAKE_BINARY_DIR}/boot/
    COMMAND ${CMAKE_SOURCE_DIR}/limine/limine-deploy ${CMAKE_BINARY_DIR}/Freax.iso
    COMMENT "Creating bootable Limine ISO"
)

# Add a custom target for running in QEMU
add_custom_target(run
    COMMAND qemu-system-x86_64
        -M q35
        -m 2G
        -cdrom ${CMAKE_BINARY_DIR}/Freax.iso
        -boot d
    DEPENDS freax.elf
    COMMENT "Running Freax in QEMU"
)
