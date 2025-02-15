CC := clang
OBJCOPY := llvm-objcopy
QEMU := qemu-system-riscv32

CFLAGS := -std=c11 -O2 -g3 -Wall -Wextra --target=riscv32-unknown-elf \
          -fno-stack-protector -ffreestanding -nostdlib

# Output files
OUTPUTS := shell.elf shell.bin shell.bin.o kernel.elf disk.tar

.PHONY: all clean run
.DEFAULT_GOAL := all

all: kernel.elf disk.tar

# Pattern rule for object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

shell.elf: shell.o user.o common.o user.ld
	$(CC) $(CFLAGS) -Wl,-Tuser.ld -Wl,-Map=shell.map -o $@ shell.o user.o common.o

shell.bin: shell.elf
	$(OBJCOPY) --set-section-flags .bss=alloc,contents -O binary $< $@

shell.bin.o: shell.bin
	$(OBJCOPY) -Ibinary -Oelf32-littleriscv $< $@

kernel.elf: kernel.o common.o shell.bin.o kernel.ld
	$(CC) $(CFLAGS) -Wl,-Tkernel.ld -Wl,-Map=kernel.map -o $@ kernel.o common.o shell.bin.o

disk.tar:
	cd disk && tar cf ../disk.tar --format=ustar *.txt

run: kernel.elf disk.tar
	$(QEMU) -machine virt -bios default -nographic -serial mon:stdio --no-reboot \
		-d unimp,guest_errors,int,cpu_reset -D qemu.log \
		-drive id=drive0,file=disk.tar,format=raw,if=none \
		-device virtio-blk-device,drive=drive0,bus=virtio-mmio-bus.0 \
		-kernel kernel.elf

clean:
	rm -f *.o $(OUTPUTS) *.map qemu.log

# Dependency tracking
shell.o: shell.c
user.o: user.c
common.o: common.c
kernel.o: kernel.c
