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
# Note that you should run the `root/config.sh` script before        #
# building anything.                                                 #
#                                                                    #
#====================================================================#
#====================================================================#
# VARIABLES
#

# Target Architecture
ARCH?=i386

# Input Directories
ARCHDIR=arch/$(ARCH)

# Includes
# All the objects are defined in those files
include $(ARCHDIR)/make.config
include string/make.config
include stdio/make.config

# Output directories
DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX=$(PREFIX)/include
LIBDIR?=$(EXEC_PREFIX)/lib        # The library will be installed here

# Compiler Flags
CFLAGS:=\
$(CFLAGS) \
$(ARCH_CFLAGS) \
$(STRING_CFLAGS) \
$(STDIO_CFLAGS)

LIBK_CFLAGS:=\
$(LIBK_CFLAGS) \
$(LIBK_ARCH_CFLAGS) \
$(LIBK_STRING_CLAGS) \
$(LIBK_STDIO_CFLAGS)

CFLAGS?=-O2 -g
LDFLAGS?=
LIBS?=
CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra -Wpedantic \
        -Werror -Wconversion
LIBK_CFLAGS:=$(CFLAGS) $(LIBK_CFLAGS)


#====================================================================#
# OBJECT FILES
#

# Objects for both libk and libc
COMMON_OBJS:=\
$(COMMON_ARCH_OBJS) \
$(COMMON_STRING_OBJS) \
$(COMMON_STDIO_OBJS)

# libk
LIBK_OBJS:=\
$(COMMON_OBJS) \
$(LIBK_ARCH_OBJS) \
$(LIBK_STRING_OBJS) \
$(LIBK_STDIO_OBJS) \

# libc
LIBC_BJS:=\
$(COMMON_OBJS) \
$(LIBC_ARCH_OBJS) \
$(LIBC_STRING_OBJS) \
$(LIBC_STDIO_OBJS) \

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
	@$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS)

.c.S:
	@echo " * [CC] $@"
	@$(CC) -MD -c $< -o $@ $(CFLAGS)

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

install-libs: $(BINARIES)
	@echo " * Installing libs... $@"
	@mkdir -p $(DESTDIR)$(LIBDIR)
	@cp $(BINARIES) $(DESTDIR)$(LIBDIR)

-include $(HOSTEDOBJS:.o=.d)
-include $(LIBK_OBJS:.o=.d)

#
# End fof Makefile
#====================================================================#
