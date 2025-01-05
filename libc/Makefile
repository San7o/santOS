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

# Output directories
DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX=$(PREFIX)/include
LIBDIR?=$(EXEC_PREFIX)/lib        # The library will be installed here
CFLAGS:=$(CFLAGS) $(ARCH_CFLAGS)
LIBK_CFLAGS:=$(LIBK_CFLAGS) $(KERNEL_ARCH_CFLAGS)

# Subprojects
include $(ARCHDIR)/make.config

# Compiler Flags
CFLAGS?=-O2 -g
LDFLAGS?=
LIBS?=
CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra -Wpedantic \
        -Werror -Wconversion
LIBK_CFLAGS:=$(CFLAGS)


#====================================================================#
# OBJECT FILES
#

FREEOBJS=\
$(ARCH_FREEOBJS) \
string/strlen.o \

HOSTEDOBJS= \
$(ARCH_HOSTEDOBJS) \

OBJS=\
$(FREEOBJS) \
$(HOSTEDOBJS) \

LIBK_OBJS=$(FREEOBJS:.o=.libk.o)
#BINARIES=libc.a libk.a                       # libc not available yet
BINARIES=libk.a


#====================================================================#
# BUILDING
#

.PHONY: all clean install install-headers install-libs
.SUFFIXES: .o .libk.o .c. S

all: $(BINARIES)

libc.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

libk.a: $(LIBK_OBJS)
	$(AR) rcs $@ $(LIBK_OBJS)

.c.o:
	@echo " * [CC] $@"
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS)

.c.S:
	$(CC) -MD -c $< -o $@ $(CFLAGS)

.c.libk.o:
	@echo " * [CC] $@"
	$(CC) -MD -c $< -o $@ -std=gnu11 $(LIBK_CFLAGS)

.S.libk.o:
	@echo " * [CC] $@"
	$(CC) -MD -c $< -o $@ $(LIBK_CFLAGS)


#====================================================================#
# ADDITIONAL COMMANDS
#

clean:
	rm -f $(BINARIES) *.a
	rm -f $(OBJS) $(LIBK_OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) $(LIBK_OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-libs

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-libs: $(BINARIES)
	mkdir -p $(DESTDIR)$(LIBDIR)
	cp $(BINARIES) $(DESTDIR)$(LIBDIR)

-include $(OBJS:.o=.d)
-include $(LIBK_OBJS:.o=.d)

#
# End fof Makefile
#====================================================================#
