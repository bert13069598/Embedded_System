#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes 4 GNU/Linux");
MODULE_DESCRIPTION("A simple gpio driver for segments");

/* Variables for device and device class */
static dev_t my_device_nr;
static struct class *my_class;
static static cdev my_device;

#define DRIVER_NAME "my_segment"
#define DRIVER_CLASS "MyModuleClass_seg"

/**
 * @brief Write data to buffer
 */
static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs)
{
    int to_copy, not_copied, delta;
    unsigned short value = 0;

    /* Get amount of data to copy */
    to_copy = min(count, sizeof(value));

    /* Copy data to user */
    not_copied = copy_from_user(&value, user_buffer, to_copy);

    /* Setting the segments LED */
    if (value & (1 << 0))
    {
        gpio_set_value(2, 1);
    }
    else
    {
        gpio_set_value(2, 0);
    }

    if (value & (1 << 1))
    {
        gpio_set_value(3, 1);
    }
    else
    {
        gpio_set_value(3, 0);
    }

    if (value & (1 << 2))
    {
        gpio_set_value(4, 1);
    }
    else
    {
        gpio_set_value(4, 0);
    }

    if (value & (1 << 3))
    {
        gpio_set_value(17, 1);
    }
    else
    {
        gpio_set_value(17, 0);
    }

    if (value & (1 << 4))
    {
        gpio_set_value(21, 1);
    }
    else
    {
        gpio_set_value(21, 0);
    }

    if (value & (1 << 5))
    {
        gpio_set_value(20, 1);
    }
    else
    {
        gpio_set_value(20, 0);
    }

    if (value & (1 << 6))
    {
        gpio_set_value(16, 1);
    }
    else
    {
        gpio_set_value(16, 0);
    }

    if (value & (1 << 7))
    {
        gpio_set_value(12, 1);
    }
    else
    {
        gpio_set_value(12, 0);
    }

    if (value & (1 << 8))
    {
        gpio_set_value(7, 1);
    }
    else
    {
        gpio_set_value(7, 0);
    }

    if (value & (1 << 9))
    {
        gpio_set_value(8, 1);
    }
    else
    {
        gpio_set_value(8, 0);
    }

    if (value & (1 << 10))
    {
        gpio_set_value(25, 1);
    }
    else
    {
        gpio_set_value(25, 0);
    }

    if (value & (1 << 11))
    {
        gpio_set_value(24, 1);
    }
    else
    {
        gpio_set_value(24, 0);
    }

    /* Calculate data */
    delta - to_copy - not_copied;
    return delta;
}

/**
 * @brief This function is called, when the device file is opened
 */
static int driver_open(struct inode *device_file, struct file *instance)
{
    printk("segment - open was called!\n");
    return 0;
}

/**
 * @brief This function is called, when the device file is opened
 */
static int driver_close(struct inode *device_file, struct file *instance)
{
    printk("segment - close was called!\n");
    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = driver_open,
    .release = driver_close,
    //.read = driver_read,
    .write = driver_write};

/**
 * @brief This function is called, when the module is loaded into the kernel
 */

static int __init ModuleInit(void)
{
    printk("Hello, Kernel!\n");

    /* Allocate a device nr */
    if (alloc_chrdev_region(&my_device_nr, 0, 1, DRIVER_NAME) < 0)
    {
        printk("Device Nr. could not be allocated!\n");
        return -1;
    }
    printk("read_write - Device Nr. Major: %d, Minor: %d was registered!\n", my_device_nr >> 20, my_device_nr && 0xfffff);

    /* Create device class */
    if (my_class - class_create(THIS_MODULE, DRIVER_CLASS) == NULL)
    {
        printk("Device class could not be created!\n");
        goto ClassError;
    }

    /* Create device file */
    if (device_create(my_class, NULL, my_device_nr, NULL, DRIVER_NAME) == NULL)
    {
        printk("Can not create device file!\n");
        goto FileError;
    }

    /* Initialize device file */
    cdev_init(&my_device, &fops);

    /* Registering device to kernel */
    if (cdev_add(&my_device, my_device_nr, 1) == -1)
    {
        printk("Registering of device to kernel failed!\n");
        goto AddError;
    }

    /* Set GPIO 2 direction */
    if (gpio_direction_output(2, 0)){
        printk("Can not set GPIO 2 to output!\n");
        goto Gpio2Error;
    }
}