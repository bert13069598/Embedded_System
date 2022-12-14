#include <linux/module.h>
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
static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs)
{
    int to_copy, not_copied, delta;
    char tmp;

    /* Get amount of data to copy */
    to_copy = min(count, sizeof(tmp));

    /* Read value of button */
    if ((gpio_get_value(26) + '0') == '1')
    {
        tmp = 1 + '0';
    }
    else if ((gpio_get_value(27) + '0') == '1')
    {
        tmp = 2 + '0';
    }
    else
        tmp = '0';

    /* Copy data to user */
    not_copied = copy_to_user(user_buffer, &tmp, to_copy);

    /* Calculate data */
    delta = to_copy - not_copied;

    return delta;
}

// /**
//  * @brief Write data to the buffer
//  */
//  static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *off
s)
{
    //     int to_copy, not_copied, delta;
    //     char value;

    //     /* Get amount of data to copy */
    //     to_copy = min(count, sizeof(value));

    //     /* Copy data to user */
    //     not_copied = copy_from_user(&value, user_buffer, to_copy);

    //     /* Setting the LED */
    //     switch(value){
    //         case '0':
    //             gpio_set_value(4, 0);
    //             break;
    //         case '1':
    //             gpio_set_value(4, 1);
    //             break;
    //         default:
    //             printk("Invalid Input!\n");
    //             break;
    //     }

    //     /* Calculate data */
    //     delta = to_copy - not_copied;
    //     return delta;
    //  }

    /**
     * @brief This function is called, when the device file is opened
     */
    static int driver_open(struct inode * device_file, struct file * instance)
    {
        printk("led_button - open was called!\n");
        return 0;
    }
    /**
     * @brief This function is called, when the device file is opened
     */
    static int driver_close(struct inode * device_file, struct file * instance)
    {
        printk("led_button - close was called!\n");
        return 0;
    }

    static struct file_operations fops = {
        .owner = THIS_MODULE,
        .open = driver_open,
        .release = driver_close,
        .read = driver_read,
        // .write = driver_write
    };

    static int __init ModuleInit_gpio(void)
    {
        printk("Hello, Kernel! \n");

        if (alloc_chrdev_region(&my_device_nr, 0, 1, DRIVER_NAME) < 0)
        {
            printk("Device Nr. could not be allocated!\n");
            return -1;
        }
        printk("read_write - Device Nr. Major: %d, Minor: %d was registered! \n", my_device _nr >> 20, my_device_nr && 0xfffff);

        if ((my_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL)
        {
            printk("Device class can not e created!\n");
            goto ClassError;
        }

        if (device_create(my_class, NULL, my_device_nr, NULL, DRIVER_NAME) == NULL)
        {
            printk("Can not create device file!\n");
            goto FileError;
        }
        cdev_init(&my_device, &fops);

        if (cdev_add(&my_device, my_device_nr, 1) == -1)
        {
            printk("Registering of device to kernel failde! \n");
            goto AddError;
        }

        // if (gpio_request(4, "rpi-gpio-4")) {
        // printk("Can not allocate GPIO4 \n");
        // goto AddError;
        // }

        // if (gpio_direction_output(4,0)) {
        // printk("Can not set GPIO4 to output! \n");
        // goto Gpio4Error;
        // }

        if (gpio_request(26, "rpi-gpio-26"))
        {
            printk("Can not allocate  GPIO26! \n");
            goto AddError;
        }

        if (gpio_direction_input(26))
        {
            printk("Can not set  GPIO26 to input! \n");
            goto Gpio26Error;
        }

        if (gpio_request(27, "rpi-gpio-27"))
        {
            printk("Can not allocate  GPIO27! \n");
            goto AddError;
        }

        if (gpio_direction_input(27))
        {
            printk("Can not set  GPIO27 to input! \n");
            goto Gpio27Error;
        }
        return 0;
    // Gpio4Error:
    // gpio_free(4);
    Gpio26Error:
        gpio_free(26);
    Gpio27Error:
        gpio_free(27);
    AddError:
        device_destroy(my_class, my_device_nr);
    FileError:
        class_destroy(my_class);
    ClassError:
        unregister_chrdev_region(my_device_nr, 1);
        return -1;
    }

    static void __exit ModuleExit_gpio(void)
    {
        // gpio_set_value(4, 0);
        // gpio_free(4);
        gpio_free(26);
        gpio_free(27);
        cdev_del(&my_device);
        device_destroy(my_class, my_device_nr);
        class_destroy(my_class);
        unregister_chrdev_region(my_device_nr, 1);
        printk("Goodbye, Kernel \n");
    }

    module_init(ModuleInit_gpio);
    module_exit(ModuleExit_gpio);