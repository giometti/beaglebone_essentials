#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <libusb.h>

/* The g_zero default USB vendor:device product numbers */
#define VENDOR_ID	0x0525
#define PRODUCT_ID	0xa4a0

uint8_t buffer[512];

int main(int argc, char *argv[])
{
	libusb_device_handle *handle;
	int flag = 0;		/* set to 1 if kernel driver detached */
	int n;
	int ret;

	/* Init th libusb */
	ret = libusb_init(NULL);
	if (ret < 0) {
        	fprintf(stderr, "unable to init libusb\n");
        	exit(-1);
	}

	/* Check for g_zero device is connected to the system */
	handle = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, PRODUCT_ID);
	if (!handle) {
		fprintf(stderr, "unable to open g_zero device\n");
		exit(-1);
	}
	printf("g_zero device found\n");

	/*
	 * Check whether a kernel driver is attached to interface #0.
	 * If so, we'll need to detach it.
	 */
	if (libusb_kernel_driver_active(handle, 0)) {
		ret = libusb_detach_kernel_driver(handle, 0);
		if (ret == 0) {
			flag = 1;
		} else {
			fprintf(stderr, "error detaching kernel driver.\n");
			exit(-1);
		}
	}

	/* Claim interface #0 */
	ret = libusb_claim_interface(handle, 0);
	if (ret) {
		fprintf(stderr, "error claiming interface.\n");
		exit(-1);
	}

	/* Send an all-zeros message to endpoint 0x01 */
	ret = libusb_bulk_transfer(handle, 0x01, buffer,
					sizeof(buffer), &n, 100);
	if (ret) {
    		fprintf(stderr, "error sending message to device ret=%d\n",
				ret);
		exit(-1);
	}
	printf("%d bytes transmitted successfully\n", n);

	/* Receive an all-zeros message from endpoint 0x81 */
	ret = libusb_bulk_transfer(handle, 0x81, buffer,
					sizeof(buffer), &n, 100);
	if (ret) {
		fprintf(stderr, "error receiving message from device ret=%d\n",
				ret);
		exit(-1);
	}
	if (n != sizeof(buffer)) {
		fprintf(stderr, "error receiving %d bytes while expecting %ld\n",
					n, sizeof(buffer));
		exit(-1);
	}
	printf("%d bytes received successfully\n", n);

	/* Release interface #0 */
	ret = libusb_release_interface(handle, 0);
	if (ret) {
		fprintf(stderr, "error releasing interface\n");
		exit(-1);
	}

	/*
	 * If we detached a kernel driver from interface #0 earlier, we'll now 
	 * need to attach it again.
	 */
	if (flag)
		libusb_attach_kernel_driver(handle, 0);

	libusb_exit(NULL);

	return 0;
}
