#!/bin/sh

# This is an helper tool to build the kernel in different ways and
# to provide additional functionalities.

ISO_OUTPUT_NAME=myos.iso

function build
{
  banner "Building..."

  mkdir -p "$SYSROOT"

  # Install headers first
  for PROJECT in $SYSTEM_HEADER_PROJECTS; do
    (cd $PROJECT && $MAKE install-headers)
  done

  for PROJECT in $PROJECTS; do
    (cd $PROJECT && $MAKE install)
  done
  return
}

function clean
{
  banner "Cleaning..."

  for PROJECT in $PROJECTS; do
    echo "Cleaning $PROJECT..."
    echo "Make is $MAKE"
    (cd $PROJECT && $MAKE clean)
  done

  rm -rf $SYSROOT 2>/dev/null || :
  rm $ISO_OUTPUT_NAME 2>/dev/null || :
  return
}

function iso
{
  banner "Creating the iso..."

  mkdir $SYSROOT/boot/grub || :
  cp grub.cfg $SYSROOT/boot/grub/
  $GRUB_DIR/grub-mkrescue -o $ISO_OUTPUT_NAME sysroot
  return
}

function qemu
{
  banner "Launching qemu..."
  $QEMU_DIR/qemu-system-i386 -cdrom myos.iso
  return
}

function banner
{
  echo "=========================================="
  echo "$1"
  echo "=========================================="
}

function help
{
  echo "Usage: $0 [build|clean|iso|qemu|help]"
  return
}

if [ $# -eq 0 ]; then
  help
  exit
fi

# Load the config environment
set -e 
. ./config.sh

# Parse args
if [ "$1" == "build" ]; then
  build
  exit
fi
if [ "$1" == "clean" ]; then
  clean
  exit
fi
if [ "$1" == "iso" ]; then
  build
  iso
  exit
fi
if [ "$1" == "qemu" ]; then
  build
  iso
  qemu
  exit
fi
if [ "$1" == "help" ]; then
  help
  exit
fi
