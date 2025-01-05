# santOS

This project aims to build a modern and fast operating system for
the i686 architecture.

## Roadmap

The OSdev wiki suggests to implement the following first:
- printk
- output to a serial port for debugging purposes
- having a working interrupt/exception handling system that can dump
  the contents of the registers
- plan your memory map
- malloc and free

## Cross compiler

To compile the kernel, you will need to build cross compiler that can
compile for i686 architecture. Follow [this](https://wiki.osdev.org/GCC_Cross-Compiler)
guide carefully in order to install it.

# Compile the project

You can compile and run everytinh just by running:
```bash
make
```

Or you can compile different parts with the following commands:
```bash
make kernel   # kernel
make boot     # create boot image with grub
make qemu     # run qemu
```
