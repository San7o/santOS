# santOS

This project aims to build an operating system supporting the i386
architecture.

## Roadmap

- [x] setup and build system
- [x] printk
- [x] access multiboot info
- [x] testing infrastructure
- [x] output to a serial port for debugging purposes
- [ ] having a working interrupt/exception handling system that can dump
      the contents of the registers
- [ ] plan the memory map
- [ ] malloc and free

# Compile the kernel

Please read the instruction is [BUILD](./BUILD.md) to continue.

## Tests

The kernel has a test framework, you can find more information in
[TESTING](./TESTING.md).
