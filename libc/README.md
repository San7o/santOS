# libc

This directory contains an implementation of the glibc standard library.
The library is compiled seperately from the kernel, and comes in two
versions:
- `libk`: freestanding library, that it, It does not require syscalls
  and It deviates from the standard. Most of the functions that could
  be reused multiple times in the kernel will live here.
- `libc`: host library which uses system calls and implements the
  standard library.

## Conventions

This directory contains a separate directory for each header file (such
as stdio, stirng...) and each function is implemented on Its own file
unless It really does not make sense.

The name of `.c` files can end in either:
- `_kernel.c`: for libk sources
- `_user.c`: for libc sources
- `_common.c`: for both

Each directory has Its own `make.config` and each source file will be
added to this in the corresponding field. The `make.config` will be
imported and used my the make located in the `libc` directory.

All the code that is architecture dependent should live in `arch`.
The build system will compile only the architecture selected when
building. Note that the headers in `arch/$(ARCH)/include` will be
available when building.
