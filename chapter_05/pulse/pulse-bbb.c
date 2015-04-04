#include <linux/errno.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include "pulse.h"

/* Declare all the needed pulse devices */
static struct {
	int gpio;
	char *name;
} pulse_data[] = {
	{
		.gpio = 67,
		.name = "oil",
	},
	{
		.gpio = 68,
		.name = "water",
	},
};

static DEFINE_MUTEX(update_lock);
static struct pulse_device *first_pulse_dev[ARRAY_SIZE(pulse_data)];

/*
 * Module stuff
 */

static int __init first_pulse_init(void)
{
	int i, ret = 0;

	/* Request the needed pulse devices */
	for (i = 0; i < ARRAY_SIZE(pulse_data); i++) {
		first_pulse_dev[i] = pulse_device_register(pulse_data[i].name,
							pulse_data[i].gpio);
		if (!first_pulse_dev[i]) {
			ret = -ENODEV;
			goto free_gpio;
		}
	}

	return 0;

free_gpio:
	while (--i >= 0)
		pulse_device_unregister(first_pulse_dev[i]);

	return ret;
}

static void __exit first_pulse_exit(void)
{
	int i;

	/* Released all the allocated pulse devices */
	for (i = 0; i < ARRAY_SIZE(pulse_data); i++)
		pulse_device_unregister(first_pulse_dev[i]);
}

module_init(first_pulse_init);
module_exit(first_pulse_exit);

MODULE_AUTHOR("Rodolfo Giometti <giometti@hce-engineering.com>");
MODULE_DESCRIPTION("Pulse support for the BeagleBone Black counting-board");
MODULE_LICENSE("GPL");
