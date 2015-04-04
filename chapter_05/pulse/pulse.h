#include <linux/device.h>
#include <linux/atomic.h>

#define PULSE_NAME_LEN			32

/*
 * Pulse port basic structs
 */

/* Main struct */
struct pulse_device {
	int gpio;
	char name[PULSE_NAME_LEN];

	atomic_t counter;
	unsigned int old_status;

	struct timer_list ktimer;

	struct device *dev;
	void *private_data;
};

/*
 * Exported functions
 */

#define to_class_dev(obj) container_of((obj), struct class_device, kobj)
#define to_pulse_device(obj) container_of((obj), struct pulse_device, class)

extern struct pulse_device *pulse_device_register(char *name, int gpio);
extern void pulse_device_unregister(struct pulse_device *dev);
