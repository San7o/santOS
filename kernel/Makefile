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
# Note that you should source the config.sh script before building   #
# anything.                                                          #
#                                                                    #
#====================================================================#
#====================================================================#
# VARIABLES
#

# General
OUT_NAME=myos.kernel
TEST?=True

# Target
ARCH?=i386

# Input directories
ARCHDIR=arch/$(ARCH)
LINKER_SCRIPT=$(ARCHDIR)/linker.ld

# Output Directories
DESTDIR?=
PREFIX?=build/
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot       # The kernel will be installed here
INCLUDEDIR?=$(PREFIX)/include         # Headers will be installed here

# Subprojects
SUBPROJECTS_MAKE_CONFIG=\
$(ARCHDIR)/make.config \
kernel/make.config \
boot/make.config

include $(SUBPROJECTS_MAKE_CONFIG)

# Compiler Flags
CFLAGS?=-O2 -g -Wall -Wextra -Wpedantic \
        -Werror -Wno-pointer-arith
LDFLAGS?=
LIBS?=
CFLAGS:=$(CFLAGS) -ffreestanding -nostdlib
LDFLAGS:=$(LDFLAGS)
# The order here is important, we need archives that have test
# symbols to be linked with --whole-archive
LIBS:=$(LIBS) -nostdlib -L$(PREFIX)/lib -lgcc \
                -Wl,--whole-archive -lk -Wl,--no-whole-archive
ifeq ($(TEST),True)
LIBS+=-lktest
endif

CFLAGS:=$(CFLAGS) $(KERNEL_CFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_LDFLAGS)
LIBS:=$(LIBS) $(KERNEL_LIBS)

ifeq ($(TEST),True)
CFLAGS+=-D KERNEL_TEST
endif


#====================================================================#
# OBJECT FILES
#

OBJS=\
$(KERNEL_OBJS)

ifeq ($(TEST),True)
OBJS+=$(TESTS_OBJS)
endif

LINK_LIST=\
$(LDFLAGS) \
$(OBJS) \
$(LIBS)


#====================================================================#
# BUILDING
#

.PHONY: all clean install install-headers install-kernel
.SUFFIXES: .o .c .S

all: $(OUT_NAME)

$(OUT_NAME): $(OBJS) $(LINKER_SCRIPT) $(SYSROOT)/lib/libk.a
	@echo " * [LD] $@"
	@$(CC) -T $(LINKER_SCRIPT) -o $@ $(CFLAGS) $(LINK_LIST)
	@grub-file --is-x86-multiboot $(OUT_NAME)

$(ARCHDIR)/crtbegin.o $(ARCHDIR)/crtend.o:
	@echo " * [CC] crtbegin.o and crtend.o"
	@OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` \
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
	@echo " * Cleaning kernel build files..."
	@rm -f $(OUT_NAME)
	@rm -f $(OBJS) $(TEST_OBJS) *.o */*.o */*/*.o
	@rm -f $(OBJS:.o=.d) $(TEST_OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-kernel

install-headers:
	@echo " * Installing headers..."
	@mkdir -p $(DESTDIR)$(INCLUDEDIR)
	@cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-kernel: $(OUT_NAME)
	@echo " * Installing the kernel..."
	@mkdir -p $(DESTDIR)$(BOOTDIR)
	@cp $(OUT_NAME) $(DESTDIR)$(BOOTDIR)

-include $(OBJS:.o=.d)


#
# End of Makefile
#====================================================================#
