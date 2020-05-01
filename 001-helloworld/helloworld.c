#include <linux/module.h>
#include <linux/init.h>

MODULE_AUTHOR("Cooper Van Kampen");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Basic hello world module");

static __init lkm_init(void) {
	pr_info("Hello to the cruel world of kernel programming\n");
	return 0;
}

static void __exit lkm_exit(void) {
	pr_info("Exiting hellwold lkm\n");
}

module_init(lkm_init);
module_exit(lkm_exit);
