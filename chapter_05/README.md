Chapter 5 - Device Drivers
==========================

Here the code from chapter 5 - "Device Drivers" of
the book "BeagleBone Essentials" written by Rodolfo Giometti and
published by Packt Publishing (ISBN 978-1-78439-352-6).

This example code implements simple programs:

* pulse_gen.sh: a simple pulse generator

* pulse: the "pulse" kernel driver


See the URL
https://www.packtpub.com/hardware-and-creative/beaglebone-essentials
for further info.

FAQs
----

**Questions**

1) I am trying your pulse driver in Chapter 5. My Beagle bone black rev c is 3.8.13-bone71.

I did the following on my Debian workstation:

    $ git clone https://github.com/RobertCNelson/bb-kernel.git
    ==>"uname -r" on beaglebone showed 3.8.13.-bone71 <==
    $ cd bb-kernel  (on local Debian)
    $ git tag  (This shows what versions can be checked out.)
    $ git checkout 3.8.13-bone71 -b 3.8.13-bone71
    $ cp system.sh.sample system.sh
    $ nano system.sh
    ==> add CC=/usr/bin/arm-linux-gnueabihf-
    ==> save system.sh
    $ ./build_kernel.sh

The above is successful and since I did not specify `LINUX_GIT` in `system.sh`, it downloaded linux stable automatically.

I went to the directory where my pulse driver is and ran the command below and I get errors as shown below:

    tbielenda@debian:~/BeagleBone_Essentials/chapter_05/pulse$ make KERNEL_DIR=../../../kernelsrc/bb-kernel/KERNEL modules
    make -C ../../../kernelsrc/bb-kernel/KERNEL ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- \
    SUBDIRS=/home/tbielenda/BeagleBone_Essentials/chapter_05/pulse modules make[1]: Entering directory '/home/tbielenda/kernelsrc/bb-kernel/KERNEL'
    CC [M] /home/tbielenda/BeagleBone_Essentials/chapter_05/pulse/pulse.o
    /home/tbielenda/BeagleBone_Essentials/chapter_05/pulse/pulse.c:65:8: warning: type defaults to int in declaration of DEVICE_ATTR_RO [-Wimplicit-int]
    /home/tbielenda/BeagleBone_Essentials/chapter_05/pulse/pulse.c:65:1: warning: parameter names (without types) in function declaration [enabled by default]
    /home/tbielenda/BeagleBone_Essentials/chapter_05/pulse/pulse.c:74:8: warning: type defaults to int in declaration of DEVICE_ATTR_RO [-Wimplicit-int]
    /home/tbielenda/BeagleBone_Essentials/chapter_05/pulse/pulse.c:74:1: warning: parameter names (without types) in function declaration [enabled by default]
    ...

**Answers**

1) This kernel uses a new API to implement `sysfs` files. Try using the patch into file `pulse_sysfs_API.patch` with the following command:

    $ cd BeagleBone_Essentials/github/chapter_05/
    $ git apply pulse_sysfs_API.patch

Then try to recompile the driver as usual.
