Chapter 1 - Installing the Developing System
============================================

Here the code from chapter 1 - "Installing the Developing System" of
the book "BeagleBone Essentials" written by Rodolfo Giometti and
published by Packt Publishing (ISBN 978-1-78439-352-6).

This example code implements simple programs:

* helloworld.c: classic Hello World C program

* led_aging.sh: correct version of the file /etc/init.d/led_aging.sh

* test.php: a simple PHP/MySQL tester


See the URL
https://www.packtpub.com/hardware-and-creative/beaglebone-essentials
for further info.

FAQs
----

**Questions**

1) I executed the command below to re-install a Debian distribution on my
microSD card:

    $ sudo ./setup_sdcard.sh --mmc /dev/sdd --dtb BeagleBone

But I get the following error:

    ERROR: This script does not currently recognize the selected: [--dtb BeagleBone] option..
    Please rerun setup_sdcard.sh with a valid [--dtb <device>] option from the list below:
    -----------------------------
    #        --dtb beaglebone (BeagleBone & BeagleBone Black:supported)
    #        --dtb imx51-babbage (Freescale i.MX51 Babbage Board:supported)
    #        --dtb imx53-qsb (Freescale i.MX53 Quick Start Board:supported)
    #        --dtb imx53-qsrb (Freescale i.MX53 Quick Start-R Board:supported)
    #        --dtb imx6dl-udoo (Freescale based i.MX6 UDOO (Dual Core):supported)
    #        --dtb imx6q-sabresd (Freescale i.MX6q SABRE Board for Smart Devices:supported)
    #        --dtb imx6q-udoo (Freescale based i.MX6 UDOO (Quad Core):supported)
    #        --dtb omap3-beagle (BeagleBoard Ax/Bx/Cx/Dx:supported)
    #        --dtb omap3-beagle-xm (BeagleBoard xM:supported)
    #        --dtb omap4-panda-a4 (PandaBoard A4-A+:supported)
    #        --dtb omap4-panda (PandaBoard EA1-A3:supported)
    #        --dtb omap4-panda-es-b3 (PandaBoard ES Rev B3:supported)
    #        --dtb omap4-panda-es (PandaBoard ES:supported)
    #        --dtb omap5-uevm (SVTRONICS OMAP5432 EVM:supported)
    #        --dtb wandboard-dual (Freescale based i.MX6 Wandboard (Dual Core):supported)
    #        --dtb wandboard-quad (Freescale based i.MX6 Wandboard (Quad Core):supported)
    #        --dtb wandboard-solo (Freescale based i.MX6 Wandboard (Single Core):supported)

How can fix this issue?

2) I executed the command below to re-install a Debian distribution on my
microSD card:

    $ sudo ./setup_sdcard.sh --mmc /dev/sdd --dtb beaglebone

But I get the following error:

    Downloading Device's Bootloader
    -----------------------------
    2016-04-18 17:02:40 URL:https://rcn-ee.net/deb/tools/latest/bootloader-ng [5361/5361] -> "/tmp/tmp.XNTVBCiNeO/dl/bootloader-ng" [1]
    http://rcn-ee.net/deb/tools/am335x_evm/MLO-am335x_evm-v2015.01-r7:
    2016-04-18 17:02:40 ERROR 404: Not Found.

How can fix this issue?


**Answers**

1) The command reported into the book was wrong! The correct command is:

    $ sudo ./setup_sdcard.sh --mmc /dev/sdd --dtb beaglebone

That is with the string "beaglebone" with all lowercase.

[However this command may fail... in this case see the next answer too!]

2) Some readers report that the site rcn-ee.net has moved some files and
the procedure described into the book may fail. In this case the quick&dirty
thing to do is to follow the instructions reported here:

   https://eewiki.net/display/linuxonarm/BeagleBone+Black
