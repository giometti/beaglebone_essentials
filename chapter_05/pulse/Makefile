KERNEL_DIR := .			# to be set from the command line
PWD := $(shell pwd)
CROSS_COMPILE = arm-linux-gnueabihf-

obj-m = pulse.o
obj-m += pulse-bbb.o

all: modules

modules clean:
	$(MAKE) -C $(KERNEL_DIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) \
		SUBDIRS=$(PWD) $@
