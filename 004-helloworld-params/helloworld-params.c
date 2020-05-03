#define pr_fmt(fmt)	KBUILD_MODNAME " -> %s:%d -> " fmt,__func__,__LINE__
#include <linux/module.h>
#include <linux/init.h>

MODULE_AUTHOR("Cooper Van Kampen");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Basic hello world module");

static int __init lkm_init(void) {
	// Log level details can be found in linux/kern_levels.h
	// Wrapper around printk(<LEVEL> pr_fmt(fmt), ##__VA__ARGS__)
	pr_info("Hello to the cruel world of kernel programming\n");	// KERN_INFO
	pr_alert("Uhoh, something really bad happened\n");		// KERN_ALERT
	pr_debug("Just a debug message\n");				// KERN_DEBUG
	pr_emerg("The world has fallen apart\n");			// KERN_EMERG

	return 0;
}

static void __exit lkm_exit(void) {
	pr_info("Exiting hellwold lkm\n");
}

module_init(lkm_init);
module_exit(lkm_exit);
