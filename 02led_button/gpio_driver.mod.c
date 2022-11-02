#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x37f3c8fe, "module_layout" },
	{ 0x41ba40fe, "cdev_del" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xeb03b542, "class_destroy" },
	{ 0x95d51e32, "device_destroy" },
	{ 0xb53de3e2, "gpiod_direction_input" },
	{ 0xfe990052, "gpio_free" },
	{ 0x5f7fe64f, "gpiod_direction_output_raw" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xe75d82fd, "cdev_add" },
	{ 0x26418e92, "cdev_init" },
	{ 0xdfc8bbb4, "device_create" },
	{ 0xcd2888ea, "__class_create" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0xac0d0df4, "gpiod_get_raw_value" },
	{ 0x3ea1b6e4, "__stack_chk_fail" },
	{ 0xdcb764ad, "memset" },
	{ 0x179ac410, "gpiod_set_raw_value" },
	{ 0x93b361a5, "gpio_to_desc" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0x92997ed8, "_printk" },
	{ 0x1fdc7df2, "_mcount" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "696D98C08ED933238640D19");
