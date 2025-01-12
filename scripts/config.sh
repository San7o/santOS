#====================================================================#
# From this file you will be able to change the configuration of
# the kernel build.
# To activate the configuration, run:
# ```
# source config.sh
# ```


# Projects to compile
# Note that the order of the projects will be the order in which they
# will be built, from left to right.
export SYSTEM_HEADER_PROJECTS="libktest libc kernel"
export PROJECTS="libktest libc kernel"

# Do tests
export TEST=True

# Program locations
AR_DIR=~/opt/cross/bin
AS_DIR=~/opt/cross/bin
CC_DIR=~/opt/cross/bin
export QEMU_DIR=/usr/bin
export GRUB_DIR=~/opt/bin

# Target
export HOST=i686-elf
export ARCH=i386                             # The target architecture

# Compilers
export MAKE=${MAKE:-make}
export AR=${AR_DIR}/${HOST}-ar
export AS=${AS_DIR}/${HOST}-as
export CC=${CC_DIR}/${HOST}-gcc

# Compiler Flags
export CFLAGS='-O2 -g -Wall -Wextra -Werror -Wpedantic -Wno-pointer-arith'

# Output directories
export PREFIX=$(pwd)/sysroot
export EXEC_PREFIX=$PREFIX
export BOOTDIR=$PREFIX/boot
export LIBDIR=$EXEC_PREFIX/lib
export INCLUDEDIR=$PREFIX/include

# Output names
export ISO_OUTPUT_NAME=myos.iso

# Configure the cross-compiler to use the desired system root.
export SYSROOT="$(pwd)/sysroot"
export CC="$CC --sysroot=$SYSROOT"

# Work around that the -elf gcc targets doesn't have a system include
# directory because it was configured with --without-headers rather
# than --with-sysroot.
if echo "$HOST" | grep -Eq -- '-elf($|-)'; then
  export CC="$CC -isystem $INCLUDEDIR"
fi


#
# End of config.sh
#====================================================================#
