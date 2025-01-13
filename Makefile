#====================================================================#
#                                                                    #
#                          ROOT MAKEFILE                             #
#                                                                    #
# ------------------------------------------------------------------ #
#                                                                    #
# This makefile is the central place for the kernel build system,    #
# from here all the subprojects are built. The build system can      #
# be configured from config.sh file, generated with "make config". Y #
# You should source it before trying to compile. Usually, you can    #
# just run this to build everything you need:                        #
# ```                                                                #
# source config.sh                                                   #
# make                                                               #
# ```                                                                #
# Please refer to the official documentation for necessary           #
# dependencies and additional information.                           #
#                                                                    #
#====================================================================#
#====================================================================#
# DEFAULT VARIABLES
#
# You can set those variables in config.sh and source it with
# "source config.sh" before running this makefile. The following
# variables are the default values in case you forgot to source the
# config file, to ensure that you don't install everything in
# random places in your system.
#

# Projects to compile
# Note that the order of the projects will be the order in which they
# will be built, from left to right.
SYSTEM_HEADER_PROJECTS?=libktest libc kernel
PROJECTS?=libktest libc kernel

# Do tests
TEST?=True

# Program locations
AR_DIR?=~/opt/cross/bin
AS_DIR?=~/opt/cross/bin
CC_DIR?=~/opt/cross/bin
QEMU_DIR?=/usr/bin
GRUB_DIR?=~/opt/bin

# Target
HOST?=i686-elf
# The target architecture
ARCH?=i386

# Configure the cross-compiler to use the desired system root.
SYSROOT?=$(shell pwd)/sysroot

# Compiler Flags
CFLAGS?=-O2 -g -Wall -Wextra -Werror -Wpedantic

# Output directories
PREFIX?=$(shell pwd)/sysroot
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(PREFIX)/boot
LIBDIR?=$(EXEC_PREFIX)/lib
INCLUDEDIR?=$(PREFIX)/include

# Compilers
MAKE?=make
# Override the default one
ifeq ($(AR),ar)
AR=$(AR_DIR)/$(HOST)-ar
endif
ifeq ($(AS),as)
AS=$(AS_DIR)/$(HOST)-as
endif
ifeq ($(CC),cc)
CC=$(CC_DIR)/$(HOST)-gcc  --sysroot=$(SYSROOT) -isystem $(INCLUDEDIR)
endif

# We need this to pass those variables to sub-makes
override AR:=$(AR)
override AS:=$(AS)
override CC:=$(CC)

# Output names
ISO_OUTPUT_NAME?=myos.iso

SUB_MAKE_VARIABLES:=QEMU_DIR GRUB_DIR ARCH SYSROOT MAKE AR AS CC \
CFLAGS PREFIX EXEC_PREFIX BOOTDIR LIBDIR INCLUDEDIR TEST


#====================================================================#
# FUNCTIONS
#

define print_banner
  echo "=======================================================";
endef


#====================================================================#
# COMMANDS
#

.PHONY: all iso qemu clean help

# Builds the selected projects
all:
	@for PROJECT in $(SYSTEM_HEADER_PROJECTS); do \
	  NAME=$$(echo $$PROJECT | tr '[:lower:]' '[:upper:]'); \
          echo " * INSTALLING HEADERS FOR $$NAME..."; \
	  env $(foreach var, $(SUB_MAKE_VARIABLES),$(var)="$($(var))") \
            $(MAKE) -C $$PROJECT install-headers; \
	  $(call print_banner) \
	done
	@for PROJECT in $(PROJECTS); do \
	  NAME=$$(echo $$PROJECT | tr '[:lower:]' '[:upper:]'); \
          echo " * INSTALLING $$NAME..."; \
	  env $(foreach var, $(SUB_MAKE_VARIABLES),$(var)="$($(var))") \
            $(MAKE) -C $$PROJECT install; \
	  $(call print_banner) \
	done

config:
	@cp scripts/config.sh config.sh
	@echo "config.sh created in $(shell pwd)"

iso:
	@echo " * CREATING ISO..."
	@mkdir $(SYSROOT)/boot/grub || :
	@cp boot/grub.cfg $(SYSROOT)/boot/grub/
	@$(GRUB_DIR)/grub-mkrescue -o $(ISO_OUTPUT_NAME) sysroot
	@$(call print_banner)

# Some notes on qemu usage:
# "-serial stdio -serial tcp::4444,server" will send UART 0 to your
# terminal and connect UART 1 to a TCP server on port 4444 which you
# can then connect to with netcat or similar utility. (Peter Maydell)
qemu: iso
	@echo " * LAUNCHING QEMU..."
	@$(QEMU_DIR)/qemu-system-i386 -cdrom $(ISO_OUTPUT_NAME) -serial stdio

clean:
	@echo " * CLEANING PROJECT..."
	@for PROJECT in $(PROJECTS); do \
	  env $(foreach var, $(SUB_MAKE_VARIABLES),$(var)="$($(var))") \
            $(MAKE) -C $$PROJECT clean; \
	  $(call print_banner) \
	done
	@rm $(ISO_OUTPUT_NAME) 2>/dev/null || :
	@rm -rf $(SYSROOT) 2>/dev/null || :

help:
	@$(call print_banner)
	@cat help.txt
	@$(call print_banner)

#
# End of Makefile
#====================================================================#
