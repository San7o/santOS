#====================================================================#
#                                                                    #
#                            LIBC MAKEFILE                           #
#                                                                    #
# ------------------------------------------------------------------ #
#                                                                    #
# This Makefile is responsible for building the libc implementation. #
# This library, compiled separately from the kernel and statically   #
# liked to it, will provide useful utilities from the glib such as   #
# string and IO operations.                                          #
# Additionally, the following commands are provided:                 #
# - install-libs: installs the library in $DESTDIR/$LIBDIR           #
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

# Do test
TEST?=True

# Target Architecture
ARCH?=i386

# Input Directories
ARCHDIR=arch/$(ARCH)

# Includes
# All the objects are defined in those files
SUBPROJECTS_MAKE_CONFIG=\
$(ARCHDIR)/make.config \
string/make.config \
stdio/make.config

include $(SUBPROJECTS_MAKE_CONFIG)

# Output directories
DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX=$(PREFIX)/include
LIBDIR?=$(EXEC_PREFIX)/lib        # The library will be installed here

# Compiler Flags
CFLAGS?=-O2 -g -Wall -Wextra -Wpedantic \
        -Werror -Wno-pointer-arith
LDFLAGS?=
LIBS?=
CFLAGS:=$(CFLAGS) -ffreestanding -nostdlib
LIBC_CFLAGS:= $(CFLAGS) $(LIBC_CFLAGS)
LIBK_CFLAGS:= $(CFLAGS) $(LIBK_CFLAGS) 


#====================================================================#
# OBJECT FILES
#

# Objects for both libk and libc
COMMON_OBJS:=\
$(COMMON_OBJS)
ifeq ($(TEST),True)
COMMON_OBJS+=$(KTEST_OBJS)
endif

# libk
LIBK_OBJS:=$(COMMON_OBJS) $(LIBK_OBJS)

# libc
LIBC_BJS:=$(COMMON_OBJS) $(LIBC_OBJS)

OBJS:=\
$(COMMON_OBJS) \
$(LIBK_OBJS) \
$(LIBC_OBJS)

LIBK_OBJS:=$(LIBK_OBJS:.o=.libk.o)
BINARIES=libk.a #libc.a                       # libc not available yet


#====================================================================#
# BUILDING
#

.PHONY: all clean install install-headers install-libs
.SUFFIXES: .o .libk.o .c .S

all: $(BINARIES)

libc.a: $(LIBC_OBJS)
	@echo " * [AR] $@"
	@$(AR) rcs $@ $(LIBC_OBJS)

libk.a: $(LIBK_OBJS)
	@echo " * [AR] $@"
	@$(AR) rcs $@ $(LIBK_OBJS)

.c.o:
	@echo " * [CC] $@"
	@$(CC) -MD -c $< -o $@ -std=gnu11 $(LIBC_CFLAGS)

.c.S:
	@echo " * [CC] $@"
	@$(CC) -MD -c $< -o $@ $(LIBC_CFLAGS)

.c.libk.o:
	@echo " * [CC] $@"
	@$(CC) -MD -c $< -o $@ -std=gnu11 $(LIBK_CFLAGS)

.S.libk.o:
	@echo " * [CC] $@"
	@$(CC) -MD -c $< -o $@ $(LIBK_CFLAGS)


#====================================================================#
# ADDITIONAL COMMANDS
#

clean:
	@echo " * Cleaning libc build files... $@"
	@rm -f $(BINARIES) *.a
	@rm -f $(LIBC_OBJS) $(LIBK_OBJS) *.o */*.o */*/*.o
	@rm -f $(LIBC_OBJS:.o=.d) $(LIBK_OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-libs

install-headers:
	@echo " * Installing Headers... $@"
	@mkdir -p $(DESTDIR)$(INCLUDEDIR)
	@cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.
	@cp -R --preserve=timestamps arch/$(ARCH)/include/. $(DESTDIR)$(INCLUDEDIR)/.

install-libs: $(BINARIES)
	@echo " * Installing libs... $@"
	@mkdir -p $(DESTDIR)$(LIBDIR)
	@cp -R $(BINARIES) $(DESTDIR)$(LIBDIR)

-include $(LIBK_OBJS:.o=.d)

#
# End fof Makefile
#====================================================================#
