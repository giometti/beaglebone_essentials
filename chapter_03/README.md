Chapter 3 - Compiling versus Cross-Compiling
============================================

Here the code from chapter 3 - "Compiling versus Cross-Compiling" of
the book "BeagleBone Essentials" written by Rodolfo Giometti and
published by Packt Publishing (ISBN 978-1-78439-352-6).

This example code implements simple programs:

* helloworld.c: classic Hello World C program for PicoC

* helloworld.picoc: stript version of helloworld.c for PicoC

* hwrng.picoc: simple tool for PicoC to read bytes from a file

* module: really simple kernel module


See the URL
https://www.packtpub.com/hardware-and-creative/beaglebone-essentials
for further info.

FAQs
----

**Questions**

1) I tried to checkout the branch "am33x-v3.13" but I got the an error
as below:

    $ git checkout am33x-v3.13
    error: pathspec 'am33x-v3.13' did not match any file(s) known to git.

How can I safely checkout a new kernel release for my BeagleBone Black?


**Answers**

1) The kernel sourses has been moved on another location, so the new commands
to get them are now the following:

    $ git clone https://github.com/RobertCNelson/bb-kernel.git
    $ cd bb-kernel/
    $ git checkout origin/am33x-v3.13 -b am33x-v3.13

And then we can continue as described in the book.
