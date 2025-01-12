#====================================================================#
#                                                                    #
#                         LIBKTEST MAKEFILE                          #
#                                                                    #
# ------------------------------------------------------------------ #
#                                                                    #
# This Makefile is responsible for building the testing framework.   #
# This library, compiled separately from the kernel and statically   #
# liked to it, will provide a testing framework for the kernel and   #
# subprojects.                                                       #
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

# Includes
# All the objects are defined in those files
SUBPROJECTS_MAKE_CONFIG=\
ktest/make.config

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
KTEST_CFLAGS+=$(CFLAGS) -ffreestanding -nostdlib


#====================================================================#
# OBJECT FILES
#

OBJS:=$(KTEST_OBJS)

BINARIES=libktest.a


#====================================================================#
# BUILDING
#

.PHONY: all clean install install-headers install-libs
.SUFFIXES: .o .libk.o .c .S

all: $(BINARIES)

libktest.a: $(OBJS)
	@echo " * [AR] $@"
	@$(AR) rcs $@ $(OBJS)

.c.o:
	@echo " * [CC] $@"
	@$(CC) -MD -c $< -o $@ -std=gnu11 $(KTEST_CFLAGS)

.c.S:
	@echo " * [CC] $@"
	@$(CC) -MD -c $< -o $@ $(KTEST_CFLAGS)


#====================================================================#
# ADDITIONAL COMMANDS
#

clean:
	@echo " * Cleaning libc build files... $@"
	@rm -f $(BINARIES) *.a
	@rm -f $(OBJS) *.o */*.o */*/*.o
	@rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-libs

install-headers:
	@echo " * Installing Headers... $@"
	@mkdir -p $(DESTDIR)$(INCLUDEDIR)
	@cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-libs: $(BINARIES)
	@echo " * Installing libs... $@"
	@mkdir -p $(DESTDIR)$(LIBDIR)
	@cp $(BINARIES) $(DESTDIR)$(LIBDIR)

-include $(OBJS:.o=.d)

#
# End fof Makefile
#====================================================================#
