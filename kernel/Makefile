#====================================================================#
#                                                                    #
#                          KERNEL MAKEFILE                           #
#                                                                    #
# -------------------------------------------------------------------#
#                                                                    #
# This Makefile is responsible for building the main kernel          #
# executable, all the files in the `root/kernel` directory will be   #
# compiled here.                                                     #
# Additionally, the following commands are provided:                 #
# - install-kernel: installs the kernel in $DESTDIR                  #
# - install-headers: copies the headers inside $DESTDIR/$INCLUDEDIR  #
# - install: combines the previous two                               #
# - clean: cleans the build files                                    #
#                                                                    #
# Note that you should run the `root/config.sh` script before        #
# building anything.                                                 #
#                                                                    #
#====================================================================#


#====================================================================#
# VARIABLES
#

# Target
ARCH?=i386

# Input directories
ARCHDIR=arch/$(ARCH)

# Output Directories
DESTDIR?=
PREFIX?=build/
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot       # The kernel will be installed here
INCLUDEDIR?=$(PREFIX)/include         # Headers will be installed here

# Subprojects
include $(ARCHDIR)/make.config

# Compiler Flags
CFLAGS?=-O2 -g
LDFLAGS?=
LIBS?=
CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra -Werror \
        -Wpedantic
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -L$(PREFIX)/lib -lk -lgcc    # -lk is the libk

CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)


#====================================================================#
# OBJECT FILES
#

KERNEL_OBJS=\
$(KERNEL_ARCH_OBJS) \
kernel/kernel.o \

OBJS=\
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \

LINK_LIST=\
$(LDFLAGS) \
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(LIBS) \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \


#====================================================================#
# BUILDING
#

.PHONY: all clean install install-headers install-kernel
.SUFFIXES: .o .c .S

all: myos.kernel

myos.kernel: $(OBJS) $(ARCHDIR)/linker.ld
	$(CC) -T $(ARCHDIR)/linker.ld -o $@ $(CFLAGS) $(LINK_LIST)
	grub-file --is-x86-multiboot myos.kernel

$(ARCHDIR)/crtbegin.o $(ARCHDIR)/crtend.o:
	OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` \
        && cp "$$OBJ" $@

.c.o:
	@echo " * [CC] $@"
	@$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS)

.S.o:
	@echo " * [CC] $@"
	@$(CC) -MD -c $< -o $@ $(CFLAGS)


#====================================================================#
# ADDITIONAL COMMANDS
#

clean:
	rm -f myos.kernel
	rm -f $(OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-kernel

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-kernel: myos.kernel
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp myos.kernel $(DESTDIR)$(BOOTDIR)

-include $(OBJS:.o=.d)


#
# End of Makefile
#====================================================================#
