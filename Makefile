BUILD_DIR   = build
OUTPUT_NAME = myos.bin
ISO_NAME    = myos.iso
AS  = ~/opt/cross/bin/i686-elf-as
GCC = ~/opt/cross/bin/i686-elf-gcc

all:
	make kernel
	make boot
	make qemu

kernel:
	mkdir -p $(BUILD_DIR)
	$(AS) boot.s -o $(BUILD_DIR)/boot.o
	$(GCC) -c kernel.c -o $(BUILD_DIR)/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	$(GCC) -T linker.ld -o $(BUILD_DIR)/$(OUTPUT_NAME) -ffreestanding -O2 -nostdlib $(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel.o -lgcc
	grub-file --is-x86-multiboot $(BUILD_DIR)/$(OUTPUT_NAME)

boot:
	rm -rf $(BUILD_DIR)/$(ISO_NAME)
	mkdir -p $(BUILD_DIR)/boot/grub
	cp $(BUILD_DIR)/$(OUTPUT_NAME) $(BUILD_DIR)/boot/
	cp grub.cfg $(BUILD_DIR)/boot/grub/
	grub-mkrescue -o $(BUILD_DIR)/$(ISO_NAME) $(BUILD_DIR)/

qemu:
	qemu-system-i386 -cdrom $(BUILD_DIR)/myos.iso

clean:
	rm -rf $(BUILD_DIR)/
