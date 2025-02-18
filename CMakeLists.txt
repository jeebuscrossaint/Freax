cmake_minimum_required(VERSION 3.31.5)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/toolchain.cmake CACHE STRING "Toolchain file")
project(Freax LANGUAGES CXX C ASM)

add_compile_options(
    -ffreestanding # No libc
    -fno-builtin # fuck no builtins?
    -nostdlib
    -mcmodel=kernel
    -mno-red-zone
    -fno-exceptions
    -fno-rtti
    -Wall -Wextra -Werror # Enable warrings with strictness
    --target=x86_64-elf # clang target
)

add_executable(freax.elf
  kernel/main.cc # add more sources when necessary
)

set_target_properties(freax.elf PROPERTIES
  LINK_FLAGS "-fuse-ld=lld -T ${CMAKE_SOURCE_DIR}/linker.ld"
)

add_Custom_command(TARGET freax.elf POST_BUILD
  COMMAND llvm-objcopy --strip-unneeded -O elf64-x86-64 freax.elf boot/freax.elf
  COMMAND cp boot/limine.cfg boot/
  COMMAND cp ${CMAKE_SOURCE_DIR}/limine* boot/
  COMMAND xorriso -as mkisofs -b limine-bios.sys -no-emul-boot -boot-load-size 4 -boot-info-table -o Freax.iso boot/
  COMMAND ./limine/limine-deploy Freax.iso
  COMMENT "Creating bootable Limine ISO"
)
