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

# Note that the order of the projects will be the order in which they
# will be built, from left to right.
SYSTEM_HEADER_PROJECTS?=libktest libc kernel # headers to install
PROJECTS?=libktest libc kernel # projects to compile

TEST?=True # Do tests

# Program locations
AR_DIR?=~/opt/cross/bin # ar executable path
AS_DIR?=~/opt/cross/bin # as executable path
CC_DIR?=~/opt/cross/bin # cc executable path
QEMU_DIR?=/usr/bin # qemu executable path
GRUB_DIR?=~/opt/bin # grub executable path

HOST?=i686-elf # host
ARCH?=i386 # target architecture

SYSROOT?=$(shell pwd)/sysroot # system root

CFLAGS?=-O2 -g -Wall -Wextra -Werror -Wpedantic # compiler flags

# Output directories
PREFIX?=$(shell pwd)/sysroot # outpu prefix
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(PREFIX)/boot
LIBDIR?=$(EXEC_PREFIX)/lib
INCLUDEDIR?=$(PREFIX)/include

# Compilers
MAKE?=make # make program
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
ISO_OUTPUT_NAME?=myos.iso # iso output name

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

.PHONY: all iso qemu clean docs help

# Builds the selected projects
all: # default, builds the selected projects
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

config: # generate config.sh used to configure the build system
	@cp scripts/config.sh config.sh
	@echo "config.sh created in $(shell pwd)"

iso: # generate an iso after the kernel is built
iso:
	@echo " * CREATING ISO..."
	@mkdir $(SYSROOT)/boot/grub || :
	@cp boot/grub.cfg $(SYSROOT)/boot/grub/ || :
	@$(GRUB_DIR)/grub-mkrescue -o $(ISO_OUTPUT_NAME) sysroot
	@$(call print_banner)

# Some notes on qemu usage:
# "-serial stdio -serial tcp::4444,server" will send UART 0 to your
# terminal and connect UART 1 to a TCP server on port 4444 which you
# can then connect to with netcat or similar utility. (Peter Maydell)
qemu: # run qemu emulator to test the kernel
qemu: iso
	@echo " * LAUNCHING QEMU..."
	@$(QEMU_DIR)/qemu-system-i386 \
               -cdrom $(ISO_OUTPUT_NAME) \
               -serial mon:stdio \
               -d int,exec,pcall,cpu_reset \
               -D /tmp/qemu.log

clean: # clean the build files
	@echo " * CLEANING PROJECT..."
	@for PROJECT in $(PROJECTS); do \
	  env $(foreach var, $(SUB_MAKE_VARIABLES),$(var)="$($(var))") \
            $(MAKE) -C $$PROJECT clean; \
	  $(call print_banner) \
	done
	@rm $(ISO_OUTPUT_NAME) 2>/dev/null || :
	@rm -rf $(SYSROOT) 2>/dev/null || :

docs: # generate html documentation
	@doxygen doxygen.conf

# thanks to Julio Merino
help: # show help message
	@$(call print_banner)
	@echo -e "* Kernel Build System\n"
	@echo -e "make variables:\n"
	@sed -e 's/^\([^ 	]\+\)[ 	]*?=[^#]\+#\(.*\)$$/\1 \2/p;d' Makefile | column -t -l 2 | sort
	@echo
	@echo "make targets:"
	@echo
	@sed -e's/^\([^: 	]\+\):.*#\(.*\)$$/\1 \2/p;d' Makefile | column -t -l 2 | sort
	@$(call print_banner)

#
# End of Makefile
#====================================================================#
