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

OUT_NAME=myos.kernel

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
include $(ARCHDIR)/make.config
include kernel/make.config
include tests/make.config
include boot/make.config

# Compiler Flags
CFLAGS?=-O2 -g
LDFLAGS?=
LIBS?=
CFLAGS:=$(CFLAGS) -ffreestanding
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -L$(PREFIX)/lib -lk -lgcc    # -lk is the libk

CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)


#====================================================================#
# OBJECT FILES
#

OBJS=\
$(KERNEL_ARCH_OBJS) \
$(BOOT_OBJS) \
$(KERNEL_OBJS)

LINK_LIST=\
$(LDFLAGS) \
$(OBJS) \
$(LIBS)

TEST_OBJS=\
$(TESTS_OBJS)\
$(OBJS)

TEST_LINK_LIST=\
$(LDFLAGS) \
$(TEST_OBJS) \
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

$(OUT_NAME)_test: $(TEST_OBJS) $(LINKER_SCRIPT) $(SYSROOT)/lib/libk.a
	@echo " * [LD] $@"
	@$(CC) -T $(LINKER_SCRIPT) -o $@ $(CFLAGS) $(TEST_LINK_LIST)
	@grub-file --is-x86-multiboot $(OUT_NAME)_test


#====================================================================#
# ADDITIONAL COMMANDS
#

clean:
	@echo " * Cleaning kernel build files..."
	@rm -f $(OUT_NAME) $(OUT_NAME)_test
	@rm -f $(OBJS) $(TEST_OBJS) *.o */*.o */*/*.o
	@rm -f $(OBJS:.o=.d) $(TEST_OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-kernel
install-test: install-headers install-test-kernel

install-headers:
	@echo " * Installing headers..."
	@mkdir -p $(DESTDIR)$(INCLUDEDIR)
	@cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-kernel: $(OUT_NAME)
	@echo " * Installing the kernel..."
	@mkdir -p $(DESTDIR)$(BOOTDIR)
	@cp $(OUT_NAME) $(DESTDIR)$(BOOTDIR)

install-test-kernel: $(OUT_NAME)_test
	@echo " * Installing the kernel..."
	@mkdir -p $(DESTDIR)$(BOOTDIR)
	@cp $(OUT_NAME)_test $(DESTDIR)$(BOOTDIR)/$(OUT_NAME)

-include $(OBJS:.o=.d)


#
# End of Makefile
#====================================================================#
