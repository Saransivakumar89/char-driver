#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

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
	{ 0xbe12153d, "module_layout" },
	{ 0x8a04e75, "device_destroy" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xd3f933, "cdev_del" },
	{ 0x2deca16e, "class_destroy" },
	{ 0xba5b58fe, "device_create" },
	{ 0xd0eb2e65, "__class_create" },
	{ 0x70633238, "cdev_add" },
	{ 0xc85584e5, "cdev_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xfd43d459, "latent_entropy" },
	{ 0xcd9053ec, "arm_copy_to_user" },
	{ 0x5f754e5a, "memset" },
	{ 0x2cfde9a2, "warn_slowpath_fmt" },
	{ 0x5ca386a8, "arm_copy_from_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0xc5850110, "printk" },
};

MODULE_INFO(depends, "");

