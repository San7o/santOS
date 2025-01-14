# Kernel Code

This directory contains the kernel and Its subsystems code. All the
architecture dependent code resides in `arch/$(ARCH)/`. You may find
architecture dependent counterparts of the main kernel directories,
for example there may exists a `boot/` directory and a `arch/$(ARCH)/boot`
counterpart that implements some architecture dependent code.

The headers present in `arch/$(ARCH)/include` will be installed together
with all the other headers. You can set the `ARCH` variable in your
environment, anyway you should source the `config.sh` file generated
from `make config` in the root directory before doing anything.

