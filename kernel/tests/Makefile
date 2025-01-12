SUPER_MAKE_VARIABLES=QEMU_DIR GRUB_DIR ARCH SYSROOT MAKE AR AS CC \
CFLAGS PREFIX EXEC_PREFIX BOOTDIR LIBDIR INCLUDEDIR

.PHONY: install

all: install

install: 
	@env $(foreach var, $(SUPER_MAKE_VARIABLES),$(var)="$($(var))") \
            $(MAKE) -C .. install-test;
