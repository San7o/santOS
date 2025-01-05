BUILD_DIR   = build
OUTPUT_NAME = myos.bin
ISO_NAME    = myos.iso
AS          = ~/opt/cross/bin/i686-elf-as
CC          = ~/opt/cross/bin/i686-elf-gcc

ARCH_DIR    = arch
ARCH        = i386

KERNEL_DIR  = kernel

INCLUDE_DIR = include

# Libc
LIBC_DIR = libc

LIBC_SRCS=\
$(LIBC_DIR)/string/strlen.c

LIBC_OBJS=\
$(LIB_DIR)/string/strlen.o

# Kernel
KERNEL_DIR= kernel

KERNEL_SRCS=\
$(KERNEL_DIR)/kernel.c

KERNEL_OBJS=\
$(KERNEL_DIR)/kernel.o

.PHONY: clean all kernel boot qemu build

all:
	make kernel
	make boot
	make qemu

.c.o: 

kernel: 
	mkdir -p $(BUILD_DIR)
	$(AS) $(ARCH_DIR)/$(ARCH)/boot.s -o $(BUILD_DIR)/boot.o
	$(CC) -c $(KERNEL_DIR)/kernel.c -o $(BUILD_DIR)/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I $(INCLUDE_DIR)
	$(CC) -T $(ARCH_DIR)/$(ARCH)/linker.ld -o $(BUILD_DIR)/$(OUTPUT_NAME) -ffreestanding -O2 -nostdlib $(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel.o -lgcc -I $(INCLUDE_DIR)
	$(GRUB_DIR)/grub-file --is-x86-multiboot $(BUILD_DIR)/$(OUTPUT_NAME)

boot:
	rm -rf $(BUILD_DIR)/$(ISO_NAME)
	mkdir -p $(BUILD_DIR)/boot/grub
	cp $(BUILD_DIR)/$(OUTPUT_NAME) $(BUILD_DIR)/boot/
	cp grub.cfg $(BUILD_DIR)/boot/grub/
	$(GRUB_DIR)/grub-mkrescue -o $(BUILD_DIR)/$(ISO_NAME) $(BUILD_DIR)/

qemu:
	$(QEMU_DIR)/qemu-system-i386 -cdrom $(BUILD_DIR)/myos.iso

clean:
	rm -rf $(BUILD_DIR)/
