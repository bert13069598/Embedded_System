#include <linux/modue.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes 4 GNU/Linux");
MODULE_DESCRIPTION("A simple gpio driver for setting a LED and reading a button");

/* Variables for device and device class */
static dev_t my_device_nr;
static struct class *my_class;
static struct cdev my_device;

#define DRIVER_NAME "my_gpio"
#define DRIVER_CLASS "MyModuleClass"

/**
 * @brief Read data out of the buffer
 */
static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs){
    int to_copy, not_copied, delta;
    char tmp;

    /* Get amount of data to copy */
    to_copy = min(count, sizeof(tmp));

    /* Read value of button */
    tmp = gpio_get_value(17) + '0';

    /* Copy data to user */
    not_copied = copy_to_user(user_buffer, &tmp, to_copy);

    /* Calculate data */
    delta = to_copy - not_copied;

    return delta;
}

/**
 * @brief Write data to the buffer
 */
 static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs){
    itn to_copy, not_copied, delta;
    char value;

    /* Get amount of data to copy */
    to_copy = min(count, sizeof(value));

    /* Copy data to user */
    not_copied = copy_from_user(&value, user_buffer, to_copy);

    /* Setting the LED */
    switch(value){
        case '0':
            gpio_set_value(4, 0);
            break;
        case '1':
            gpio_set_value(4, 1);
            break;
        default:
            printk("Invalid Input!\n");
            break;
    }

    /* Calculate data */
    delta = to_copy - not_copied;
    return delta;
 }

 /**
 * @brief This function is called, when the device file is opened
 */
 static int driver_close(struct inode *device_file, struct file *instance){
    printk("led_button - close was called!\n");
    return 0;
 }

 static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = driver_open,
    .release = driver_close,
    .read = driver_read,
    .write = driver_write
 };
 