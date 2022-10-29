static int __init ModuleInit (void) {
	printk("Hello, Kernel! \n");

	if (alloc_chrdev_region(&my_device_nr, 0, 1, DRIVER_NAME) < 0) {
		printk("Device Nr. could not be allocated!  \n");
		return -1;
	}
	printk("read_write - Device Nr. Major: %d, Minor: %d was registered! \n", my_device_nr >> 20, my_device_nr && 0xfffff)

	if ((my_class = class_create(THIS_MODULE, DRIVER_CLASS))==NULL) {
		printk("Device class can not e created! \n");
		goto ClassError;
		}

	if (device_create(my_class, NULL, my_device_nr, NULL, DRIVER_NAME) ==NULL) {
		printk("Can not create device file! \n");
		goto FileError;
	}

	cdev_init(&my_device, &fops);

	if (cdev_add(&my_device, my_device_nr, 1) == -1) {
		printk("Registering of device to kernel failde! \n");
		goto AddError;
	}

	if (gpio_request(4, "rpi-gpio-4")) {
		printk("Can not allocate GPIO4 \n");
		goto AddError;
	}

	if (gpio_direction_output(4,0)) {
		printk("Can not set  GPIO4 to output! \n");
		goto Gpio4Error;
	}

	if (gpio_request(17, "rpi-gpio-17")) {
		printk("Can not allocate  GPIO17! \n");
		goto AddError;
	}

	if (gpio_direction_input(17)) {
		printk("Can not set  GPIO17 to input! \n");
		goto Gpio17Error;
	}

	return 0;
Gpio4Error:
	gpio_free(4);
Gpio17Error:
	gpio_free(17);
AddError:
	device_destory(my_class, my_device_nr);
FileError:
	class_destory(my_class);
ClassError:
	unregister_chrdev_region(my_device_nr, 1);
	return -1;
}


static void __exit ModuleExit(void) {
	gpio_set_value(4, 0);
	gpio_free(4);
	gpio_free(17);
	cdev_del(&my_device);
	device_destory(my_class, my_device_nr);
	class_destory(my_class);
	unregister_chrdev_region(my_device_nr.1);
	printk("Goodbye, Kernel \n");
}

mdoule_init(ModuleInit);
mdoule_exit(ModuleExit);

