#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/kernel.h>
#include <linux/ctype.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/gpio.h>

#include "pulse.h"

#define DRIVER_NAME             "pulse"
#define DRIVER_VERSION          "0.50.0"

#define KTIMER_FREQ		(HZ / 100)

/*
 * Local variables
 */

static struct class *pulse_class;

/*
 * Timer function
 */

static void pulse_ktimer_event(unsigned long ptr)
{
	struct pulse_device *pdev = (struct pulse_device *) ptr;

	/* Get the gpio status */
	int status = !!gpio_get_value(pdev->gpio);

	/* Check for the first event  */
	if (pdev->old_status == -1) {
		pdev->old_status = status;
		goto end;
	}

	/* Check for the state changing and, in case, increment the counting */
	if (pdev->old_status != status) {
		pdev->old_status = status;
		atomic_inc(&pdev->counter);
	}

end:
	/* Reschedule the kernel timer */
	mod_timer(&pdev->ktimer, jiffies + KTIMER_FREQ);
}

/*
 * sysfs methods
 */

static ssize_t gpio_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct pulse_device *pdev = dev_get_drvdata(dev);

	return sprintf(buf, "%d\n", pdev->gpio);
}
static DEVICE_ATTR_RO(gpio);

static ssize_t counter_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct pulse_device *pdev = dev_get_drvdata(dev);

	return sprintf(buf, "%d\n", atomic_read(&pdev->counter));
}
static DEVICE_ATTR_RO(counter);

static ssize_t counter_and_reset_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct pulse_device *pdev = dev_get_drvdata(dev);
	int counter = atomic_read(&pdev->counter);

	atomic_set(&pdev->counter, 0);
	return sprintf(buf, "%d\n", counter);
}
static DEVICE_ATTR_RO(counter_and_reset);

static ssize_t set_to_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	struct pulse_device *pdev = dev_get_drvdata(dev);
	int status, ret;

	ret = sscanf(buf, "%d", &status);
	if (ret != 1)
		return -EINVAL;

	atomic_set(&pdev->counter, status);

	return count;
}
static DEVICE_ATTR_WO(set_to);

/*
 * Class attributes
 */

static struct attribute *pulse_attrs[] = {
        &dev_attr_gpio.attr,
        &dev_attr_counter.attr,
        &dev_attr_counter_and_reset.attr,
        &dev_attr_set_to.attr,
        NULL,
};

static const struct attribute_group pulse_group = {
        .attrs = pulse_attrs,
};

static const struct attribute_group *pulse_groups[] = {
        &pulse_group,
        NULL,
};


static void pulse_device_destruct(struct device *dev)
{
        struct pulse_device *pdev = dev_get_drvdata(dev);

        kfree(dev);
        kfree(pdev);
}

/*
 * Exported functions
 */

struct pulse_device *pulse_device_register(char *name, int gpio)
{
	struct pulse_device *pdev;
	int ret;

	/* First allocate a new pulse device */
	pdev = kmalloc(sizeof(struct pulse_device), GFP_KERNEL);
	if (unlikely(!pdev))
		return ERR_PTR(-ENOMEM);

	/* Create the device abd init the device's data */
	pdev->dev = device_create(pulse_class, NULL, gpio, pdev, "%s", name);
	if (unlikely(IS_ERR(pdev->dev))) {
		dev_err(pdev->dev, "unable to create device %s at gpio %d\n",
							name, gpio);
		ret = PTR_ERR(pdev->dev);
		goto error_device_create;
	}
	dev_set_drvdata(pdev->dev, pdev);
	pdev->dev->release = pulse_device_destruct;

	strncpy(pdev->name, name, PULSE_NAME_LEN);
	pdev->gpio = gpio;

	atomic_set(&pdev->counter, 0);
	pdev->old_status = -1;

	/* Then request GPIO */
	ret = gpio_request(gpio, pdev->name);
	if (ret) {
		dev_err(pdev->dev, "unable to request gpio %d\n", gpio);
		goto error_gpio_request;
	}
	gpio_direction_input(gpio);

	/* And as last task start the kernel timer */
	setup_timer(&pdev->ktimer, pulse_ktimer_event, (unsigned long) pdev);
	mod_timer(&pdev->ktimer, jiffies + KTIMER_FREQ);

	dev_info(pdev->dev, "pulse %s added on gpio%d\n",
					pdev->name, pdev->gpio);

	return pdev;

error_gpio_request:
	device_destroy(pulse_class, 0);

error_device_create:
	kfree(pdev);

	return ERR_PTR(ret);
}
EXPORT_SYMBOL(pulse_device_register);

void pulse_device_unregister(struct pulse_device *pdev)
{
	/* Drop all allocated resources */

	del_timer_sync(&pdev->ktimer);
	gpio_free(pdev->gpio);

	device_destroy(pulse_class, pdev->gpio);

	dev_info(pdev->dev, "pulse %s removed\n", pdev->name);
}
EXPORT_SYMBOL(pulse_device_unregister);

/*
 * Module stuff
 */

static int __init pulse_init(void)
{
	printk(KERN_INFO "Pulse driver support v. " DRIVER_VERSION
		" - (C) 2014 Rodolfo Giometti\n");

	/* Create the new class */
	pulse_class = class_create(THIS_MODULE, "pulse");
	if (!pulse_class) {
		printk(KERN_ERR "pulse: failed to allocate class\n");
		return -ENOMEM;
	}
	pulse_class->dev_groups = pulse_groups;

	return 0;
}

static void __exit pulse_exit(void)
{
	class_destroy(pulse_class);
}

module_init(pulse_init);
module_exit(pulse_exit);

MODULE_AUTHOR("Rodolfo Giometti <giometti@hce-engineering.com>");
MODULE_DESCRIPTION("Pulse driver support v. " DRIVER_VERSION);
MODULE_LICENSE("GPL");
