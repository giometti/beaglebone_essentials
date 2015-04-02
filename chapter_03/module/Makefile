KERNEL_DIR ?= .		# to be set from the command line

# This specifies the kernel module to be compiled
obj-m += module.o

# The default action
all: modules

# The main tasks
modules clean:
	make -C $(KERNEL_DIR) ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- \
		SUBDIRS=$(PWD) $@
