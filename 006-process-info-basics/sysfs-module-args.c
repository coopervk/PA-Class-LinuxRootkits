#define pr_fmt(fmt)	KBUILD_MODNAME " -> %s:%d -> " fmt,__func__,__LINE__
#include <linux/module.h>
#include <linux/init.h>

MODULE_AUTHOR("Cooper Van Kampen");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Basic hello world module");

// Arguments
static int counter;
module_param(counter, int, S_IWUSR | S_IRUGO);
MODULE_PARM_DESC(counter, "Simple int based counter");

static char *message = NULL;
module_param(message, charp, S_IWUSR | S_IRUGO);
MODULE_PARM_DESC(message, "A message from user space");

// "Main"
static int __init lkm_init(void) {
	pr_info("Arguments: counter = %d, message = %s\n", counter, message);

	return 0;
}

// Exit routine
static void __exit lkm_exit(void) {
	pr_info("Arguments on exit: counter = %d, message = %s\n", counter, message);
	pr_info("Exiting hellwold lkm\n");
}

// Define init and exit functions
module_init(lkm_init);
module_exit(lkm_exit);
