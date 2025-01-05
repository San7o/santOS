#!/bin/sh

# This is an helper tool to build the kernel
# in different ways and to provide additional
# functionalities.

function build
{
  # Load the config environment
  set -e 
  . ./config.sh

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

function iso
{
  mkdir $SYSROOT/boot/grub || :
  cp grub.cfg $SYSROOT/boot/grub/
  $GRUB_DIR/grub-mkrescue -o myos.iso sysroot
  return
}

function qemu
{
  $QEMU_DIR/qemu-system-i386 -cdrom myos.iso
  return
}

function help
{
  echo "Usage: $0 [build|iso|qemu|help]"
  return
}

if [ $? -eq 1 ]; then
  help
  exit
fi

if [ "$1" == "build" ]; then
  build
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
