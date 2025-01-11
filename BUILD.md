# Kernel Build System

Welcome to the Kernel Build System. The kernel uses Makefiles to build
and is divided into multiple projects.
So far, the projects include:
- kernel
- libc
- test

## Dependencies

To build the OS you will need the following:
- A C crosscompiler toolchain for your target architecture (currently
  only i386 is supported). To build the cross compiler from source
  please read the instructions [here](https://wiki.osdev.org/GCC_Cross-Compiler).
- grub with BIOS support (you may have to recompile It from source if
  your system only supports UEFI)
- qemu to run the OS
- make

## config.sh

Before building anything, you should create your config.sh. This
file contains the configuration options that will be used throughout
this build, you should make all the necessary modification there.

```bash
make config
```

The config file will be generated in the local directory, you will
find descriptions of the options in the file itself.

## Build

To build the project that you selected, you can simply run:

```bash
source config.sh
make
```

## ISO

To create a bootable iso, run "make iso" after the" first make. This
will genearte an iso file that can be booted in BIOS mode.

```bash
make iso
```

## Qemu

If you have qemu for the target architecture, you can boot the kernel
with "make qemu"."
```bash
make qemu
```
