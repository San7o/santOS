# How to build the OS

To build the OS you will need the following:
- A C crosscompiler toolchain for your target architecture (currently
  only i386 is supported). To build the cross compiler from source
  please read the instructions [here](https://wiki.osdev.org/GCC_Cross-Compiler).
- grub with BIOS support (you may have to recompile It from source if
  your system only supports UEFI)
- qemu to run the OS
- make

To build the OS, first you need to `source config.sh`. This file
exports all the required configuration, you can change It as you
require. After this, you can call `make` to build everything,
or `make help` to get more informations.
```bash
source config.sh
make help
```

