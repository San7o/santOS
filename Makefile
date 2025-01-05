#====================================================================#
#                                                                    #
#                          ROOT MAKEFILE                             #
#                                                                    #
# ------------------------------------------------------------------ #
#                                                                    #
# This makefile is the central place for the kernel build system,    #
# from here all the subprojects are built. The build system can      #
# be configured from the `root/config.sh` file, you should source    #
# it before trying to compile. Usually, you can just run this to     #
# build everything you need:                                         #
# ```                                                                #
# source config.sh                                                   #
# make                                                               #
# ```                                                                #
# Please refer to the official documentation for necessary           #
# dependencies and additional information.                           #
#                                                                    #
#====================================================================#


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
	@for PROJECT in $(PROJECTS); do \
	  NAME=$$(echo $$PROJECT | tr '[:lower:]' '[:upper:]'); \
          echo "* INSTALLING $$NAME..."; \
	  $(MAKE) -C $$PROJECT install; \
	  $(call print_banner) \
	done

iso:
	@echo "* CREATING ISO..."
	mkdir $(SYSROOT)/boot/grub || :
	cp grub.cfg $(SYSROOT)/boot/grub/
	$(GRUB_DIR)/grub-mkrescue -o $(ISO_OUTPUT_NAME) sysroot
	@$(call print_banner)

qemu: iso
	@echo "* LAUNCHING QEMU..."
	$(QEMU_DIR)/qemu-system-i386 -cdrom myos.iso

clean:
	@echo "* CLEANING PROJECT..."
	make -C kernel clean
	make -C libc clean
	rm $(ISO_OUTPUT_NAME) 2>/dev/null || :
	rm -rf $(SYSROOT) 2>/dev/null || :
	@$(call print_banner)

help:
	@$(call print_banner)
	@echo "Welcome to the kernel build system. This makefile is the"
	@echo "central place were all the subsystems are built."
	@echo "Before building anything, you should take a look at"
	@echo "config.sh. This file contains the configuration options"
	@echo "that will be used throughout this build, you should make"
	@echo "all the necessary modification there."
	@echo ""
	@echo "To build the project that you selected, you can simply"
	@echo "run \"source config.sh\" and then \"make\"."
	@echo ""
	@echo "To create a bootable iso, run \"make iso\" after the"
	@echo "first make. This will genearte an iso file that can be".
	@echo "booted in BIOS mode."
	@echo "If you have qemu for the target architecture, you can"
	@echo "boot the kernel with \"make qemu\"."
	@$(call print_banner)

#
# End of Makefile
#====================================================================#
