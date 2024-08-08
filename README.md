# santOS

This project aims to build a modern and fast operating system for the x86 architecture.
This is a long term project, I'll record my progress on my [youtube channel](https://www.youtube.com/@giovannis-diary).

## Roadmap

- [x] Boot the kernel, [link](https://wiki.osdev.org/Bare_Bones)
  - use pixed buffers instead of VGA

- [ ] kernel skeleton, [link](https://wiki.osdev.org/Meaty_Skeleton)

- [ ] Memory Management
  - Implement [paging](https://wiki.osdev.org/Paging) 
  - Develop a basic memory allocator (e.g., buddy system)

- [ ] Interrupt Handling
  - Set up the Interrupt Descriptor Table (IDT)
  - Implement basic interrupt handlers (e.g., keyboard input)

- [ ] Task Management
  - Create a basic scheduler
  - Implement process/thread management

- [ ] File System Support
  - Develop or integrate a simple file system (e.g., FAT12/16/32)

- [ ] System Calls
  - Define and implement system calls for user-space interaction

- [ ] Device Drivers
  - Write drivers for essential hardware components (e.g., disk drives, network cards)

- [ ] Basic User Interface
  - Develop a command-line interface (CLI) or shell for user interaction

- [ ] GUI ???


## Cross compiler

To compile the kernel, you will need to build cross compiler. This is because
your compiler (gcc) was built for linux, and we are not using linux to run
the program. This means you will have to compile your own binutils and gcc
version targeting `i686-elf`. Follow [this](https://wiki.osdev.org/GCC_Cross-Compiler)
guide carefully in order to achieve this.

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
