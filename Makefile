# General settings
DEFAULT_HOST    = i686
HOST           ?= $(DEFAULT_HOST)
HOST_ARCH       = i386

# Folder and names
BUILD_DIR       = build/
ARCH_DIR        = arch/$(HOST_ARCH)/
INCLUDE_DIR     = usr/include/
KERNEL_BIN_NAME = santos.bin
KERNEL_IMG_NAME = santos.iso

# Compilers
AS              = ~/opt/cross/bin/$(HOST)-elf-as
CC              = ~/opt/cross/bin/$(HOST)-elf-gcc

# Default Flags (of not specified)
CFLAGS         ?= -O2 -Wall -Wextra
LIBS           ?=
LDFLAGS        ?=

# Flags
CFLAGS         := $(CFLAGS) -ffreestanding -std=gnu99
LIBS           := $(LIBS) -nostdlib -lgcc

.PHONY: boot qemu clean
.SUFFIXES: .c .o .S

# This defines KERNEL_ARCH_* variables
include $(ARCH_DIR)/make.config

# Kernel objects
KERNEL_OBJS=\
$(KERNEL_ARCH_OBJS) \
kernel/kernel.o \

# General objects
OBJS=\
$(ARCH_DIR)/crti.o \
$(ARCH_DIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(ARCH_DIR)/crtend.o \
$(ARCH_DIR)/crtn.o \

# Link list
LINK_LIST=\
$(LDFLAGS) \
$(ARCH_DIR)/crti.o \
$(ARCH_DIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(LIBS) \
$(ARCH_DIR)/crtend.o \
$(ARCH_DIR)/crtn.o \

CFLAGS  += $(KERNEL_ARCH_CFLAGS)
LDFLAGS += $(KERNEL_ARCH_LDFLAGS)
LIBS    += $(KERNEL_ARCH_LIBS)

# Targets

all:
	make install
	make boot
	make qemu

# Build the kernel
$(KERNEL_BIN_NAME): $(OBJS) $(ARCH_DIR)/linker.ld
	$(CC) -T $(ARCH_DIR)linker.ld -o $(KERNEL_BIN_NAME) $(CFLAGS) $(LINK_LIST)
	grub-file --is-x86-multiboot $(KERNEL_BIN_NAME)

# Copy the crt files
$(ARCH_DIR)/crtbegin.o $(ARCH_DIR)/crtend.o:
	OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

# Compile C into object files
.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS)

# Compile ASM into object files
.S.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS)

install: install-headers install-kernel

install-headers:
	mkdir -p $(BUILD_DIR)$(INCLUDE_DIR)
	cp -R --preserve=timestamps include/. $(BUILD_DIR)$(INCLUDE_DIR)

install-kernel: $(KERNEL_BIN_NAME)
	mkdir -p $(BUILD_DIR)boot
	mv $(KERNEL_BIN_NAME) $(BUILD_DIR)boot/

boot:
	rm -rf $(BUILD_DIR)$(KERNEL_IMG_NAME)
	mkdir -p $(BUILD_DIR)/boot/grub
	cp grub.cfg $(BUILD_DIR)boot/grub/
	grub-mkrescue -o $(BUILD_DIR)$(KERNEL_IMG_NAME) $(BUILD_DIR)/

qemu:
	qemu-system-i386 -cdrom $(BUILD_DIR)$(KERNEL_IMG_NAME)

clean:
	rm -rf $(BUILD_DIR)
