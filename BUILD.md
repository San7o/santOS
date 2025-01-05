# How to build the OS

To build the OS you will need the following:
- A C crosscompiler toolchain for your target architecture (currently
  only i386 is supported). To build the cross compiler from source
  please read the instructions [here](https://wiki.osdev.org/GCC_Cross-Compiler).
- grub with BIOS support (you may have to recompile It from source if
  your system only supports UEFI)
- qemu to run the OS
- make

To build the OS you can use the handy [build.sh](./build.sh) bash
script wich provides useful functionalities.
```bash
bash> ./build.sh help
Usage: ./build.sh [build|iso|qemu|help]
```

