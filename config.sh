SYSTEM_HEADER_PROJECTS="libc kernel"
PROJECTS="libc kernel"

# Program locations
AR_DIR=~/opt/cross/bin
AS_DIR=~/opt/cross/bin
CC_DIR=~/opt/cross/bin
export QEMU_DIR=/usr/bin
export GRUB_DIR=~/opt/bin

export MAKE=${MAKE:-make}
export HOST=i686-elf

export AR=${AR_DIR}/${HOST}-ar
export AS=${AS_DIR}/${HOST}-as
export CC=${CC_DIR}/${HOST}-gcc

export PREFIX=$(pwd)/sysroot
export EXEC_PREFIX=$PREFIX
export BOOTDIR=$PREFIX/boot
export LIBDIR=$EXEC_PREFIX/lib
export INCLUDEDIR=$PREFIX/include

export CFLAGS='-O2 -g'
export CPPFLAGS=''

# Configure the cross-compiler to use the desired system root.
export SYSROOT="$(pwd)/sysroot"
export CC="$CC --sysroot=$SYSROOT"

# Work around that the -elf gcc targets doesn't have a system include directory
# because it was configured with --without-headers rather than --with-sysroot.
if echo "$HOST" | grep -Eq -- '-elf($|-)'; then
  export CC="$CC -isystem $INCLUDEDIR"
fi
